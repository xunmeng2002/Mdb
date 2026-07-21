#pragma once
#include <DBInterface/Schema.h>


class SchemaRegistry
{
public:
    virtual ~SchemaRegistry() = default;

    virtual const TableSchema* GetSchema(unsigned int tableID) const = 0;
    virtual const TableSchema* const* GetAllSchemas() const = 0;
    virtual int GetTableCount() const = 0;
};
