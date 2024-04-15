#pragma once

#include "listModel.h"

#include <QObject>
#include <QString>
#include <string>

class Benchmark : public QObject
{
    Q_OBJECT

  public:
    static Q_INVOKABLE QString benchmarks(ListModel *model);

  private:
    static size_t getCurrentMemoryUsage();
    static std::string benchmark(ListModel *model, const unsigned int elementCount);
};