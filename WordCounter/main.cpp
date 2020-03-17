//WordCounter

#include "pch.h"
#include "text.h"
#include <locale>
#include <codecvt>


int main(int argc, char* argv[], char* envp[])
{
  auto timer = std::make_unique<timing>();

  setlocale(LC_ALL, "");

  std::cout << "main thread start id: " << std::this_thread::get_id() << '\n';

  auto init_path = [&argc, &argv]()
  {
    std::string in = "";
    std::string out = "";

    if (argc > 1)
    {
      for (int i = 0; i < argc; i++)
      {
        std::string str = argv[i];

        if (str == "/i" && argc > i)
          in = argv[i + 1];

        if (str == "/o" && argc > i)
          out = argv[i + 1];
      }

      if (in.length() <= 0)
        in = "test.txt";

      if (out.length() <= 0)
        out = "out.txt";
    }
    else
    {
      std::cout << "Params are empty (path to in/out file)! Using test.txt and out.txt" << '\n';

      in = "test.txt";
      out = "out.txt";
    }

    return std::make_pair(in, out);
  };

  const auto [in_path, out_path] = init_path();

  std::wifstream inf(in_path);

  if (!inf.is_open())
  {
    std::cerr << "Can't open input file!\n";
    return 1;
  }
  else
    inf.close();

  std::wofstream of(out_path, std::ios::binary);

  if (!of.is_open())
  {
    std::cerr << "Can't open output file!\n";
    inf.close();
    return 1;
  }
  of.imbue(std::locale(""));

  std::cout << "input file: " << in_path << '\n';

  const auto res = await([=]
    {
      auto queue = std::make_unique<wordcounter>(in_path);
      return queue->get();
    });

  for (const auto& r : res.words_amount)
    of << r.first << " - " << r.second << '\n';

  of.close();

  std::cout << "output file: " << out_path << '\n';
  std::cout << "symbols amount: " << res.symbol_amount << '\n';

  auto wordcounter = 0u;
  for (const auto& wc : res.words_amount)
    wordcounter += wc.second;

  std::cout << "words amount: " << wordcounter << '\n';
  std::cout << "main thread finish id: " << std::this_thread::get_id() << " with time: " << timer->get() << '\n';

  return 0;
}
