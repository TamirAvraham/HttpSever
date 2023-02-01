#include "TcpServer.h"
#include "HtmlFileReader.h"
#include "HttpParser.h"
#include "JsonObject.h"
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
	const char* jsonStr = R"(
{
    "name": "John Smith",
    "age": 35,
    "isEmployed": true,
    "address": {
        "street": "123 Main St",
        "city": "Anytown",
        "state": "Anystate",
        "zipcode": 12345
    },
    "phoneNumbers": [
        {"type": "home", "number": "555-555-5555"},
        {"type": "work", "number": "555-555-5556"}
    ]
}
)";
	//"{\"data1\":[1,2.2,true,\"str\",[1,2,3],{ uwu:\"testing\"}],\"data2\":{\"test\":[6,7,8]}}"
	http::json::JsonObject json = std::string(jsonStr);
	std::cout << json.ToString() << '\n';
	return 0;
}