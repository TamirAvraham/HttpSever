#include "HttpParser.h"

http::HttpTokenizer::HttpTokenizer(std::string request)
{
	std::cout << request << '\n';
	_error = HttpNoError;
	parse(request);
}

std::string http::HttpTokenizer::GetBody()
{
	return _body;
}

std::string http::HttpTokenizer::GetRoute()
{
	return _route;
}

http::HttpRequestType http::HttpTokenizer::GetType()
{
	return _requestType;
}

http::HttpErrorType http::HttpTokenizer::GetError()
{
	return _error;
}

void http::HttpTokenizer::parse(std::string req)
{
	std::stringstream ss(req);
	std::getline(ss, _header);

	std::stringstream headerAsString(_header);
	std::string type,route;

	headerAsString >> type;
	headerAsString >> route;
	std::cout << type<<'\n'<<route<<'\n';

	_route = route;
	try
	{
		_requestType = StringToHttpRequestType(type);
	}
	catch (HttpErrorType error)
	{
		_error = error;
	}
	getBody(req);
}

void http::HttpTokenizer::getBody(std::string req)
{
	auto isHtml = req.find("html");
	auto isJson = req.find("json");
	//data is json
	if (isHtml == std::string::npos && isJson != std::string::npos) 
	{
		std::cout << "is json" << '\n';
		size_t startOfData = req.find('{');
		size_t endOfData = req.find('}');
		if (startOfData == std::string::npos || endOfData == std::string::npos) {
			std::cout << "no data" << '\n';
			_error = Http400;
			return;
		}
		_body = req.substr(startOfData, endOfData);
		return;
	}
	//data is html
	if (isHtml != std::string::npos && isJson == std::string::npos) {
		std::cout << "is html" << '\n';
		size_t startOfData = req.find("<");
		size_t endOfData = req.find_last_of(">");
		if (startOfData == std::string::npos || endOfData == std::string::npos) {
			std::cout << "no data" << '\n';
			_error = Http400;
			return;
		}
		_body = req.substr(startOfData, endOfData);
		return;
	}
	_error = Http400;
}

http::HttpRequestType http::HttpTokenizer::StringToHttpRequestType(std::string requestType)
{
	if (requestType=="GET")
	{
		return HttpGET;
	}
	if (requestType=="PUT")
	{
		return HttpPUT;
	}
	if (requestType=="POST")
	{
		return HttpPOST;
	}
	if (requestType=="DELETE")
	{
		return HttpDELETE;
	}
	if (requestType=="OPTIONS")
	{
		return HttpOPTIONS;
	}
	throw Http400;
}

