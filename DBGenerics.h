#pragma once
#include "JsonValue.h"
#include "Query.h"
namespace db {
	class DBResult {
		
	};
	using DBMultiResult = std::vector<DBResult>;
	class DB {
		virtual DBMultiResult Query(const db::query::Query& query)=0;
	};
}