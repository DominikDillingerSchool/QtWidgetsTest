#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QShortcut>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>

#include <psapi.h>
#elif defined(__linux__)
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <unistd.h>
#elif defined(__EMSCRIPTEN__)
#include <emscripten.h>
#endif

MainWindow::MainWindow(QMainWindow *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_Q), this);
    connect(shortcut, &QShortcut::activated, this, []() {
        size_t memoryUsage = 0;

#if defined(_WIN32) || defined(_WIN64)
        PROCESS_MEMORY_COUNTERS_EX pmc;
        if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&pmc, sizeof(pmc)))
        {
            memoryUsage = pmc.WorkingSetSize;
        }
#elif defined(__linux__)
        std::ifstream memInfo("/proc/self/status");
        if (memInfo.is_open())
        {
            std::string line;
            while (std::getline(memInfo, line))
            {
                if (line.substr(0, 6) == "VmRSS:")
                {
                    std::istringstream iss(line.substr(7));
                    iss >> memoryUsage;
                    memoryUsage *= 1024;
                    break;
                }
            }
            memInfo.close();
        }
#elif defined(__EMSCRIPTEN__)
        memoryUsage = emscripten_run_script_int(R"(
    if (typeof window.performance !== 'undefined' && typeof window.performance.memory !== 'undefined') {
        window.performance.memory.usedJSHeapSize;
    }
    )");
#endif
        std::cout << "RAM: " << memoryUsage << " bytes\n";
    });
}
