#include <Mdb/MysqlWrapper/MysqlWrapper.h>
#include <mysqlx/xdevapi.h>
#include <cstring>
#include <sstream>
#include <vector>


namespace
{

    mysqlx::Value FieldToValue(const FieldDescriptor& field, const void* record)
    {
        const char* data = static_cast<const char*>(record) + field.offset;
        switch (field.type)
        {
        case FieldType::Int:
            return mysqlx::Value(*reinterpret_cast<const int*>(data));
        case FieldType::Int64:
            return mysqlx::Value(static_cast<int64_t>(*reinterpret_cast<const long long*>(data)));
        case FieldType::Double:
            return mysqlx::Value(*reinterpret_cast<const double*>(data));
        case FieldType::Char:
        {
            std::string str(data, field.arraySize);
            auto null_pos = str.find('\0');
            if (null_pos != std::string::npos)
                str = str.substr(0, null_pos);
            return mysqlx::Value(str);
        }
        case FieldType::Bool:
            return mysqlx::Value(*reinterpret_cast<const bool*>(data));
        }
        return mysqlx::Value();
    }

    std::string MakeCreateTableSql(const TableSchema* schema)
    {
        std::ostringstream sql;
        sql << "CREATE TABLE IF NOT EXISTS `" << schema->tableName << "`(";
        for (int i = 0; i < schema->fieldCount; ++i)
        {
            if (i > 0) sql << ", ";
            const auto& f = schema->fields[i];
            sql << "`" << f.name << "` ";
            switch (f.type)
            {
            case FieldType::Int:    sql << "int"; break;
            case FieldType::Int64:  sql << "bigint"; break;
            case FieldType::Double: sql << "double"; break;
            case FieldType::Char:   sql << "char(" << f.arraySize << ")"; break;
            case FieldType::Bool:   sql << "bool"; break;
            }
        }
        if (schema->primaryKeyCount > 0)
        {
            sql << ", PRIMARY KEY(";
            for (int i = 0; i < schema->primaryKeyCount; ++i)
            {
                if (i > 0) sql << ", ";
                sql << "`" << schema->fields[schema->primaryKeyIndices[i]].name << "`";
            }
            sql << ")";
        }
        sql << ") ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
        return sql.str();
    }

    std::string MakeInsertSql(const TableSchema* schema)
    {
        std::ostringstream sql;
        sql << "INSERT INTO `" << schema->tableName << "` (";
        for (int i = 0; i < schema->fieldCount; ++i)
        {
            if (i > 0) sql << ", ";
            sql << "`" << schema->fields[i].name << "`";
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
        sql << "UPDATE `" << schema->tableName << "` SET ";
        for (int i = 0; i < schema->fieldCount; ++i)
        {
            if (i > 0) sql << ", ";
            sql << "`" << schema->fields[i].name << "`=?";
        }
        sql << " WHERE ";
        for (int i = 0; i < schema->primaryKeyCount; ++i)
        {
            if (i > 0) sql << " AND ";
            int idx = schema->primaryKeyIndices[i];
            sql << "`" << schema->fields[idx].name << "`=?";
        }
        sql << ";";
        return sql.str();
    }

    std::string MakeDeleteSql(const TableSchema* schema,
                               const int* keyFieldIndices, int keyFieldCount)
    {
        std::ostringstream sql;
        sql << "DELETE FROM `" << schema->tableName << "` WHERE ";
        for (int i = 0; i < keyFieldCount; ++i)
        {
            if (i > 0) sql << " AND ";
            sql << "`" << schema->fields[keyFieldIndices[i]].name << "`=?";
        }
        sql << ";";
        return sql.str();
    }

    void ReadRow(const mysqlx::Row& row, const TableSchema* schema, void* record)
    {
        char* data = static_cast<char*>(record);
        for (int i = 0; i < schema->fieldCount; ++i)
        {
            const auto& field = schema->fields[i];
            char* dest = data + field.offset;
            int colIndex = i + 1;
            switch (field.type)
            {
            case FieldType::Int:
                *reinterpret_cast<int*>(dest) = row[colIndex].get<int>();
                break;
            case FieldType::Int64:
                *reinterpret_cast<long long*>(dest) = row[colIndex].get<int64_t>();
                break;
            case FieldType::Double:
                *reinterpret_cast<double*>(dest) = row[colIndex].get<double>();
                break;
            case FieldType::Char:
            {
                std::string val = row[colIndex].get<std::string>();
                std::memcpy(dest, val.c_str(), field.arraySize);
                break;
            }
            case FieldType::Bool:
                *reinterpret_cast<bool*>(dest) = row[colIndex].get<bool>();
                break;
            }
        }
    }

} // anonymous namespace


struct MysqlWrapper::Impl
{
    explicit Impl(mysqlx::Session&& session)
        : session(std::move(session))
    {
    }

    mysqlx::Session session;
};

MysqlWrapper::MysqlWrapper(const std::string& host)
    : m_Impl(nullptr), m_Host(host)
{
    m_Impl = new Impl(mysqlx::Session(host));
}
MysqlWrapper::~MysqlWrapper()
{
    DisConnect();
    delete m_Impl;
}

bool MysqlWrapper::Connect()
{
    return m_Impl != nullptr;
}
void MysqlWrapper::DisConnect()
{
    if (m_Impl)
    {
        m_Impl->session.close();
    }
}
void MysqlWrapper::Exec(const char* sql)
{
    m_Impl->session.sql(sql).execute();
}

void MysqlWrapper::CreateTable(const TableSchema* schema)
{
    Exec(MakeCreateTableSql(schema).c_str());
}
void MysqlWrapper::DropTable(const char* tableName)
{
    std::string sql = "DROP TABLE IF EXISTS `";
    sql += tableName;
    sql += "`;";
    Exec(sql.c_str());
}
void MysqlWrapper::TruncateTable(const char* tableName)
{
    std::string sql = "TRUNCATE TABLE `";
    sql += tableName;
    sql += "`;";
    Exec(sql.c_str());
}

void MysqlWrapper::CreateTables(const TableSchema* const* schemas, int count)
{
    for (int i = 0; i < count; ++i)
        CreateTable(schemas[i]);
}
void MysqlWrapper::DropTables(const TableSchema* const* schemas, int count)
{
    for (int i = 0; i < count; ++i)
        DropTable(schemas[i]->tableName);
}
void MysqlWrapper::TruncateTables(const TableSchema* const* schemas, int count)
{
    for (int i = 0; i < count; ++i)
        TruncateTable(schemas[i]->tableName);
}

void MysqlWrapper::Insert(const TableSchema* schema, const void* record)
{
    std::string sql = MakeInsertSql(schema);
    std::vector<mysqlx::Value> params;
    params.reserve(schema->fieldCount);
    for (int i = 0; i < schema->fieldCount; ++i)
        params.push_back(FieldToValue(schema->fields[i], record));
    m_Impl->session.sql(sql).bind(params).execute();
}
void MysqlWrapper::BatchInsert(const TableSchema* schema, const void* const* records, int count)
{
    Exec("START TRANSACTION;");
    for (int i = 0; i < count; ++i)
        Insert(schema, records[i]);
    Exec("COMMIT;");
}
void MysqlWrapper::Update(const TableSchema* schema, const void* record)
{
    std::string sql = MakeUpdateSql(schema);
    std::vector<mysqlx::Value> params;
    params.reserve(schema->fieldCount + schema->primaryKeyCount);
    for (int i = 0; i < schema->fieldCount; ++i)
        params.push_back(FieldToValue(schema->fields[i], record));
    for (int i = 0; i < schema->primaryKeyCount; ++i)
    {
        int idx = schema->primaryKeyIndices[i];
        params.push_back(FieldToValue(schema->fields[idx], record));
    }
    m_Impl->session.sql(sql).bind(params).execute();
}
void MysqlWrapper::Delete(const TableSchema* schema, const void* record,
                            const int* keyFieldIndices, int keyFieldCount)
{
    std::string sql = MakeDeleteSql(schema, keyFieldIndices, keyFieldCount);
    std::vector<mysqlx::Value> params;
    params.reserve(keyFieldCount);
    for (int i = 0; i < keyFieldCount; ++i)
        params.push_back(FieldToValue(schema->fields[keyFieldIndices[i]], record));
    m_Impl->session.sql(sql).bind(params).execute();
}

void MysqlWrapper::SelectAll(const TableSchema* schema, void* recordsList,
                               const RecordFactory& factory)
{
    std::string sql = "SELECT * FROM `";
    sql += schema->tableName;
    sql += "`;";

    auto result = m_Impl->session.sql(sql).execute();
    while (auto row = result.fetchOne())
    {
        void* record = factory.Allocate();
        ReadRow(row, schema, record);
        factory.PushBack(recordsList, record);
    }
}
void MysqlWrapper::SelectWithSql(const char* sql, const TableSchema* schema,
                                   void* recordsList, const RecordFactory& factory)
{
    auto result = m_Impl->session.sql(sql).execute();
    while (auto row = result.fetchOne())
    {
        void* record = factory.Allocate();
        ReadRow(row, schema, record);
        factory.PushBack(recordsList, record);
    }
}
