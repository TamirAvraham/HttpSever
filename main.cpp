#include "TcpServer.h"
#include "JsonObject.h"
#include "ThreadPool.h"
#include "HttpSocket.h"
#include "HttpServer.h"
#include "HtmlFileReader.h"
#include "DocDB.h"
#include <iostream>


int main() {
	DB db;
	try
	{
		auto collection = db.createCollection("testCollection");
		1 == 1;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
	

	return 0;
}
