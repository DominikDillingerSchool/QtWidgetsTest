#include "mainwindow.h"

#include <QApplication>
#include <QWidget>

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#endif

int main(int argc, char *argv[])
{
#ifdef _WIN32
    if (AttachConsole(ATTACH_PARENT_PROCESS))
    {
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    }
#elif __linux__
    if (isatty(STDOUT_FILENO))
    {
        // Open /dev/tty for writing, which represents the terminal
        int tty_fd = open("/dev/tty", O_WRONLY);
        if (tty_fd != -1)
        {
            // Redirect stdout and stderr to /dev/tty
            dup2(tty_fd, STDOUT_FILENO);
            dup2(tty_fd, STDERR_FILENO);
        }
    }
#endif

    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}