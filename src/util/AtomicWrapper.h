#ifndef ZEMNI_ATOMICWRAPPER_H
#define ZEMNI_ATOMICWRAPPER_H

#include <atomic>

namespace Zemni
{
    template<typename T>
    struct AtomicWrapper
    {
        std::atomic<T> _a;

        AtomicWrapper()
                : _a() {}

        explicit AtomicWrapper(const std::atomic<T> &a)
                : _a(a.load()) {}

        AtomicWrapper(const AtomicWrapper &other)
                : _a(other._a.load()) {}

        AtomicWrapper &operator=(const AtomicWrapper &other)
        {
            _a.store(other._a.load());
        }
    };
}
#endif //ZEMNI_ATOMICWRAPPER_H
