#pragma once
#include <variant>
#include "QueryModifier.h"


namespace db
{
	namespace query
	{
		struct Query
		{
			LocationModifier location;
			DataModifier data;
			std::vector<std::pair<std::string,std::function<bool(http::json::JsonValue)>>> limiters;
			bool joinDiffrentLocations;
		};
	}
}

