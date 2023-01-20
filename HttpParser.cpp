#include "HttpParser.h"

http::HttpParser::HttpParser(std::string request)
{
	parse(request);
}

void http::HttpParser::parse(std::string req)
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
		_requestType = HttpRequestTypeToString(type);
	}
	catch (HttpErrorType error)
	{
		_error = error;
	}
}

http::HttpRequestType http::HttpParser::HttpRequestTypeToString(std::string requestType)
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
