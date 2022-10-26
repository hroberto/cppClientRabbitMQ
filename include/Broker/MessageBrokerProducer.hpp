#ifndef  __MESSAGEBROKERPRODUCER_H__
#define  __MESSAGEBROKERPRODUCER_H__


#include "Broker/MessageBrokerBase.hpp"
#include "Broker/MessageBrokerInfo.h"
#include <memory>


namespace libapp {


class MessageBrokerProducer :  public IMessageQueueProducer, public MessageBrokerBase 
{
public:
    explicit MessageBrokerProducer();
    virtual ~MessageBrokerProducer();

    void open(const MessageBroker_Type& info);
    void publish(const MessagePublish_Type& properties);
    
};


}

#endif // __MESSAGEBROKERPRODUCER_H__