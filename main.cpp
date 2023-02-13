#include "TcpServer.h"
#include "JsonObject.h"
#include "ThreadPool.h"
#include "HttpSocket.h"
#include "HttpServer.h"
#include <iostream>

void testFuncForTCPServer(SOCKET sock) {
	http::HttpSocket mySocket(sock);
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
})";
	/*"{\"data1\":[1,2.2,true,\"str\",[1,2,3],{ uwu:\"testing\"}],\"data2\":{\"test\":[6,7,8]}}"*/
	http::json::JsonObject json = std::string(jsonStr);
	mySocket.bindMsg(http::HttpStatus::OK,json);
	

}

int main() {

	/*tcp::TcpServer server(8080, "127.0.0.1");
	
	while (true)
	{
		server.HandleConection(testFuncForTCPServer);
	}
	
	http::json::JsonValue jv(http::json::JsonType::Array, "[1,2.2,true,\"str\",[1,2,3],{ uwu:\"testing\"}]\n");*/
	


	/*ThreadPool pool(10);
	auto fut1 = pool.async([]() {
		std::this_thread::sleep_for(std::chrono::seconds(3));
		return 8 * 8; 
	});
	auto fut2 = pool.async([]() { return 9 * 8; });
	fut1.wait();
	fut2.wait();
	std::cout << fut1.get()<<"\n\n\n\n\n"<<fut2.get();
	pool.cancel();
	return 0;*/
	http::HttpServer server(8080, "127.0.0.1");
	server.HandleRoute(http::HttpGET, { "/:id",[](http::HttpServer::HttpContext context) {
		std::cout << context.GetParam("id");
	} });
	server.serve();
}
/*
* 
*/