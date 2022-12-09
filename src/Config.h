#pragma once

#define ZEMNI_NETWORK_SECONDS_TIMEOUT 5
#define MPMC_QUEUE_BUFFER_SIZE  256
#define ZMQ_PARALLELISM 10
#define QUEUE_MANAGER_AVG_COUNTER_LEN 5 //todo 修改
#define QUEUE_MANAGER_LENGTH_DETECT_INTERVAL 2

//#define QUEUE_TYPE_MOODYCAMEL // use moodyCamel queue, undef to use folly queue
//#define ENABLE_SNAPPY

//#define ENABLE_DEBUG
#ifdef __APPLE__

#include "TargetConditionals.h"

#ifdef TARGET_OS_MAC

#define NTP_HOST "127.0.0.1"
#else
#   error "Unknown Apple platform"
#endif
#elif __linux__

#define NTP_HOST "10.61.0.168"

#else
#   error "Unknown compiler"
#endif
