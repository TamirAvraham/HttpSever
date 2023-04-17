#include "DocDB.h"




DB::DB(const std::string& path, DbSettings& settings) noexcept(false)
{
    openEnv(path, settings);
}

DB::DB(std::string&& path, DbSettings& settings) noexcept(false)
{
    openEnv(std::move(path), settings);
}

DB::DB(const char* path, DbSettings& settings) noexcept(false)
{
    openEnv(path, settings);
}

DB::DB() noexcept(false) {
    const std::string defaultPath = "./myLMDB";
    auto settings = DbSettings();
    openEnv(defaultPath, settings);
}

DbSettings DB::getSettings() const noexcept
{
    return _settings;
}

DbSettings DB::setSettings(DbSettings settings) noexcept
{
    _settings = settings;
    if (_settings._saveSettingsInDBAfterChange)
    {
        saveSettings();
    }
    return DbSettings();
}

Collection DB::openCollection(const std::string name) 
{

    auto [db,txn] = openDb(name);
    Collection ret(name, db, _env);
    if (txn) {
        mdb_txn_abort(txn);
    }
    return ret;
}

Collection DB::openCollection(const std::string& name)
{
    auto [db, txn] = openDb(name);
    Collection ret(name, db, _env);
    if (txn) {
        mdb_txn_abort(txn);
    }
    return ret;
}

Collection DB::openCollection(const std::string&& name)
{
    auto [db, txn] = openDb(name);
    Collection ret(name.c_str(), db, _env);
    if (txn) {
        mdb_txn_abort(txn);
    }
    return ret;
}

void DB::openEnv(const std::string& path, DbSettings& settings) noexcept(false) {
    //creating base env
    int rc = mdb_env_create(&_env);
    if (rc != MDB_SUCCESS) {
        throw std::runtime_error("Failed to create LMDB environment");
    }
    _settings = settings;
    
    rc = mdb_env_set_mapsize(_env, _settings._startMemorySize); 
    if (rc != MDB_SUCCESS) {
        mdb_env_close(_env);
        throw std::runtime_error("Failed to set LMDB map size");
    }

    rc = mdb_env_set_maxdbs(_env, _settings._NumberOfCollections+1); 
    if (rc != MDB_SUCCESS) {
        mdb_env_close(_env);
        throw std::runtime_error("Failed to set LMDB max number of named databases");
    }

    // Create directory for the database if it does not exist
    const std::filesystem::path dirPath = std::filesystem::path(path).parent_path();
    if (!std::filesystem::exists(dirPath)) {
        std::filesystem::create_directories(dirPath);
        //creating settings in it
        if (_settings._saveSettingsInDBAfterChange)
        {
            saveSettings();
        }
    }

    rc = mdb_env_open(_env, path.c_str(), MDB_WRITEMAP | MDB_CREATE, 0664);
    if (rc != MDB_SUCCESS) {
        mdb_env_close(_env);
        throw std::runtime_error("Failed to open LMDB environment");
    }
    //loading settings from db
    if (_settings._readSettingsFromDb)
    {
        readDbSettings();
        rc = mdb_env_set_mapsize(_env, _settings._startMemorySize);
        if (rc != MDB_SUCCESS) {
            mdb_env_close(_env);
            throw std::runtime_error("Failed to set LMDB map size");

        }
        rc = mdb_env_set_maxdbs(_env, _settings._NumberOfCollections + 1);
        if (rc != MDB_SUCCESS) {
            mdb_env_close(_env);
            throw std::runtime_error("Failed to set LMDB max number of named databases");
        }
        
    }
}

std::pair<MDB_dbi&, MDB_txn*> DB::openDb(const std::string& name) const noexcept(false) {
    MDB_dbi db;
    MDB_txn* txn;
    const int flags = MDB_CREATE;
    if (mdb_txn_begin(_env, nullptr, 0, &txn) != 0) {
        throw std::runtime_error("Failed to begin LMDB transaction");
    }
    if (mdb_dbi_open(txn, name.c_str(), flags, &db) != 0) {
        throw std::runtime_error("Failed to open LMDB database");
    }
    if (mdb_txn_commit(txn) != 0) {
        throw std::runtime_error("Failed to commit LMDB transaction");
    }
    return { db,txn };
}


inline void DB::readDbSettings()
{
    std::string settingsName = "dbsettings";

    auto [settingsDbi, settingsTxn] = openDb(settingsName);

    MDB_val settingsKey{ sizeof("settings"), const_cast<char*>("settings") };
    MDB_val settingsValue{ 0, nullptr };

    int rc = mdb_get(settingsTxn, settingsDbi, &settingsKey, &settingsValue);
    if (rc != MDB_SUCCESS) {
        throw std::runtime_error("Failed to read database settings: " + std::string(mdb_strerror(rc)));
    }

    DbSettings readSettings;
    std::memcpy(&readSettings, settingsValue.mv_data, settingsValue.mv_size);
    _settings = readSettings;

    if (settingsTxn) {
        mdb_txn_abort(settingsTxn);
    }
}


inline void DB::saveSettings()
{
    if (!_settings._saveSettingsInDBAfterChange) {
        return;
    }

    std::string settingsName = "dbsettings";

    auto [settingsDbi, settingsTxn] = openDb(settingsName);

    MDB_val settingsKey{ sizeof("settings"), const_cast<char*>("settings") };
    MDB_val settingsValue{ sizeof(DbSettings), const_cast<DbSettings*>(&_settings) };

    int rc = mdb_put(settingsTxn, settingsDbi, &settingsKey, &settingsValue, 0);
    if (rc != MDB_SUCCESS) {
        throw std::runtime_error("Failed to save database settings: " + std::string(mdb_strerror(rc)));
    }

    rc = mdb_txn_commit(settingsTxn);
    if (rc != MDB_SUCCESS) {
        throw std::runtime_error("Failed to commit transaction: " + std::string(mdb_strerror(rc)));
    }

    if (settingsTxn)
    {
        mdb_txn_abort(settingsTxn);
    }
}

void DB::incMemory() 
{
    int rc = mdb_env_set_mapsize(_env,(_currentMemory+_settings._MemUpdateSize));
    if (rc != MDB_SUCCESS) {
        mdb_env_close(_env);
        throw std::runtime_error("Failed to set LMDB map size");
    }
    _currentMemory += _settings._MemUpdateSize;
}

void DB::incCollections() 
{
    _settings._NumberOfCollections++;
    int rc = mdb_env_set_maxdbs(_env, _settings._NumberOfCollections + 1);
    if (rc != MDB_SUCCESS) {
        mdb_env_close(_env);
        throw std::runtime_error("Failed to set LMDB max number of named databases");
    }
    setSettings(_settings);
}

void DB::decCollections()
{
    _settings._NumberOfCollections--;
    int rc = mdb_env_set_maxdbs(_env, _settings._NumberOfCollections + 1);
    if (rc != MDB_SUCCESS) {
        mdb_env_close(_env);
        throw std::runtime_error("Failed to set LMDB max number of named databases");
    }
    setSettings(_settings);
}

