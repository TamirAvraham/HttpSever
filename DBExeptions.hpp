#pragma once
#include <stdexcept>
#define CreateException(newClassName,InherClassName) class newClassName: InherClassName{\
    newClassName(const std::string& message):InherClassName(message) {} \
    newClassName(const char* message):InherClassName(message){}\
    const char* what() const noexcept override { \
        return InherClassName::what(); \
    } \
};

namespace DocDB {
    class DbException : public std::runtime_error {
    public:
        DbException(const std::string& message) : std::runtime_error(message) {}
        DbException(const char* message) : std::runtime_error(message) {}
        const char* what() const noexcept override {
            return std::runtime_error::what();
        }
    };

    class NotFoundException : public DbException {
    public:
        NotFoundException(const std::string& message) : DbException(message) {}
        NotFoundException(const char* message) : DbException(message) {}
        const char* what() const noexcept override {
            return DbException::what();
        }
    };
    namespace doc {
        class DocumentNotFoundException : NotFoundException {
            DocumentNotFoundException(const std::string& message) :NotFoundException(message) {} DocumentNotFoundException(const char* message) :NotFoundException(message) {} const char* what() const noexcept override {
                return NotFoundException::what();
            }
        };
    }
}