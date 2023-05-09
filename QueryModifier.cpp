#include "QueryModifier.h"
#include "JsonType.h"
std::function<bool(http::json::JsonValue)> db::query::LimiterModifier::getFilterAsFunction()
{
    auto ret = [this](http::json::JsonValue jsonValue)->bool {
		switch (jsonValue.type())
		{
		case http::json::JsonType::String:
			return jsonValue.string_value() == LimitValue.string_value();
		case http::json::JsonType::Integer:
			return jsonValue.integer_value() == LimitValue.integer_value();
		case http::json::JsonType::Boolean:
			return jsonValue.boolean_value() == LimitValue.boolean_value();
		case http::json::JsonType::Real:
			return jsonValue.real_value() == LimitValue.real_value();
		default:
			return false;
		}
    };

    return ret;
}
