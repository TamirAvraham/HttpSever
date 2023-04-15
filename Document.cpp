#include "Document.h"

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

Document::Document(const std::string& name, const http::json::JsonObject& data) :_name(name),JsonObject(data)
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
