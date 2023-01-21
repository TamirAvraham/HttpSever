#pragma once
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
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
		Http400 = 400,
		HttpNoError=1,
	};

	class HttpTokenizer
	{
	public:
		HttpTokenizer(std::string request);
		std::string GetBody();
		std::string GetRoute();
		HttpRequestType GetType();
		HttpErrorType GetError();
	private:
		std::string _header;
		std::string _body;
		std::string _route;
		HttpRequestType _requestType;
		HttpErrorType _error;

		void parse(std::string req);
		void getBody(std::string req);
		HttpRequestType StringToHttpRequestType(std::string requestType);
	};
};

