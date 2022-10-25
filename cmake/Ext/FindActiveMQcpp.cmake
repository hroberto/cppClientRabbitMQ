
# find_path( ActiveMQ_CPP_INCLUDE_DIR
#            NAMES ActiveMQCPP.h
#            PATH_SUFFIXES activemq-cpp-3.10.0/activemq/library
#            DOC "ActiveMQ include files" )

find_path( ActiveMQcpp_INCLUDE_DIR
           NAMES activemq/library
           PATH_SUFFIXES activemq-cpp-3.10.0
           DOC "ActiveMQ include files" )

find_library( ActiveMQcpp_LIBRARY
              NAMES activemq-cpp-3.10.0
              DOC "ActiveMQ library" )
              

