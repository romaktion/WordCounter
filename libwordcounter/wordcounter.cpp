#include "wordcounter.h"
#include "text.h"
#include "constants.h"
#include "timing.h"
#include <iostream>
#include <fstream>


wordcounter::wordcounter(const std::string& path)
{
  auto buffers = chunking(path);
  if (buffers.empty())
  {
    std::cerr << "WordCounter: prepare result is empty!\n";

    return;
  }

  //launch workers
  for (auto& b : buffers)
    if (b)
    {
      _count++;
      _threads.push_back(std::thread{ &wordcounter::worker, this, std::ref(b) });
    }

  wait();
}

wordcounter::wordcounter(const std::string& path, success_fn&& success_callback, failure_fn&& failure_callback)
{
  _success_callback = std::move(success_callback);
  _failure_callback = std::move(failure_callback);

  auto buffers = chunking(path);
  if (buffers.empty())
  {
    failure_callback("WordCounter: prepare result is empty!\n");

    return;
  }

  //launch workers
  for (auto& b : buffers)
    if (b)
    {
      _count++;
      _threads.push_back(std::thread{ &wordcounter::worker, this, std::ref(b) });
    }

  for (auto& t : _threads)
    if (t.joinable())
      t.join();
}

wordcounter::~wordcounter()
{
  for (auto& t : _threads)
    if (t.joinable())
      t.join();

  delete _text;
}

const parse_result& wordcounter::get() const
{
  return _parse_result;
}

void wordcounter::wait()
{
  std::unique_lock<std::mutex> lock(_mutex);
  _cond.wait(lock, [this]() {return _work; });
}

void wordcounter::proceed()
{
  _work = true;
  _cond.notify_all();

  if (_success_callback)
    _success_callback(_parse_result);
}

void wordcounter::worker(const wchar_t* buffer)
{
  auto timer = std::make_unique<timing>();

  parse_result res;
  parse(buffer, res);

  std::scoped_lock<std::mutex> lock(_micro_mutex);

  for (std::map<std::wstring, unsigned>::const_iterator it = res.words_amount.begin(); it != res.words_amount.end(); ++it)
    insert_word_to_word_counter(_parse_result, it);

  _parse_result.symbol_amount += res.symbol_amount;
  _count--;

  std::cout << "complete thread: " << std::this_thread::get_id() << ", time: " << timer->get() << '\n';

  if (_count <= 0)
    proceed();
}

void wordcounter::parse(const wchar_t* in_buffer, parse_result& out_parse_result)
{
  if (!in_buffer)
  {
    std::cerr << "Parse: buffer is NULL!\n";
    return;
  }

  auto prevois_is_alpha = false;

  const auto end_of_buffer = in_buffer + wcslen(in_buffer);

  while (in_buffer)
  {
    const wchar_t* end_of_word = nullptr;
    auto word = in_buffer;

    in_buffer = wcspbrk(in_buffer, DELIMITER);
    if (in_buffer)
    {
      if (in_buffer - word < 1)
      {
        in_buffer++;
        continue;
      }

      end_of_word = in_buffer;
      in_buffer++;
    }
    else
    {
      end_of_word = end_of_buffer;
    }

    for (auto it = word; it != end_of_word; ++it)
    {
      if (!iswalpha(*it))
      {
        //delete symbol condition
        if (*it != "'"[0] || !prevois_is_alpha || !iswalpha(*(it + 1)) || it == word)
        {
          for (auto p = (wchar_t*)it; p < end_of_word; p++)
            *p = *(p + 1);

          it--;
          end_of_word--;
        }

        prevois_is_alpha = false;
      }
      else
      {
        //separate word if two words detected in one word
        if (!prevois_is_alpha && it != word && *(it - 1) != "'"[0])
        {
          insert_word_to_word_counter(out_parse_result, std::wstring(word, it));
          word = it;
        }
        prevois_is_alpha = true;
      }
      out_parse_result.symbol_amount++;
    }

    if (word != end_of_word)
      insert_word_to_word_counter(out_parse_result, std::wstring(word, end_of_word));
  }
}

void wordcounter::insert_word_to_word_counter(parse_result& out_res, const std::wstring& in_word)
{
  std::map<std::wstring, unsigned>::iterator it = out_res.words_amount.find(in_word);
  if (it != out_res.words_amount.end())
    it->second++;
  else
    out_res.words_amount.insert(std::pair<std::wstring, unsigned>(in_word, 1));
}

void wordcounter::insert_word_to_word_counter(parse_result& out_res, const std::map<std::wstring, unsigned>::const_iterator& in_word_counter)
{
  std::map<std::wstring, unsigned>::iterator it = out_res.words_amount.find(in_word_counter->first);
  if (it != out_res.words_amount.end())
    it->second += in_word_counter->second;
  else
    out_res.words_amount.insert(std::pair<std::wstring, unsigned>(in_word_counter->first, in_word_counter->second));
}

const std::vector<const wchar_t*>& wordcounter::chunking(const std::string& path)
{
  if (path.empty())
  {
    std::cerr << "WordCounter: input file path is empty!\n";

    return _buffers;
  }

  auto timer = std::make_unique<timing>();

  //open
  std::ifstream inf(path, std::ios::binary);
  if (!inf.is_open())
  {
    std::cerr << "WordCounter: can't open input file!\n";
    return _buffers;
  }

  //calc lenght
  inf.seekg(0, std::ios::end);
  const auto lenght = (unsigned)inf.tellg();
  inf.seekg(0, std::ios::beg);

  std::cout << "Input file size: " << lenght / (float)1000000 << " Mb\n";

  //split up
  const auto part = (lenght % _threads_amount) == 0
    ? lenght / _threads_amount : lenght / _threads_amount + 1;
  auto l_buffer = new char[lenght + UTF8_SEQUENCE_MAXLEN];

  //read
  inf.read(l_buffer, lenght);
  inf.close();
  l_buffer[lenght] = '\0';

  //convert
  _text = new text(l_buffer, "");
  delete[] l_buffer;

  //tune
  _threads.reserve(_threads_amount);
  _buffers.reserve(_threads_amount);

  auto beg = _text->wide_string().c_str();
  const size_t wlenght = wcslen(beg) + UTF8_SEQUENCE_MAXLEN;
  const wchar_t* const end = beg + wlenght;
  auto pch = beg + part;

  for (unsigned i = 0; i < _threads_amount; i++)
  {
    pch = wcspbrk(pch, DELIMITER);
    if (!pch)
    {
      if (beg) _buffers.push_back(beg);

      break;
    }

    //modify one char in source string!
    auto m_pch = (wchar_t*)pch;
    *m_pch = '\0';

    _buffers.push_back(beg);

    if (!(pch + 1 + part))
    {
      beg = pch + 1;

      if (beg) _buffers.push_back(beg);

      break;
    }

    beg = pch + 1;
    pch = pch + 1 + part;

    if (pch >= end)
    {
      if (beg) _buffers.push_back(beg);

      break;
    }
  }

  std::cout << "file chunking time: " << timer->get() << '\n';

  return _buffers;
}
