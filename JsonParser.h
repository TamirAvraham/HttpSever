#pragma once
#include "JsonType.h"
#include "JsonValue.h"
#include <vector>
#include <sstream>

#define JsonLine	std::pair<std::string, std::string>
namespace http {
	namespace json {
		class JsonParser
		{
		public:
			static JsonValue parse(std::string);
		private:
			static JsonLine getJsonLine(std::string&);
			static std::string GetLineName(std::string);
			static std::pair<std::string,int> GetLineValue(std::string);
			static size_t findMatchingClosingBracket(std::string jsonString, size_t startIndex=0);
			static JsonValue parseValue(std::string);
		};
	}
}


