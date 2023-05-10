#pragma once
#include "JsonObject.h"
#include "DBGenerics.h"
namespace db
{
    namespace doc
    {
        class Document :
            public http::json::JsonObject,public db::DBResult
        {
        public:

            Document() = delete;
            //name only ctors


            Document(const std::string& name)noexcept;
            Document(const std::string&& name)noexcept;
            Document(const char* name)noexcept;
            //name and data ctors

            Document(const std::string& name, const http::json::JsonObject& data)noexcept;
            Document(const std::string& name, const http::json::JsonObject&& data)noexcept;
            Document(const std::string&& name, const http::json::JsonObject& data)noexcept;
            Document(const std::string&& name, const http::json::JsonObject&& data)noexcept;
            Document(const char* name, const http::json::JsonObject& data)noexcept;
            Document(const char* name, const http::json::JsonObject&& data)noexcept;
            //getters and setters

            virtual std::string getName()const noexcept override;
            virtual JsonValue getValue(const std::string& valueName)override;
            virtual JsonValue updateValue(const std::string& valueName,const JsonValue& newValue)override;
            //bool operations

            bool isValid()const noexcept;
            //transformers

            http::json::JsonObject toJson()const;
        protected:
            std::string _name;
        };
    }
}

