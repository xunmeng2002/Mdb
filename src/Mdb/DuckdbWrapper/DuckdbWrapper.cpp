#include <Mdb/DuckdbWrapper/DuckdbWrapper.h>
#include <duckdb.h>
#include <cstring>
#include <sstream>


namespace
{

    class PreparedStatement
    {
    public:
        PreparedStatement(duckdb_connection connection, const char* sql)
        {
            if (duckdb_prepare(connection, sql, &stmt_) == DuckDBSuccess && stmt_)
            {
                valid_ = true;
            }
        }
        ~PreparedStatement()
        {
            if (stmt_)
            {
                duckdb_destroy_prepare(&stmt_);
            }
        }

        PreparedStatement(const PreparedStatement&) = delete;
        PreparedStatement& operator=(const PreparedStatement&) = delete;

        bool IsValid() const { return valid_; }
        duckdb_prepared_statement Get() const { return stmt_; }

    private:
        duckdb_prepared_statement stmt_ = nullptr;
        bool valid_ = false;
    };

    class StringGuard
    {
    public:
        explicit StringGuard(duckdb_string str) : str_(str) {}
        ~StringGuard()
        {
            if (str_.data)
            {
                duckdb_free(str_.data);
            }
        }
        StringGuard(const StringGuard&) = delete;
        StringGuard& operator=(const StringGuard&) = delete;

        const char* Data() const { return str_.data; }
        idx_t Size() const { return str_.size; }

    private:
        duckdb_string str_;
    };

    void BindField(duckdb_prepared_statement stmt, int index,
                   const FieldDescriptor& field, const void* record)
    {
        const char* data = static_cast<const char*>(record) + field.offset;
        switch (field.type)
        {
        case FieldType::Int:
            duckdb_bind_int32(stmt, index, *reinterpret_cast<const int*>(data));
            break;
        case FieldType::Int64:
            duckdb_bind_int64(stmt, index, *reinterpret_cast<const long long*>(data));
            break;
        case FieldType::Double:
            duckdb_bind_double(stmt, index, *reinterpret_cast<const double*>(data));
            break;
        case FieldType::Char:
            duckdb_bind_varchar_length(stmt, index, data, field.arraySize);
            break;
        case FieldType::Bool:
            duckdb_bind_boolean(stmt, index, *reinterpret_cast<const bool*>(data) ? 1 : 0);
            break;
        }
    }

    void BindAllFields(duckdb_prepared_statement stmt, const TableSchema* schema,
                       const void* record)
    {
        for (int i = 0; i < schema->fieldCount; ++i)
        {
            BindField(stmt, i + 1, schema->fields[i], record);
        }
    }

    void BindKeyFields(duckdb_prepared_statement stmt, const TableSchema* schema,
                       const void* record, const int* keyIndices, int keyCount)
    {
        for (int i = 0; i < keyCount; ++i)
        {
            BindField(stmt, i + 1, schema->fields[keyIndices[i]], record);
        }
    }

    void ReadRow(duckdb_result& result, idx_t row, const TableSchema* schema, void* record)
    {
        char* data = static_cast<char*>(record);
        for (int i = 0; i < schema->fieldCount; ++i)
        {
            const auto& field = schema->fields[i];
            char* dest = data + field.offset;
            switch (field.type)
            {
            case FieldType::Int:
                *reinterpret_cast<int*>(dest) = duckdb_value_int32(&result, i, row);
                break;
            case FieldType::Int64:
                *reinterpret_cast<long long*>(dest) = duckdb_value_int64(&result, i, row);
                break;
            case FieldType::Double:
                *reinterpret_cast<double*>(dest) = duckdb_value_double(&result, i, row);
                break;
            case FieldType::Char:
            {
                StringGuard str(duckdb_value_string(&result, i, row));
                if (str.Data())
                {
                    std::memcpy(dest, str.Data(), field.arraySize);
                }
                break;
            }
            case FieldType::Bool:
                *reinterpret_cast<bool*>(dest) = duckdb_value_boolean(&result, i, row) != 0;
                break;
            }
        }
    }

    std::string MakeCreateTableSql(const TableSchema* schema)
    {
        std::ostringstream sql;
        sql << "CREATE TABLE IF NOT EXISTS " << schema->tableName << "(";
        for (int i = 0; i < schema->fieldCount; ++i)
        {
            if (i > 0) sql << ", ";
            const auto& f = schema->fields[i];
            sql << f.name << " ";
            switch (f.type)
            {
            case FieldType::Int:    sql << "INTEGER"; break;
            case FieldType::Int64:  sql << "BIGINT"; break;
            case FieldType::Double: sql << "DOUBLE"; break;
            case FieldType::Char:   sql << "VARCHAR"; break;
            case FieldType::Bool:   sql << "BOOLEAN"; break;
            }
        }
        if (schema->primaryKeyCount > 0)
        {
            sql << ", PRIMARY KEY(";
            for (int i = 0; i < schema->primaryKeyCount; ++i)
            {
                if (i > 0) sql << ", ";
                sql << schema->fields[schema->primaryKeyIndices[i]].name;
            }
            sql << ")";
        }
        sql << ");";
        return sql.str();
    }

    std::string MakeInsertSql(const TableSchema* schema)
    {
        std::ostringstream sql;
        sql << "INSERT INTO " << schema->tableName << " (";
        for (int i = 0; i < schema->fieldCount; ++i)
        {
            if (i > 0) sql << ", ";
            sql << schema->fields[i].name;
        }
        sql << ") VALUES (";
        for (int i = 0; i < schema->fieldCount; ++i)
        {
            if (i > 0) sql << ", ";
            sql << "?";
        }
        sql << ");";
        return sql.str();
    }

    std::string MakeUpdateSql(const TableSchema* schema)
    {
        std::ostringstream sql;
        sql << "UPDATE " << schema->tableName << " SET ";
        for (int i = 0; i < schema->fieldCount; ++i)
        {
            if (i > 0) sql << ", ";
            sql << schema->fields[i].name << "=?";
        }
        sql << " WHERE ";
        for (int i = 0; i < schema->primaryKeyCount; ++i)
        {
            if (i > 0) sql << " AND ";
            int idx = schema->primaryKeyIndices[i];
            sql << schema->fields[idx].name << "=?";
        }
        sql << ";";
        return sql.str();
    }

    std::string MakeDeleteSql(const TableSchema* schema,
                              const int* keyFieldIndices, int keyFieldCount)
    {
        std::ostringstream sql;
        sql << "DELETE FROM " << schema->tableName << " WHERE ";
        for (int i = 0; i < keyFieldCount; ++i)
        {
            if (i > 0) sql << " AND ";
            sql << schema->fields[keyFieldIndices[i]].name << "=?";
        }
        sql << ";";
        return sql.str();
    }

} // anonymous namespace


struct DuckdbWrapper::Impl
{
    duckdb_database database = nullptr;
    duckdb_connection connection = nullptr;
};

DuckdbWrapper::DuckdbWrapper(const std::string& dbName)
    : m_Impl(new Impl)
{
    if (duckdb_open(dbName.c_str(), &m_Impl->database) != DuckDBSuccess)
    {
        m_Impl->database = nullptr;
        return;
    }
    duckdb_connect(m_Impl->database, &m_Impl->connection);
}
DuckdbWrapper::~DuckdbWrapper()
{
    DisConnect();
    delete m_Impl;
}

bool DuckdbWrapper::Connect()
{
    return m_Impl->connection != nullptr;
}
void DuckdbWrapper::DisConnect()
{
    if (m_Impl->connection)
    {
        duckdb_disconnect(&m_Impl->connection);
        m_Impl->connection = nullptr;
    }
    if (m_Impl->database)
    {
        duckdb_close(&m_Impl->database);
        m_Impl->database = nullptr;
    }
}
void DuckdbWrapper::Exec(const char* sql)
{
    if (!m_Impl->connection) return;
    duckdb_result result;
    duckdb_query(m_Impl->connection, sql, &result);
    duckdb_destroy_result(&result);
}

void DuckdbWrapper::CreateTable(const TableSchema* schema)
{
    Exec(MakeCreateTableSql(schema).c_str());
}
void DuckdbWrapper::DropTable(const char* tableName)
{
    std::string sql = "DROP TABLE IF EXISTS ";
    sql += tableName;
    sql += ";";
    Exec(sql.c_str());
}
void DuckdbWrapper::TruncateTable(const char* tableName)
{
    std::string sql = "DELETE FROM ";
    sql += tableName;
    sql += ";";
    Exec(sql.c_str());
}

void DuckdbWrapper::CreateTables(const TableSchema* const* schemas, int count)
{
    for (int i = 0; i < count; ++i)
    {
        CreateTable(schemas[i]);
    }
}
void DuckdbWrapper::DropTables(const TableSchema* const* schemas, int count)
{
    for (int i = 0; i < count; ++i)
    {
        DropTable(schemas[i]->tableName);
    }
}
void DuckdbWrapper::TruncateTables(const TableSchema* const* schemas, int count)
{
    for (int i = 0; i < count; ++i)
    {
        TruncateTable(schemas[i]->tableName);
    }
}

void DuckdbWrapper::Insert(const TableSchema* schema, const void* record)
{
    std::string sql = MakeInsertSql(schema);
    PreparedStatement stmt(m_Impl->connection, sql.c_str());
    if (!stmt.IsValid()) return;
    BindAllFields(stmt.Get(), schema, record);
    duckdb_result result;
    duckdb_execute_prepared(stmt.Get(), &result);
    duckdb_destroy_result(&result);
}
void DuckdbWrapper::BatchInsert(const TableSchema* schema, const void* const* records, int count)
{
    Exec("BEGIN;");
    for (int i = 0; i < count; ++i)
    {
        Insert(schema, records[i]);
    }
    Exec("COMMIT;");
}
void DuckdbWrapper::Update(const TableSchema* schema, const void* record)
{
    std::string sql = MakeUpdateSql(schema);
    PreparedStatement stmt(m_Impl->connection, sql.c_str());
    if (!stmt.IsValid()) return;

    for (int i = 0; i < schema->fieldCount; ++i)
    {
        BindField(stmt.Get(), i + 1, schema->fields[i], record);
    }
    int paramIndex = schema->fieldCount + 1;
    for (int i = 0; i < schema->primaryKeyCount; ++i)
    {
        int idx = schema->primaryKeyIndices[i];
        BindField(stmt.Get(), paramIndex, schema->fields[idx], record);
        paramIndex++;
    }

    duckdb_result result;
    duckdb_execute_prepared(stmt.Get(), &result);
    duckdb_destroy_result(&result);
}
void DuckdbWrapper::Delete(const TableSchema* schema, const void* record,
                           const int* keyFieldIndices, int keyFieldCount)
{
    std::string sql = MakeDeleteSql(schema, keyFieldIndices, keyFieldCount);
    PreparedStatement stmt(m_Impl->connection, sql.c_str());
    if (!stmt.IsValid()) return;
    BindKeyFields(stmt.Get(), schema, record, keyFieldIndices, keyFieldCount);

    duckdb_result result;
    duckdb_execute_prepared(stmt.Get(), &result);
    duckdb_destroy_result(&result);
}

void DuckdbWrapper::SelectAll(const TableSchema* schema, void* recordsList,
                              const RecordFactory& factory)
{
    std::string sql = "SELECT * FROM ";
    sql += schema->tableName;
    sql += ";";

    duckdb_result result;
    if (duckdb_query(m_Impl->connection, sql.c_str(), &result) != DuckDBSuccess)
    {
        duckdb_destroy_result(&result);
        return;
    }

    idx_t rowCount = duckdb_row_count(&result);
    for (idx_t row = 0; row < rowCount; ++row)
    {
        void* record = factory.Allocate();
        ReadRow(result, row, schema, record);
        factory.PushBack(recordsList, record);
    }
    duckdb_destroy_result(&result);
}
void DuckdbWrapper::SelectWithSql(const char* sql, const TableSchema* schema,
                                  void* recordsList, const RecordFactory& factory)
{
    duckdb_result result;
    if (duckdb_query(m_Impl->connection, sql, &result) != DuckDBSuccess)
    {
        duckdb_destroy_result(&result);
        return;
    }

    idx_t rowCount = duckdb_row_count(&result);
    for (idx_t row = 0; row < rowCount; ++row)
    {
        void* record = factory.Allocate();
        ReadRow(result, row, schema, record);
        factory.PushBack(recordsList, record);
    }
    duckdb_destroy_result(&result);
}
