
#include <iostream>
#include <thread>
#include <cassert>
#include <functional>
#include <unistd.h>

#include "Broker/MessageBrokerConsumer.hpp"
#include "Broker/MessageBrokerProducer.hpp"



int main(int argc, char const *const *argv) 
{
    char const *hostname = "localhost"; 
    int port = 5672;
    char const *exchange_type = "direct";
    char const *exchange_name = "business";
    char const *routing_key_request = "task_request";
    char const *routing_key_response= "task_response";


    std::stringstream ssAppID ;
    ssAppID << "rpc-server." << getpid() << "-" << std::this_thread::get_id();

    libapp::MessageBrokerConsumer srv_consumer;
    srv_consumer.open(
        libapp::MessageBrokerInfo_T{
            .host = hostname, 
            .port = port,
            .exchange_type = exchange_type,
            .exchange_name = exchange_name,
            .routing_key = routing_key_request,
            .consumer_tag = ssAppID.str(),
            .exchange_durable = true,
            .queue_durable = false
        });


    libapp::MessageBrokerProducer srv_producer;
    srv_producer.open( 
        libapp::MessageBrokerInfo_T{
            .host = hostname, 
            .port = port,
            .exchange_type = exchange_type,
            .exchange_name = exchange_name,
            .routing_key = routing_key_response,
            .exchange_durable = true,
            .queue_durable = false
        });



    const auto byte_to_string = [](auto &byte) { return std::string( (char*)byte.bytes, byte.len); };

    while( true )
    {
        
        std::shared_ptr<libapp::MessageWrapper>  wrapper_shr = srv_consumer.read();
        assert( wrapper_shr != nullptr );

        srv_consumer.ack_message( wrapper_shr );    

        // if( ! strcmp( (*wrapper_shr)->message.properties.content_type, srv_consumer.getBrokerInfo().routing_key)) {
        //     continue;
        // }
        
        std::string from_routing_key = byte_to_string( (*wrapper_shr)->message.properties.reply_to );

        std::cout << "|>  SERVER [" << srv_consumer.getBrokerInfo().consumerQueueName << "] - received msg:= " << wrapper_shr->toString() << std::endl;

        srv_producer.publish( libapp::MessagePublishProperties {
                .content_type{ byte_to_string( (*wrapper_shr)->message.properties.content_type ) },
                .reply_to{ srv_consumer.getBrokerInfo().routing_key }, 
                .message_body{ "Process ok" },
                .correlation_id{ byte_to_string( (*wrapper_shr)->message.properties.correlation_id ) },
                .app_id{ byte_to_string( (*wrapper_shr)->message.properties.app_id) },
                .routing_key{ from_routing_key  }
                });
        // std::cout << "|>  SERVER [" << srv_consumer.getBrokerInfo().consumerQueueName << "] - from_routing_key:= " << from_routing_key << " - response." << std::endl;
    }

    return EXIT_SUCCESS;
}
