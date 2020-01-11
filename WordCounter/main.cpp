//WordCounter

#include "pch.h"
#include "text.h"


int main(int argc, char* argv[], char* envp[])
{
  auto timer = std::make_unique<timing>();

  setlocale(LC_ALL, "");

  /*auto t = std::make_unique<text>(L"Привет от юникода1!");
  auto r = t->byte_string();

  std::cout << r << '\n';

  auto t2 = std::make_unique<text>("Привет от юникода2!", "UTF-8");
  auto r2 = t2->byte_string();

  std::cout << r2 << '\n';

  return 0;*/

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
      std::cout << "First param [path to file] is missing! Using test.txt" << '\n';

      in = "test.txt";
      out = "out.txt";
    }

    return std::make_pair(in, out);
  };

  const auto [in_path, out_path] = init_path();

  std::ifstream inf(in_path);

  if (!inf.is_open())
    return 1;
  else
    inf.close();

  std::ofstream of(out_path);

  if (!of.is_open())
  {
    inf.close();
    return 1;
  }

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
