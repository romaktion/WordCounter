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

  if (filePath.isEmpty())
  {
    ui.label->setText("No file selected!");
    return;
  }
  
  ui.label->setText("...");

  std::thread([&, filePath]
    {
      std::make_unique<wordcounter>(filePath.toStdString(),
        [this](const parse_result& res) { OnSuccess(res); },
        [this](const std::string error) { OnFailure(error); });
    }).detach();
}

void QtWindow::OnSuccess(const parse_result& res)
{
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

  //display result
  auto wordcounter = 0u;
  for (const auto& wc : res.words_amount)
    wordcounter += wc.second;

  ui.label->setText("Words: " + QString::fromStdString(std::to_string(wordcounter))
    + "\n Symbols: " + QString::fromStdString(std::to_string(res.symbol_amount)));
}

void QtWindow::OnFailure(const std::string error)
{
  ui.label->setText(QString::fromStdString(error));
}
