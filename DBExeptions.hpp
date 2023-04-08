#pragma once
#include <stdexcept>

namespace DocDB {
	using DBExeption = std::exception;
	class NotFound :DBExeption {};
	
}