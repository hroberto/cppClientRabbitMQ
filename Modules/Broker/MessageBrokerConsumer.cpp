#include "Broker/MessageBrokerConsumer.hpp"

#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
#include <cassert>
#include <iostream>

#include "Broker/utils.h"
#include "Broker/MessageBrokerInfo.h"


namespace libapp {


MessageBrokerConsumer::MessageBrokerConsumer()
{
}


MessageBrokerConsumer::~MessageBrokerConsumer()
{
}


void MessageBrokerConsumer::open(const MessageBroker_Type& info) 
{
    info_ = info;

    open_socket();
    exchange_declare( info );
    prepare();
    prepare_consumer();
}


/**
 * @brief 
 * 
 * @return std::shared_ptr<MessageWrapper> 
 */
std::shared_ptr<MessageWrapper> MessageBrokerConsumer::read() 
{
    amqp_rpc_reply_t res;
    amqp_envelope_t envelope;

    amqp_maybe_release_buffers(conn_);

    res = amqp_consume_message(conn_, &envelope, NULL, 0);

    if (AMQP_RESPONSE_NORMAL != res.reply_type) {
        return NULL;
    }

    std::shared_ptr<MessageWrapper> wrapper_shr = std::make_shared<MessageWrapper>( std::move( envelope ) );
    return wrapper_shr;
}


/**
 * @brief  Informa o recebimento da mensagem para o Broker
 * 
 * @param envelope 
 */
void MessageBrokerConsumer::ack_message(const MessageWrapper& wrapper) {
    amqp_basic_ack( conn_, info_.channel, wrapper->delivery_tag, false );
}

void MessageBrokerConsumer::ack_message(const std::shared_ptr<MessageWrapper> wrapper_shr) {
    amqp_basic_ack( conn_, info_.channel, (*wrapper_shr)->delivery_tag, false );
}

}
