#include "QtWindow.h"

QtWindow::QtWindow(QWidget* parent)
  : QMainWindow(parent)
{
  label = new QLabel("Hello from label!", this);
  ui.setupUi(this);
}
