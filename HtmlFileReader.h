#pragma once
#include<string>
#include <fstream>
#include <sstream>
namespace http {
	class HtmlFileReader
	{
	public:
		HtmlFileReader(std::string filename);
		~HtmlFileReader();
		std::string getFileAsString();
		size_t getSizeOfFile();
		void refreshFile();
	private:
		std::string _fileName;
		std::string _currentFileReadOut;


		std::string readfile();
	};
}


