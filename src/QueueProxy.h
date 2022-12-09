#pragma once

#ifndef ZEMNI_QUEUEPROXY_H
#define ZEMNI_QUEUEPROXY_H

#include "Common.h"
#include <set>

namespace Zemni
{
    class QueueProxy
    {
    private:
        int id;
        int producerNodeId; //同一队列只允许一个producer Node 和一个consumer Node
        int consumerNodeId;
//        uint16_t consumerNodePort; //一经创建，port不再修改

    public:
        explicit QueueProxy(int id) :
                id(id),
                producerNodeId(-1),
                consumerNodeId(-1)
        {
            CHECK(id >= 0);
        }

    public:
        int getId() const
        {
            return id;
        }

        void setId(int id)
        {
            QueueProxy::id = id;
        }

        /**
         * Add consumer
         * @param nodeId consumer node id
         * @return previous consumer id. If there is no previous
         * consumer node, return -1.
         */
        int setConsumer(int nodeId)
        {
            int preId = consumerNodeId;
            this->consumerNodeId = nodeId;
            return preId;
        }

        /**
         * Add producer
         * @param nodeId producer node id
         * @return previous producer id. If there is no previous
         * producer node, return -1.
         */
        int setProducer(int nodeId)
        {
            int preId = producerNodeId;
            this->producerNodeId = nodeId;
            return preId;
        }

        int getConsumer()
        {
            return consumerNodeId;
        }

        int getProducer()
        {
            return producerNodeId;
        }

        void removeConsumer()
        {
            consumerNodeId = -1;
        }

        void removeProducer()
        {
            producerNodeId = -1;
        }

        bool hasConsumer()
        {
            return consumerNodeId != -1;
        }

        bool hasProducer()
        {
            return producerNodeId != -1;
        }
    };
}
#endif //ZEMNI_QUEUEPROXY_H
