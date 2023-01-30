#include "JsonObject.h"

http::json::JsonObject::JsonObject():_jsonMap()
{
}

http::json::JsonObject::JsonObject(const std::string jsonString)
{
	try
	{
		_jsonMap = http::json::JsonParser::parse(jsonString);
	}
	catch (int errorCode) {
		if (errorCode==PARSE_ERROR)
		{
			throw std::invalid_argument("invalid argument");
		}
	}
	catch (const std::exception&)
	{
		throw std::invalid_argument("invalid argument");
	}
}

http::json::JsonObject::JsonObject(const std::map<std::string, http::json::JsonValue> jsonMap):_jsonMap(jsonMap)
{
}

http::json::JsonObject& http::json::JsonObject::operator=(std::string jsonString)
{
	*this = JsonObject(jsonString);
	return *this;
}

http::json::JsonObject& http::json::JsonObject::operator=(const std::map<std::string, http::json::JsonValue> jsonMap)
{
	*this = JsonObject(jsonMap);
	return *this;
}

http::json::JsonValue http::json::JsonObject::operator[](const std::string name)
{
	JsonValue ret;
	try {
		ret = _jsonMap.at(name);
	}
	catch (std::exception& error) {
		throw std::invalid_argument("name was not in json");
	}
	return ret;
}

void http::json::JsonObject::insert(JsonKeyValuePair keyValuePair)
{
	_jsonMap.insert(keyValuePair);
}

std::string http::json::JsonObject::ToString()
{
	return std::string();
}
