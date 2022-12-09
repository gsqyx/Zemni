#pragma once

#ifndef ZEMNI_QUEUE_H
#define ZEMNI_QUEUE_H

#include "util/AvgCounter.h"
#include "Config.h"
#include "queue/blockingconcurrentqueue.h"
#include "util/AtomicWrapper.h"
#include "util/AtomicAvgCounter.h"
#include "QueueType.h"
#include "message/StreamElement.pb.h"

namespace Zemni
{
    using Zemni_protobuf::StreamElements;

    class Queue
    {
    protected:
        int id;

    public:
        explicit Queue(int id) : id(id) {}

        virtual ~Queue() = default;

    public:
        virtual void blockingWrite(StreamElements &&element) = 0;

        virtual void blockingWrite(StreamElements &&element, void *buffer) = 0;

        virtual bool tryWrite(StreamElements &&element) = 0;

        virtual bool tryWriteUtil(StreamElements &&element, int useconds) = 0;

        virtual void blockingRead(StreamElements &element) = 0;

        virtual bool tryRead(StreamElements &element) = 0;

        virtual bool tryReadUtil(StreamElements &element, int useconds) = 0;

        virtual void addLengthToAvgCounter() = 0;

        virtual bool isEmpty() = 0;

        virtual double getAvg() = 0;

        int getId() const
        {
            return id;
        }

        virtual size_t sizeGuess() = 0;

        virtual string toString() = 0;

        virtual QueueType getType() = 0;

        virtual string getTypeName() = 0;
    };
}

#endif //ZEMNI_QUEUE_H
