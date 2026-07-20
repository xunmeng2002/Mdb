#pragma once
#include <DBInterface/Schema.h>
#include <string>
#include <list>
#include <vector>


class DB
{
public:
    virtual ~DB() = default;

    virtual bool Connect() = 0;
    virtual void DisConnect() = 0;
    virtual void InitDB() = 0;
    virtual void Exec(const char* sql) = 0;

    virtual void CreateTable(const TableSchema* schema) = 0;
    virtual void DropTable(const char* tableName) = 0;
    virtual void TruncateTable(const char* tableName) = 0;

    virtual void CreateTables(const TableSchema* const* schemas, int count) = 0;
    virtual void DropTables(const TableSchema* const* schemas, int count) = 0;
    virtual void TruncateTables(const TableSchema* const* schemas, int count) = 0;

    virtual void Insert(const TableSchema* schema, const void* record) = 0;
    virtual void BatchInsert(const TableSchema* schema, const void* const* records, int count) = 0;
    virtual void Update(const TableSchema* schema, const void* record) = 0;
    virtual void Delete(const TableSchema* schema, const void* record,
                        const int* keyFieldIndices, int keyFieldCount) = 0;

    virtual void SelectAll(const TableSchema* schema, void* recordsList,
                           const RecordFactory& factory) = 0;
    virtual void SelectWithSql(const char* sql, const TableSchema* schema,
                               void* recordsList, const RecordFactory& factory) = 0;
};
