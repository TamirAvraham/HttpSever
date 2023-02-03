#pragma once


#include<string>
#include<set>
#include<vector>

#include "HttpParser.h"
#include "HttpStatus.h"
#include "JsonObject.h"
#include "TcpServer.h"
#include "ThreadPool.h"


namespace http {
	struct HttpRouteParam
	{
		std::string _paramName;
		std::string _paramValue;
	};
	class HttpContext {
	public:
		std::string GetParam(std::string paramName);
		std::string GetBody();
		json::JsonObject GetBodyAsJson();
	private:
		std::string _body;
		std::vector<HttpRouteParam> _params;
		
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
		ThreadPool _threadPool;
	};
}


