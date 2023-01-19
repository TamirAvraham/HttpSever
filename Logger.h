#pragma once
#include <string>
#include <iostream>
class Logger
{
public:
	void log(const std::string& message);
	void exitWithError(const std::string& errorMessage);
	void SetOutput(std::ostream output);
private:
	std::ostream _output;
};

