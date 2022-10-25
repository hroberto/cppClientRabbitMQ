#ifndef __MESSAGEBROKERINFO_H__
#define __MESSAGEBROKERINFO_H__

#include <string>
#include <rabbitmq-c/amqp.h>


namespace libapp {

struct MessageBrokerInfo_T {
    std::string host{ "localhost" };
    int port{ 5672 };    

    std::string exchange_type;
    std::string exchange_name;
    std::string routing_key{};
    std::string consumerQueueName;

    std::string consumer_tag;

    bool exchange_passive{ false };
    bool exchange_durable{ false };
    bool exchange_auto_delete{ false };
    bool exchange_internal{ false };


    bool queue_passive{ false };
    bool queue_durable{ false };
    bool queue_auto_delete{ false };
    bool queue_exclusive{ false };    

    bool consumer_no_local{ false };
    bool consumer_no_ack{ false };
    bool consumer_exclusive{ false };
    
    std::string login_user{ "guest" };
    std::string login_password{ "guest" };
    std::string vhost{ "/" };

//     MessageBrokerInfo_T& operator=(const MessageBrokerInfo_T& m) {
//         return (*this);
//     }

};

}

#endif // __MESSAGEBROKERINFO_H__
