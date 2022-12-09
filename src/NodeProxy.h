#pragma once

#include <ostream>
#include "Common.h"

namespace Zemni
{
    class NodeProxy
    {
    private:
        int id;
        string host;
        uint16_t actor_port;
        uint16_t zmq_port;
        time_t latestTimestamp;

    public:
        NodeProxy() :
                id(-1),
                actor_port(0),
                zmq_port(0),
                latestTimestamp(getMillisecondTimestamp()) {}

    public:
        int getId() const { return id; }

        void setId(int id) { NodeProxy::id = id; }

        const string &getHost() const { return host; }

        void setHost(const string &host) { NodeProxy::host = host; }

        uint16_t getActor_port() const
        {
            return actor_port;
        }

        void setActor_port(uint16_t actor_port)
        {
            NodeProxy::actor_port = actor_port;
        }

        uint16_t getZmq_port() const
        {
            return zmq_port;
        }

        void setZmq_port(uint16_t zmq_port)
        {
            NodeProxy::zmq_port = zmq_port;
        }

        friend std::ostream &operator<<(std::ostream &os, const NodeProxy &proxy)
        {
            os << "id: " << proxy.id
               << " host: " << proxy.host
               << " actor_port: " << proxy.actor_port
               << " zmq_port: " << proxy.zmq_port
               << " latestTimestamp: " << proxy.latestTimestamp;
            return os;
        }

        void updateTimestamp() { this->latestTimestamp = getMillisecondTimestamp(); }
    };
}
