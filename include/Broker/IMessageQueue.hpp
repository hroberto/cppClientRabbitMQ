
#ifndef __IMESSAGEQUEUE_H__
#define __IMESSAGEQUEUE_H__


#include "Broker/MessageWrapper.hpp"
#include "Broker/QueueProperties.h"
#include "Broker/MessageBrokerInfo.h"
#include "Broker/MessagePublishProperties.h"

#include <memory>


namespace libapp {


class IMessageQueueConsumer {
public:
    virtual void open(const struct MessageBrokerInfo_T& info) = 0;

    virtual std::shared_ptr<MessageWrapper> read() = 0;
    virtual void ack_message(const MessageWrapper& wrapper) = 0;
};


class IMessageQueueProducer {
public:
    virtual void open(const struct MessageBrokerInfo_T& info) = 0;
    virtual void publish(const MessagePublishProperties& properties) = 0;
};

}

#endif // __IMESSAGEQUEUE_H__