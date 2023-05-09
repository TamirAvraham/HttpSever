#pragma once
#include <string>
#include <vector>
#include <functional>
#include "JsonValue.h"
namespace db
{
	namespace query
	{
		struct QueryModifier
		{
		};
		struct DataModifier :public QueryModifier {
			std::vector<std::string> dataFileds;
		};
		struct LocationModifier :public QueryModifier {
			std::vector<std::string> collections;
		};
		struct LimiterModifier :public QueryModifier {
			std::string VarToLimitName;
			enum { Greater, GreaterEqual, Equal, LessThenEqual, LessThen } LimitType;
			http::json::JsonValue LimitValue;
			std::function<bool(http::json::JsonValue)> getFilterAsFunction();
		};
	}
}

