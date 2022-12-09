#pragma once

#ifndef ZEMNI_COMMON_H
#define ZEMNI_COMMON_H

#include <unordered_map>
#include <ctime>
#include <chrono>
#include <iostream>
#include <folly/MPMCQueue.h>
#include "util/NTPClient.h"

namespace Zemni
{
    using std::string;
    using std::unordered_map;
    using std::shared_ptr;
    using std::vector;
    using std::make_shared;
    using std::cout;
    using std::endl;
    using std::stringstream;
    using std::cerr;
    using folly::MPMCQueue;

    inline time_t getMillisecondTimestamp()
    {
        std::time_t timestamp =
                std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::time_point_cast<std::chrono::milliseconds>(
                                std::chrono::system_clock::now()).time_since_epoch()).count();
        return timestamp;
    }

    inline time_t getNtpMillisecondTimestamp()
    {
        return NTPClient::getInstance().getMillisecondTimestamp();
    }

    template<typename Map>
    inline bool contains(const Map &map, const typename Map::key_type &key)
    {
        return (map.find(key) != map.end());
    }

    inline string handle_eptr(std::exception_ptr eptr) // passing by value is ok
    {
        try
        {
            if (eptr)
            {
                std::rethrow_exception(eptr);
            }
            return "Unknown exception";
        } catch (const std::exception &e)
        {
            stringstream ss;
            ss << "Caught exception \"" << e.what();
            return ss.str();
        }
    }
}

#endif //ZEMNI_COMMON_H
