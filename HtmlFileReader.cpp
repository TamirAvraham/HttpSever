#include "HtmlFileReader.h"

http::HtmlFileReader::HtmlFileReader(std::string filename):_fileName(filename)
{
	_currentFileReadOut = readfile();
}

http::HtmlFileReader::~HtmlFileReader()
{
}

std::string http::HtmlFileReader::getFileAsString()
{
	return _currentFileReadOut;
}

size_t http::HtmlFileReader::getSizeOfFile()
{
	return _currentFileReadOut.size();
}

void http::HtmlFileReader::refreshFile()
{

	_currentFileReadOut = readfile();
}

std::string http::HtmlFileReader::readfile()
{
	std::string lineFromFile, ret;
	std::ifstream file;
	file.open(_fileName);
	while (std::getline(file,lineFromFile))
	{
		ret += lineFromFile;
		ret += '\n';
	}

	return ret;
}
