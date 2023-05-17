#pragma once
#include "JsonParser.h"
using http::json::JsonValue;
namespace http {
	namespace json {


		/*TODO:
		* improve the TOString Chasing
		*/

		class JsonObject
		{
		public:
			JsonObject();
			JsonObject(const std::string jsonString);
			JsonObject(const std::map<std::string,http::json::JsonValue> jsonMap);

			JsonObject& operator=(std::string jsonString);
			JsonObject& operator=(const std::map<std::string, http::json::JsonValue> jsonMap);


			JsonValue operator[](const std::string& name)const;
			virtual void insert(JsonKeyValuePair keyValuePair);
			virtual void set(const std::string& keyName,http::json::JsonValue val) noexcept;
			std::string ToString()const;

		private:
			std::map<std::string, http::json::JsonValue> _jsonMap;
			mutable std::string _asString;
			bool _changed;
		};
	}
}

