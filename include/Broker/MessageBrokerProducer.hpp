#ifndef  __MESSAGEBROKERPRODUCER_H__
#define  __MESSAGEBROKERPRODUCER_H__


#include "Broker/MessageBrokerBase.hpp"
#include "Broker/MessageBrokerInfo.h"
#include "Broker/MessagePublishProperties.h"
#include <memory>


namespace libapp {


class MessageBrokerProducer :  public IMessageQueueProducer, public MessageBrokerBase 
{
public:
    explicit MessageBrokerProducer();
    virtual ~MessageBrokerProducer();


    void open(const struct MessageBrokerInfo_T& info);
    void publish(const MessagePublishProperties& properties);
    
};


}

#endif // __MESSAGEBROKERPRODUCER_H__