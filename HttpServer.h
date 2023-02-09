#pragma once


#include<string>
#include<set>
#include<vector>

#include "HttpParser.h"
#include "HttpStatus.h"
#include "JsonObject.h"
#include "TcpServer.h"
#include "ThreadPool.h"
#include "HttpSocket.h"

namespace http {
	struct HttpRouteParam
	{
		std::string _paramName;
		std::string _paramValue;
		inline HttpRouteParam(std::string name, std::string value):_paramName(name),_paramValue(value) {}
	};


	class HttpContext {

	public:
		HttpContext(std::string body,std::vector<HttpRouteParam> params,SOCKET sock);
		std::string GetParam(std::string paramName)const;

		std::string GetBody()const noexcept;
		json::JsonObject GetBodyAsJson()const noexcept;

		void sendJson(http::HttpStatus status,http::json::JsonObject jsonObject,http::HttpHeaders headers);

		void sendHtml(http::HttpStatus status,http::HtmlFileReader htmlfile,http::HttpHeaders headers);

	private:
		std::string _body;
		std::vector<HttpRouteParam> _params;
		http::HttpSocket _sock;
	};

	struct HttpRoute
	{
		HttpRequestType _type;
		std::string _route;
		std::function<void(HttpContext)> _handler;
	};
	
	class HttpServer:protected tcp::TcpServer
	{
	public:
		HttpServer(int port, std::string ip);
		void HandleRoute(HttpRoute);
		

	private:
		void ConnHandler(SOCKET sock);
		HttpContext getContextFromReq(std::string req, SOCKET sock);
		std::pair<bool,std::vector<HttpRouteParam>> getParamsFromRoute(std::string route, std::string templateRoute)const;
		std::vector<HttpRouteParam> matchRoute(std::string gotRoute, http::HttpRequestType reqType);
		
		/*std::vector<http::HttpRouteParam> getRouteParams(std::string route,std::string parttern)const;*/
		std::set<HttpRoute> _routes;
		ThreadPool _threadPool;
	};
}


