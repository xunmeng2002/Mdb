#pragma once
#include <DBInterface/Schema.h>


namespace mdb
{

const TableSchema* GetSchemaByTableID(unsigned int tableID);

extern const TableSchema* const kAllSchemas[];
extern const int kTableCount;

} // namespace mdb
