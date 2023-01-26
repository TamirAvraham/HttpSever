#pragma once
#include <string>
#include <iostream>
namespace http {
	namespace json {
		enum class JsonType {
			String,
			Integer,
			Real,
			Boolean,
			Array,
			Object
		};
		JsonType getTypeFromString(std::string stringAsValue);
	}
}