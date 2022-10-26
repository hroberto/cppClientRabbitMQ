#include "Broker/MessageBrokerProducer.hpp"

#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
#include <cassert>
#include <iostream>

#include "Broker/utils.h"
#include "Broker/MessageBrokerInfo.h"


namespace libapp {

MessageBrokerProducer::MessageBrokerProducer()
    : MessageBrokerBase()
{
}


MessageBrokerProducer::~MessageBrokerProducer()
{
}


void MessageBrokerProducer::open(const MessageBroker_Type& info) 
{
    info_ = info;

    open_socket();
    exchange_declare( info );
    // prepare();
}



void MessageBrokerProducer::publish(const MessagePublish_Type& info) 
{
    amqp_basic_properties_t props;

    props._flags = AMQP_BASIC_DELIVERY_MODE_FLAG;
    props.delivery_mode = info.property.delivery_mode;

    if (!info.message.content_type.empty()) {
        props._flags |= AMQP_BASIC_CONTENT_TYPE_FLAG;
        props.content_type = amqp_cstring_bytes(info.message.content_type.c_str());
    }

    if (!info.property.app_id.empty()) {
        props._flags |= AMQP_BASIC_APP_ID_FLAG;
        props.app_id = amqp_cstring_bytes(info.property.app_id.c_str());
    }

    if (!info.property.reply_to.empty()) {
        props._flags |= AMQP_BASIC_REPLY_TO_FLAG;
        props.reply_to = amqp_cstring_bytes(info.property.reply_to.c_str());
    }

    if (!info.property.correlation_id.empty()) {
        props._flags |= AMQP_BASIC_CORRELATION_ID_FLAG;
        props.correlation_id = amqp_cstring_bytes(info.property.correlation_id.c_str());
    }
    if (!info.property.expiration.empty()) {
        props._flags |= AMQP_BASIC_EXPIRATION_FLAG;
        props.expiration = amqp_cstring_bytes(info.property.expiration.c_str());
    }

    if (!info.message.message_id.empty()) {
        props._flags |= AMQP_BASIC_MESSAGE_ID_FLAG;
        props.message_id = amqp_cstring_bytes(info.message.message_id.c_str());
    }

    std::string routing_key = ( !info.delivery.routing_key.empty() ? info.delivery.routing_key : info_.queue.routing_key );
    assert( ! routing_key.empty() && "Field \"routing_key\" cannot be empty");

    die_on_error(amqp_basic_publish(conn_, info_.channel, amqp_cstring_bytes(info_.exchange.name.c_str()),
                                    amqp_cstring_bytes( routing_key.c_str()),
                                    info.delivery.mandatory, info.delivery.immediate,
                                    &props, amqp_cstring_bytes(info.message.body.c_str())),
                 "Publishing");

}


}
