#include "DocDB.h"
#include <functional>

using namespace db::doc;


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
        updateSettings();
    }
    return settings;
}


Collection DB::openCollection(const std::string& name) 
{
    MDB_dbi db;
    auto txn = openDb(name,&db);
    
    Collection ret(name, db, _env,{std::bind(&DB::getTxn,this),std::bind(&DB::incMemory,this)});
    
    return ret;
}

Collection DB::openCollection(const std::string&& name) 
{
    MDB_dbi db;
    auto txn = openDb(name, &db);
    Collection ret(name, db, _env, { std::bind(&DB::getTxn,this),std::bind(&DB::incMemory,this) });
    return ret;
}

Collection DB::createCollection(const std::string& collectionName)
{
    incCollections();
    MDB_dbi db;
    auto txn = openDb(collectionName, &db,MDB_CREATE);
    
    Collection ret(collectionName, db, _env, { std::bind(&DB::getTxn,this),std::bind(&DB::incMemory,this) });
    
    return ret;
}

Collection DB::createCollection(const std::string&& collectionName)
{
    incCollections();
    MDB_dbi db;
    auto txn = openDb(collectionName, &db, MDB_CREATE);
    Collection ret(collectionName, db, _env, { std::bind(&DB::getTxn,this),std::bind(&DB::incMemory,this) });
    return ret;
}

bool DB::deleteCollection(const std::string& CollectionName)
{
    decCollections();
    MDB_dbi db;
    auto txn = openDb(CollectionName, &db);
    int res = mdb_drop(txn, db, 0);
    if (res)
    {
        return false;
    }
    return true;
}

bool DB::deleteCollection(const std::string&& CollectionName)
{
    decCollections();
    MDB_dbi db;
    auto txn = openDb(CollectionName, &db);
    int res = mdb_drop(txn, db, 0);
    if (res)
    {
        return false;
    }
    return true;
}

db::DBMultiResult DB::Query(const db::query::Query& query)
{
    db::DBMultiResult ret;
    for (const auto& collectionName : query.location.collections) {
        auto documents = openCollection(collectionName).getAllDocuments();
        for (const auto& document : documents) {
            bool passedChecks = true;
            for (const auto& filter:query.limiters)
            {
                try
                {
                    if (!filter.second(document[filter.first]))
                    {
                        passedChecks = false;
                        break;
                    }
                }
                catch (...){ /*ignore the error*/ }
            }
            if (passedChecks) {
                if (query.data.dataFileds.empty())
                {
                    ret.push_back(std::make_shared<Document>(document));
                }
                else
                {
                    Document filteredDoc(document.getName());
                    bool hadAllFileds = true;
                    for (const auto& filed : query.data.dataFileds)
                    {
                        try
                        {
                            std::cout << filteredDoc.ToString() << '\n' << document.getName() << std::endl;
                            filteredDoc.insert({ filed,document[filed] });
                        }
                        catch (...)
                        {
                            hadAllFileds = false;
                            break;
                        }
                    }
                    if (hadAllFileds)
                    {
                        ret.push_back(std::make_shared<Document>(filteredDoc));
                    }
                    
                }
            }
        }
    }
    return ret;
}

void DB::openEnv(const std::string& path, DbSettings& settings) noexcept(false) {
    //creating base env
    _path=path;
    bool save = false;
    int rc = mdb_env_create(&_env);
    if (rc != MDB_SUCCESS||_env==nullptr) {
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
    
    const std::filesystem::path dirPath = std::filesystem::path(path);
    if (!std::filesystem::exists(dirPath)) {
        std::filesystem::create_directories(dirPath);
        //creating settings in it
        save = _settings._saveSettingsInDBAfterChange;
        
    }


    rc = mdb_env_open(_env, path.c_str(), MDB_WRITEMAP | MDB_CREATE | MDB_NOTLS, 0664);
    if (rc != MDB_SUCCESS) {
        mdb_env_close(_env);
        throw std::runtime_error("Failed to open LMDB environment");
    }
    if (save)
    {
        saveSettings();
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
        
    }
    _currentMemory = _settings._startMemorySize;
}

MDB_txn* DB::openDb(const std::string& name, MDB_dbi* db, int flag) const noexcept(false) {
    
    MDB_txn* txn = nullptr;
    const int flags = flag;
    int res = 0;

    res = mdb_txn_begin(_env, nullptr, 0, &txn);
    if (res != MDB_SUCCESS) {
        throw std::runtime_error("Failed to begin LMDB transaction");
    }
    if (txn == nullptr) {
        throw std::runtime_error("Failed to initialize LMDB transaction");
    }

    res = mdb_dbi_open(txn, name.c_str(), flags, db);
    if (res != MDB_SUCCESS) {
        mdb_txn_abort(txn);
        if (MDB_NOTFOUND==res)
        {
            throw db::exceptions::NotFoundException("db was not found");
        }
        
        throw std::runtime_error("Failed to open LMDB database");
    }

    res = mdb_txn_commit(txn);
    if (res != MDB_SUCCESS) {
        throw std::runtime_error("Failed to commit LMDB transaction");
    }
    txn = getTxn();
    return txn;
}




inline void DB::readDbSettings()
{
    std::string settingsName = "dbsettings";
    MDB_dbi settingsDbi;
    auto settingsTxn = openDb(settingsName,&settingsDbi);

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
        auto ret = mdb_txn_commit(settingsTxn);
        mdb_dbi_close(_env, settingsDbi);
    }
}


inline void DB::saveSettings()
{
    std::cout << "settings saved" << std::endl;
    if (!_settings._saveSettingsInDBAfterChange) {
        return;
    }

    std::string settingsName = "dbsettings";
    MDB_dbi settingsDbi;
    auto settingsTxn = openDb(settingsName, &settingsDbi,MDB_CREATE);

    MDB_val settingsKey{ sizeof("settings"), const_cast<char*>("settings") };
    MDB_val settingsValue{ sizeof(DbSettings), const_cast<DbSettings*>(&_settings) };

    int rc = mdb_put(settingsTxn, settingsDbi, &settingsKey, &settingsValue,MDB_APPEND);
    
    if (rc != MDB_SUCCESS) {
        std::cout << "Failed to commit transaction: " + std::string(mdb_strerror(rc));
        throw std::runtime_error("Failed to save database settings: " + std::string(mdb_strerror(rc)));
    }

    rc = mdb_txn_commit(settingsTxn);
    if (rc != MDB_SUCCESS) {
        std::cout << "Failed to commit transaction: " + std::string(mdb_strerror(rc));
        throw std::runtime_error("Failed to commit transaction: " + std::string(mdb_strerror(rc)));
    }

    if (settingsTxn)
    {
        mdb_txn_abort(settingsTxn);
    }
}

inline void DB::updateSettings()
{
    if (!_settings._saveSettingsInDBAfterChange) {
        return;
    }

    std::string settingsName = "dbsettings";
    
    MDB_dbi settingsDbi;
    auto settingsTxn = openDb(settingsName, &settingsDbi);

    MDB_val settingsKey{ sizeof("settings"), const_cast<char*>("settings") };
    MDB_val settingsValue{ sizeof(_settings),&_settings };
    {
        
        doesValueExist(&settingsDbi);
        
    }
    
    int rc = mdb_put(settingsTxn, settingsDbi, &settingsKey, &settingsValue,0);
    if (rc != MDB_SUCCESS) {
        mdb_txn_abort(settingsTxn);
        mdb_dbi_close(_env, settingsDbi);
        std::cout << "Failed to commit transaction: " + std::string(mdb_strerror(rc));
        throw std::runtime_error("Failed to save database settings: " + std::string(mdb_strerror(rc)));
    }

    rc = mdb_txn_commit(settingsTxn);
    if (rc != MDB_SUCCESS) {
        mdb_txn_abort(settingsTxn);
        mdb_dbi_close(_env, settingsDbi);
        std::cout << "Failed to commit transaction: " + std::string(mdb_strerror(rc));
        throw std::runtime_error("Failed to commit transaction: " + std::string(mdb_strerror(rc)));
    }

    mdb_dbi_close(_env, settingsDbi);
}

MDB_txn* DB::getTxn() const
{
    rlock lock(_operationMutex);
    MDB_txn* txn = nullptr;
    int res = -1;
    res = mdb_txn_begin(_env, nullptr, 0, &txn);
    if (res != MDB_SUCCESS) {
        throw std::runtime_error("Failed to begin LMDB transaction");
    }
    if (txn == nullptr) {
        throw std::runtime_error("Failed to initialize LMDB transaction");
    }
    return txn;
}



void DB::setCollections(int numOfCollections)
{
    rlock readLock(_operationMutex);
    mdb_env_close(_env);
    _env = nullptr;
    int rc = mdb_env_create(&_env);
    if (rc != MDB_SUCCESS || _env == nullptr) {
        throw std::runtime_error("Failed to create LMDB environment");
    }
    rc = mdb_env_set_mapsize(_env, _currentMemory);
    if (rc != MDB_SUCCESS) {
        mdb_env_close(_env);
        throw std::runtime_error("Failed to set LMDB map size");
    }

    rc = mdb_env_set_maxdbs(_env, numOfCollections);
    if (rc != MDB_SUCCESS) {
        mdb_env_close(_env);
        throw std::runtime_error("Failed to set LMDB max number of named databases");
    }

    rc = mdb_env_open(_env, _path.c_str(), MDB_WRITEMAP | MDB_CREATE | MDB_NOTLS, 0664);
    if (rc != MDB_SUCCESS) {
        mdb_env_close(_env);
        throw std::runtime_error("Failed to open LMDB environment");
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
    setCollections(_settings._NumberOfCollections);
    setSettings(_settings);
}

void DB::decCollections()
{
    _settings._NumberOfCollections--;
    setCollections(_settings._NumberOfCollections);
    setSettings(_settings);
}

inline bool DB::doesValueExist(MDB_dbi* db) const
{
    MDB_val settingsKey{ sizeof("settings"), const_cast<char*>("settings") };
    MDB_val settingsValue{ 0, nullptr };
    int res = mdb_get(getTxn(), *db, &settingsKey, &settingsValue);
    bool isNotFound = res == MDB_NOTFOUND;
    return !res;
    
}

