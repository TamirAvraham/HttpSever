#include "TcpServer.h"
#include "HtmlFileReader.h"
#include "HttpParser.h"
#include "JsonValue.h"
#include "JsonParser.h"
#include <iostream>

void testFuncForTCPServer(SOCKET sock) {
	http::simpleSocket mySocket(sock);
	
	http::HttpTokenizer parser(mySocket.read(3000));

}

int main() {

	/*http::TcpServer server(8080, "127.0.0.1");
	server.HandleConection(testFuncForTCPServer);
	while (true)
	{

	}*/
	
	http::json::JsonValue jv(http::json::JsonType::Array, "[1,2.2,true,\"str\",[1,2,3],{ uwu:\"testing\"}]\n");
	http::json::JsonParser::parse("{\"data1\":[1,2.2,true,\"str\",[1,2,3],{ uwu:\"testing\"}],\"data2\":{\"test\":[6,7,8]}}");
	return 0;
}