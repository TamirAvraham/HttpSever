#include "TcpServer.h"
#include "HtmlFileReader.h"
#include "HttpParser.h"
#include "JsonValue.h"
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
	
	http::json::JsonValue jv(http::json::JsonType::Array, "[1,2.2,true,\"str\",[1,2,3],{ uwu:\"testing\"}\n");
	auto vec = jv.getValue<std::vector<http::json::JsonValue>>();
	return 0;
}