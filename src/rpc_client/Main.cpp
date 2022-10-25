
#include <iostream>
#include <cassert>
#include <thread>
#include <functional>
#include <unistd.h>
#include <string.h>
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
    int ttl_expires = 50000;

    boost::program_options::options_description optDesc("Opções:");
    optDesc.add_options()
                ("hostname,h", boost::program_options::value(&hostname), fmt::format("Hostname do servidor do RabbitMQ. Default \"{}\"", hostname ).c_str() )
                ("port,p", boost::program_options::value(&port), fmt::format("Porta do servidor do RabbitMQ. Default \"{}\"", port ).c_str() )
                ("exchange_type,t", boost::program_options::value(&exchange_type), fmt::format("Tipo da Exchange (fanout, topic, direct) no RabbitMQ. Default \"{}\"", exchange_type ).c_str() )
                ("exchange_name,n", boost::program_options::value(&exchange_name), fmt::format("Nome da Exchange no RabbitMQ. Default \"{}\"", exchange_name ).c_str() )
                ("consumerQueueName,q", boost::program_options::value(&consumerQueueName),  "Nome da fila de leitura." )
                ("routing_key_request,s", boost::program_options::value(&routing_key_request), fmt::format("Chave de roteamento da solicitacao. Default \"{}\"", routing_key_request ).c_str() )
                ("routing_key_response,r", boost::program_options::value(&routing_key_response), fmt::format("Chave de roteamento da resposta. Default \"{}\"", routing_key_response ).c_str() )
                ("appId,r", boost::program_options::value(&routing_key_response), fmt::format("Chave de roteamento da resposta. Default \"{}\"", routing_key_response ).c_str() )
                ("timeToLive-expires,e", boost::program_options::value(&ttl_expires), fmt::format("Tempo de vida em milisegundos da mensagem. Default \"{}\"", ttl_expires ).c_str() )
                ("help,h", "Exibe este menu de ajuda");

    auto vm = libapp::parseOptions(argc, argv, optDesc);
    if (vm.count("help")) {
        std::cout << optDesc << "\n";
        return EXIT_SUCCESS;
    }


    std::stringstream ssAppID ;
    ssAppID << "rpc-client." << getpid() << "-" << std::this_thread::get_id();
    if ( consumerQueueName.empty() ) {
        ssAppID << "rpc-client." << getpid() << "-" << std::this_thread::get_id();
    } else {        
        ssAppID << consumerQueueName;
    }

    const auto wait_for_message = [&](auto& consumer, const std::string& app_id, const std::string& correlation_id) {
        std::shared_ptr<libapp::MessageWrapper>  wrapper_shr;
        bool found{ false };
        do {
            wrapper_shr = consumer.read();
            consumer.ack_message( wrapper_shr );
        
            found = !strcmp(  app_id.c_str(), (char*) (*wrapper_shr)->message.properties.app_id.bytes )
                 &&  !strcmp( correlation_id.c_str(), (char*) (*wrapper_shr)->message.properties.correlation_id.bytes );


            // std::cout << fmt::format("|> <trace> CLIENT [\"{}] - msg:= {} - found:= {}" , ssAppID.str(), wrapper_shr->toString(), found ) << std::endl;

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
            .consumerQueueName = consumerQueueName,
            .consumer_tag = ssAppID.str(),
            .exchange_durable = true,
            .queue_durable = true,
        });

    libapp::MessageBrokerProducer cli_producer;
    cli_producer.open( 
        libapp::MessageBrokerInfo_T{
            .host = hostname, 
            .port = port,
            .exchange_type = exchange_type,
            .exchange_name = exchange_name,
            .routing_key = routing_key_request,
            .exchange_durable = true
        });





    std::stringstream ss ;
    size_t sequence{};

    while( true )
    {
        ++sequence;
        std::cout << "|> CLIENT [\"" << ssAppID.str() << "] - sending routing_key - " << cli_producer.getBrokerInfo().routing_key << "\" - sequence:= " << sequence << std::endl;
        const std::string correlation_id{ std::to_string( sequence ) };

        cli_producer.publish( libapp::MessagePublishProperties {
                .content_type = "text/plain",
                // .reply_to = cli_consumer.getBrokerInfo().consumerQueueName,
                .reply_to = routing_key_response,
                .message_body = "Consumer producer new message",
                .correlation_id = correlation_id,
                .app_id = ssAppID.str(),
                .expiration = ( ttl_expires > 0 ? std::to_string(ttl_expires) : "" ),
                });
        

        std::shared_ptr<libapp::MessageWrapper>  wrapper_shr = wait_for_message( cli_consumer, ssAppID.str(), correlation_id );
        std::cout << fmt::format("|> CLIENT [\"{}] - msg:= {}" , ssAppID.str(), wrapper_shr->toString() ) << std::endl;

        assert( !strcmp( ssAppID.str().c_str(), (char*) (*wrapper_shr)->message.properties.app_id.bytes ) );
        assert( !strcmp( correlation_id.c_str(), (char*) (*wrapper_shr)->message.properties.correlation_id.bytes ) );

        std::this_thread::sleep_for( std::chrono::milliseconds(100) );
    }


    return EXIT_SUCCESS;
}
