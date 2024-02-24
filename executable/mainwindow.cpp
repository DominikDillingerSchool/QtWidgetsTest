#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "listDelegate.h"
#include "listModel.h"

MainWindow::MainWindow(QMainWindow *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ListModel *model = new ListModel(this);
    ListDelegate *delegate = new ListDelegate(this);

    model->appendEntry("Test123");

    ui->listView->setModel(model);
    ui->listView->setItemDelegate(delegate);

    connect(ui->BTN_Add, &QPushButton::clicked, this, [&](bool) {
        const QString text = ui->textEdit->toPlainText();
        const QString trimmedText = text.trimmed();
        if (trimmedText.isEmpty())
        {
            return;
        }
        dynamic_cast<ListModel *>(ui->listView->model())->appendEntry(trimmedText);
    });

    connect(delegate, &ListDelegate::deleteClicked, this, [&](const unsigned int &entryId) {
        dynamic_cast<ListModel *>(ui->listView->model())->deleteEntry(entryId);
    });
}
