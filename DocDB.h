#pragma once
#include <lmdb.h>
#include <filesystem>
#include <string>
#include <stdexcept>
#include "Collection.h"

constexpr int defultMemSize = 10485760;
constexpr int defultMemUpdateSize = defultMemSize;

/**
 * @brief A struct that stores settings for the database.
 *
 * This struct contains various settings for the database, such as the starting memory size, the memory update size, the number of collections, and whether the number of collections is fixed or not. Additionally, it includes flags to indicate whether to read the settings from the database and save settings to the database after changes have been made.
 */
struct DbSettings
{
    long _startMemorySize = defultMemSize; ///< The starting memory size for the database.
    long _MemUpdateSize = defultMemUpdateSize; ///< The amount by which to increase the memory size when it runs out of memory.
    int _NumberOfCollections = 0; ///< The number of collections in the database.
    bool _fixedNumberOfCollections; ///< Whether the number of collections is fixed or not.
    bool _readSettingsFromDb = true; ///< Whether to read the settings from the database.
    bool _saveSettingsInDBAfterChange = true; ///< Whether to save the settings to the database after changes have been made.
};


class DB {
public:
    //ctors

    explicit DB(const std::string& path, DbSettings& settings) noexcept(false);
    explicit DB(std::string&& path, DbSettings& settings) noexcept(false);
    explicit DB(const char* path, DbSettings& settings) noexcept(false);
    explicit DB() noexcept(false);

    //settings related

    DbSettings getSettings()const noexcept;
    DbSettings setSettings(DbSettings settings)noexcept;


    /*TODO:
    * add Collection Not Found error
    */
    Collection openCollection(const std::string name);
    Collection openCollection(const std::string& name);
    Collection openCollection(const std::string&& name);

private:

    //ctor helper for env

    inline void openEnv(const std::string& path, DbSettings& settings) noexcept(false);

    //private function to open a db and a txn for it

    inline  std::pair<MDB_dbi&, MDB_txn*> openDb(const std::string& name)const noexcept(false);
    
    //fields

    MDB_env* _env = nullptr;
    DbSettings _settings;
    long _currentMemory;


    //ctors settings helpers

    inline void readDbSettings();
    inline void saveSettings();


    //runtime settings related

    void incMemory();
    void incCollections();
    void decCollections();
};
