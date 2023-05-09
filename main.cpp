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
		auto collection = db.getSettings()._NumberOfCollections;
		std::cout << "amount of collections are " << collection << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
	

	return 0;
}
