#ifndef __WEB_BINANCEEXCHANGE_H__
#define __WEB_BINANCEEXCHANGE_H__

#include "Web/WebSocketSession.h"
#include "Web/WebSocketCallbacksImpl.h"

#include <string>
#include <iostream>
#include <functional>

#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>


//------------------------------------------------------------------------------
class ClientExchange : public WebSocketCallbacksImpl
{
public:
    explicit ClientExchange();
    virtual ~ClientExchange();
	// ClientExchange(net::io_context& ioc, ssl::context& ctx);

	void SubscribeCryptoCurrency(const std::string& symbol, const std::string& target);
	void UnSubscribeCryptoCurrency(const std::string& symbol);

    void stop();

    ClientExchange& setHostAndPort(std::string const& host, std::string const& port ) { host_ = host; port_ = port; return *this; }

    typedef std::function< void(std::string, std::string) >  ON_MESSAGE_TYPE;
    void setCallback( const ON_MESSAGE_TYPE on_message ) { do_onMessage_ = on_message; };

private:


    // wss://stream.binance.com:9443
	std::string host_{ "stream.binance.com" };
	std::string port_{ "9443" };
	// std::string targetFmt_{  "/ws/%1%@depth@100ms" };


	net::io_context _ioc;
	ssl::context _ctx;
	std::map<std::string, std::shared_ptr<WebSocketSession>> _cryptoCurrencySubscriptions;
	
	//implement useful application logic with the websocket callbacks ->

	//notifies of a connection failure - socket disconnect, problem passing message, socket closed etc
	virtual void WebSocketConnectionDown(const std::string& sessionName) override;

	//a message received on the socket
	virtual void WebSocketMessageReceived(const std::string& sessionName, const std::string& jsonResult) override;

    ON_MESSAGE_TYPE do_onMessage_ = nullptr;

};



#endif // __WEB_BINANCEEXCHANGE_H__