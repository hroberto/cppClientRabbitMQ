#ifndef  __MESSAGEPUBLISHPROPERTIES_H__
#define  __MESSAGEPUBLISHPROPERTIES_H__


#include <rabbitmq-c/amqp.h>
#include <string>

namespace libapp {


struct MessagePublishProperties {
    std::string content_type{ "text/plain" };
    uint8_t delivery_mode{ AMQP_DELIVERY_PERSISTENT };
    std::string reply_to;

    std::string message_body{};
    std::string correlation_id{};
    std::string message_id{};
    std::string app_id{};
    
    amqp_boolean_t mandatory{ 0 };
    amqp_boolean_t immediate{ 0 };

    std::string routing_key{};
    
    std::string expiration{};
};

}

#endif // __MESSAGEPUBLISHPROPERTIES_H__