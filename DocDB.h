#include "lmdb/lmdb.h"
#include <filesystem>
#include <string>
#include <stdexcept>

class DB {
public:
    // Constructor with reference to the path of the database
    explicit DB(const std::string& path) noexcept(false) {
        openEnv(path);
        
    }

    // Constructor with rvalue reference to the path of the database
    explicit DB(std::string&& path) noexcept(false) {
        openEnv(std::move(path));
        
    }

    // Constructor with just a std::string path to the database
    explicit DB(const char* path) noexcept(false) {
        openEnv(path);
        
    }

    // Default constructor with a default path to the database
    explicit DB() noexcept(false) {
        const std::string defaultPath = "./myLMDB";
        openEnv(defaultPath);
        
    }

private:
    MDB_env* _env = nullptr;
    






    inline void openEnv(const std::string& path) noexcept(false) {
        int rc = mdb_env_create(&_env);
        if (rc != MDB_SUCCESS) {
            throw std::runtime_error("Failed to create LMDB environment");
        }

        rc = mdb_env_set_mapsize(_env, 10485760); // Set map size to 10 MB
        if (rc != MDB_SUCCESS) {
            mdb_env_close(_env);
            throw std::runtime_error("Failed to set LMDB map size");
        }

        rc = mdb_env_set_maxdbs(_env, 4); // Set maximum number of named databases to 4
        if (rc != MDB_SUCCESS) {
            mdb_env_close(_env);
            throw std::runtime_error("Failed to set LMDB max number of named databases");
        }

        // Create directory for the database if it does not exist
        const std::filesystem::path dirPath = std::filesystem::path(path).parent_path();
        if (!std::filesystem::exists(dirPath)) {
            std::filesystem::create_directories(dirPath);
        }

        rc = mdb_env_open(_env, path.c_str(), MDB_WRITEMAP | MDB_CREATE, 0664);
        if (rc != MDB_SUCCESS) {
            mdb_env_close(_env);
            throw std::runtime_error("Failed to open LMDB environment");
        }
    }
    inline struct { MDB_dbi& _d; MDB_txn* _t; } openDb(std::string& name) noexcept(false) {
        MDB_dbi _db;
        MDB_txn* _txn;
            const int flags = MDB_CREATE;
            if (mdb_txn_begin(_env, nullptr, 0, &_txn) != 0) {
                throw std::runtime_error("Failed to begin LMDB transaction");
            }
            if (mdb_dbi_open(_txn, name.c_str(), flags, &_db) != 0) {
                throw std::runtime_error("Failed to open LMDB database");
            }
            if (mdb_txn_commit(_txn) != 0) {
                throw std::runtime_error("Failed to commit LMDB transaction");
            }
            return { _db,_txn };
    }
    
};
