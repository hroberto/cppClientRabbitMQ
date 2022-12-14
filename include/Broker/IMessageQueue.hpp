
#ifndef __IMESSAGEQUEUE_H__
#define __IMESSAGEQUEUE_H__


#include "Broker/MessageWrapper.hpp"
#include "Broker/QueueProperties.h"
#include "Broker/MessageBrokerInfo.h"

#include <memory>


namespace libapp {


class IMessageQueueConsumer {
public:
    virtual void open(const MessageBroker_Type& info) = 0;

    virtual std::shared_ptr<MessageWrapper> read() = 0;
    virtual void ack_message(const std::shared_ptr<MessageWrapper>) = 0;
};


class IMessageQueueProducer {
public:
    virtual void open(const MessageBroker_Type& info) = 0;
    virtual void publish(const MessagePublish_Type& properties) = 0;
};

}

#endif // __IMESSAGEQUEUE_H__