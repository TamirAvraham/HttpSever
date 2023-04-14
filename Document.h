#pragma once
#include "JsonObject.h"
class Document :
    public http::json::JsonObject
{
public:
    std::string getName()const noexcept;
    bool isValid()const noexcept;
    http::json::JsonObject toJson()const;
protected:
    std::string _name;
};

