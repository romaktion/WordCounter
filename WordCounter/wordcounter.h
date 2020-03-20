#pragma once
#include "types.h"

class wordcounter
{
public:
  wordcounter(const std::string& path);
  ~wordcounter();

  const parse_result& get() const;

private:
  void wait();
  void proceed();
  void worker(const wchar_t* buffer);
  void parse(const wchar_t* in_buffer, parse_result& out_parse_result);
  void insert_word_to_word_counter(parse_result& out_res, const std::wstring& in_word);
  void insert_word_to_word_counter(parse_result& out_res, const std::map<std::wstring, unsigned>::const_iterator& wordcounter);

  std::vector<std::thread> _threads;
  std::mutex _mutex;
  std::mutex _micro_mutex;
  std::condition_variable _cond;
  bool _work = false;
  unsigned _count = 0;
  parse_result _parse_result;
  const unsigned _threads_amount = std::thread::hardware_concurrency() > 0
    ? std::thread::hardware_concurrency() : 1;
  const wchar_t* _buffer = nullptr;
};