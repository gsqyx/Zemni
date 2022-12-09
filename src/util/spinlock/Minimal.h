#pragma once

#include <vector>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <list>

namespace Zemni
{
    // A macro to disallow the copy constructor and operator= functions

    template<typename Small, typename Large>

    Small downCast(const Large &large)
    {
        Small small = static_cast<Small>(large);
        assert(large - small == 0);
        return small;
    };

    #ifndef DISALLOW_COPY_AND_ASSIGN
    #define DISALLOW_COPY_AND_ASSIGN(TypeName) \
        TypeName(const TypeName&) = delete;             \
        TypeName& operator=(const TypeName&) = delete;
    #endif

    #define expect_true(expr)   __builtin_expect((expr), true)

    #define expect_false(expr)   __builtin_expect((expr), false)

    /**
     * Return the size of the given type as a uint32_t. This convenience macro
     * tavoids having downcasts everywhere we take sizeof, which returns size_t,
     * but want a uint32_t instead. Stay tuned for a fancier templated version
     * by syang0 and ongaro...
     */
    #define sizeof32(type) downCast<uint32_t>(sizeof(type))
}
