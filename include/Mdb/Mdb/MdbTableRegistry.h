#pragma once
#include <Mdb/Mdb/MdbExport.h>
#include <DBAdapters/DBInterface/SchemaRegistry.h>


namespace mdb
{

class MDB_EXPORTS MdbTableRegistry : public SchemaRegistry
{
public:
    static MdbTableRegistry& Instance();

    const TableSchema* GetSchema(unsigned int tableID) const override;
    const TableSchema* const* GetAllSchemas() const override;
    int GetTableCount() const override;

private:
    MdbTableRegistry() = default;
    MdbTableRegistry(const MdbTableRegistry&) = delete;
    MdbTableRegistry& operator=(const MdbTableRegistry&) = delete;
};

} // namespace mdb
