#ifndef  __MESSAGEBROKERCONSUMER_H__
#define  __MESSAGEBROKERCONSUMER_H__

#include "Broker/MessageBrokerBase.hpp"

#include <memory>

namespace libapp {
   
class MessageBrokerConsumer :  public IMessageQueueConsumer, public MessageBrokerBase 
{
public:
    explicit MessageBrokerConsumer();
    virtual ~MessageBrokerConsumer();

    void open(const MessageBroker_Type& info);

    std::shared_ptr<MessageWrapper> read();
    void ack_message(const std::shared_ptr<MessageWrapper> wrapper_shr);
};

}


#endif // __MESSAGEBROKERCONSUMER_H__