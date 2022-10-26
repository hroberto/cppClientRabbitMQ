
#include <iostream>
#include <thread>
#include <cassert>
#include <functional>
#include <unistd.h>
#include <fmt/format.h>

#include "Broker/MessageBrokerConsumer.hpp"
#include "Broker/MessageBrokerProducer.hpp"
#include "Tools/ProgramOptionsUtils.h"



int main(int argc, char const *argv[])
{
    std::string hostname = "localhost"; 
    int port = 5672;
    std::string exchange_type = "topic";
    std::string exchange_name = "business";
    std::string routing_key_request = "task_request";
    std::string routing_key_response= "task_response";
    std::string consumerQueueName;
    int ttl_expires = 0;


    boost::program_options::options_description optDesc("Opções:");
    optDesc.add_options()
                ("hostname,h", boost::program_options::value(&hostname), fmt::format("Hostname do servidor do RabbitMQ. Default \"{}\"", hostname ).c_str() )
                ("port,p", boost::program_options::value(&port), fmt::format("Porta do servidor do RabbitMQ. Default \"{}\"", port ).c_str() )
                ("exchange_type,t", boost::program_options::value(&exchange_type), fmt::format("Tipo da Exchange (fanout, topic, direct) no RabbitMQ. Default \"{}\"", exchange_type ).c_str() )
                ("exchange_name,n", boost::program_options::value(&exchange_name), fmt::format("Nome da Exchange no RabbitMQ. Default \"{}\"", exchange_name ).c_str() )
                ("consumerQueueName,q", boost::program_options::value(&consumerQueueName),  "Nome da fila de leitura." )
                ("routing_key_request,s", boost::program_options::value(&routing_key_request), fmt::format("Chave de roteamento da solicitacao. Default \"{}\"", routing_key_request ).c_str() )
                ("routing_key_response,r", boost::program_options::value(&routing_key_response), fmt::format("Chave de roteamento da resposta. Default \"{}\"", routing_key_response ).c_str() )
                ("timeToLive-expires,e", boost::program_options::value(&ttl_expires), fmt::format("Tempo de vida em milisegundos da mensagem. Default \"{}\"", ttl_expires ).c_str() )
                ("help,h", "Exibe este menu de ajuda");

    auto vm = libapp::parseOptions(argc, argv, optDesc);
    if (vm.count("help")) {
        std::cout << optDesc << "\n";
        return EXIT_SUCCESS;
    }


    std::stringstream ssAppID ;
    ssAppID << ( consumerQueueName.empty() ? "rpc-server.1" : consumerQueueName );

    libapp::MessageBrokerConsumer srv_consumer;
    srv_consumer.open(
        {
            .host = hostname, 
            .port = port,
            .exchange = { 
                .type = exchange_type,
                .name = exchange_name,
                .durable = true,
                },
            .queue = {
                .routing_key = routing_key_request,
                .queue_name = consumerQueueName,
                .durable = true,
            },
            .consumer = {
                .tag = ssAppID.str(),
            }
        });

    libapp::MessageBrokerProducer srv_producer;
    srv_producer.open( 
        {
            .host = hostname, 
            .port = port,
            .exchange = { 
                .type = exchange_type,
                .name = exchange_name,
                .durable = true,
                },
            .queue = {
                .routing_key = routing_key_response,
                .queue_name = consumerQueueName,
                .durable = true,
            }
        });



    const auto byte_to_string = [](auto &byte) { return std::string( (char*)byte.bytes, byte.len); };

    while( true )
    {
        std::shared_ptr<libapp::MessageWrapper>  wrapper_shr = srv_consumer.read();
        assert( wrapper_shr != nullptr );

        srv_consumer.ack_message( wrapper_shr );    
        
        std::string from_routing_key = byte_to_string( (*wrapper_shr)->message.properties.reply_to );

        std::cout << "|>  SERVER [" << srv_consumer.getBrokerInfo().queue.queue_name << "] - received msg:= " << wrapper_shr->toString() << std::endl;

        srv_producer.publish( libapp::MessagePublishProperties {
                .content_type = byte_to_string( (*wrapper_shr)->message.properties.content_type ),
                .reply_to = srv_consumer.getBrokerInfo().queue.routing_key, 
                .message_body = "Process ok",
                .correlation_id = byte_to_string( (*wrapper_shr)->message.properties.correlation_id ),
                .app_id = byte_to_string( (*wrapper_shr)->message.properties.app_id),
                .routing_key = from_routing_key,
                .expiration = ( ttl_expires > 0 ? std::to_string(ttl_expires) : "" )
                });
        // std::cout << "|>  SERVER [" << srv_consumer.getBrokerInfo().consumerQueueName << "] - from_routing_key:= " << from_routing_key << " - response." << std::endl;
    }

    return EXIT_SUCCESS;
}
