#include "Web/WebSocketSession.h"


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


	
void WebSocketSession::ConnectAndReceive()
{
	// Look up the domain name
	auto const tcpResolverResults = _tcpResolver.resolve(_host, _port);

	// Make the connection on the IP address we get from a lookup
	_tcpEndPoint = net::connect(get_lowest_layer(*_ws), tcpResolverResults);

	// Set SNI Hostname (many hosts need this to handshake successfully)
	if (!SSL_set_tlsext_host_name(_ws->next_layer().native_handle(), _host.c_str())) {
		throw beast::system_error(
			beast::error_code(static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()), "Failed to set SNI Hostname");
	}
	// Update the _host string. This will provide the value of the
	// Host HTTP header during the WebSocket handshake.
	// See https://tools.ietf.org/html/rfc7230#section-5.4
	_hostAndPort = _host + ':' + std::to_string(_tcpEndPoint.port());

	// Perform the SSL handshake
	_ws->next_layer().handshake(ssl::stream_base::client);

	// Set a decorator to change the User-Agent of the handshake
	_ws->set_option(websocket::stream_base::decorator(
		[](websocket::request_type& req)
		{
			req.set(http::field::user_agent,
				std::string(BOOST_BEAST_VERSION_STRING) +
				" websocket-client-coro");
		}));

	// Perform the websocket handshake
	_ws->handshake(_hostAndPort, _target);

	//Read Messages
	try {
		bool terminating = false;
		while (true) //expect to read continuously until a connection failure or manually terminated
		{
			if (!_terminate || terminating) {
				_ws->read(_buffer);

				if (_buffer.size() > 0)
				{
					std::string bufferString = boost::beast::buffers_to_string(_buffer.data());
					// auto jsonResult = _jsonParser.parse(bufferString);

					_callbacks.WebSocketMessageReceived(_sessionIdenfitier, bufferString);

				}
				_buffer.clear();
			}
			else { //termination has been requested
				_ws->close(boost::beast::websocket::close_code::normal);
				terminating = true; //we could break out of while here, but then we would not read any final message from cpty after we send close?
			}
		}
	}
	catch (beast::system_error const& se) {
		if (se.code() == websocket::error::closed) {
			std::cerr << "socket was closed." << std::endl;
		}
		else {
			// std::cerr << "WebSocketSession::system_error | exception: " << se.code() << ", " << se.code().message() << ", " << std::endl;
		}
		_terminate = true;
	}
	catch (std::exception& ex) {
		// std::cerr << "WebSocketSession | exception: " << ex.what() << std::endl;
		_terminate = true;
	}

	_callbacks.WebSocketConnectionDown(_sessionIdenfitier);
}


void WebSocketSession::Stop() {
	_terminate = true;
	_workerThread.join();
	_workerThread.~thread();
	
}




//Connect to WebSocket on a new thread and start receiving data
void WebSocketSession::Start()
{
	_workerThread = std::thread(
		[this]
		{
			while (!_terminate) {

				_ws = std::make_shared<websocket::stream<beast::ssl_stream<tcp::socket>>>(_ioContext, _sslContext);

				try {
					//initalizes websocket and polls continuously for messages until socket closed or an exception occurs
					ConnectAndReceive();
				}
				//catches network or ssl handshake errors in attemting to establish the websocket
				catch (beast::system_error const& se) {

					_callbacks.WebSocketConnectionDown(_sessionIdenfitier); //notify failed connection

					std::cerr << "exception: " << se.code() << ", " << se.code().message() << std::endl;
					std::this_thread::sleep_for( std::chrono::milliseconds(1000) ); //just wait a little while to not spam
				}

				_ws.reset();
			}
			//if we get to here Stop has been Called
			// std::cout << "WebSocketSession Stopped." << std::endl;
		});
}
