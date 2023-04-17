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
        class AlreadyExistsExcpetion : DbException {
        public:
            AlreadyExistsExcpetion(const std::string& message) :DbException(message) {} AlreadyExistsExcpetion(const char* message) :DbException(message) {} const char* what() const noexcept override {
                return DbException::what();
            }
        };
        namespace doc {

            /// <summary>
            /// Document exceptions
            /// </summary>

            class DocumentNotFoundException : NotFoundException {
            public:
                DocumentNotFoundException(const std::string& message) :NotFoundException(message) {} DocumentNotFoundException(const char* message) :NotFoundException(message) {} const char* what() const noexcept override {
                    return NotFoundException::what();
                }
            };
            class DocumentAlreadyExistsExcpetion : AlreadyExistsExcpetion {
            public:
                DocumentAlreadyExistsExcpetion(const std::string& message) :AlreadyExistsExcpetion(message) {} DocumentAlreadyExistsExcpetion(const char* message) :AlreadyExistsExcpetion(message) {} const char* what() const noexcept override {
                    return AlreadyExistsExcpetion::what();
                }
            };
            class DocumentCantOpenException : CantOpenException {
            public: DocumentCantOpenException(const std::string& message) :CantOpenException(message) {} DocumentCantOpenException(const char* message) :CantOpenException(message) {} const char* what() const noexcept override {
                return CantOpenException::what();
            }
            };

            /// <summary>
            /// Collection exceptions
            /// </summary>

            class CollectionNotFoundException : NotFoundException {
            public: CollectionNotFoundException(const std::string& message) :NotFoundException(message) {} CollectionNotFoundException(const char* message) :NotFoundException(message) {} const char* what() const noexcept override {
                return NotFoundException::what();
            }
            };

            class CollectionCantOpenException : CantOpenException {
            public: CollectionCantOpenException(const std::string& message) :CantOpenException(message) {} CollectionCantOpenException(const char* message) :CantOpenException(message) {} const char* what() const noexcept override {
                return CantOpenException::what();
            }
            };

            class CollectionAlreadyExistsExcpetion : AlreadyExistsExcpetion {
            public: CollectionAlreadyExistsExcpetion(const std::string& message) :AlreadyExistsExcpetion(message) {} CollectionAlreadyExistsExcpetion(const char* message) :AlreadyExistsExcpetion(message) {} const char* what() const noexcept override {
                return AlreadyExistsExcpetion::what();
            }
            };

        }
    }
}