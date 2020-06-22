//WordCounter

#include <QtWidgets/QApplication>
#include "QtWindow.h"


int main(int argc, char* argv[], char* envp[])
{
  QApplication a(argc, argv);
  QtWindow w;
  w.setWindowTitle("Word Counter");
  w.show();
  return a.exec();
}
