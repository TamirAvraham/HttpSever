#pragma once
#include <stdexcept>

namespace DocDB {
    class DbException : public std::runtime_error {
    public:
        DbException(const std::string& message) : std::runtime_error(message) {}
        DbException(const char* message) : std::runtime_error(message) {}
        const char* what() const noexcept override {
            return std::runtime_error::what();
        }
    };

    class NotFoundError : public DbException {
    public:
        NotFoundError(const std::string& message) : DbException(message) {}
        NotFoundError(const char* message) : DbException(message) {}
        const char* what() const noexcept override {
            return DbException::what();
        }
    };
	
}