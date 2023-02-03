#pragma once
#include "TcpServer.h"
#include "HttpStatus.h"
#include "HttpParser.h"
#include "HtmlFileReader.h"
#include "JsonObject.h"


namespace http{

	struct HttpHeaders {
		std::map<std::string, std::string> headers;

		void AddHeader(const std::string& key, const std::string& value) {
			headers[key] = value;
		}

		std::string GetHeaderValue(const std::string& key) const {
			auto header = headers.find(key);
			if (header != headers.end()) {
				return header->second;
			}
			return "";
		}
	};

	class HttpSocket :protected tcp::simpleSocket
	{
	public:
		inline HttpSocket(SOCKET socket) :tcp::simpleSocket(socket){};
		void bind(const HttpStatus status,const json::JsonObject json,const HttpHeaders headers)const;
		void bind(const HttpStatus status,const json::JsonObject json)const;
		void bind(const HttpStatus status,const HtmlFileReader htmlFile,const HttpHeaders headers)const;
		void bind(const HttpStatus status,const HtmlFileReader htmlFile)const;

	private:
		std::string generateHttpResponceFromRequst(HttpStatus status, json::JsonObject json, const HttpHeaders headers)const;
		std::string generateHttpResponceFromRequst(HttpStatus status, HtmlFileReader htmlFile, const HttpHeaders headers)const;
	};
}

