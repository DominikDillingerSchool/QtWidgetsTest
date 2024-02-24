#include "mainwindow.h"

#include <QApplication>
#include <QWidget>

#ifdef _WIN32
#include <Windows.h>
#endif

int main(int argc, char *argv[])
{
#ifdef _WIN32
    if (AttachConsole(ATTACH_PARENT_PROCESS))
    {
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    }
#endif

    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}