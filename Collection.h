#pragma once
#include <lmdb.h>
#include <stdexcept>
#include <string>
#include "Document.h"

class Collection {
    
public :
    // Constructor with rvalue reference to the name of the collection
    explicit Collection(const std::string && name, MDB_dbi db, MDB_env* env) noexcept(false);

    // Constructor with reference to the name of the collection
    explicit Collection(const std::string& name, MDB_dbi db, MDB_env* env) noexcept(false);
    
    // Constructor with just a std::string name for the collection
    explicit Collection(const char* name, MDB_dbi db, MDB_env* env) noexcept(false);

    // Destructor to close transaction
    ~Collection() noexcept(false);


    std::string getName()const noexcept;

    Document getDocument(const std::string&& docName)const;
    Document getDocument(const std::string& docName)const;
    Document getDocument(const char* docName)const;

    Document createDocument(const std::string&& docName)const ;
    Document createDocument(const std::string& docName)const ;
    Document createDocument(const char* docName)const ;
    Document createDocument(const std::string&& docName,const http::json::JsonObject&& docContent)const ;
    Document createDocument(const std::string&& docName,const http::json::JsonObject& docContent)const ;
    Document createDocument(const std::string& docName,const http::json::JsonObject&& docContent)const ;
    Document createDocument(const std::string& docName,const http::json::JsonObject& docContent)const ;
    Document createDocument(const char* docName,const http::json::JsonObject&& docContent)const ;
    Document createDocument(const char* docName,const http::json::JsonObject& docContent)const ;
    

    bool deleteDocument(const std::string&& docName)const noexcept;
    bool deleteDocument(const std::string& docName)const noexcept;
    bool deleteDocument(const char* docName)const noexcept;

    Document updateDocument(const Document& docToUpdate)const;
    
    

private:
    MDB_dbi _db;
    MDB_txn* _txn;
    std::string _name;

    void handleErrors(int errorCode);
    
};

// Implementation of rvalue reference constructor



