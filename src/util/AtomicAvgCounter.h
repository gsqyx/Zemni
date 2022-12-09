#pragma once

#include "../Common.h"
#include <glog/logging.h>
#include "AtomicWrapper.h"

namespace Zemni
{
    template<typename T>
    class AtomicAvgCounter
    {
    private:
        size_t length;
        uint64_t cursor;
        std::vector<AtomicWrapper < T>> values;

    public:
        explicit AtomicAvgCounter(size_t length);

        void add(T value);

        double getAvg() const;

        void reset();
    };

    template<typename T>
    AtomicAvgCounter<T>::AtomicAvgCounter(size_t length): length(length), cursor(0)
    {
        CHECK(length > 0);
        for (size_t i = 0; i < length; ++i)
            values.emplace_back((T) 0);
    }

    template<typename T>
    void AtomicAvgCounter<T>::add(T value)
    {
        values[cursor++ % length]._a.operator=(value);
    }

    template<typename T>
    double AtomicAvgCounter<T>::getAvg() const
    {
        double sum = 0;
        for (size_t j = 0; j < length; ++j)
        {
            sum += values[j]._a;
        }
        return sum / length;
    }

    template<typename T>
    void AtomicAvgCounter<T>::reset()
    {
        for (int i = 0; i < length; ++i)
        {
            values[i]._a.operator=(0);
        }
        cursor = 0;
    }
}
