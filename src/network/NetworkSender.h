#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#ifndef ZEMNI_NETWORKSENDER_H
#define ZEMNI_NETWORKSENDER_H

#include "../Common.h"
#include "../cppzmq/zmq.hpp"
#include "../Queue.h"
#include "../message/StreamElement.pb.h"
#include "../util/spinlock/SpinLock.h"
#include <folly/MPMCQueue.h>
#include <snappy.h>
#include <atomic>
#include "NetworkSenderCacheContent.h"
#include <folly/synchronization/MicroSpinLock.h>
#include <folly/SpinLock.h>
#include <folly/SharedMutex.h>

namespace Zemni
{
    using Zemni_protobuf::StreamElement;

    class Queue_network_sender;

    class NetworkSender
    {
    private:

        bool _hasInit;
        bool _hasStart;
        bool _isRunning;
        zmq::context_t &zmqContext;
        vector<zmq::socket_t *> senders;
        vector<int> sender_to_node_id;
//        std::unordered_map<int, int> queue_id_to_node_id;
//        int sender_to_node_id[100];
        size_t queue_id_to_sender_index[10000];
//        folly::MPMCQueue<StreamElement> *queue;
//        shared_ptr<Queue> queues[10000];
//        vector <shared_ptr<Queue>> queues;
//        std::atomic_bool queues_active[10000];
//        std::atomic_int queues_size;
//        std::set<int> queue_ids;
//        std::vector<std::shared_ptr<NetworkSenderRunnable>> runnables;
//        folly::MPMCQueue<NetworkSenderCacheContent> cacheQueue;
//        folly::SpinLock sender_lock;
        folly::SharedMutex sender_lock;
        SpinLock lock;

    public:
        explicit NetworkSender(zmq::context_t &zmqContext);

        NetworkSender(const NetworkSender &) = delete;

        NetworkSender(NetworkSender &&) = delete;

        ~NetworkSender();

//        bool hasQueue(int queue_id)
//        {
////            for (int i = 0; i < queues_size; ++i)
////            {
////                if (queues_active[i] && queues[i]->getId() == queue_id)
////                {
////                    return true;
////                }
////            }
////            return false;
//        }

//        void addQueue(const shared_ptr<Queue> &queue)
//        {
//            for (int i = 0; i < queues_size; ++i)
//            {
//                if (!queues_active[i])
//                {
//                    queues[i] = queue;
//                    queues_active[i] = true;
////                    for (auto &runnable : runnables)
////                    {
////                        runnable->setQueue(queue, i);
////                    }
//                    return;
//                }
//            }
//            queues[queues_size] = queue;
//            queues_active[queues_size] = true;
////            for (auto &runnable : runnables)
////            {
////                runnable->setQueue(queue, queues_size);
////                runnable->incQueueSize();
////            }
//            ++queues_size;
//        }

//        void registerQueueToNode(int queue_id, int node_id, const string &host, uint16_t port)
        void registerNetworkQueueSender(const shared_ptr<Queue_network_sender> &queue, int node_id, uint16_t port);

        void unregisterQueue(int queue_id);

        void init();


//        void start()
//        {
//            CHECK(_hasInit);
//            CHECK(!_hasStart);
//            CHECK(!_isRunning);
//            _hasStart = true;
//            _isRunning = true;
//
//            string element;
////            string serializedString;
//
//#ifdef ENABLE_SNAPPY
//            DLOG(INFO) << "Zemni::DataStreamProducer::Enable snappy";
//            string compressedString;
//#endif
//
////            for (int i = 0; i < 10; i++)
////            {
////                std::thread([&]()
////                            {
////                                while (true)
////                                {
////                                    sleep(5);
////                                    string element;
////                                    for (size_t index = 0; index < queues_size; ++index)
////                                    {
////                                        if (queues_active[index])
////                                        {
////                                            static_cast<Queue_network_sender *>(queues[index].get())->getQueue().blockingRead(
////                                                    element);
////                                        }
////                                    }
////                                }
////                            }).detach();
////            }
//
//            int index = queues_size;
////            NetworkSenderCacheContent content;
//            while (true)
//            {
////                    cacheQueue.blockingRead(content);
////                    senders.at(content.sender_index)->send(content.content.data(), content.content.length());
//                if (queues_size == 0)
//                {
//                    sleep(1);
//                    continue;
//                }
//                if (index == 0)
//                {
//                    index = queues_size;
//                }
//                --index;
//                if (!queues_active[index])
//                {
//                    continue;
//                }
//                try
//                {
////                        if (!queues[i]->tryRead(element))
////                            continue;
////                    if (queues[index].get()->getId() != 3 && queues[index].get()->getId() != 2)
//                    if (!static_cast<Queue_network_sender *>(queues[index].get())->getQueue().readIfNotEmpty(
//                            element))
//                        continue;
//
////                        element.set_queue_id(queues[i]->getId());
//
////                        if (element.type() == Zemni_protobuf::STREAM_ELEMENT_TYPE_OPERATOR_STOP_SIGNAL)
////                        {
////                            _isRunning = false;
////                            return;
////                        }
//
////#ifdef ENABLE_DEBUG
////                        if (element.getElementType() == Zemni_protobuf::STREAM_ELEMENT_TYPE_TIMESTAMP)
////                        {
////                            time_t _time = getNtpMillisecondTimestamp();
////                            stringstream ss;
////                            ss << element.second.getValue();
////                            ss << "DataStreamProducer:\t\t" << _time - atol(_element.value2.c_str()) << "\n";
////                            _element.setValue(ss.str());
////                            _element.setValue2(std::to_string(_time));
////                        }
////#endif
////                        element.SerializeToString(&serializedString);
////                    if (queue_id_to_sender_index[queues[index]->getId()] <= 0)
////                    {
////                        DLOG(FATAL) << "queue_id_to_node_id not contains queue_id";
////                        continue;
////                    }
//
//#ifdef ENABLE_SNAPPY
//                    snappy::Compress(serializedString.data(), serializedString.size(), &compressedString);
//                    senders.at(queue_id_to_node_id.at(element.first))->send(compressedString.data(), compressedString.length());
//#else
////                        senders.at(queue_id_to_sender_index[queues[i]->getId()])->send(
////                                serializedString.data(), serializedString.length());
//                    senders.at(queue_id_to_sender_index[queues[index]->getId()])->send(
//                            element.data(), element.length());
//#endif
//                } catch (...)
//                {
//                    DLOG(FATAL) << handle_eptr(std::current_exception());
//                }
//            }
//        }

        inline void send(int queue_id, const void *data, size_t length)
        {
            try
            {
                if (queue_id_to_sender_index[queue_id] <= 0)
                {
                    DLOG(FATAL) << "queue_id_to_node_id not contains queue_id";
                    return;
                }
                std::lock_guard<folly::SharedMutex> _(sender_lock);
                senders.at(queue_id_to_sender_index[queue_id])->send(data, length);
            } catch (...)
            {
                DLOG(FATAL) << handle_eptr(std::current_exception());
            }
        }
    };
}


#endif //ZEMNI_NETWORKSENDER_H

#pragma clang diagnostic pop