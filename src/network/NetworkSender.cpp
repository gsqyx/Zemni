#include "NetworkSender.h"
#include "../Queue_network_sender.h"

namespace Zemni
{
    NetworkSender::NetworkSender(zmq::context_t &zmqContext) :
            _hasInit(false),
            _hasStart(false),
            _isRunning(false),
            zmqContext(zmqContext),
            lock("Zemni::NetworkSender::lock")
    {
        senders.push_back(nullptr);
        sender_to_node_id.push_back(0);
        memset(&queue_id_to_sender_index, 0, sizeof(queue_id_to_sender_index));
    }

    NetworkSender::~NetworkSender()
    {
        DLOG(ERROR) << "Deconstruct NetworkSender";
        for (auto &sender : senders)
        {
            if (sender != nullptr)
            {
                sender->close();
                sender = nullptr;
            }
        }
        zmqContext.close();
    }

    void
    NetworkSender::registerNetworkQueueSender(const shared_ptr<Queue_network_sender> &queue, int node_id, uint16_t port)
    {
        if (queue_id_to_sender_index[queue->getId()] != 0)
        {
            queue->setSender(this);
            return;
        }
        SpinLock::Guard _(lock);

        for (size_t index = 1; index < sender_to_node_id.size(); ++index)
        {
            if (sender_to_node_id[index] == node_id)
            {
                queue_id_to_sender_index[queue->getId()] = index;
                queue->setSender(this);
                return;
            }
        }

        senders.emplace_back(new zmq::socket_t(zmqContext, ZMQ_PUSH));
        sender_to_node_id.push_back(node_id);
        queue_id_to_sender_index[queue->getId()] = senders.size() - 1;
        stringstream url;
        url << "tcp://*:" << port;
        senders.back()->bind(url.str());
        senders.back()->setsockopt(ZMQ_RCVTIMEO, ZEMNI_NETWORK_SECONDS_TIMEOUT);
        DLOG(INFO) << "NetworkSender bind node " << node_id << " @ " << url.str();
        queue->setSender(this);
    }

    void NetworkSender::unregisterQueue(int queue_id)
    {
        SpinLock::Guard _(lock);
        queue_id_to_sender_index[queue_id] = 0;
    }

    void NetworkSender::init()
    {
        CHECK(!_hasInit);
        _hasInit = true;

        for (auto &runnable : runnables)
            std::thread(&NetworkSenderRunnable::operator(), runnable.get()).detach();

        std::thread([&]()
                    {
                        while (true)
                        {
                            sleep(5);
                            for (size_t i = 0; i < queues_size; ++i)
                            {
                                if (queues_active[i])
                                {
                                    DLOG(ERROR) << "NetworkSender Queue: id=" << queues[i]->getId()
                                                << "\tLen="
                                                << (double) queues[i]->sizeGuess() / MPMC_QUEUE_BUFFER_SIZE;
                                    DLOG(ERROR) << " ";
                                }
                            }
                        }
                    }).detach();
    }
}
