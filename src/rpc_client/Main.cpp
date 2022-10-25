
#include <iostream>
#include <cassert>
#include <thread>
#include <functional>
#include <unistd.h>
#include <string.h>
#include <fmt/format.h>


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
    ssAppID << "rpc-client." << getpid() << "-" << std::this_thread::get_id();

    const auto wait_for_message = [&](auto& consumer, const std::string& app_id, const std::string& correlation_id) {
        std::shared_ptr<libapp::MessageWrapper>  wrapper_shr;
        bool found{ false };
        do {
            wrapper_shr = consumer.read();
            consumer.ack_message( wrapper_shr );
        
            found = !strcmp(  app_id.c_str(), (char*) (*wrapper_shr)->message.properties.app_id.bytes )
                 &&  !strcmp( correlation_id.c_str(), (char*) (*wrapper_shr)->message.properties.correlation_id.bytes );

        } while ( !found );

        return wrapper_shr;
    };


    libapp::MessageBrokerConsumer cli_consumer;
    cli_consumer.open(
        libapp::MessageBrokerInfo_T{
            .host = hostname, 
            .port = port,
            .exchange_type = exchange_type,
            .exchange_name = exchange_name,
            .routing_key = routing_key_response,
            .consumer_tag = ssAppID.str(),
            .exchange_durable = true,
            .queue_durable = false,
            .queue_exclusive = true
        });

    libapp::MessageBrokerProducer cli_producer;
    cli_producer.open( 
        libapp::MessageBrokerInfo_T{
            .host = hostname, 
            .port = port,
            .exchange_type = exchange_type,
            .exchange_name = exchange_name,
            .routing_key = routing_key_request,
            .exchange_durable = true,
            .queue_durable =  false
        });





    std::stringstream ss ;
    size_t sequence{};

    while( true )
    {
        ++sequence;
        // std::cout << "|> CLIENT [\"" << ssAppID.str() << "] - sending routing_key - " << cli_producer.getBrokerInfo().routing_key << "\" - sequence:= " << sequence << std::endl;
        const std::string correlation_id{ std::to_string( sequence ) };

        cli_producer.publish( libapp::MessagePublishProperties {
                .content_type = "text/plain",
                // .reply_to = cli_consumer.getBrokerInfo().consumerQueueName,
                .reply_to = routing_key_response,
                .message_body = "Consumer producer new message",
                .correlation_id = correlation_id,
                .app_id = ssAppID.str()
                });
        

        std::shared_ptr<libapp::MessageWrapper>  wrapper_shr = wait_for_message( cli_consumer, ssAppID.str(), correlation_id );
        std::cout << fmt::format("|> CLIENT [\"{}] - msg:= {}" , ssAppID.str(), wrapper_shr->toString() ) << std::endl;

        assert( !strcmp( ssAppID.str().c_str(), (char*) (*wrapper_shr)->message.properties.app_id.bytes ) );
        assert( !strcmp( correlation_id.c_str(), (char*) (*wrapper_shr)->message.properties.correlation_id.bytes ) );

        std::this_thread::sleep_for( std::chrono::milliseconds(100) );
    }


    return EXIT_SUCCESS;
}
