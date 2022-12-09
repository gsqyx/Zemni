#pragma once

#include <utility>
#include "../Common.h"

namespace Zemni
{
    class DataStream
    {
    private:
        int queueId;
        string dst_host;
        uint16_t dst_port;
        int dst_host_id;
        bool _isConnectAsConsumer;

    public:
        DataStream(int queueId, string dst_host, uint16_t dst_port, int dst_host_id, bool asConsumer) :
                queueId(queueId),
                dst_host(std::move(dst_host)),
                dst_port(dst_port),
                dst_host_id(dst_host_id),
                _isConnectAsConsumer(asConsumer) {}

        DataStream(int queueId, string dst_host, uint16_t dst_port, int dst_host_id) :
                queueId(queueId),
                dst_host(std::move(dst_host)),
                dst_port(dst_port),
                dst_host_id(dst_host_id),
                _isConnectAsConsumer(false) {}

        int getQueueId() const { return queueId; }

        bool isConnectAsConsumer() const { return _isConnectAsConsumer; }

        const string &getDst_host() const
        {
            return dst_host;
        }

        uint16_t getDst_port() const
        {
            return dst_port;
        }

        int getDst_host_id() const
        {
            return dst_host_id;
        }
    };
}
