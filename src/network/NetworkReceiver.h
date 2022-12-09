#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#ifndef ZEMNI_NETWORKRECEIVER_H
#define ZEMNI_NETWORKRECEIVER_H

#include "../Common.h"
#include "../cppzmq/zmq.hpp"
#include "../Queue.h"
#include "../message/StreamElement.pb.h"
#include "../util/spinlock/SpinLock.h"

namespace Zemni
{
    class NetworkReceiver
    {
    private:
        bool _hasInit;
        bool _hasStart;
        bool _isRunning;
        bool __isRunning;
        uint16_t port;
        zmq::context_t &zmqContext;
        zmq::socket_t receiver;
        std::set<string> receiverConnected;
//        shared_ptr<Queue> outQueue;
        SpinLock lock;
        std::vector<shared_ptr < Queue>> queues; // index start with 1
        int queues_size;
        size_t queue_index[10000];
//        folly::MPMCQueue<string> cacheQueue;

    public:
        explicit NetworkReceiver(zmq::context_t &zmqContext) :
                _hasInit(false),
                _hasStart(false),
                _isRunning(false),
                __isRunning(false),
//                port(0),
                zmqContext(zmqContext),
                receiver(zmqContext, ZMQ_PULL),
                lock("Zemni::NetworkReceiver::lock"),
                queues_size(0)
//                cacheQueue(40960)
        {
            queues.push_back(nullptr);
            queues_size = static_cast<int>(queues.size());
            memset(&queue_index, 0, sizeof(queue_index));
        }

        NetworkReceiver(const NetworkReceiver &) = delete;

        NetworkReceiver(NetworkReceiver &&) = delete;

        ~NetworkReceiver()
        {
            receiver.close();
        }

        void connect(const string &host)
        {
            {
                SpinLock::Guard _(lock);
                if (contains(receiverConnected, host))
                    return;
                receiverConnected.insert(host);
            }
            stringstream url;
            url << "tcp://" + host + ":" << port;
            DLOG(INFO) << "NetworkReceiver connect url:" << url.str();
            receiver.connect(url.str());
        }

        void disconnect(const string &host)
        {
            if (!contains(receiverConnected, host))
                return;
            stringstream url;
            url << "tcp://" + host + ":" << port;
            DLOG(INFO) << "NetworkReceiver disconnect url:" << url.str();
            receiver.disconnect(url.str());
        }

        void init()
        {
            CHECK(!_hasInit);
            _hasInit = true;
            for (int i = 0; i < 8; ++i)
            {
                std::thread([&]()
                            {
                                zmq::message_t message;
                                zmq::socket_t _in_receiver(zmqContext, ZMQ_PULL);
                                _in_receiver.connect("inproc://networkreceiver");
                                while (true)
                                {
                                    if (!_in_receiver.recv(&message))
                                        continue;
                                    StreamElements elements;

#ifdef ENABLE_SNAPPY
                                    snappy::Uncompress(static_cast<char *>(message.data()), message.size(), &uncompressedString);
                                    element.ParseFromString(uncompressedString);
#else
                                    elements.ParseFromArray(message.data(), message.size());
#endif

#ifdef ENABLE_DEBUG

                                    if (element.getElementType() == Zemni::STREAM_ELEMENT_TYPE_TIMESTAMP)
                        {
                            time_t _time = getNtpMillisecondTimestamp();
                            stringstream ss;
                            ss << element.getValue();
                            ss << "DataStreamConsumer:\t\t" << _time - atol(element.value2.c_str()) << "\n";
                            element.setValue(ss.str());
                            element.setValue2(std::to_string(_time));
                        }
#endif
                                    size_t _queue_index = queue_index[elements.queue_id()];
//                    if (_queue_index <= 0 || _queue_index >= queues_size)
//                    {
//                        DLOG(FATAL) << element.queue_id() << " not set"; //todo
//                        continue;
//                    }
                                    if (queues.at(_queue_index) == nullptr)
                                    {
                                        DLOG(ERROR) << "queue id == null";
                                        continue;
                                    }
                                    queues.at(_queue_index)->blockingWrite(std::move(elements));
                                }
                            }).detach();
            }
        }

        void start()
        {
            CHECK(_hasInit);
            CHECK(!_hasStart);
            CHECK(!_isRunning);
            _hasStart = true;
            _isRunning = true;
            __isRunning = true;

#ifdef ENABLE_SNAPPY
            DLOG(INFO) << "Zemni::NetworkReceiver::Enable snappy";
            string uncompressedString;
#endif
            StreamElement element;
            zmq::message_t message;
            zmq::socket_t _in_sender(zmqContext, ZMQ_PUSH);
            _in_sender.bind("inproc://networkreceiver");
            while (__isRunning)
            {
                try
                {
                    if (!receiver.recv(&message))
                        continue;
                    _in_sender.send(message);
                } catch (...)
                {
                    DLOG(FATAL) << handle_eptr(std::current_exception());
                }
            }
            receiver.setsockopt(ZMQ_LINGER, 0);
            receiver.close();
            zmqContext.close();
            _isRunning = false;
        }

        void stop()
        {
            __isRunning = false;
        }

        bool isRunning()
        {
            return _isRunning;
        }

        void addQueue(const shared_ptr <Queue> &queue)
        {
            SpinLock::Guard _(lock);
            if (queue_index[queue->getId()] > 0)
            {
                for (auto &_queue : queues)
                {
                    if (queue->getId() == _queue->getId())
                        _queue = queue;
                }
            } else
            {
                queues.push_back(queue);
                queues_size = static_cast<int>(queues.size());
                queue_index[queue->getId()] = queues_size - 1;
//                DLOG(INFO) << "add queue " << queue->getId()
//                           << " ,queue_index[" << queue->getId() << "]=" << queue_index[queue->getId()]
//                           << " queue_ptr=" << queues.back().get();
            }
        }

        void removeQueue(int queue_id)
        {
            size_t _index = queue_index[queue_id];
            queue_index[queue_id] = 0;
            queues[_index] = nullptr;
        }

        void setPort(uint16_t port)
        {
            NetworkReceiver::port = port;
        }
    };
}


#endif //ZEMNI_NETWORKRECEIVER_H

#pragma clang diagnostic pop