#pragma once

#ifndef ZEMNI_QUEUE_FOLLY_H
#define ZEMNI_QUEUE_FOLLY_H

#include "Common.h"
#include "util/AvgCounter.h"
#include "Config.h"
#include "Queue.h"

namespace Zemni
{
    class Queue_folly : public Queue
    {
    private:
        AtomicAvgCounter<long> avgLength;
        folly::MPMCQueue<StreamElements> queue;

    public:
        explicit Queue_folly(int id) :
                Queue(id),
                avgLength(QUEUE_MANAGER_AVG_COUNTER_LEN),
                queue(MPMC_QUEUE_BUFFER_SIZE) {}

        ~Queue_folly() override = default;

        void blockingWrite(StreamElements &&element) override
        {
            queue.blockingWrite(std::forward<StreamElements>(element));
        }

        void blockingWrite(StreamElements &&element, void *buffer) override
        {
            queue.blockingWrite(std::forward<StreamElements>(element));
        }

        bool tryWrite(StreamElements &&element) override
        {
            return queue.writeIfNotFull(std::forward<StreamElements>(element));
        }

        virtual bool tryWriteUtil(StreamElements &&element, int useconds) override
        {
            return queue.tryWriteUntil(
                    std::chrono::system_clock::now() + std::chrono::microseconds(useconds),
                    std::forward<StreamElements>(element));
        }

        void blockingRead(StreamElements &element) override
        {
            return queue.blockingRead(element);
        }

        bool tryReadUtil(StreamElements &element, int useconds) override
        {
            return queue.tryReadUntil(std::chrono::system_clock::now() + std::chrono::microseconds(useconds), element);
        }

        bool tryRead(StreamElements &element) override
        {
            return queue.readIfNotEmpty(element);
        }

        bool isEmpty() override
        {
            return queue.isEmpty();
        }

        void addLengthToAvgCounter() override
        {
            avgLength.add(queue.size());
        }

        double getAvg() override
        {
            return avgLength.getAvg();
        }

        string toString() override
        {
            stringstream ss;
            ss << "<Queue<QUEUE_TYPE>>Folly id=" << id;
            return ss.str();
        }

        size_t sizeGuess() override
        {
            return queue.sizeGuess();
        }

        QueueType getType() override
        {
            return QUEUE_FOLLY;
        }

        string getTypeName() override
        {
            return "queue_folly";
        }
    };
}

#endif //ZEMNI_QUEUE_FOLLY_H
