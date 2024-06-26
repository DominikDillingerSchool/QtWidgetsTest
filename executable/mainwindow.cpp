#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "benchmark.h"
#include "listDelegate.h"
#include "listModel.h"

#include <QShortcut>

MainWindow::MainWindow(QMainWindow *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ListModel *model = new ListModel(this);
    ListDelegate *delegate = new ListDelegate(this);

    ui->listView->setModel(model);
    ui->listView->setItemDelegate(delegate);

    connect(ui->BTN_Add, &QPushButton::clicked, this, [&](bool) {
        const QString text = ui->textEdit->toPlainText();
        dynamic_cast<ListModel *>(ui->listView->model())->appendEntry(text);
    });

    connect(delegate, &ListDelegate::deleteClicked, this, [&](const unsigned int &entryId) {
        dynamic_cast<ListModel *>(ui->listView->model())->deleteEntry(entryId);
    });

    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_Q), this);
    connect(shortcut, &QShortcut::activated, this, [&]() {
        const QString benchmarkString = Benchmark::benchmarks(dynamic_cast<ListModel *>(ui->listView->model()));
        ui->textEdit->setText(benchmarkString);
    });
}