#ifndef __WEB_WEBSOCKETCALLBACKSIMPL__
#define __WEB_WEBSOCKETCALLBACKSIMPL__

#include <string>


class WebSocketCallbacksImpl
{
public:
	//notifies of a connection failure - socket disconnect, problem passing message, socket closed etc
	virtual void WebSocketConnectionDown(const std::string & sessionName) = 0; 

	virtual void WebSocketMessageReceived(const std::string& sessionName, const std::string& jsonResult) = 0;


	//a message received on the socket
	// virtual void WebSocketMessageReceived(const std::string& sessionName, simdjson::simdjson_result<simdjson::dom::element>& jsonResult) = 0;
};

#endif // __WEB_WEBSOCKETCALLBACKSIMPL__
