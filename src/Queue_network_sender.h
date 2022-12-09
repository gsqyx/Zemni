#include <utility>

#ifndef ZEMNI_QUEUE_NETWORK_H
#define ZEMNI_QUEUE_NETWORK_H

#include "Queue.h"
#include "message/NetworkStreamElement.h"
#include "network/NetworkSender.h"

namespace Zemni
{
    class Queue_network_sender : public Queue
    {
    private:
        NetworkSender *sender;

    public:
        explicit Queue_network_sender(int id) :
                Queue(id),
                sender(nullptr)
        {
        }

        void setSender(NetworkSender *sender)
        {
            this->sender = sender;
        }

        void blockingWrite(StreamElements &&elements) override
        {
            elements.set_queue_id(id);
            string str = elements.SerializeAsString();
            if (sender != nullptr)
                sender->send(id, str.data(), str.length());
        }

        void blockingWrite(StreamElements &&elements, void *buffer) override
        {
            elements.set_queue_id(id);
            size_t size = elements.ByteSizeLong();
            elements.SerializeToArray(buffer, size);
            if (sender != nullptr)
                sender->send(id, buffer, size);
            else
                DLOG(ERROR) << "sender is null";
        }

        bool tryWrite(StreamElements &&element) override
        {
            DLOG(FATAL) << "Call tryWrite on queue network";
            return false;
        }

        bool tryWriteUtil(StreamElements &&element, int useconds) override
        {
            DLOG(FATAL) << "Call tryWriteUtil on queue network";
        }

        void blockingRead(StreamElements &element) override
        {
            CHECK(false) << "Call read on Queue_network_sender";
        }

        bool tryRead(StreamElements &element) override
        {
            CHECK(false) << "Call read on Queue_network_sender";
            return false;
        }

        bool tryReadUtil(StreamElements &element, int useconds) override
        {
            CHECK(false) << "Call read on Queue_network_sender";
            return false;
        }

        void addLengthToAvgCounter() override
        {

        }

        double getAvg() override
        {
            return 0;
        }

        bool isEmpty() override
        {
            return true;
        }

        string toString() override
        {
            stringstream ss;
            ss << "<Queue_network_sender id=" << id;
            return ss.str();
        }

        size_t sizeGuess() override
        {
            return 0;
        }

        QueueType getType() override
        {
            return QUEUE_NETWORK_SENDER;
        }

        string getTypeName() override
        {
            return "queue_network_sender";
        }
    };
}

#endif //ZEMNI_QUEUE_NETWORK_H
