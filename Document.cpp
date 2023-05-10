#include "Document.h"
using namespace db::doc;
http::json::JsonObject Document::toJson() const
{
    return *this;
}

Document::Document(const std::string& name) noexcept:_name(name)
{
    set("name", name);
}

Document::Document(const std::string&& name) noexcept :_name(name)
{
    set("name", name);
}

Document::Document(const char* name) noexcept :_name(name)
{
    set("name", _name);
}

Document::Document(const std::string& name, const http::json::JsonObject& data)noexcept :_name(name),JsonObject(data)
{
    set("name", name);
}

Document::Document(const std::string& name, const http::json::JsonObject&& data) noexcept :_name(name), JsonObject(data)
{
	set("name", name);
}

Document::Document(const std::string&& name, const http::json::JsonObject& data) noexcept :_name(name), JsonObject(data)
{
	set("name", name);
}

Document::Document(const std::string&& name, const http::json::JsonObject&& data) noexcept :_name(name), JsonObject(data)
{
	set("name", name);
}
Document::Document(const char* name, const http::json::JsonObject& data) noexcept :_name(name), JsonObject(data)
{
	set("name", _name);
}

Document::Document(const char* name, const http::json::JsonObject&& data) noexcept :_name(name), JsonObject(data)
{
	set("name", _name);
}

std::string Document::getName() const noexcept
{
    return _name;
}

JsonValue db::doc::Document::getValue(const std::string& valueName)
{
	return this->operator[](valueName);
}

JsonValue db::doc::Document::updateValue(const std::string& valueName, const JsonValue& newValue)
{
	this->set(valueName, newValue);
	return this->operator[](valueName);
}

bool Document::isValid() const noexcept
{
	try
	{
		this->operator[]("name");
		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}
