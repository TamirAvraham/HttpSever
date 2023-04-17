#pragma once
#include <stdexcept>

///<summary>creates an exception based on an inherited exception</summary>
///<param name="newClassName">: name of new exception </param>
///<param name="InherClassName">: name of inherited exception  </param>
#define CreateException(newClassName,InherClassName) class newClassName: InherClassName{\
public: \
\
    newClassName(const std::string& message):InherClassName(message) {} \
    newClassName(const char* message):InherClassName(message){}\
    const char* what() const noexcept override { \
        return InherClassName::what(); \
    } \
};

namespace db {
    namespace exceptions
    {
        class DbException : public std::runtime_error {
        public:
            DbException(const std::string& message) : std::runtime_error(message) {}
            DbException(const char* message) : std::runtime_error(message) {}
            const char* what() const noexcept override {
                return std::runtime_error::what();
            }
        };
        class CantOpenException : DbException {
        public: CantOpenException(const std::string& message) :DbException(message) {} CantOpenException(const char* message) :DbException(message) {} const char* what() const noexcept override {
            return DbException::what();
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
        class AlreadyFoundExcpetion : DbException {
        public:
            AlreadyFoundExcpetion(const std::string& message) :DbException(message) {} AlreadyFoundExcpetion(const char* message) :DbException(message) {} const char* what() const noexcept override {
                return DbException::what();
            }
        };
        namespace doc {
            class DocumentNotFoundException : NotFoundException {
            public:
                DocumentNotFoundException(const std::string& message) :NotFoundException(message) {} DocumentNotFoundException(const char* message) :NotFoundException(message) {} const char* what() const noexcept override {
                    return NotFoundException::what();
                }
            };
            class DocumentAlreadyFoundExcpetion : AlreadyFoundExcpetion {
            public:
                DocumentAlreadyFoundExcpetion(const std::string& message) :AlreadyFoundExcpetion(message) {} DocumentAlreadyFoundExcpetion(const char* message) :AlreadyFoundExcpetion(message) {} const char* what() const noexcept override {
                    return AlreadyFoundExcpetion::what();
                }
            };
            class DocumentCantOpenException : CantOpenException {
            public: DocumentCantOpenException(const std::string& message) :CantOpenException(message) {} DocumentCantOpenException(const char* message) :CantOpenException(message) {} const char* what() const noexcept override {
                return CantOpenException::what();
            }
            };

        }
    }
}