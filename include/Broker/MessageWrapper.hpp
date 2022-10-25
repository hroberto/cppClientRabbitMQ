#ifndef __MESSAGEWRAPPER_H__
#define __MESSAGEWRAPPER_H__


#include <string>
#include <rabbitmq-c/amqp.h>
#include <sstream>
#include <vector>
#include <any>
#include "Tools/StringUtils.h"


namespace libapp {

class MessageWrapper {
public:
    MessageWrapper(amqp_envelope_t &&e) 
        : envelope_{std::move(e)} 
    {}

    virtual ~MessageWrapper() {
        amqp_destroy_envelope(&envelope_);            
    }

    std::string toString() {
        std::stringstream ss("");

        const auto byte_to_string = [](amqp_bytes_t &byte) { return std::string( (char*)byte.bytes, byte.len); };

        std::vector<std::pair<std::string, std::any> > proper;
        if (envelope_.message.properties._flags & AMQP_BASIC_DELIVERY_MODE_FLAG) proper.emplace_back(std::make_pair("delivery_mode", (int) envelope_.message.properties.delivery_mode));
        if (envelope_.message.properties._flags & AMQP_BASIC_CONTENT_TYPE_FLAG) proper.emplace_back(std::make_pair("content_type", byte_to_string(envelope_.message.properties.content_type)));
        if (envelope_.message.properties._flags & AMQP_BASIC_APP_ID_FLAG) proper.emplace_back(std::make_pair("app_id", byte_to_string(envelope_.message.properties.app_id)));
        if (envelope_.message.properties._flags & AMQP_BASIC_REPLY_TO_FLAG) proper.emplace_back(std::make_pair("reply_to", byte_to_string(envelope_.message.properties.reply_to)));
        if (envelope_.message.properties._flags & AMQP_BASIC_CORRELATION_ID_FLAG) proper.emplace_back(std::make_pair("correlation_id", byte_to_string(envelope_.message.properties.correlation_id)));
        if (envelope_.message.properties._flags & AMQP_BASIC_MESSAGE_ID_FLAG) proper.emplace_back(std::make_pair("message_id", byte_to_string(envelope_.message.properties.message_id)));
        if (envelope_.message.properties._flags & AMQP_BASIC_EXPIRATION_FLAG) proper.emplace_back(std::make_pair("expiration", byte_to_string(envelope_.message.properties.expiration)));

        std::string json = libapp::to_string_json(
            {{ "delivery_tag", (unsigned)envelope_.delivery_tag },
            { "consumer_tag", byte_to_string(envelope_.consumer_tag) },
            { "exchange", byte_to_string(envelope_.exchange) },
            { "routing_key", byte_to_string(envelope_.routing_key) },
            { "msg-properties", proper },
            { "message-body", byte_to_string(envelope_.message.body) }
        });

        return json;
    }

    const amqp_envelope_t_& get() const& { return envelope_; }
    const amqp_envelope_t_* operator->() const& { return &envelope_; }

private:
    amqp_envelope_t envelope_;
};

}

#endif // __MESSAGEWRAPPER_H__
