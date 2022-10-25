#ifndef __WEBSOCKETSESSION_H__
#define __WEBSOCKETSESSION_H__


// #include "./root_certificates.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>

#include "Web/WebSocketCallbacksImpl.h"


#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>



namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


//Class to connect to a websocket endpoint and receive data continuously counterparty closes or stopped manually
class WebSocketSession : public std::enable_shared_from_this<WebSocketSession>
{
	WebSocketCallbacksImpl & _callbacks;
	std::string _sessionIdenfitier;
	net::io_context & _ioContext;
	ssl::context& _sslContext;
	tcp::resolver _tcpResolver;
	std::string _host;
	const std::string _port;
	std::string _target;
	std::string _hostAndPort;


	std::shared_ptr<websocket::stream<beast::ssl_stream<tcp::socket>>> _ws;
	beast::flat_buffer _buffer;
	std::string _text;
	tcp::endpoint _tcpEndPoint;
	bool _terminate = false;
	// simdjson::dom::parser _jsonParser;
	std::thread _workerThread;
		
	void ConnectAndReceive();

public:

	// Resolver and socket require an io_context
	explicit
		WebSocketSession(WebSocketCallbacksImpl & callbacks,
			const std::string & sessionIdenfitier,
			net::io_context& ioc,
			ssl::context& ctx,
			const std::string& host,
			const std::string& port,
			const std::string& target)
		: 
		_callbacks(callbacks),
		_sessionIdenfitier(sessionIdenfitier),
		_ioContext(ioc),
		_sslContext(ctx),
		_tcpResolver(_ioContext),
		_host(host),
		_port(port),
		_target(target),
		_hostAndPort()
	{
	}

	void Stop();

	//Connect to WebSocket on a new thread and start receiving data
	void Start();

};

#endif // __WEBSOCKETSESSION_H__