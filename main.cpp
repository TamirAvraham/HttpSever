#include "TcpServer.h"
#include "HtmlFileReader.h"
#include "HttpParser.h"
#include <iostream>

void testFuncForTCPServer(SOCKET sock) {
	http::simpleSocket mySocket(sock);
	
	http::HttpTokenizer parser(mySocket.read(3000));

}

int main() {

	http::TcpServer server(8080, "127.0.0.1");
	server.HandleConection(testFuncForTCPServer);
	while (true)
	{

	}
	return 0;
}