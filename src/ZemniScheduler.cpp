#include "ZemniScheduler.h"
#include <glog/logging.h>
#include "NodeProxy.h"
#include "Config.h"
#include "message/Queue.pb.h"
#include "util/CAFUtil.h"
#include "message/ZemniSchedulerMsgActor.h"
#include "message/StreamElement.pb.h"

namespace Zemni
{
    ZemniScheduler::ZemniScheduler(int actor_port, uint16_t nodeActorPortStart,
                                   uint16_t nodeZmqPortStart) :
            _hasInit(false),
            _hasStart(false),
            _newNodeId(0),
            _newNodeActorPort(nodeActorPortStart),
            _newNodeZmqPort(nodeZmqPortStart),
            _actor_port(actor_port) {}

    int ZemniScheduler::getNewNodeId()
    {
        return _newNodeId.operator++();
    }

    uint16_t ZemniScheduler::getNewNodeActorPort()
    {
        return _newNodeActorPort.operator++();
    }

    uint16_t ZemniScheduler::getNewNodeZmqPort()
    {
        return _newNodeZmqPort.operator++();
    }

    string ZemniScheduler::handleMessage(int msgType, const string &msgContent)
    {
        switch (msgType)
        {
            case Zemni_protobuf::NODE_REGISTER:
            {
                return processMsgNodeRegister(msgContent);
            }
            case Zemni_protobuf::QUEUE_REQUEST:
            {
                processMsgQueueRequest(msgContent);
                return std::string();
            }
            default:
            {
                CHECK(false) << "Unknown request";
                return std::string();
            }
        }
    }

    void ZemniScheduler::init()
    {
        CHECK(!_hasInit);
        _hasInit = true;

        messageActor = CAFUtil::getInstance().getActorSystem()->spawn<ZemniSchedulerMsgActor>(*this);
        auto expected_port = CAFUtil::getInstance().getActorSystem()->middleman().publish(
                messageActor, (uint16_t) _actor_port);
        CHECK(expected_port) << "Publish message actor for Zemni ZemniScheduler failed at port " << _actor_port;
        cout << "Public message actor for Zemni ZemniScheduler at port " << _actor_port << endl;
    }

    void ZemniScheduler::start()
    {
        CHECK(_hasInit && !_hasStart);
        _hasStart = true;
    }

    string ZemniScheduler::processMsgNodeRegister(const string &msg)
    {
        Zemni_protobuf::NodeRegister registerMsg;
        registerMsg.ParseFromString(msg);
        auto proxy = make_shared<NodeProxy>();
        proxy->setId(getNewNodeId());
        proxy->setHost(registerMsg.host());
        proxy->setActor_port(getNewNodeActorPort());
        proxy->setZmq_port(getNewNodeZmqPort());
        nodeProxies.insert(std::make_pair(proxy->getId(), proxy));

        Zemni_protobuf::NodeRegisterReply replyMsg;
        replyMsg.set_id(proxy->getId());
        replyMsg.set_actor_port(proxy->getActor_port());
        replyMsg.set_zmq_port(proxy->getZmq_port());
        return replyMsg.SerializeAsString();
    }

    void ZemniScheduler::processMsgQueueRequest(const string &msg)
    {
        Zemni_protobuf::QueueRequest registerMsg;

        registerMsg.ParseFromString(msg);
        shared_ptr<QueueProxy> queueProxy;
        if (contains(queueProxies, registerMsg.queue_id()))
        {
            queueProxy = queueProxies.at(registerMsg.queue_id());
            if (registerMsg.as_consumer())
            {
                if (registerMsg.node_id() == queueProxy->getConsumer())
                    return;
            } else
            {
                if (registerMsg.node_id() == queueProxy->getProducer())
                    return;
            }
        } else
        {
            queueProxy = make_shared<QueueProxy>(registerMsg.queue_id());
            queueProxies.insert(std::make_pair(registerMsg.queue_id(), queueProxy));
        }

        int preNodeId = -1;
        if (registerMsg.as_consumer())
        {
            preNodeId = queueProxy->setConsumer(registerMsg.node_id());
        } else
        {
            preNodeId = queueProxy->setProducer(registerMsg.node_id());
        }

        if (queueProxy->hasConsumer() && queueProxy->hasProducer())
        {
            auto &producerNode = nodeProxies.at(queueProxy->getProducer());
            auto &consumerNode = nodeProxies.at(queueProxy->getConsumer());
            if (producerNode->getId() != consumerNode->getId())
            {
                Zemni_protobuf::DataStreamProducerReset dataStreamProducerReset;
                dataStreamProducerReset.set_queue_id(queueProxy->getId());
                dataStreamProducerReset.set_dst_host_id(consumerNode->getId());
                dataStreamProducerReset.set_dst_port(consumerNode->getZmq_port());
                CAFUtil::getInstance().sendMessage(producerNode->getHost(),
                                                   producerNode->getActor_port(),
                                                   Zemni_protobuf::DATASTREAM_PRODUCER_RESET,
                                                   dataStreamProducerReset.SerializeAsString());

                Zemni_protobuf::DataStreamConsumerConnect dataStreamConsumerConnect;
                dataStreamConsumerConnect.set_dst_host(producerNode->getHost());
                dataStreamConsumerConnect.set_dst_host_id(producerNode->getId());
                CAFUtil::getInstance().sendMessage(consumerNode->getHost(),
                                                   consumerNode->getActor_port(),
                                                   Zemni_protobuf::DATASTREAM_CONSUMER_CONNECT,
                                                   dataStreamConsumerConnect.SerializeAsString());
            }
        }

        if (preNodeId != -1 && preNodeId != registerMsg.node_id())
        {
            Zemni_protobuf::QueueReleaseRequest queueReleaseRequest;
            queueReleaseRequest.set_queue_id(registerMsg.queue_id());
            queueReleaseRequest.set_as_consumer(registerMsg.as_consumer());
            queueReleaseRequest.set_delete_(
                    queueProxy->getProducer() != preNodeId && queueProxy->getConsumer() != preNodeId);
            auto &preNodeProxy = nodeProxies.at(preNodeId);

            CAFUtil::getInstance().sendMessage(preNodeProxy->getHost(),
                                               preNodeProxy->getActor_port(),
                                               Zemni_protobuf::QUEUE_RELEASE_REQUEST,
                                               queueReleaseRequest.SerializeAsString());
        }
    }
}
