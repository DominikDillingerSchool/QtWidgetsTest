#include "benchmark.h"

#include <chrono>
#include <iostream>
#include <math.h>
#include <sstream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>

#include <psapi.h>
#elif defined(__linux__)
#include <fstream>
#include <unistd.h>
#elif defined(__EMSCRIPTEN__)
#include <emscripten.h>
#endif

Q_INVOKABLE QString Benchmark::benchmarks(ListModel *model)
{
    std::stringstream stream;
    stream << benchmark(model, 0);
    for (unsigned int elementCountExponent = 0; elementCountExponent < 6; elementCountExponent++)
    {
        const unsigned int elementCount = pow(10, elementCountExponent);
        stream << benchmark(model, elementCount);
    }

    std::string benchmarkString = stream.str();

    std::cout << benchmarkString;

    return QString::fromUtf8(benchmarkString.c_str());
}

size_t Benchmark::getCurrentMemoryUsage()
{
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

    return memoryUsage;
}

std::string Benchmark::benchmark(ListModel *model, const unsigned int elementCount)
{
    model->clearModel();

    auto startPoint = std::chrono::high_resolution_clock::now();

    for (unsigned int element = 0; element < elementCount; element++)
    {
        model->appendEntry("Entry");
    }

    auto endPoint = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endPoint - startPoint).count();
    const size_t memoryUsage = getCurrentMemoryUsage();

    std::stringstream stream;
    stream << "Element " << elementCount << ": RAM: " << memoryUsage << " bytes"
           << " Runtime: " << duration << "ns\n";
    return stream.str();
}