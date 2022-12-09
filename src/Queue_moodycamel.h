#pragma once

#ifndef ZEMNI_QUEUE_MOODY_CAMEL_H
#define ZEMNI_QUEUE_MOODY_CAMEL_H

#include "Common.h"
#include "message/StreamElement.h"
#include "util/AvgCounter.h"
#include "Config.h"
#include "queue/blockingconcurrentqueue.h"
#include "Queue.h"

namespace Zemni
{
    class Queue_moodyCamel : public Queue
    {
    private:
        AtomicAvgCounter<long> avgLength;
        moodycamel::BlockingConcurrentQueue<StreamElements> queue;

    public:
        explicit Queue_moodyCamel(int id) :
                Queue(id),
                avgLength(QUEUE_MANAGER_AVG_COUNTER_LEN),
                queue(MPMC_QUEUE_BUFFER_SIZE) {}

        void blockingWrite(StreamElements &&element) override
        {
            while (!queue.try_enqueue(element))
            {
            };
        }

        virtual void blockingWrite(StreamElements &&element, void *buffer) override
        {
            while (!queue.try_enqueue(element))
            {
            };
        }

        bool tryWrite(StreamElements &&element) override
        {
            return queue.try_enqueue(element);
        }

        bool tryWriteUtil(StreamElements &&element, int useconds) override
        {
            CHECK(false);
        }

        void blockingRead(StreamElements &element) override
        {
            queue.wait_dequeue(element);
        }

        bool tryReadUtil(StreamElements &element, int useconds) override
        {
            return queue.wait_dequeue_timed(element, useconds);
        }

        bool tryRead(StreamElements &element) override
        {
            return queue.try_dequeue(element);
        }

        void addLengthToAvgCounter() override
        {
            avgLength.add(queue.size_approx());
        }

        double getAvg() override
        {
            return avgLength.getAvg();
        }

        bool isEmpty() override
        {
            return queue.size_approx() <= 0;
        }

        string toString() override
        {
            stringstream ss;
            ss << "Queue::MoodyCamel id=" << id;
            return ss.str();
        }

        size_t sizeGuess() override
        {
            return queue.size_approx();
        }

        QueueType getType() override
        {
            return QUEUE_MOODYCAMEL;
        }

        string getTypeName() override
        {
            return "queue_moodycamel";
        }
    };
}

#endif //ZEMNI_QUEUE_MOODY_CAMEL_H
