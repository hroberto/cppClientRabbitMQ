#include "gtest/gtest.h"
// #include "gmock/gmock.h"


#include "Broker/MessageBrokerInfo.h"
#include "Broker/MessageBrokerConsumer.hpp"
// #include "Broker/MessageBrokerProducer.hpp"


TEST(MessageBrokerTest, OpenConnect_Sucess) {
    libapp::MessageBrokerConsumer cli_consumer;

    ASSERT_NO_THROW( cli_consumer.open(
        {
            .host = "localhost", 
            .exchange = { 
                .type = "fanout",
                .name = "unitTest",
                .durable = false,
                },
            .queue = {
                .routing_key = "ut.*",
                .queue_name = "ut.connect",
                .durable = false,
            },
            .consumer = {
                .tag = "consumerUT",
            }
        })
    );
}


TEST(MessageBrokerTest, OpenConnect_SocketFailure) {
    libapp::MessageBrokerConsumer cli_consumer;

    ASSERT_THROW(
        cli_consumer.open( { .host = "127.9.9.9", .port = 9999 });
        , MessageBrokerOpenSocketException 
    );
}
