#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWindow.h"
#include <qlabel.h>

class QtWindow : public QMainWindow
{
  Q_OBJECT

public:
  QtWindow(QWidget* parent = Q_NULLPTR);

private:
  QLabel* label = nullptr;
  Ui::QtWidgetsApplication1Class ui;
};