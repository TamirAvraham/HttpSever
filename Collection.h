#pragma once
#include <lmdb.h>
#include <stdexcept>
#include <functional>
#include <string>
#include "Document.h"
#include "DBExeptions.hpp"

namespace db
{
    namespace doc
    {
        struct CollectionRequriedFunctions
        {
            std::function<MDB_txn* ()> _getTxn;
            std::function<void()> _incMemorey;
        };
        class Collection {
        public:
            // Constructor with rvalue reference to the name of the collection
            explicit Collection(const std::string&& name, MDB_dbi db, MDB_env* env, CollectionRequriedFunctions  reqFunctions) noexcept(false);

            // Constructor with reference to the name of the collection
            explicit Collection(const std::string& name, MDB_dbi db, MDB_env* env, CollectionRequriedFunctions reqFunctions) noexcept(false);

            // Constructor with just a std::string name for the collection
            explicit Collection(const char* name, MDB_dbi db, MDB_env* env, CollectionRequriedFunctions reqFunctions) noexcept(false);

            // Destructor to close transaction
            ~Collection() noexcept(false);


            std::string getName()const noexcept;

            Document getDocument(const std::string&& docName)const;
            Document getDocument(const std::string& docName)const;
            Document getDocument(const char* docName)const;

            Document createDocument(const std::string&& docName)const;
            Document createDocument(const std::string& docName)const;
            Document createDocument(const char* docName)const;
            Document createDocument(const std::string&& docName, const http::json::JsonObject&& docContent)const;
            Document createDocument(const std::string&& docName, const http::json::JsonObject& docContent)const;
            Document createDocument(const std::string& docName, const http::json::JsonObject&& docContent)const;
            Document createDocument(const std::string& docName, const http::json::JsonObject& docContent)const;
            Document createDocument(const char* docName, const http::json::JsonObject&& docContent)const;
            Document createDocument(const char* docName, const http::json::JsonObject& docContent)const;


            bool deleteDocument(const std::string&& docName)const noexcept;
            bool deleteDocument(const std::string& docName)const noexcept;
            bool deleteDocument(const char* docName)const noexcept;

            Document updateDocument(const Document& docToUpdate)const;



        private:
            MDB_dbi _db;
            mutable MDB_txn* _txn;
            std::string _name;
            CollectionRequriedFunctions _requriedFunctions;
            template<class F, class... Args>
            void handleErrors(int errorCode, std::function<F(Args...)> function);

        };
    }
}

// Implementation of rvalue reference constructor



