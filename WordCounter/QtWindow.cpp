#include "QtWindow.h"
#include <QFileDialog>
#include "await.h"
#include "wordcounter.h"
#include <fstream>
#include <iostream>

QtWindow::QtWindow(QWidget* parent)
  : QMainWindow(parent)
{
  ui.setupUi(this);

  connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(pushButtonHandle()));
}

void QtWindow::pushButtonHandle()
{
  auto filePath = QFileDialog::getOpenFileName(this, tr("Open source text file"), "c:/", tr("Text Files (*.txt)"));

  ui.label->setText("...");

  auto res = await([&filePath]()
    {
      auto queue = std::make_unique<wordcounter>(filePath.toStdString());

      return queue->get();
    });

  //open file to write
  std::wofstream of("out.txt", std::ios::binary);
  if (!of.is_open())
  {
    std::cerr << "Can't open output file!\n";
    return;
  }
  of.imbue(std::locale(""));

  //write result
  for (const auto& r : res.words_amount)
    of << r.first << " - " << r.second << '\n';
  of.close();

  auto wordcounter = 0u;
  for (const auto& wc : res.words_amount)
    wordcounter += wc.second;

  ui.label->setText("Words: " + QString::fromStdString(std::to_string(wordcounter))
    + "\n Symbols: " + QString::fromStdString(std::to_string(res.symbol_amount)));
}
