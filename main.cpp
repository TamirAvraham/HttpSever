#include "TcpServer.h"
#include "JsonObject.h"
#include "ThreadPool.h"
#include "HttpSocket.h"
#include "HttpServer.h"
#include "HtmlFileReader.h"
#include "DocDB.h"
#include <iostream>


int main() {
	db::doc::DB db;
	try
	{
		constexpr auto collectionName = "second collection";
		auto collection = db.createCollection(collectionName);

		auto doc1=collection.createDocument("doc 1");

		auto doc1PreChange = doc1;

		doc1.insert({ "doc number",{"1"} });
		doc1.insert({ "random value",{"true"} });
		doc1.insert({ "my name",{"Tamir"} });


		collection.updateDocument(doc1);

		doc1 = collection.getDocument("doc 1");

		if (doc1.ToString()!=doc1PreChange.ToString())
		{
			std::cout << "tested 1 passed->\n"<< doc1.ToString() <<std::endl;
		}
		else
		{
			std::cout << "there is an error in updating the document and getting it";
		}

		auto doc2 = collection.createDocument("doc 2");

		auto doc2PreChange = doc2;

		doc2.insert({ "doc number",{"2"} });
		doc2.insert({ "random value",{"false"} });
		doc2.insert({ "this is unimportant value",{"logo"} });

		collection.updateDocument(doc2);

		db::query::Query query;
		query.location.collections.push_back(collectionName);
		query.data.dataFileds.push_back("doc number");
		query.data.dataFileds.push_back("random value");

		auto queryResult = db.Query(query);

		std::cout << "test 2 passed"<<std::endl;

		auto collections=db.getAllCollections();
		std::cout << "got collections" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
	

	return 0;
}
