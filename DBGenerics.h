#pragma once
#include "JsonValue.h"
#include "Query.h"
namespace db {
	class DBResult {
	public:
		
		virtual http::json::JsonValue getValue(const std::string& valueName) = 0;
		virtual http::json::JsonValue updateValue(const std::string& valueName,const JsonValue& newValue) = 0;
		virtual std::string getName()const noexcept = 0;
		
	};
	using DBMultiResult = std::vector<std::shared_ptr<DBResult>>;
	class DB {
	public:
		virtual DBMultiResult Query(const db::query::Query& query)=0;
	};
}