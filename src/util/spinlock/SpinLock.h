#pragma once

#include <mutex>
#include <string>
#include "Minimal.h"
#include "Atomic.h"

namespace Zemni
{
    using std::string;

/**
 * This class implements locks that never block the thread: if the lock
 * isn't available during a lock operation, the thread spins until the
 * lock becomes available.  SpinLocks are intended for situations where
 * locks are not held for long periods of time, such as locks used for
 * mutual exclusion.  These locks are not recursive: if a thread attempts
 * to lock a SpinLock while holding it, the thread will deadlock.
 *
 * This class implements the Boost "Lockable" concept, so SpinLocks can be
 * used with the Boost locking facilities.
 */
    class SpinLock
    {
    public:
        explicit SpinLock(string name);

        virtual ~SpinLock();

        void lock();

        bool try_lock();

        void unlock();

        void setName(string name);

//        static void getStatistics(ProtoBuf::SpinLockStatistics *stats);

        static int numLocks();

        /*
         * This class automatically acquires a SpinLock on construction and
         * automatically releases it on destruction.
         */
        typedef std::lock_guard<SpinLock> Guard;

    private:
        /// Implements the lock: 0 means free, anything else means locked.
        Atomic<int> mutex;

        /// Descriptive name for this SpinLock. Used to identify the purpose of
        /// the lock, what it protects, where it exists in the codebase, etc.
        /// It is used when the getStatistics() method is invoked.
        string name;

        /// Total number of times this lock has been acquired.
        uint64_t acquisitions;

        /// Number of times this lock has been acquired, but not on the first try
        /// (that is, it was already locked).
        uint64_t contendedAcquisitions;

        /// Count of the number of processor ticks spent waiting to acquire this
        /// lock due to it having already been held.
        uint64_t contendedTicks;

    };

/**
 * This class can be used to create unnamed SpinLocks; it's intended
 * for use in array constructors. Making it a subclass has the advantage
 * that programmers must explicitly request it (they can't accidentally
 * forget to provide a name to SpinLock).
 */
    class UnnamedSpinLock : public SpinLock
    {
    public:
        UnnamedSpinLock() : SpinLock("unnamed")
        {}
    };

} // end RAMCloud
