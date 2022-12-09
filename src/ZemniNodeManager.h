#include <utility>

#pragma once

#include <caf/actor.hpp>
#include "Common.h"
#include <folly/MPMCQueue.h>
#include "util/AvgCounter.h"
#include "message/StreamElement.h"
#include "util/spinlock/SpinLock.h"
#include "Queue_folly.h"
#include "network/NetworkManager.h"
#include "ResetQueueListener.h"
#include <boost/signals2.hpp>

namespace Zemni
{
    class ZemniNodeManagerMsgActor;

    typedef shared_ptr<Queue> QueuePtr;
    typedef boost::signals2::signal<void(QueuePtr)>::slot_type ResetQueueCallbackSlot;

    class ZemniNodeManager
    {
    private:
        struct QueueInfo
        {
            explicit QueueInfo(QueuePtr queue) : queue(std::move(queue)) {}

            QueuePtr &getQueue()
            {
                return queue;
            }

            boost::signals2::signal<void(QueuePtr)> &getSignal()
            {
                return signal;
            }

        private:
            QueuePtr queue;
            boost::signals2::signal<void(QueuePtr)> signal;
        };

    private:
        int id;
        string hostname;
        uint16_t actor_port;
        uint16_t zmq_port;
        bool _hasInit;
        bool _hasStart;
        bool _isRunning;
        caf::actor schedulerActor;
        caf::actor messageActor;
        std::unordered_map<int, QueueInfo> queueInfos;
//        std::unordered_map<int, AvgCounter<ssize_t>> avgLength;
        SpinLock lock;
        NetworkManager networkManager;
        string scheduler_host;
        uint16_t scheduler_port;

        friend class ZemniNodeManagerMsgActor;

        friend class NetworkManager;

    public:
        explicit ZemniNodeManager(string hostname, string scheduler_host, uint16_t scheduler_port);

        ZemniNodeManager(ZemniNodeManager &) = delete;

        ZemniNodeManager(ZemniNodeManager &&) = delete;

    private:
        string handleMessage(int msgType, const string &msgContent);

        void processMsgDataStreamProducerReset(const string &msgContent);

        void processMsgDataStreamConsumerConnect(const string &msgContent);

        void processMsgQueueReleaseRequest(const string &msgContent);

    public:
        void init();

        void start();

        void stop();

        double getAvgQueueLength(int queue_id);

        /*
         * Get max average length of input queues
         */
        double getAvgQueueLength(vector<int> &queueIds);

        bool containsQueue(int queue_id);

        shared_ptr<Queue> blockingGetLocalQueue(int queue_id);

        /**
         * Get queue, automatic released when the queue exists in other node
         * @param queue_id
         * @param asConsumer
         * @return
         */
        QueuePtr getQueue(int queue_id, bool asConsumer);

        QueuePtr getQueueForProducer(int queue_id, ResetQueueCallbackSlot slot);

        boost::signals2::signal<void(QueuePtr)> &getResetSignal(int queue_id);

        QueuePtr getQueueForConsumer(int queue_id);
    };
}
