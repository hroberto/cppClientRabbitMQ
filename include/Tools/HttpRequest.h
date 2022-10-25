#ifndef __LIB_INFRA_HTTPREQUEST_H__
#define __LIB_INFRA_HTTPREQUEST_H___


#include <string>
#include <boost/beast/http.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/dynamic_body.hpp>

namespace libapp {

boost::beast::http::response<boost::beast::http::dynamic_body>
send_https(const boost::beast::http::verb& method, const std::string& host, const std::string& port, const std::string& target, const std::string& messageIN) ;


}


#endif // __LIB_INFRA_HTTPREQUEST_H__