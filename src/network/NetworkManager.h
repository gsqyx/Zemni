#pragma once

#include "../Common.h"
#include "../message/StreamElement.pb.h"
#include "../message/StreamElement.h"
#include "DataStream.h"
#include "NetworkSender.h"
#include "NetworkReceiver.h"

namespace Zemni
{
    using Zemni_protobuf::StreamElement;

    class ZemniNodeManager;

    class NetworkManager
    {
    private:
        bool _hasInit;
        bool _hasStart;
        ZemniNodeManager &queueNodeManager;
        zmq::context_t zmqContext;
        NetworkSender networkSender;
        std::vector<std::shared_ptr<NetworkReceiver>> networkReceivers;

    public:
        explicit NetworkManager(ZemniNodeManager &queueNodeManager);

        NetworkManager(const NetworkManager &) = delete;

        NetworkManager(NetworkManager &&) = delete;

        ~NetworkManager() = default;

    public:
        void init();

        void start();

        void disConnectDataStream(int queueId, bool asConsumer);

        void setReceiverZmqPort(uint16_t port);

        void registerQueueAsConsumer(shared_ptr<Queue> queue);

        void registerQueueAsProducer(int queue_id, uint16_t dst_port, int dst_host_id);

        void unregisterQueueAsProducer(int queue_id);

        void connectToProducer(string host);
    };
}
