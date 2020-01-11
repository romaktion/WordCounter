#include "pch.h"
#include "wordcounter.h"


wordcounter::wordcounter(const std::string& path)
{
  auto timer = std::make_unique<timing>();

  auto inf = std::ifstream(path, std::ios::binary);
  if (!inf.is_open())
    return;

  inf.seekg(0, std::ios::end);
  const auto lenght = (unsigned)inf.tellg();

  std::cout << "Input file size: " << lenght / (double)1000000 << " Mb\n";

  inf.seekg(0, std::ios::beg);
  const unsigned part = (lenght % _threads_amount) == 0
    ? lenght / _threads_amount : lenght / _threads_amount + 1;
  _buffer = new char[lenght + 1];
  inf.read(_buffer, lenght);
  inf.close();
  _buffer[lenght] = '\0';
  char* beg = _buffer;

  _threads.reserve(_threads_amount);
  std::vector<char*> buffers;
  buffers.reserve(_threads_amount);

  const char* const end = beg + lenght;
  char* pch = beg + part;

  auto push_back = [&buffers](char* str) { buffers.push_back(str); };

  for (unsigned i = 0; i < _threads_amount; i++)
  {
    pch = strpbrk(pch, DELIMITER);
    if (!pch)
    {
      if (beg)
        push_back(beg);

      break;
    }

    *pch = '\0';
    push_back(beg);

    if (!(pch + 1 + part))
    {
      beg = pch + 1;

      if (beg)
        push_back(beg);

      break;
    }

    beg = pch + 1;
    pch = pch + 1 + part;

    if (pch >= end)
    {
      if (beg)
        push_back(beg);

      break;
    }
  }

  for (auto& b : buffers)
    if (b)
    {
      _count++;
      _threads.push_back(std::thread{ &wordcounter::worker, this, std::ref(b) });
    }

  std::cout << "file chunking time: " << timer->get() << '\n';

  wait();
}

wordcounter::~wordcounter()
{
  for (auto& t : _threads)
    if (t.joinable())
      t.join();

  delete[] _buffer;
}

const parse_result wordcounter::get() const
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
}

void wordcounter::worker(char* buffer)
{
  auto timer = std::make_unique<timing>();

  const auto& res = parse(buffer);

  std::scoped_lock<std::mutex> lock(_micro_mutex);

  for (std::map<std::string, unsigned>::const_iterator it = res.words_amount.begin(); it != res.words_amount.end(); ++it)
    insert_word_to_word_counter(_parse_result, it);

  _parse_result.symbol_amount += res.symbol_amount;
  _count--;

  std::cout << "complete thread: " << std::this_thread::get_id() << ", time: " << timer->get() << '\n';

  if (_count <= 0)
    proceed();
}

parse_result wordcounter::parse(char* buffer)
{
  if (!buffer)
    return parse_result();

  bool prevois_is_alpha = false;
  parse_result res;

#ifdef _DEBUG
  //remove not valid chars
  char* const end_of_buffer = std::remove_if(&buffer[0], &buffer[strlen(buffer)], [](char const c) { return ((int)c < -1 || (int)c > 255 || c == '\r'); });
  *end_of_buffer = '\0';
#else
  char* const end_of_buffer = buffer + strlen(buffer);
#endif

  while (buffer)
  {
    char* end_of_word = nullptr;
    auto word = buffer;

    buffer = strpbrk(buffer, DELIMITER);
    if (buffer)
    {
      if (buffer - word < 1)
      {
        buffer++;
        continue;
      }

      end_of_word = buffer;
      buffer++;
    }
    else
    {
      end_of_word = end_of_buffer;
    }

    //word analysis cycle TODO Change to ptr++
    for (auto it = word; it != end_of_word; ++it)
    {
      if (!isalpha(*it))
      {
        //delete symbol condition
        if (*it != "'"[0] || !prevois_is_alpha || !isalpha(*(it + 1)) || it == word)
        {
          for (auto p = it; p < end_of_word; p++)
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
          insert_word_to_word_counter(res, std::string(word, it));
          word = it;
        }

        prevois_is_alpha = true;
      }

      res.symbol_amount++;
    }

    if (word != end_of_word)
      insert_word_to_word_counter(res, std::string(word, end_of_word));
  }

  return res;
}

void wordcounter::insert_word_to_word_counter(parse_result& res, const std::string& in_word)
{
  std::map<std::string, unsigned>::iterator it = res.words_amount.find(in_word);
  if (it != res.words_amount.end())
    it->second++;
  else
    res.words_amount.insert(std::pair<std::string, unsigned>(in_word, 1));
}

void wordcounter::insert_word_to_word_counter(parse_result& res, const std::map<std::string, unsigned>::const_iterator& in_word_counter)
{
  std::map<std::string, unsigned>::iterator it = res.words_amount.find(in_word_counter->first);
  if (it != res.words_amount.end())
    it->second += in_word_counter->second;
  else
    res.words_amount.insert(std::pair<std::string, unsigned>(in_word_counter->first, in_word_counter->second));
}
