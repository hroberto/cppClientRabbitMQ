#ifndef __MESSAGEBROKERINFO_H__
#define __MESSAGEBROKERINFO_H__

#include <string>
#include <rabbitmq-c/amqp.h>


namespace libapp {

typedef struct MessageBrokerExchangeInfo {

    std::string type;
    std::string name;

    bool passive{ false };
    bool durable{ false };
    bool auto_delete{ false };
    bool internal{ false };

    amqp_table_t arguments{ amqp_empty_table };

} MessageBrokerExchange_Type;


typedef struct MessageBrokerQueueInfo {
    std::string routing_key;
    std::string queue_name;

    bool passive{ false };
    bool durable{ false };
    bool auto_delete{ false };
    bool exclusive{ false };

    amqp_table_t arguments{ amqp_empty_table };
} MessageBrokerQueue_Type;


typedef struct MessageBrokerConsumerInfo {
    std::string tag;
    bool no_local{ false };
    bool no_ack{ false };
    bool exclusive{ false };

    amqp_table_t arguments{ amqp_empty_table };    
} MessageBrokerConsumer_Type;


typedef struct MessageBrokerInfo {
    std::string host{ "localhost" };
    int port{ 5672 };
    std::string login_user{ "guest" };
    std::string login_password{ "guest" };
    std::string vhost{ "/" };
    amqp_channel_t channel{1};

    MessageBrokerExchange_Type exchange;
    MessageBrokerQueueInfo queue;
    MessageBrokerConsumer_Type consumer;
} MessageBroker_Type;



typedef struct MessagePublishInfo {

    struct {        
        uint8_t delivery_mode{ AMQP_DELIVERY_PERSISTENT };
        std::string reply_to;
        std::string app_id;
        std::string correlation_id;
        std::string expiration;       // Time-to-live for this message in milliseconds
    } property;

    struct {
        std::string message_id;
        std::string content_type{ "text/plain" };
        std::string body;
    } message;

    struct {
        amqp_boolean_t mandatory{ false };
        amqp_boolean_t immediate{ false };
        std::string routing_key;
    } delivery;
    
} MessagePublish_Type;

}

#endif // __MESSAGEBROKERINFO_H__
