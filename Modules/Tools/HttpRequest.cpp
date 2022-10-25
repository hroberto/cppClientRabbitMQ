#include "Tools/HttpRequest.h"

#include <boost/beast/core.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/ssl.hpp>
#include <openssl/ssl.h>

namespace beast = boost::beast;  // from <boost/beast.hpp>
namespace http = beast::http;    // from <boost/beast/http.hpp>
namespace net = boost::asio;     // from <boost/asio.hpp>
namespace ssl = net::ssl;        // from <boost/asio/ssl.hpp>
using tcp = net::ip::tcp;        // from <boost/asio/ip/tcp.hpp>


namespace libapp {


/**
 * @brief Metodo generico para consumo de end-point HTTPS.
 * 
 * @param method 
 * @param messageIN 
 * @return boost::beast::http::response<boost::beast::http::dynamic_body> 
 */
boost::beast::http::response<boost::beast::http::dynamic_body>
send_https(const boost::beast::http::verb& method, const std::string& host, const std::string& port, const std::string& target, const std::string& messageIN) 
{
    net::io_context ioc;
    ssl::context ctx(ssl::context::tlsv12_client);
    ctx.set_verify_mode(ssl::verify_none);

    // These objects perform our I/O
    tcp::resolver resolver(ioc);
    beast::ssl_stream<beast::tcp_stream> stream(ioc, ctx);

    // Set SNI Hostname (many hosts need this to handshake successfully)
    if (!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str())) {
        beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
        throw beast::system_error{ec};
    }

    // Look up the domain name
    auto const results = resolver.resolve(host, port);
    beast::get_lowest_layer(stream).connect(results);
    stream.handshake(ssl::stream_base::client);

    http::request<http::string_body> req{method, target, 11};
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    if (!messageIN.empty()) {
        req.body() = messageIN.c_str();
    }

    http::write(stream, req);

    beast::flat_buffer buffer;
    http::response<http::dynamic_body> res;

    http::read(stream, buffer, res);

    // Gracefully close the stream
    beast::error_code ec;
    stream.shutdown(ec);

    return res;
}

}
