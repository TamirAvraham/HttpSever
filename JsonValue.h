#pragma once
#include <map>
#include <vector>
#include <string>


namespace http {
	namespace json {
		class JsonValue
		{
		public: 
			enum class Type {
				String,
				Integer,
				Real,
				Boolean,
				Array,
				Object
			};
			JsonValue() : type_(Type::String), _string_value("") {}
			JsonValue(Type type, const std::string& value) : type_(type), _string_value(value) {}

			Type type() const { return type_; }
			std::string string_value() const { return _string_value; }
			int integer_value() const { return std::stoi(_string_value); }
			double real_value() const { return std::stod(_string_value); }
			bool boolean_value() const { return _string_value == "true"; }

			// array and object type
			std::vector<JsonValue> array_value() const;
			std::map<std::string, JsonValue> object_value() const;

		private:
			Type type_;
			std::string _string_value;
			std::string key;
		};
	};
}

