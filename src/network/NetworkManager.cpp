#include "NetworkManager.h"
#include <thread>
#include <utility>
#include "../Config.h"
#include "../ZemniNodeManager.h"
#include "../message/StreamElement.pb.h"
#include "../Queue_network_sender.h"

namespace Zemni
{
    NetworkManager::NetworkManager(ZemniNodeManager &queueNodeManager) :
            _hasInit(false),
            _hasStart(false),
            queueNodeManager(queueNodeManager),
            zmqContext(ZMQ_PARALLELISM),
            networkSender(zmqContext)
//            networkReceiver(zmqContext),
//            queue(NETWORK_QUEUE_BUFFER_SIZE)
    {
        for (int i = 0; i < 1; i++)
            networkReceivers.push_back(std::make_shared<NetworkReceiver>(zmqContext));
//        networkSender.setQueue(&queue);
    }

    void NetworkManager::disConnectDataStream(int queueId, bool asConsumer)
    {
        if (asConsumer)
        {
            for (auto &networkReceiver:   networkReceivers)
                networkReceiver->removeQueue(queueId);
        } else
        {
            networkSender.unregisterQueue(queueId);
        }
    }

    void NetworkManager::init()
    {
        CHECK(!_hasInit);
        _hasInit = true;
    }

    void NetworkManager::start()
    {
        CHECK(!_hasStart);
        _hasStart = true;

//#ifdef ENABLE_DEBUG
//        std::thread([](folly::MPMCQueue<StreamElement> *queue)
//                    {
//                        while (true)
//                        {
//                            sleep(5);
//                            DLOG(ERROR) << "NetworkSender queueLen="
//                                        << (double) queue->sizeGuess() / NETWORK_QUEUE_BUFFER_SIZE;
//                        }
//                    }, &queue).detach();
//#endif

        std::thread([&]()
                    {
                        try
                        {
                            networkSender.init();
//                            networkSender.start(); //todo uncommnet this;
                        }
                        catch (const std::exception &exc)
                        {
                            // catch anything thrown within try block that derives from std::exception
                            DLOG(FATAL) << exc.what();
                        }
                        catch (...)
                        {
                            DLOG(FATAL) << handle_eptr(std::current_exception());
                        }
                    }).detach();
        for (auto &networkReceiver : networkReceivers)
        {
            std::thread([&](shared_ptr<NetworkReceiver> networkReceiver)
                        {
                            try
                            {
                                networkReceiver->init();
                                networkReceiver->start();
                            }
                            catch (const std::exception &exc)
                            {
                                // catch anything thrown within try block that derives from std::exception
                                DLOG(FATAL) << exc.what();
                            }
                            catch (...)
                            {
                                DLOG(FATAL) << handle_eptr(std::current_exception());
                            }
                        }, networkReceiver).detach();
        }
    }

    void NetworkManager::setReceiverZmqPort(uint16_t port)
    {
        for (auto &networkReceiver : networkReceivers)
            networkReceiver->setPort(port);
    }

    void NetworkManager::registerQueueAsConsumer(shared_ptr<Queue> queue)
    {
        for (auto &networkReceiver : networkReceivers)
            networkReceiver->addQueue(queue);
    }

    void NetworkManager::registerQueueAsProducer(int queue_id, uint16_t dst_port, int dst_host_id)
    {
        QueuePtr old_queue = queueNodeManager.blockingGetLocalQueue(queue_id);
//        networkSender.registerQueueToNode(old_queue, dst_host_id, dst_port);

        auto queue_network_sender = std::static_pointer_cast<Queue>(
                make_shared<Queue_network_sender>(queue_id));
        queueNodeManager.queueInfos.at(queue_id).getQueue().swap(queue_network_sender);

        auto new_queue = queueNodeManager.queueInfos.at(queue_id).getQueue();
//        DLOG(INFO) << "call queueListener id=" << queue_id;

        queueNodeManager.queueInfos.at(queue_id).getSignal().operator()(new_queue); // todo test

        networkSender.registerNetworkQueueSender(
                std::static_pointer_cast<Queue_network_sender>(new_queue), dst_host_id, dst_port);

        //todo uncomment this
        std::thread([](QueuePtr _old_queue, QueuePtr _new_queue)
                    {
                        try
                        {
                            StreamElements ele;
                            while (!_old_queue->isEmpty())
                            {
                                _old_queue->blockingRead(ele);
                                _new_queue->blockingWrite(std::move(ele));
                            }
                        } catch (...)
                        {
                            DLOG(FATAL) << handle_eptr(std::current_exception());
                        }
                    }, old_queue, new_queue).detach();
    }

    void NetworkManager::unregisterQueueAsProducer(int queue_id)
    {
        networkSender.unregisterQueue(queue_id);
    }

    void NetworkManager::connectToProducer(string host)
    {
        for (auto &networkReceiver : networkReceivers)
            networkReceiver->connect(host);
    }
}  // namespace Zemni
