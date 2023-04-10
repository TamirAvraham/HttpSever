#include "lmdb/lmdb.h"
#include <stdexcept>
#include <string>
class DB;
class Collection {
    
protected :
    // Constructor with rvalue reference to the name of the collection
    explicit Collection(std::string&& name, MDB_dbi& db, MDB_env* env) noexcept(false);

    // Constructor with reference to the name of the collection
    explicit Collection(const std::string& name, MDB_dbi& db, MDB_env* env) noexcept(false);

    // Constructor with just a std::string name for the collection
    explicit Collection(const char* name, MDB_dbi& db, MDB_env* env) noexcept(false);

    // Destructor to close transaction
    ~Collection() noexcept(false);

private:
    MDB_dbi& _db;
    MDB_txn* _txn;
    std::string _name;


    friend class DB;
};

// Implementation of rvalue reference constructor
inline Collection::Collection(std::string&& name, MDB_dbi& db,MDB_env* env) noexcept(false) : _db(db), _name(name) {
    
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
