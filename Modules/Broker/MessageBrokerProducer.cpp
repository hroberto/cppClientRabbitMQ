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


void MessageBrokerProducer::open(const struct MessageBrokerInfo_T& info) 
{
    info_ = info;

    open_socket();
    exchange_declare( info );
    // prepare();
}



void MessageBrokerProducer::publish(const MessagePublishProperties& properties) 
{
    amqp_basic_properties_t props;

    props._flags = AMQP_BASIC_DELIVERY_MODE_FLAG;
    props.delivery_mode = properties.delivery_mode;

    if (!properties.content_type.empty()) {
        props._flags |= AMQP_BASIC_CONTENT_TYPE_FLAG;
        props.content_type = amqp_cstring_bytes(properties.content_type.c_str());
    }

    if (!properties.app_id.empty()) {
        props._flags |= AMQP_BASIC_APP_ID_FLAG;
        props.app_id = amqp_cstring_bytes(properties.app_id.c_str());
    }

    if (!properties.reply_to.empty()) {
        props._flags |= AMQP_BASIC_REPLY_TO_FLAG;
        props.reply_to = amqp_cstring_bytes(properties.reply_to.c_str());
    }

    if (!properties.correlation_id.empty()) {
        props._flags |= AMQP_BASIC_CORRELATION_ID_FLAG;
        props.correlation_id = amqp_cstring_bytes(properties.correlation_id.c_str());
    }
    if (!properties.message_id.empty()) {
        props._flags |= AMQP_BASIC_MESSAGE_ID_FLAG;
        props.expiration = amqp_cstring_bytes(properties.expiration.c_str());
    }
    // 
    if (!properties.message_id.empty()) {
        props._flags |= AMQP_BASIC_EXPIRATION_FLAG;
        props.message_id = amqp_cstring_bytes(properties.message_id.c_str());
    }

    std::string routing_key = ( !properties.routing_key.empty() ? properties.routing_key : info_.routing_key );

    die_on_error(amqp_basic_publish(conn_, CHANNEL_, amqp_cstring_bytes(info_.exchange_name.c_str()),
                                    amqp_cstring_bytes( routing_key.c_str()),
                                    properties.mandatory, properties.immediate,
                                    &props, amqp_cstring_bytes(properties.message_body.c_str())),
                 "Publishing");

}


}
