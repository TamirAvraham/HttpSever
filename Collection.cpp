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
        throw db::exceptions::CantOpenException("cant open collection");
    }
}

// Implementation of reference constructor
Collection::Collection(const std::string& name, MDB_dbi db, MDB_env* env) noexcept(false) : _db(db), _name(name) {
    if (mdb_txn_begin(env, nullptr, 0, &_txn) != 0) {
        throw db::exceptions::CantOpenException("cant open collection");
    }
}



// Implementation of const char* constructor
Collection::Collection(const char* name, MDB_dbi db, MDB_env* env) noexcept(false) : _db(db), _name(name) {
    if (mdb_txn_begin(env, nullptr, 0, &_txn) != 0) {
        throw db::exceptions::CantOpenException("cant open collection");
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


Document Collection::createDocument(const std::string&& docName) const noexcept
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


