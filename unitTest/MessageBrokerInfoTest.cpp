#include "gtest/gtest.h"
// #include "gmock/gmock.h"


#include "Broker/MessageBrokerInfo.h"
// #include "Broker/MessageBrokerConsumer.hpp"
// #include "Broker/MessageBrokerProducer.hpp"


TEST(MessageBrokerInfoTest, DefaultValues_Sucess) {
    libapp::MessageBroker_Type info{};

    EXPECT_EQ( "localhost", info.host );
    EXPECT_EQ( 5672, info.port );
    EXPECT_EQ( "guest", info.login_user );
    EXPECT_EQ( "guest", info.login_password );
    EXPECT_EQ( "/", info.vhost );
    EXPECT_EQ( 1, info.channel );
}


TEST(MessageBrokerInfoTest, DefaultValuesExchange_Sucess) {
    libapp::MessageBroker_Type info;

    EXPECT_EQ( 0u, info.exchange.type.size() );
    EXPECT_EQ( 0u, info.exchange.name.size() );
    ASSERT_FALSE( info.exchange.passive );
    ASSERT_FALSE( info.exchange.durable );
    ASSERT_FALSE( info.exchange.auto_delete );
    ASSERT_FALSE( info.exchange.internal );
    EXPECT_EQ( amqp_empty_table.entries, info.exchange.arguments.entries );
    EXPECT_EQ( amqp_empty_table.num_entries, info.exchange.arguments.num_entries );
}


TEST(MessageBrokerInfoTest, DefaultValuesQueue_Sucess) {
    libapp::MessageBroker_Type info;

    EXPECT_EQ( 0u, info.queue.routing_key.size() );
    EXPECT_EQ( 0u, info.queue.queue_name.size() );
    ASSERT_FALSE( info.queue.passive );
    ASSERT_FALSE( info.queue.durable );
    ASSERT_FALSE( info.queue.auto_delete );
    ASSERT_FALSE( info.queue.exclusive );
    EXPECT_EQ( amqp_empty_table.entries, info.queue.arguments.entries );
    EXPECT_EQ( amqp_empty_table.num_entries, info.queue.arguments.num_entries );
}


TEST(MessageBrokerInfoTest, DefaultValuesConsumer_Sucess) {
    libapp::MessageBroker_Type info;

    EXPECT_EQ( 0u, info.consumer.tag.size() );
    ASSERT_FALSE( info.consumer.no_local );
    ASSERT_FALSE( info.consumer.no_ack );
    ASSERT_FALSE( info.consumer.exclusive );
    EXPECT_EQ( amqp_empty_table.entries, info.consumer.arguments.entries );
    EXPECT_EQ( amqp_empty_table.num_entries, info.consumer.arguments.num_entries );
}


TEST(MessageBrokerInfoTest, DefaultValuesMessagePublish_Sucess) {
    libapp::MessagePublish_Type msgPush;

    EXPECT_EQ( AMQP_DELIVERY_PERSISTENT, msgPush.property.delivery_mode );
    EXPECT_EQ( 0u, msgPush.property.reply_to.size() );
    EXPECT_EQ( 0u, msgPush.property.app_id.size() );
    EXPECT_EQ( 0u, msgPush.property.correlation_id.size() );
    EXPECT_EQ( 0u, msgPush.property.expiration.size() );
    EXPECT_EQ( 0u, msgPush.message.message_id.size() );
    EXPECT_EQ( "text/plain", msgPush.message.content_type );
    EXPECT_EQ( 0u, msgPush.message.body.size() );
    ASSERT_FALSE( msgPush.delivery.mandatory );
    ASSERT_FALSE( msgPush.delivery.immediate );
    EXPECT_EQ( 0u, msgPush.delivery.routing_key.size() );
}
