#include "Broker/MessageBrokerBase.hpp"

#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
#include <cassert>
#include <iostream>

#include "Broker/utils.h"
#include "Broker/MessageBrokerInfo.h"


namespace libapp {

QueueProperties __queuePropertiesEmpty;



MessageBrokerBase::~MessageBrokerBase()
{
    die_on_amqp_error(amqp_channel_close(conn_, 1, AMQP_REPLY_SUCCESS), "Closing channel");
    die_on_amqp_error(amqp_connection_close(conn_, AMQP_REPLY_SUCCESS), "Closing connection");
    die_on_error(amqp_destroy_connection(conn_), "Ending connection");

    socket_ = NULL;
}


/**
 * @brief 
 * 
 * @param hostname 
 * @param port 
 * @return int 
 */
int MessageBrokerBase::open_socket() 
{
    conn_ = amqp_new_connection();
    socket_ = amqp_tcp_socket_new(conn_);

    if (!socket_) {
        std::cerr << "creating TCP socket";
        return 1;
    }

    int status = amqp_socket_open(socket_, info_.host.c_str(), info_.port );
    if (status) {
        std::cerr << "opening TCP socket";
        return 1;
    }

    die_on_amqp_error(amqp_login(conn_, info_.vhost.c_str(), 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, info_.login_user.c_str(), info_.login_password.c_str()), "Logging in");

    amqp_channel_open(conn_, info_.channel);
    die_on_amqp_error(amqp_get_rpc_reply(conn_), "Opening channel");

    return 0;
}


/**
 * @brief 
 * 
 * @param exchange_name 
 * @param exchange_type 
 */
void MessageBrokerBase::exchange_declare(const MessageBroker_Type& info) {
    assert( socket_ != NULL );

    amqp_exchange_declare(conn_, info_.channel, amqp_cstring_bytes(info.exchange.name.c_str()), amqp_cstring_bytes(info.exchange.type.c_str()),  info.exchange.passive, info.exchange.durable, info.exchange.auto_delete, info.exchange.internal,  info.exchange.arguments );
    
    die_on_amqp_error(amqp_get_rpc_reply(conn_), "Declaring exchange");
}

/**
 * @brief 
 * 
 * @param bindingkey 
 * @param nameQueue 
 * @return int 
 */
int MessageBrokerBase::prepare( ) 
{
    assert( socket_ != NULL );

    amqp_bytes_t nameQueueBytes = amqp_empty_bytes;

    bool queue_passive{ false }, queue_durable{false}, queue_exclusive{ true }, queue_auto_delete{ false };

    if ( !info_.queue.queue_name.empty() ) {
        nameQueueBytes = amqp_cstring_bytes(info_.queue.queue_name.c_str());

        queue_passive = info_.queue.passive;
        queue_durable = info_.queue.durable;
        queue_exclusive = info_.queue.exclusive;
        queue_auto_delete = info_.queue.auto_delete;
    }

    amqp_queue_declare_ok_t *result = amqp_queue_declare(
                    conn_, info_.channel, nameQueueBytes, 
                    queue_passive, queue_durable, queue_exclusive, queue_auto_delete, 
                    info_.queue.arguments );

    die_on_amqp_error(amqp_get_rpc_reply(conn_), "Declaring queue");

    if ( info_.queue.queue_name.empty() ) {
        info_.queue.queue_name = std::string((char*)result->queue.bytes, result->queue.len);
    }

    if (result->queue.bytes == NULL) {
        fprintf(stderr, "Out of memory while copying queue name");
        return 1;
    }

    return 0;
}



void MessageBrokerBase::prepare_consumer()
{
    auto queuename = amqp_cstring_bytes(info_.queue.queue_name.c_str());
    auto consumer_tag = (info_.consumer.tag.empty()) ? amqp_empty_bytes : amqp_cstring_bytes(info_.consumer.tag.c_str());

    /* constroi a ligacao o produtor direcionar para ele. */
    amqp_queue_bind(conn_, info_.channel, queuename, amqp_cstring_bytes(info_.exchange.name.c_str()), amqp_cstring_bytes(info_.queue.routing_key.c_str()), amqp_empty_table);

    consume_ok_ = amqp_basic_consume(conn_, info_.channel, queuename, consumer_tag, info_.consumer.no_local, info_.consumer.no_ack, info_.consumer.exclusive, info_.consumer.arguments);
    die_on_amqp_error(amqp_get_rpc_reply(conn_), "Consuming");
}


}
