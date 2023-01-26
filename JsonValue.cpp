#include "JsonValue.h"

std::vector<http::json::JsonValue> http::json::JsonValue::array_value() const
{
    size_t startOfArray = _string_value.find('[');
    size_t endOfArray = _string_value.find_last_of(']');
    
    if (startOfArray==std::string::npos||endOfArray==std::string::npos)
    {
        throw std::invalid_argument("value is not an a json array");
    }

    std::string arrayInside = _string_value.substr(startOfArray+1, endOfArray-1);
    std::cout << arrayInside << '\n';
    std::stringstream insideAsStream(arrayInside);
    std::string arrayIteam;
    std::vector<JsonValue> arrayValues;
    while (!insideAsStream.eof())
    {
        std::getline(insideAsStream, arrayIteam, ',');
        std::cout << arrayIteam << '\n';
        arrayValues.push_back(JsonValue(json::getTypeFromString(arrayIteam), arrayIteam));
    }

    return arrayValues;
}

std::map<std::string,http::json::JsonValue> http::json::JsonValue::object_value() const
{
    return std::map<std::string, JsonValue>();
}
template <typename T> T http::json::JsonValue::getValue() const {
    if constexpr (std::is_same<T, std::string>::value) {
        if (_type != JsonType::String) {
            throw std::runtime_error("JsonValue is not a string");
        }
        return _string_value;
    }
    else if constexpr (std::is_same<T, int>::value) {
        if (_type != JsonType::Integer) {
            throw std::runtime_error("JsonValue is not an integer");
        }
        return integer_value();
    }
    else if constexpr (std::is_same<T, double>::value) {
        if (_type != JsonType::Real) {
            throw std::runtime_error("JsonValue is not a real number");
        }
        return real_value();
    }
    else if constexpr (std::is_same<T, bool>::value) {
        if (_type != JsonType::Boolean) {
            throw std::runtime_error("JsonValue is not a boolean");
        }
        return boolean_value();
    }
    else if constexpr (std::is_same<T, std::vector<JsonValue>>::value) {
        if (_type!= JsonType::Array) {
            throw std::runtime_error("JsonValue is not an array");
        }
        return array_value();
    }
    else if constexpr (std::is_same<T, std::map<std::string, JsonValue>>::value) {
        if (_type != JsonType::Object) {
            throw std::runtime_error("JsonValue is not an object");
        }
        return object_value();
    }
    else {
        throw std::runtime_error("no type");
    }
}