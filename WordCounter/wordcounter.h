#pragma once
#include "types.h"

class wordcounter
{
public:
  wordcounter(const std::string& path);
  ~wordcounter();

  const parse_result get() const;

private:
  void wait();
  void proceed();
  void worker(wchar_t* buffer);
  parse_result parse(wchar_t* buffer);
  void insert_word_to_word_counter(parse_result& res, const std::wstring& in_word);
  void insert_word_to_word_counter(parse_result& res, const std::map<std::wstring, unsigned>::const_iterator& wordcounter);

  std::vector<std::thread> _threads;
  std::mutex _mutex;
  std::mutex _micro_mutex;
  std::condition_variable _cond;
  bool _work = false;
  unsigned _count = 0;
  parse_result _parse_result;
  const unsigned _threads_amount = std::thread::hardware_concurrency() > 0
    ? std::thread::hardware_concurrency() : 1;
  wchar_t* _buffer = nullptr;
};
