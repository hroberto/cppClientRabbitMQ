#include "Web/ClientExchange.h"
#include "Web/WebSocketCallbacksImpl.h"

#include <string>
#include <iostream>

#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>


// ClientExchange::ClientExchange(net::io_context& ioc, ssl::context& ctx)
// 	: _ioc(ioc),
// 	_ctx(ctx)
// {
// };


ClientExchange::ClientExchange()
: _ctx{ ssl::context::tlsv12_client }
{
	_ctx.set_verify_mode(ssl::verify_none);

	// This holds the root certificate used for verification
	// load_root_certificates(ctx);

	// Run the I/O service. The call will return when
	// the socket is closed.
	_ioc.run();

}

ClientExchange::~ClientExchange()
{
	stop();
}

void ClientExchange::stop()
{
	_ioc.stop();

	for( auto& [symbol_id, session_id] : _cryptoCurrencySubscriptions ) {
		session_id->Stop();
	}
	_cryptoCurrencySubscriptions.clear();
}



//notifies of a connection failure - socket disconnect, problem passing message, socket closed etc
void ClientExchange::WebSocketConnectionDown(const std::string& sessionName)
{
	// std::cout << sessionName << ": " << "Connection is down. Orderbook should be cleared" << std::endl;
}

//a message received on the socket
void ClientExchange::WebSocketMessageReceived(const std::string& sessionName, const std::string& jsonResult)
{
	if( ! do_onMessage_ ) {
		std::cout << sessionName << ": " << jsonResult << std::endl;
	}
	else {
		do_onMessage_( sessionName, jsonResult );
	}
}



void ClientExchange::SubscribeCryptoCurrency(const std::string& symbol, const std::string& target)
{
    // std::string target = boost::format("/ws/%1%@depth@100ms") % symbol.c_str();

    const bool foundSymbol = (_cryptoCurrencySubscriptions.find(symbol) == _cryptoCurrencySubscriptions.end());
    assert(foundSymbol);

    if (foundSymbol) {
        _cryptoCurrencySubscriptions[symbol] = std::make_shared<WebSocketSession>(*this, symbol, _ioc, _ctx, host_, port_, target);
        _cryptoCurrencySubscriptions[symbol]->Start();  // subscribes to market data on a new thread
    }
}

void ClientExchange::UnSubscribeCryptoCurrency(const std::string& symbol) {
	if (_cryptoCurrencySubscriptions.find(symbol) != _cryptoCurrencySubscriptions.end()) {
		_cryptoCurrencySubscriptions[symbol]->Stop();
		_cryptoCurrencySubscriptions.erase(symbol);
	}
	else {
		std::cout << "Symbol " << symbol << " not found.";
	}
}
