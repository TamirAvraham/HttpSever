#pragma once
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "JsonType.h"





namespace http {
	namespace json {
		class JsonParser;
		class JsonValue
		{
		public: 
			
			JsonValue() : _type(JsonType::String), _string_value("") {}
			JsonValue(JsonType type, const std::string& value) : _type(type), _string_value(value) {}

			JsonType type() const { return _type; }
			template <typename T> T getValue() const;
			std::string getValueAsString()const;

		private:
			JsonType _type;
			std::string _string_value;
			std::string string_value() const { return _string_value; }
			int integer_value() const { return std::stoi(_string_value); }
			double real_value() const { return std::stod(_string_value); }
			bool boolean_value() const { return _string_value == "true"; }

			// array and object type
			std::vector<JsonValue> array_value() const;
			std::map<std::string, http::json::JsonValue> object_value() const;
		};
	};
}

