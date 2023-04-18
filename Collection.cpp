#include "Collection.h"
#include "DBExeptions.hpp"


/*TODO:
* edge cases:
* bad txn
* out of mem
* 
* error handling:
* switch case of codes to create a good error handling
*/





Collection::Collection(const std::string&& name, MDB_dbi db, MDB_env* env) noexcept(false) : _db(db), _name(name) {

    if (mdb_txn_begin(env, nullptr, 0, &_txn) != 0) {
        throw db::exceptions::doc::CollectionCantOpenException("cant open collection");
    }
}

// Implementation of reference constructor
Collection::Collection(const std::string& name, MDB_dbi db, MDB_env* env) noexcept(false) : _db(db), _name(name) {
    if (mdb_txn_begin(env, nullptr, 0, &_txn) != 0) {
        throw db::exceptions::doc::CollectionCantOpenException("cant open collection");
    }
}



// Implementation of const char* constructor
Collection::Collection(const char* name, MDB_dbi db, MDB_env* env) noexcept(false) : _db(db), _name(name) {
    if (mdb_txn_begin(env, nullptr, 0, &_txn) != 0) {
        throw db::exceptions::doc::CollectionCantOpenException("cant open collection");
    }
}

// Implementation of destructor
Collection::~Collection() noexcept(false) {
    if (_txn != nullptr) {
        if (mdb_txn_commit(_txn) != 0) {
            throw db::exceptions::DbException("cant open collection");
        }
    }

}

inline std::string Collection::getName() const noexcept
{
    return _name;
}

Document Collection::getDocument(const std::string&& docName) const
{
    MDB_val docNameAsMDBKey{ docName.size(), const_cast<char*>(docName.data()) };
    MDB_val jsonStrAsMDBVal{ 0, nullptr };
    int result = mdb_get(_txn, _db, &docNameAsMDBKey, &jsonStrAsMDBVal);
    if (result== MDB_NOTFOUND)
    {
        throw db::exceptions::doc::DocumentNotFoundException("cant find doc");
    }
    else if (result!=MDB_SUCCESS || jsonStrAsMDBVal.mv_data==0)
    {
        
        throw db::exceptions::doc::DocumentCantOpenException("cant open doc");
    }
    auto json_str = std::string{ static_cast<const char*>(jsonStrAsMDBVal.mv_data) };
    return Document(docName, json_str);
}

Document Collection::getDocument(const std::string& docName) const
{
    MDB_val docNameAsMDBKey{ docName.size(), const_cast<char*>(docName.data()) };
    MDB_val jsonStrAsMDBVal{ 0, nullptr };
    int result = mdb_get(_txn, _db, &docNameAsMDBKey, &jsonStrAsMDBVal);
    if (result == MDB_NOTFOUND)
    {
        throw db::exceptions::doc::DocumentNotFoundException("cant find doc");
    }
    else if (result != MDB_SUCCESS || jsonStrAsMDBVal.mv_data == 0)
    {

        throw db::exceptions::doc::DocumentCantOpenException("cant open doc");
    }
    auto json_str = std::string{ static_cast<const char*>(jsonStrAsMDBVal.mv_data) };
    return Document(docName, json_str);
}

Document Collection::getDocument(const char* docName) const
{
    std::string docNameAsStr = docName;
    MDB_val docNameAsMDBKey{ docNameAsStr.size(), const_cast<char*>(docNameAsStr.data()) };
    MDB_val jsonStrAsMDBVal{ 0, nullptr };
    int result = mdb_get(_txn, _db, &docNameAsMDBKey, &jsonStrAsMDBVal);
    if (result == MDB_NOTFOUND)
    {
        throw db::exceptions::doc::DocumentNotFoundException("cant find doc");
    }
    else if (result != MDB_SUCCESS || jsonStrAsMDBVal.mv_data == 0)
    {

        throw db::exceptions::doc::DocumentCantOpenException("cant open doc");
    }
    auto json_str = std::string{ static_cast<const char*>(jsonStrAsMDBVal.mv_data) };
    return Document(docName, json_str);
}


Document Collection::createDocument(const std::string&& docName) const 
{
    MDB_val docNameAsMDBKey{ docName.size(), const_cast<char*>(docName.data()) };
    MDB_val jsonStrAsMDBVal{ 0, nullptr };
    int result = mdb_put(_txn, _db, &docNameAsMDBKey, &jsonStrAsMDBVal,0);
    if (result!=MDB_SUCCESS)
    {
        throw db::exceptions::DbException("cant create doc");
    }
    return Document(docName);
}

Document Collection::createDocument(const std::string& docName) const 
{
    MDB_val docNameAsMDBKey{ docName.size(), const_cast<char*>(docName.data()) };
    MDB_val jsonStrAsMDBVal{ 0, nullptr };
    int result = mdb_put(_txn, _db, &docNameAsMDBKey, &jsonStrAsMDBVal, 0);
    if (result != MDB_SUCCESS)
    {
        throw db::exceptions::DbException("cant create doc");
    }
    return Document(docName);
}

Document Collection::createDocument(const char* docName) const 
{
    std::string docNameAsString = docName;
    MDB_val docNameAsMDBKey{ docNameAsString.size(), const_cast<char*>(docNameAsString.data()) };
    MDB_val jsonStrAsMDBVal{ 0, nullptr };
    int result = mdb_put(_txn, _db, &docNameAsMDBKey, &jsonStrAsMDBVal, 0);
    if (result != MDB_SUCCESS)
    {
        throw db::exceptions::DbException("cant create doc");
    }
    return Document(docName);
}

Document Collection::createDocument(const std::string&& docName, const http::json::JsonObject&& docContent) const 
{
    MDB_val docNameAsMDBKey{ docName.size(), const_cast<char*>(docName.data()) };
    auto tempJsonAsStrVal = docContent.ToString();
    MDB_val jsonStrAsMDBVal{ tempJsonAsStrVal.size(), const_cast<char*>(tempJsonAsStrVal.data())};
    int result = mdb_put(_txn, _db, &docNameAsMDBKey, &jsonStrAsMDBVal, 0);
    if (result != MDB_SUCCESS)
    {
        throw db::exceptions::DbException("cant create doc");
    }
    return Document(docName);
}

Document Collection::createDocument(const std::string&& docName, const http::json::JsonObject& docContent) const 
{
    MDB_val docNameAsMDBKey{ docName.size(), const_cast<char*>(docName.data()) };
    auto tempJsonAsStrVal = docContent.ToString();
    MDB_val jsonStrAsMDBVal{ tempJsonAsStrVal.size(), const_cast<char*>(tempJsonAsStrVal.data()) };
    int result = mdb_put(_txn, _db, &docNameAsMDBKey, &jsonStrAsMDBVal, 0);
    if (result != MDB_SUCCESS)
    {
        throw db::exceptions::DbException("cant create doc");
    }
    return Document(docName);
}

Document Collection::createDocument(const std::string& docName, const http::json::JsonObject&& docContent) const 
{
    MDB_val docNameAsMDBKey{ docName.size(), const_cast<char*>(docName.data()) };
    auto tempJsonAsStrVal = docContent.ToString();
    MDB_val jsonStrAsMDBVal{ tempJsonAsStrVal.size(), const_cast<char*>(tempJsonAsStrVal.data()) };
    int result = mdb_put(_txn, _db, &docNameAsMDBKey, &jsonStrAsMDBVal, 0);
    if (result != MDB_SUCCESS)
    {
        throw db::exceptions::DbException("cant create doc");
    }
    return Document(docName);
}

Document Collection::createDocument(const std::string& docName, const http::json::JsonObject& docContent) const 
{
    MDB_val docNameAsMDBKey{ docName.size(), const_cast<char*>(docName.data()) };
    auto tempJsonAsStrVal = docContent.ToString();
    MDB_val jsonStrAsMDBVal{ tempJsonAsStrVal.size(), const_cast<char*>(tempJsonAsStrVal.data()) };
    int result = mdb_put(_txn, _db, &docNameAsMDBKey, &jsonStrAsMDBVal, 0);
    if (result != MDB_SUCCESS)
    {
        throw db::exceptions::DbException("cant create doc");
    }
    return Document(docName);
}

Document Collection::createDocument(const char* docName, const http::json::JsonObject&& docContent) const 
{

    std::string docNameAsString = docName;
    MDB_val docNameAsMDBKey{ docNameAsString.size(), const_cast<char*>(docNameAsString.data()) };
    auto tempJsonAsStrVal = docContent.ToString();
    MDB_val jsonStrAsMDBVal{ tempJsonAsStrVal.size(), const_cast<char*>(tempJsonAsStrVal.data()) };
    int result = mdb_put(_txn, _db, &docNameAsMDBKey, &jsonStrAsMDBVal, 0);
    if (result != MDB_SUCCESS)
    {
        throw db::exceptions::DbException("cant create doc");
    }
    return Document(docName,docContent);
}

Document Collection::createDocument(const char* docName, const http::json::JsonObject& docContent) const 
{
    std::string docNameAsString = docName;
    MDB_val docNameAsMDBKey{ docNameAsString.size(), const_cast<char*>(docNameAsString.data()) };
    auto tempJsonAsStrVal = docContent.ToString();
    MDB_val jsonStrAsMDBVal{ tempJsonAsStrVal.size(), const_cast<char*>(tempJsonAsStrVal.data()) };
    int result = mdb_put(_txn, _db, &docNameAsMDBKey, &jsonStrAsMDBVal, 0);
    if (result != MDB_SUCCESS)
    {
        throw db::exceptions::DbException("cant create doc");
    }
    return Document(docName,docContent);
}

bool Collection::deleteDocument(const std::string&& docName) const noexcept
{
    MDB_val docNameAsMDBKey{ docName.size(), const_cast<char*>(docName.data()) };
    int result = mdb_del(_txn, _db, &docNameAsMDBKey, nullptr);
    if (result != MDB_SUCCESS) {
        return false;
    }
    return true;
}

bool Collection::deleteDocument(const std::string& docName) const noexcept
{
    MDB_val docNameAsMDBKey{ docName.size(), const_cast<char*>(docName.data()) };
    int result = mdb_del(_txn, _db, &docNameAsMDBKey, nullptr);
    if (result != MDB_SUCCESS) {
        return false;
    }
    return true;
}

bool Collection::deleteDocument(const char* docName) const noexcept
{
    std::string docNameAsString = docName;
    MDB_val docNameAsMDBKey{ docNameAsString.size(), const_cast<char*>(docNameAsString.data()) };
    int result = mdb_del(_txn, _db, &docNameAsMDBKey, nullptr);
    if (result != MDB_SUCCESS) {
        return false;
    }
    return true;
}

Document Collection::updateDocument(const Document& docToUpdate) const 
{
    std::string docName = docToUpdate.getName();
    std::string docValueAsString = docToUpdate.ToString();
    MDB_val mdbKey{ docName.size(), const_cast<char*>(docName.data())};
    MDB_val mdbValue{ docValueAsString.size(), const_cast<char*>(docValueAsString.data()) };

    int result = mdb_put(_txn, _db, &mdbKey, &mdbValue, MDB_CURRENT);

    if (result != MDB_SUCCESS) {
        throw db::exceptions::DbException("error in updating a doc");
    }

    return docToUpdate;
}


