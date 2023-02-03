#pragma once


#include<string>
#include<set>


#include "HttpParser.h"
#include "HttpStatus.h"
#include "JsonObject.h"
#include "TcpServer.h"
#include "ThreadPool.h"


namespace http {
	struct HttpContext {

	};
	struct HttpRoute
	{
		HttpRequestType _type;
		std::string _route;
		std::function<void(HttpContext)> _handler;

	};
	
	class HttpServer:protected tcp::TcpServer
	{
		HttpServer(int port, std::string ip);


	private:
		std::set<HttpRoute> routes;
	};
}


