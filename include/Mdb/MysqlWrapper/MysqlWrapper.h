#pragma once
#include <Mdb/MysqlWrapper/MysqlWrapperExport.h>
#include <DBInterface/DB.h>
#include <string>


class MYSQLWRAPPER_EXPORTS MysqlWrapper : public DB
{
public:
    MysqlWrapper(const std::string& host);
    ~MysqlWrapper() override;

    bool Connect() override;
    void DisConnect() override;
    void Exec(const char* sql) override;

    void CreateTable(const TableSchema* schema) override;
    void DropTable(const char* tableName) override;
    void TruncateTable(const char* tableName) override;

    void CreateTables(const TableSchema* const* schemas, int count) override;
    void DropTables(const TableSchema* const* schemas, int count) override;
    void TruncateTables(const TableSchema* const* schemas, int count) override;

    void Insert(const TableSchema* schema, const void* record) override;
    void BatchInsert(const TableSchema* schema, const void* const* records, int count) override;
    void Update(const TableSchema* schema, const void* record) override;
    void Delete(const TableSchema* schema, const void* record, const int* keyFieldIndices, int keyFieldCount) override;

    void SelectAll(const TableSchema* schema, void* recordsList, const RecordFactory& factory) override;
    void SelectWithSql(const char* sql, const TableSchema* schema, void* recordsList, const RecordFactory& factory) override;

private:
    struct Impl;
    Impl* m_Impl;
    std::string m_Host;
};
