#include "Collection.h"

Document Collection::createDocument(const std::string&& docName) const noexcept
{
    MDB_val mdbKey{ docName.size(), const_cast<char*>(docName.data()) };
    MDB_val mdbValue{ docName.size(), const_cast<char*>(docName.data()) };
    return Document();
}
