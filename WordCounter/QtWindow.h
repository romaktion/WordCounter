#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWindow.h"
#include <qlabel.h>

struct parse_result;

class QtWindow : public QMainWindow
{
  Q_OBJECT

public:
  QtWindow(QWidget* parent = Q_NULLPTR);

private:
  Ui::QtWindowClass ui;

  void OnSuccess(const parse_result& result);
  void OnFailure(const std::string error);

private slots:
  void pushButtonHandle();
};
