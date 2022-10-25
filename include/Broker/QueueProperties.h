#ifndef  __QueueProperties_h__
#define  __QueueProperties_h__


#include <rabbitmq-c/amqp.h>
#include <string>

namespace libapp {

enum TypeConnectQueue {
    eConnectNothing = 0,
    eConnectRead = 1,
    eConnectWrite = 2,
    eConnectReadWrite = 3
};

typedef struct QueueProperties_t__ {
    //   uint16_t ticket;            /**< ticket */
    TypeConnectQueue type{eConnectNothing};

    // std::string queue;             /**< queue */
    // amqp_boolean_t passive{0};     /**< passive */
    // amqp_boolean_t durable{0};     /**< durable */
    // amqp_boolean_t exclusive{0};   /**< exclusive */
    // amqp_boolean_t auto_delete{0}; /**< auto-delete */
    // amqp_boolean_t nowait{0};      /**< nowait */

    // std::string consumer_tag;
    // amqp_boolean_t no_local{0};
    // amqp_boolean_t no_ack{0};
    // amqp_boolean_t exclusive{ 0 };
    // amqp_table_t arguments{amqp_empty_table}; /**< arguments */

} QueueProperties;

extern QueueProperties __queuePropertiesEmpty;

}


#endif // __QueueProperties_h__
