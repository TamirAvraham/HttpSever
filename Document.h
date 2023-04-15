#pragma once
#include "JsonObject.h"
class Document :
    public http::json::JsonObject
{
public: 

    Document() = delete;
    //name only ctors
    

    Document(const std::string& name)noexcept;
    Document(const std::string&& name)noexcept;
    Document(const char* name)noexcept;
    //name and data ctors

    Document(const std::string& name,const http::json::JsonObject& data)noexcept;
    Document(const std::string& name,const http::json::JsonObject&& data)noexcept;
    Document(const std::string&& name, const http::json::JsonObject& data)noexcept;
    Document(const std::string&& name, const http::json::JsonObject&& data)noexcept;
    Document(const char* name, const http::json::JsonObject& data)noexcept;
    Document(const char* name, const http::json::JsonObject&& data)noexcept;
    //getters and setters

    std::string getName()const noexcept;
    //bool operations

    bool isValid()const noexcept;
    //transformers

    http::json::JsonObject toJson()const;
protected:
    std::string _name;
};

