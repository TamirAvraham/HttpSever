#include "JsonParser.h"
#define PARSE_ERROR 400
http::json::JsonValue http::json::JsonParser::parse(std::string jsonInput)
{
    size_t firstBracket, lastBracket;
    firstBracket = jsonInput.find('{');
    lastBracket = jsonInput.find_last_of('}');
    if (firstBracket==std::string::npos||lastBracket== std::string::npos)
    {
        throw PARSE_ERROR;
    }
    
    return parseValue(jsonInput.substr(firstBracket+1,lastBracket));
}

JsonLine http::json::JsonParser::getJsonLine(std::string& jsonObject)
{
    
    std::string nameOfLine = GetLineName(jsonObject);
    auto [valueOfLine, endOfLine] = GetLineValue(jsonObject);
    /*jsonObject = jsonObject.substr(endOfLine, jsonObject.length());*/
    
    return {nameOfLine,valueOfLine};
}

std::string http::json::JsonParser::GetLineName(std::string jsonObject)
{
    size_t startOfName = jsonObject.find('\"');
    if (startOfName == std::string::npos)
    {
        return "";
    }
    size_t endOfName = jsonObject.find('\"', startOfName+1);
    if (endOfName == std::string::npos)
    {
        return "";
    }
    return jsonObject.substr(startOfName+1,endOfName-1);
}

std::pair<std::string, int> http::json::JsonParser::GetLineValue(std::string jsonObject)
{
    size_t startOfvalue = jsonObject.find(':');
    int returnNumber = -1;
    std::string returnString = "";
    if (startOfvalue == std::string::npos)
    {
        return { returnString,returnNumber };
    }
    returnString = jsonObject.substr(startOfvalue + 1, jsonObject.size());
    char firstCharOfValue = jsonObject[startOfvalue + 1];
    if (firstCharOfValue=='{'||firstCharOfValue=='[')
    {
        int endOfValue = findMatchingClosingBracket(returnString);
        if (endOfValue==-1)
        {
            return { returnString, returnNumber };
        }
        returnString = jsonObject.substr(startOfvalue+1, endOfValue+1);
    }
    else if (firstCharOfValue == '\"') {
        size_t endOfValue = jsonObject.find(startOfvalue + 1, '\"');
        if (endOfValue == std::string::npos)
        {
            return { returnString,returnNumber };
        }
        returnString = jsonObject.substr(startOfvalue + 1, endOfValue);
    }
    else
    {
        size_t endOfValue = jsonObject.find(startOfvalue + 1, ',');
        if (endOfValue == std::string::npos)
        {
            return { returnString,returnNumber };
        }
        returnString = jsonObject.substr(startOfvalue + 1, endOfValue-1);
    }
    returnNumber = (returnString.length())+startOfvalue+1;
    return { returnString,returnNumber };
}
size_t http::json::JsonParser::findMatchingClosingBracket(std::string jsonString, size_t startIndex)
{
    int bracketCount = 1;
    char openBracket = jsonString[startIndex];
    char closeBracket;
    if (openBracket == '{')
        closeBracket = '}';
    else if (openBracket == '[')
        closeBracket = ']';
    else
        return -1;
    for (size_t i = startIndex + 1; i < jsonString.size(); i++)
    {
        if (jsonString[i] == openBracket)
            bracketCount++;
        else if (jsonString[i] == closeBracket)
            bracketCount--;
        if (bracketCount == 0)
            return i;
    }
    return -1;
}
http::json::JsonValue http::json::JsonParser::parseValue(std::string json)
{
    std::cout << json<<'\n';
    JsonLine line = getJsonLine(json);
    std::cout << line.first << "   " << line.second << '\n';
    return JsonValue();
}


