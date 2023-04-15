
#include <lmdb.h>
#include <stdexcept>
#include <string>
#include "Document.h"
class DB;
class Collection {
    
public :
    // Constructor with rvalue reference to the name of the collection
    explicit Collection(const std::string && name, MDB_dbi& db, MDB_env* env) noexcept(false);

    // Constructor with reference to the name of the collection
    explicit Collection(const std::string& name, MDB_dbi& db, MDB_env* env) noexcept(false);
    
    // Constructor with just a std::string name for the collection
    explicit Collection(const char* name, MDB_dbi& db, MDB_env* env) noexcept(false);

    // Destructor to close transaction
    ~Collection() noexcept(false);


    std::string getName()const noexcept;

    Document getDocument(const std::string&& docName)const;
    Document getDocument(const std::string& docName)const;
    Document getDocument(const char* docName)const;

    Document createDocument(const std::string&& docName)const noexcept;
    Document createDocument(const std::string& docName)const noexcept;
    Document createDocument(const char* docName)const noexcept;
    Document createDocument(const std::string&& docName,const http::json::JsonObject&& docContent)const noexcept;
    Document createDocument(const std::string&& docName,const http::json::JsonObject& docContent)const noexcept;
    Document createDocument(const std::string& docName,const http::json::JsonObject&& docContent)const noexcept;
    Document createDocument(const std::string& docName,const http::json::JsonObject& docContent)const noexcept;
    Document createDocument(const char* docName,const http::json::JsonObject&& docContent)const noexcept;
    Document createDocument(const char* docName,const http::json::JsonObject& docContent)const noexcept;
    


    bool deleteDocument(const std::string&& docName)const noexcept;
    bool deleteDocument(const std::string& docName)const noexcept;
    bool deleteDocument(const char* docName)const noexcept;

    Document updateDocument(const Document& docToUpdate)const noexcept;
    
    

private:
    MDB_dbi& _db;
    MDB_txn* _txn;
    std::string _name;


    
};

// Implementation of rvalue reference constructor
inline Collection::Collection(const std::string && name, MDB_dbi& db,MDB_env* env) noexcept(false) : _db(db), _name(name) {
    
    if (mdb_txn_begin(env, nullptr, 0, &_txn) != 0) {
        throw std::runtime_error("Failed to begin LMDB transaction");
    }
}

// Implementation of reference constructor
inline Collection::Collection(const std::string& name, MDB_dbi& db, MDB_env* env) noexcept(false) : _db(db), _name(name) {
    if (mdb_txn_begin(env, nullptr, 0, &_txn) != 0) {
        throw std::runtime_error("Failed to begin LMDB transaction");
    }
}



// Implementation of const char* constructor
inline Collection::Collection(const char* name, MDB_dbi& db, MDB_env* env) noexcept(false) : _db(db), _name(name) {
    if (mdb_txn_begin(env, nullptr, 0, &_txn) != 0) {
        throw std::runtime_error("Failed to begin LMDB transaction");
    }
}

// Implementation of destructor
inline Collection::~Collection() noexcept(false) {
    if (_txn != nullptr) {
        if (mdb_txn_commit(_txn) != 0) {
            throw std::runtime_error("Failed to commit LMDB transaction");
        }
    }
    
}

inline std::string Collection::getName() const noexcept
{
    return _name;
}
