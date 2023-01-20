#pragma once
#include <string>
#include <sstream>
#include "TcpServer.h"



namespace http {

	enum HttpRequestType
	{
		HttpGET = 0,
		HttpPUT = 1,
		HttpPOST = 2,
		HttpDELETE = 3,
		HttpOPTIONS = 4
	};
	enum HttpErrorType
	{
		Http400 = 400
	};

	class HttpParser
	{
	public:
		HttpParser(std::string request);
	private:
		std::string _header;
		std::string _body;
		std::string _route;
		HttpRequestType _requestType;
		HttpErrorType _error;
		void parse(std::string req);
		HttpRequestType HttpRequestTypeToString(std::string requestType);
	};
};

