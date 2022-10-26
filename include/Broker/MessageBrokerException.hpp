#ifndef __MESSAGEBROKEREXCEPTION_H__
#define __MESSAGEBROKEREXCEPTION_H__

#include <string>
#include <exception>
#include <fmt/format.h>


class MessageBrokerException : public std::exception {
public:
    explicit MessageBrokerException(const std::string& m) {
        msg_ = fmt::format( "MessageBrokerException: \"{}\"", m );
    }

    const char* what () const& { return msg_.c_str(); }
    
private:
    std::string msg_;
};


#define DECL_EXCEPTION_BROKER(D, M) \
        class D : public MessageBrokerException { \
        public: \
            explicit D() : MessageBrokerException( M ) {} \
        }


DECL_EXCEPTION_BROKER( MessageBrokerCreateSocketException, "Creating TCP socket" );
DECL_EXCEPTION_BROKER( MessageBrokerOpenSocketException, "Opening TCP socket" );
// DECL_EXCEPTION_BROKER( MessageBrokerOpenChannelException, "Opening channel" );
 



#endif // __MESSAGEBROKEREXCEPTION_H__