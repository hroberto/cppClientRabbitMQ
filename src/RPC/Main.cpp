
#include <iostream>
#include <cassert>
#include <thread>
#include <functional>

// #include "Broker/ProducerMessageQueue.hpp"
// #include "Broker/ConsumerMessageQueue.hpp"

#include "Broker/MessageBrokerConsumer.hpp"
#include "Broker/MessageBrokerProducer.hpp"




int main(int argc, char const *const *argv) {

    // char const *hostname = "10.18.12.36";
    char const *hostname = "localhost"; 
    int port = 5672;
    char const *exchange_type = "direct";
    char const *exchange_name = "business";
    char const *routing_key = "bus_consumer_all";

    // char const *queue_name = argv[6];          // "test.client1";


    libapp::MessageBrokerConsumer consumer1;
    consumer1.open(
        libapp::MessageBrokerInfo_T{
            .host = hostname, 
            .port = port,
            .exchange_type = exchange_type,
            .exchange_name = exchange_name,
            .routing_key = routing_key,
            .exchange_durable{ true },
            .queue_durable{ false }
        });


    libapp::MessageBrokerProducer producer;
    producer.open( 
        libapp::MessageBrokerInfo_T{
            .host = hostname, 
            .port = port,
            .exchange_name = exchange_name,
            .exchange_type = exchange_type,
            .routing_key = routing_key,
            .exchange_durable{ true },
            .queue_durable{ false }
        });


    std::stringstream ss ;
    ss << "{ \"ID_ALGO\": 1, \"TIMESTAMP_ENVIO\": 0, \"TIMESTAMP_TREINAMENTO_PRD\": 1666373638972, \"DATA\": [] }";


    // // for(;;) 
    {
        std::cout << "|> producer. - queuename_:= \"" << producer.getBrokerInfo().routing_key << "\"" << std::endl;
        producer.publish( libapp::MessagePublishProperties {
                .content_type{"application/json"},
                .reply_to{consumer1.getBrokerInfo().consumerQueueName}, 
                .message_body{ ss.str() },
                .correlation_id{ "10" },
                .app_id{ "app_test_rpc" }
                });
        std::this_thread::sleep_for( std::chrono::seconds(1) );
    }


    const auto fn_reader = [&](auto& consumer){
        for(;;) {
            std::cout << "|> Reading" << std::endl;
            std::shared_ptr<libapp::MessageWrapper>  wrapper_shr = consumer.read();
            consumer.ack_message( wrapper_shr );
        
            std::cout << "|>  [" << consumer.getBrokerInfo().consumerQueueName << "]  msg:= " << wrapper_shr->toString() << std::endl;
        }
    };
    std::cout << "|> consumer1 - queuename_:= \"" << consumer1.getBrokerInfo().routing_key << "\"" << consumer1.getBrokerInfo().consumerQueueName << std::endl;
    std::thread( std::bind(fn_reader, std::ref(consumer1)) ).join();

    return EXIT_SUCCESS;
}
