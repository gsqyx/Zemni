#include <mutex>
#include <unordered_set>

#include "Cycles.h"
#include "Fence.h"
#include "SpinLock.h"

namespace Zemni
{

/// This namespace is used to keep track of all of the SpinLocks currently
/// in existence, so that we can enumerate them to monitor lock contention.
    namespace SpinLockTable
    {
        /**
         * Returns a structure containing the addresses of all SpinLocks
         * currently in existence.
         *
         * There is a function wrapper around this variable to force
         * initialization before usage. This is relevant when SpinLock is
         * initialized in the constructor of a statically declared object.
         */
        std::unordered_set<SpinLock *> *allLocks()
        {
            static std::unordered_set<SpinLock *> map;
            return &map;
        }

        /**
         * This mutex protects the map pointed to by "allLocks()".
         *
         * See the comment above for why this is a function and not a variable.
         */
        std::mutex *lock()
        {
            static std::mutex mutex;
            return &mutex;
        }
    } // namespace SpinLockTable

/**
 * Construct a new SpinLock and give it the provided name.
 */
    SpinLock::SpinLock(string name)
            : mutex(0), name(name), acquisitions(0), contendedAcquisitions(0), contendedTicks(0)
    {
        std::lock_guard<std::mutex> lock(*SpinLockTable::lock());
        SpinLockTable::allLocks()->insert(this);
    }

    SpinLock::~SpinLock()
    {
        std::lock_guard<std::mutex> lock(*SpinLockTable::lock());
        SpinLockTable::allLocks()->erase(this);
    }

/**
 * Acquire the SpinLock; blocks the thread (by continuously polling the lock)
 * until the lock has been acquired.
 */
    void
    SpinLock::lock()
    {
        uint64_t startOfContention = 0;

        while (mutex.exchange(1) != 0)
        {
            if (startOfContention == 0)
            {
                startOfContention = Cycles::rdtsc();
//            if (logWaits) {
//                printf("Waiting on SpinLock\n");
//            }
            } else
            {
                uint64_t now = Cycles::rdtsc();
                if (Cycles::toSeconds(now - startOfContention) > 1.0)
                {
                    printf("%s SpinLock locked for one second; deadlock?\n",
                           name.c_str());
                    contendedTicks += now - startOfContention;
                    startOfContention = now;
                }
            }
        }
        Fence::enter();

        if (startOfContention != 0)
        {
            contendedTicks += (Cycles::rdtsc() - startOfContention);
            contendedAcquisitions++;
        }
        acquisitions++;
    }

/**
 * Try to acquire the SpinLock; does not block the thread and returns
 * immediately.
 *
 * \return
 *      True if the lock was successfully acquired, false if it was already
 *      owned by some other thread.
 */
    bool
    SpinLock::try_lock()
    {
        int old = mutex.exchange(1);
        if (old == 0)
        {
            Fence::enter();
            return true;
        }
        return false;
    }

/**
 * Release the SpinLock.  The caller must previously have acquired the
 * lock with a call to #lock or #try_lock.
 */
    void
    SpinLock::unlock()
    {
        Fence::leave();
        mutex.store(0);
    }

/**
 * Change the name of the SpinLock. The name is intended to give some hint as
 * to the purpose of the lock, where it was declared, etc.
 *
 * \param name
 *      The string name to give this lock.
 */
    void
    SpinLock::setName(string name)
    {
        this->name = name;
    }

/**
 * Return the total of SpinLocks currently in existence; intended
 * primarily for testing.
 */
    int
    SpinLock::numLocks()
    {
        std::lock_guard<std::mutex> lock(*SpinLockTable::lock());
        return downCast<int>(SpinLockTable::allLocks()->size());
    }

} // namespace RAMCloud
