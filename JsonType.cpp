#include "JsonType.h"

http::json::JsonType http::json::getTypeFromString(std::string stringAsValue)
{
    char firstChar = stringAsValue[0], lastChar = stringAsValue[stringAsValue.length() - 1];
	std::cout << "first char: " << firstChar << " last char: " << lastChar << " \n";
	switch (firstChar)
	{
	case '[':
		if (stringAsValue.find_last_of(']') != std::string::npos)
			return JsonType::Array;
		break;
	case '{':
		if (stringAsValue.find_last_of('}')!=std::string::npos)
		{
			return JsonType::Object;
		}
		break;
	default:
		break;
	}
	if (stringAsValue=="true"||stringAsValue=="false")
	{
		return JsonType::Boolean;
	}
	if (isdigit(firstChar))
	{
		size_t dotInNumber=stringAsValue.find('.');
		if (dotInNumber!=std::string::npos)
		{
			return JsonType::Real;
		}
		return JsonType::Integer;
	}
	
    return JsonType::String;
}

