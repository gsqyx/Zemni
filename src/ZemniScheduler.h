#pragma once

#include "Common.h"
#include <caf/actor_system_config.hpp>
#include "caf/all.hpp"
#include "caf/io/all.hpp"
#include "NodeProxy.h"
#include "QueueProxy.h"
#include <atomic>

namespace Zemni
{
    class ZemniScheduler
    {
    private:
        bool _hasInit;
        bool _hasStart;
        std::atomic_int _newNodeId;
        std::atomic_uint16_t _newNodeActorPort;
        std::atomic_uint16_t _newNodeZmqPort;
        caf::actor messageActor;
        unordered_map<int, shared_ptr<NodeProxy>> nodeProxies;
        unordered_map<int, shared_ptr<QueueProxy>> queueProxies;
        const int _actor_port;


        friend class ZemniSchedulerMsgActor;

    private:
        string handleMessage(int msgType, const string &msgContent);

        int getNewNodeId();

        uint16_t getNewNodeActorPort();

        uint16_t getNewNodeZmqPort();

        string processMsgNodeRegister(const string &msg);

        void processMsgQueueRequest(const string &msg);

    public:
        ZemniScheduler(int actor_port, uint16_t nodeActorPortStart, uint16_t nodeZmqPortStart);

        ZemniScheduler(const ZemniScheduler &) = delete;

        ZemniScheduler(ZemniScheduler &&) = delete;

    public:
        void init();

        void start();
    };
}
