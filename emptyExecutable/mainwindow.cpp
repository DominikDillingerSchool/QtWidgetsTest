#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QMainWindow *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // connect(ui->BTN_storeA, &QPushButton::clicked, this, [&](bool) { m_samplesA = ui->processView->GetSamples(0); });
}
