#ifndef ZEMNI_QUEUETYPE_H
#define ZEMNI_QUEUETYPE_H

namespace Zemni
{
    enum QueueType
    {
        QUEUE_UNKNOWN,
        QUEUE_FOLLY,
        QUEUE_MOODYCAMEL,
        QUEUE_NETWORK_SENDER
    };
}

#endif //ZEMNI_QUEUETYPE_H
