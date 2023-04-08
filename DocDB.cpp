#include "DocDB.h"
#include "rocksdb/db.h"
#include <stdexcept>
#include <string>

 DocDB::Document DocDB::DB::getDoc(const std::string& collectionName,rocksdb::ColumnFamilyHandle* handle, const std::string& docName) const noexcept(false)
{
	 std::string docValueAsString = "";
	 rocksdb::ReadOptions readOptions;
	 rocksdb::Slice key(docName);
	 rocksdb::Status stat;
	 stat = _db->Get(readOptions, handle, key, &docValueAsString);
	 if (stat.ok()&&docValueAsString!="")
	 {
		 return Document(docName, collectionName, docValueAsString);
	 }
	 else if (stat.IsNotFound())
	 {
		 throw NotFound();
	 }
	 throw DBExeption();
}



 void DocDB::DB::updateDoc(const Document& doc)
 {
	 getCollection(doc.getCollectionName());


 }

rocksdb::ColumnFamilyHandle* DocDB::DB::openCollection(const std::string& collectionName) const noexcept(false)
{
	rocksdb::ColumnFamilyOptions options;
	rocksdb::ColumnFamilyHandle* ret=nullptr;
	rocksdb::Status stat = _db->CreateColumnFamily(options, collectionName, &ret);
	if (!stat.ok())
	{
		throw std::runtime_error(std::string("Error getting column"));
	}

	return ret;
}

DocDB::Collection DocDB::DB::getCollection(const std::string& collectionName) 
{
	return Collection(collectionName, (*this), openCollection(collectionName));
}

DocDB::Collection DocDB::DB::getCollection(const std::string&& collectionName)
{
	return Collection(collectionName, (*this), openCollection(collectionName));
}

DocDB::Collection DocDB::DB::createCollection(const std::string& collectionName)
{
	return Collection(collectionName, (*this), openCollection(collectionName));
}

DocDB::Collection DocDB::DB::createCollection(const std::string&& collectionName)
{
	return Collection(collectionName, (*this), openCollection(collectionName));
}

bool DocDB::DB::deleteCollection(const std::string& collectionName)
{
	auto collectionToDelete = openCollection(collectionName);
	rocksdb::Status stat = _db->DropColumnFamily(collectionToDelete);
	if (!stat.ok())
	{
		throw std::runtime_error(std::string("Error deleting column"));
	}
	return true;
}

DocDB::Collection::Collection(const std::string& name, DB& db, rocksdb::ColumnFamilyHandle* handle) noexcept :_name(name),_db(db), _columnHandle(handle)
{
}

DocDB::Collection::Collection(const std::string& name, DB& db):_name(name),_db(db)
{
	_columnHandle = _db.openCollection(name);
}

DocDB::Document DocDB::Collection::getDoc(const std::string& name) const noexcept(false)
{
	return _db.getDoc(_name,_columnHandle, name);
}








std::string DocDB::Document::getCollectionName() const noexcept
{
	return _collectionName;
}

std::string DocDB::Document::getName() const noexcept
{
	return _name;
}

DocDB::Document::Document(const std::string& name, const std::string& collectionName):_name(name), _collectionName(collectionName)
{
	insert({ std::string("name"),JsonValue(http::json::getTypeFromString(name),name) });
}

DocDB::Document::Document(const std::string&& name, const std::string&& collectionName, const std::string&& value)
{
}

DocDB::Document::Document(const std::string& name, const std::string& collectionName, const std::string& value):JsonObject(value),_name(name),_collectionName(collectionName)
{
	insert({ std::string("name"),JsonValue(http::json::getTypeFromString(name),name) });
}

DocDB::Document::Document(const std::string&& name, const std::string&& collectionName, const http::json::JsonObject&& value):_name(name),_collectionName(collectionName),JsonObject(value)
{
}

DocDB::Document::Document(const std::string& name, const std::string& collectionName, const http::json::JsonObject& value)
	:JsonObject(value), _name(name), _collectionName(collectionName)
{
	insert({ std::string("name"),JsonValue(http::json::getTypeFromString(name),name) });
}


