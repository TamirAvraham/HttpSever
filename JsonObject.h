#pragma once
#include "JsonParser.h"
using http::json::JsonValue;
namespace http {
	namespace json {
		class JsonObject
		{
		public:
			JsonObject();
			JsonObject(const std::string jsonString);
			JsonObject(const std::map<std::string,http::json::JsonValue> jsonMap);

			JsonObject& operator=(std::string jsonString);
			JsonObject& operator=(const std::map<std::string, http::json::JsonValue> jsonMap);


			JsonValue operator[](const std::string name);
			template<class T>
			T operator[](const std::string name);
			void insert(JsonKeyValuePair keyValuePair);

			std::string ToString();

		private:
			std::map<std::string, http::json::JsonValue> _jsonMap;
			
		};
	}
}

