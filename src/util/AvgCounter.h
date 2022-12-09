#pragma once

#include "../Common.h"

namespace Zemni
{
    template<typename T>
    class AvgCounter
    {
    private:
        int length;
        uint64_t cursor;
        std::vector<T> values;

    public:
        explicit AvgCounter(int length);

        void add(T value);

        double getAvg() const;

        void reset();
    };

    template<typename T>
    AvgCounter<T>::AvgCounter(int length): length(length), cursor(0)
    {
        CHECK(length > 0);
        for (int i = 0; i < length; ++i)
            values.push_back(-1);
    }

    template<typename T>
    void AvgCounter<T>::add(T value)
    {
        values[cursor++ % length] = value;
    }

    template<typename T>
    double AvgCounter<T>::getAvg() const
    {
        double sum = 0;
        for (auto &value : values)
        {
            sum += value;
        }
        return sum / length;
    }

    template<typename T>
    void AvgCounter<T>::reset()
    {
        for (auto &value : values)
        {
            value = -1;
        }
        cursor = 0;
    }
}
