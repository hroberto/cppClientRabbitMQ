#ifndef __MessageBroker_H__
#define __MessageBroker_H__


#include "Broker/IMessageQueue.hpp"
#include "Broker/MessageWrapper.hpp"
#include "Broker/QueueProperties.h"
#include "Broker/MessageBrokerInfo.h"
#include "Broker/MessagePublishProperties.h"

#include <memory>

namespace libapp {
   

class MessageBrokerBase {
public:
    explicit MessageBrokerBase() {}
    virtual ~MessageBrokerBase();

    virtual void open(const MessageBroker_Type& info) = 0;

    // void publish(const MessagePublishProperties& properties);
    // std::shared_ptr<MessageWrapper> read();
    // void ack_message(const MessageWrapper& wrapper);
    // void ack_message(const std::shared_ptr<MessageWrapper> wrapper_shr);


    const MessageBroker_Type&  getBrokerInfo() const& { return info_; }
    
protected:
    int open_socket();
    void exchange_declare(const MessageBroker_Type& info);
    int prepare();
    void prepare_consumer();

    MessageBroker_Type info_;
    
    amqp_connection_state_t conn_ {};
    amqp_socket_t *socket_{ NULL };
    // std::string queueNameConsumer_;

    amqp_basic_consume_ok_t *consume_ok_{ nullptr };
    QueueProperties properties_;

    
};

}

#endif  // __MessageBroker_H__