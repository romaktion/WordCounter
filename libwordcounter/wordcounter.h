#pragma once

#include <map>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class text;

#ifdef libwordcounter_EXPORTS
#define libwordcounter_API __declspec(dllexport)
#else
#define libwordcounter_API __declspec(dllimport)
#endif

constexpr auto delimiter = L" \n\t";
#define DELIMITER delimiter

struct
#ifdef _WIN32
  libwordcounter_API
#endif
  parse_result final
{
	unsigned symbol_amount = 0;
	std::map<std::wstring, unsigned> words_amount;

	parse_result() {}
	parse_result(unsigned in_symbol_amount, std::map<std::wstring, unsigned> in_words_amount)
	{
		symbol_amount = in_symbol_amount;
		words_amount = in_words_amount;
	}
};

class
#ifdef _WIN32
   libwordcounter_API
#endif
  wordcounter final
{
  using success_fn = std::function<void(const parse_result&)>;
  using failure_fn = std::function<void(const std::string&)>;

public:
  explicit wordcounter(const std::string& path,
    success_fn&& success_callback = nullptr,
    failure_fn&& failure_callback = nullptr);

  ~wordcounter();

  const parse_result& get() const;

private:
  void wait();
  void proceed();

  const std::vector<const wchar_t*>& chunking(const std::string& path);
  void worker(const wchar_t* buffer);
  void parse(const wchar_t* in_buffer, parse_result& out_parse_result);
  void insert_word_to_word_counter(parse_result& out_res, const std::wstring& in_word);
  void insert_word_to_word_counter(parse_result& out_res, const std::map<std::wstring,
    unsigned>::const_iterator& wordcounter);

  std::vector<std::thread> _threads;
  std::mutex _mutex;
  std::mutex _micro_mutex;
  std::condition_variable _cond;
  bool _work = false;
  unsigned _count = 0;
  parse_result _parse_result;
  const unsigned _threads_amount = std::thread::hardware_concurrency() > 0
    ? std::thread::hardware_concurrency() : 1;

  success_fn&& _success_callback = nullptr;
  failure_fn&& _failure_callback = nullptr;

  std::vector<const wchar_t*> _buffers;

  text* _text = nullptr;
};
