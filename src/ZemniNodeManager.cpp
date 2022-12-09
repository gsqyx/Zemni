#include <utility>

#include <glog/logging.h>
#include "ZemniNodeManager.h"
#include "util/CAFUtil.h"
#include "message/Queue.pb.h"
#include "Config.h"
#include "message/ZemniNodeManagerMsgActor.h"
#include "Queue_moodycamel.h"
#include "Queue_folly.h"

namespace Zemni
{
    ZemniNodeManager::ZemniNodeManager(string hostname, string scheduler_host, uint16_t scheduler_port) :
            id(-1),
            hostname(std::move(hostname)),
            _hasInit(false),
            _hasStart(false),
            _isRunning(false),
            lock("Zemni::ZemniNodeManager"),
            networkManager(*this),
            scheduler_host(std::move(scheduler_host)),
            scheduler_port(scheduler_port)
    {
#ifdef QUEUE_TYPE_MOODYCAMEL
        DLOG(INFO) << "create Zemni::ZemniNodeManager hostname=" << this->hostname << " queue=MoodyCamel";
#else
        DLOG(INFO) << "create Zemni::ZemniNodeManager hostname=" << this->hostname << " queue=Folly";
#endif
    }

    void ZemniNodeManager::init()
    {
        CHECK(!_hasInit);
        _hasInit = true;

        schedulerActor = CAFUtil::getInstance().spawnRemoteActor(scheduler_host, scheduler_port);
        Zemni_protobuf::NodeRegister register_msg;
        register_msg.set_host(hostname);
        auto res = CAFUtil::getInstance().sendMessage(schedulerActor,
                                                      Zemni_protobuf::NODE_REGISTER,
                                                      register_msg.SerializeAsString());
        Zemni_protobuf::NodeRegisterReply register_res_msg;
        CHECK(register_res_msg.ParseFromString(res)) << "Parse register reply message fail.";
        this->id = register_res_msg.id();
        this->actor_port = static_cast<uint16_t>(register_res_msg.actor_port());
        this->zmq_port = static_cast<uint16_t>(register_res_msg.zmq_port());
        this->networkManager.setReceiverZmqPort(this->zmq_port);

        DLOG(INFO) << "Zemni::ZemniNodeManager " << this->id << " reg success url=" << hostname << ":" << actor_port;

        messageActor = CAFUtil::getInstance().getActorSystem()->spawn<ZemniNodeManagerMsgActor>(*this);
        CHECK(CAFUtil::getInstance().getActorSystem()->middleman().publish(messageActor, actor_port))
        << "Zemni msgActor publish failed port=" << actor_port;

        networkManager.init();
    }

    void ZemniNodeManager::start()
    {
        CHECK(_hasInit && !_hasStart && !_isRunning);
        _hasStart = true;
        _isRunning = true;
        std::thread([&]()
                    {
                        while (_isRunning)
                        {
                            sleep(QUEUE_MANAGER_LENGTH_DETECT_INTERVAL);
                            //todo uncomment this
                            for (auto &ptr : queueInfos)
                            {
                                ptr.second.getQueue()->addLengthToAvgCounter();
                            
                        }
                    }).detach();
        networkManager.start();
    }

    void ZemniNodeManager::stop()
    {
        _isRunning = false;
    }

    string ZemniNodeManager::handleMessage(int msgType, const string &msgContent)
    {
        switch (msgType)
        {
            case Zemni_protobuf::DATASTREAM_PRODUCER_RESET:
            {
                processMsgDataStreamProducerReset(msgContent);
                return std::string();
            }
            case Zemni_protobuf::QUEUE_RELEASE_REQUEST:
            {
                processMsgQueueReleaseRequest(msgContent);
                return std::string();
            }
            case Zemni_protobuf::DATASTREAM_CONSUMER_CONNECT:
            {
                processMsgDataStreamConsumerConnect(msgContent);
                return std::string();
            }
            default:
            {
                cerr << "UNKNOWN MESSAGE EXCEPTION: type=" << msgType << " content=" << msgContent << endl;
            }
        }
        return std::string();
    }

    bool ZemniNodeManager::containsQueue(int queue_id)
    {
        return contains(queueInfos, queue_id);
    }

    double ZemniNodeManager::getAvgQueueLength(int queue_id)
    {
        return queueInfos.at(queue_id).getQueue()->getAvg() / MPMC_QUEUE_BUFFER_SIZE;
    }

    double ZemniNodeManager::getAvgQueueLength(vector<int> &queue_ids)
    {
        double max = 0;

        for (int id : queue_ids)
        {
            double res = getAvgQueueLength(id);
            DLOG(INFO) << "QUEUE id=" << id << "\tlen=" << res;
            if (res > max)
                max = res;
        }
        return max;
    }

    shared_ptr<Queue> ZemniNodeManager::blockingGetLocalQueue(int queue_id)
    {
        int remain_count = 10;
        while (remain_count-- > 0 && !contains(queueInfos, queue_id))
        {
            sleep(1);
        }
        CHECK(contains(queueInfos, queue_id)) << "Get queue " << queue_id << " failed.";
        return queueInfos.at(queue_id).getQueue();
    }

    QueuePtr ZemniNodeManager::getQueue(int queue_id, bool asConsumer)
    {
        SpinLock::Guard _(lock);

        if (!contains(queueInfos, queue_id))
        {
#ifdef QUEUE_TYPE_MOODYCAMEL
            queueInfos.emplace(queue_id, make_shared<Queue_moodyCamel>(queue_id));
#else
            queueInfos.emplace(queue_id, make_shared<Queue_folly>(queue_id));
#endif
        }

        if (asConsumer)
        {
            networkManager.registerQueueAsConsumer(queueInfos.at(queue_id).getQueue());
        }
        Zemni_protobuf::QueueRequest requestMsg;
        requestMsg.set_node_id(id);
        requestMsg.set_queue_id(queue_id);
        requestMsg.set_as_consumer(asConsumer);
        string res = CAFUtil::getInstance().sendMessage(scheduler_host,
                                                        scheduler_port,
                                                        Zemni_protobuf::QUEUE_REQUEST,
                                                        requestMsg.SerializeAsString());


//        if (!asConsumer)
//        {
//            queueInfos[queue_id].second.connect(listener);
//        }
        if (asConsumer && queueInfos.at(queue_id).getQueue()->getType() == QUEUE_NETWORK_SENDER)
        {
            QueuePtr new_queue;
#ifdef QUEUE_TYPE_MOODYCAMEL
            new_queue = make_shared<Queue_moodyCamel>(queue_id);
#else
            new_queue = make_shared<Queue_folly>(queue_id);
#endif
            queueInfos.at(queue_id).getQueue().swap(new_queue);
            queueInfos.at(queue_id).getSignal().operator()(queueInfos.at(queue_id).getQueue());

            networkManager.disConnectDataStream(queue_id, false);
        }
        return queueInfos.at(queue_id).getQueue();
    }

    QueuePtr ZemniNodeManager::getQueueForProducer(int queue_id, ResetQueueCallbackSlot slot)
    {
        auto res = getQueue(queue_id, false);
        getResetSignal(queue_id).connect(slot);
        return res;
    }

    QueuePtr ZemniNodeManager::getQueueForConsumer(int queue_id)
    {
        return getQueue(queue_id, true);
    }

    void ZemniNodeManager::processMsgDataStreamProducerReset(const string &msgContent)
    {
        Zemni_protobuf::DataStreamProducerReset requestMsg;
        requestMsg.ParseFromString(msgContent);
        if (requestMsg.dst_host_id() != id)
        {
            networkManager.registerQueueAsProducer(
                    requestMsg.queue_id(),
                    static_cast<uint16_t>(requestMsg.dst_port()),
                    requestMsg.dst_host_id());
        }
    }

    void ZemniNodeManager::processMsgDataStreamConsumerConnect(const string &msgContent)
    {
        Zemni_protobuf::DataStreamConsumerConnect requestMsg;
        requestMsg.ParseFromString(msgContent);
        if (requestMsg.dst_host_id() != id)
        {
            networkManager.connectToProducer(requestMsg.dst_host());
        }
    }

    void ZemniNodeManager::processMsgQueueReleaseRequest(const string &msgContent)
    {
        Zemni_protobuf::QueueReleaseRequest requestMsg;
        requestMsg.ParseFromString(msgContent);

        if (!contains(queueInfos, requestMsg.queue_id()))
            return;
        if (requestMsg.delete_())
            queueInfos.erase(requestMsg.queue_id());
        networkManager.disConnectDataStream(requestMsg.queue_id(), requestMsg.as_consumer());
    }

    boost::signals2::signal<void(QueuePtr)> &ZemniNodeManager::getResetSignal(int queue_id)
    {
        CHECK(contains(queueInfos, queue_id));
        return queueInfos.at(queue_id).getSignal();
    }
}
