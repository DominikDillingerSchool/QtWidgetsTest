#pragma once

#include <QListWidget>
#include <QListWidgetItem>
#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QMainWindow *parent = nullptr);

  private:
    Ui::MainWindow *ui;
};