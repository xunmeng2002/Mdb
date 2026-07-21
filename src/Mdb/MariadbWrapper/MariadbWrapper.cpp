#include <Mdb/MariadbWrapper/MariadbWrapper.h>

#include <mariadb/conncpp.hpp>

#include <cstring>
#include <cstdlib>
#include <memory>
#include <sstream>
#include <vector>


namespace
{

    void BindField(sql::PreparedStatement* pstmt, int paramIndex, const FieldDescriptor& field, const void* record)
    {
        const char* data = static_cast<const char*>(record) + field.offset;
        switch (field.type)
        {
        case FieldType::Int:
            pstmt->setInt(paramIndex, *reinterpret_cast<const int*>(data));
            break;
        case FieldType::Int64:
            pstmt->setInt64(paramIndex, *reinterpret_cast<const long long*>(data));
            break;
        case FieldType::Double:
            pstmt->setDouble(paramIndex, *reinterpret_cast<const double*>(data));
            break;
        case FieldType::Char:
        {
            std::string str(data, field.arraySize);
            auto null_pos = str.find('\0');
            if (null_pos != std::string::npos)
                str = str.substr(0, null_pos);
            pstmt->setString(paramIndex, str);
            break;
        }
        case FieldType::Bool:
            pstmt->setBoolean(paramIndex, *reinterpret_cast<const bool*>(data));
            break;
        }
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

    void ReadRow(sql::ResultSet* result, const TableSchema* schema, void* record)
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
                *reinterpret_cast<int*>(dest) = result->getInt(colIndex);
                break;
            case FieldType::Int64:
                *reinterpret_cast<long long*>(dest) = result->getInt64(colIndex);
                break;
            case FieldType::Double:
                *reinterpret_cast<double*>(dest) = static_cast<double>(result->getDouble(colIndex));
                break;
            case FieldType::Char:
            {
                sql::SQLString val = result->getString(colIndex);
                std::size_t copy_len = val.length();
                if (copy_len > static_cast<std::size_t>(field.arraySize))
                    copy_len = static_cast<std::size_t>(field.arraySize);
                std::memcpy(dest, val.c_str(), copy_len);
                break;
            }
            case FieldType::Bool:
                *reinterpret_cast<bool*>(dest) = result->getBoolean(colIndex);
                break;
            }
        }
    }

    struct SqlConnectionDeleter
    {
        void operator()(sql::Connection* conn) const
        {
            if (conn) { conn->close(); delete conn; }
        }
    };
    struct SqlStatementDeleter
    {
        void operator()(sql::Statement* stmt) const
        {
            if (stmt) { stmt->close(); delete stmt; }
        }
    };
    struct SqlPreparedStatementDeleter
    {
        void operator()(sql::PreparedStatement* pstmt) const
        {
            if (pstmt) { pstmt->close(); delete pstmt; }
        }
    };

} // anonymous namespace


struct MariadbWrapper::Impl
{
    Impl()
    {
        m_Driver = sql::mariadb::get_driver_instance();
    }

    void DisConnect()
    {
        m_Statement.reset();
        m_DBConnection.reset();
    }

    sql::Driver* m_Driver = nullptr;
    std::unique_ptr<sql::Connection, SqlConnectionDeleter> m_DBConnection;
    std::unique_ptr<sql::Statement, SqlStatementDeleter> m_Statement;
};


MariadbWrapper::MariadbWrapper(const std::string& host, const std::string& user, const std::string& passwd)
    : m_Host(host), m_User(user), m_Passwd(passwd), m_Impl(std::make_unique<Impl>())
{
}

MariadbWrapper::~MariadbWrapper()
{
    DisConnect();
}

bool MariadbWrapper::Connect()
{
#ifdef _WIN32
    _putenv_s("MARIADB_PLUGIN_DIR", MARIADB_PLUGIN_DIR);
#endif
    m_Impl->m_DBConnection.reset(
        m_Impl->m_Driver->connect(m_Host, m_User, m_Passwd));
    return m_Impl->m_DBConnection != nullptr;
}

void MariadbWrapper::DisConnect()
{
    m_Impl->DisConnect();
}

void MariadbWrapper::Exec(const char* sql)
{
    if (!m_Impl->m_Statement)
    {
        m_Impl->m_Statement.reset(
            m_Impl->m_DBConnection->createStatement());
    }
    m_Impl->m_Statement->executeUpdate(sql);
}

void MariadbWrapper::CreateTable(const TableSchema* schema)
{
    Exec(MakeCreateTableSql(schema).c_str());
}

void MariadbWrapper::DropTable(const char* tableName)
{
    std::string sql = "DROP TABLE IF EXISTS `";
    sql += tableName;
    sql += "`;";
    Exec(sql.c_str());
}

void MariadbWrapper::TruncateTable(const char* tableName)
{
    std::string sql = "TRUNCATE TABLE `";
    sql += tableName;
    sql += "`;";
    Exec(sql.c_str());
}

void MariadbWrapper::CreateTables(const TableSchema* const* schemas, int count)
{
    for (int i = 0; i < count; ++i)
        CreateTable(schemas[i]);
}

void MariadbWrapper::DropTables(const TableSchema* const* schemas, int count)
{
    for (int i = 0; i < count; ++i)
        DropTable(schemas[i]->tableName);
}

void MariadbWrapper::TruncateTables(const TableSchema* const* schemas, int count)
{
    for (int i = 0; i < count; ++i)
        TruncateTable(schemas[i]->tableName);
}

void MariadbWrapper::Insert(const TableSchema* schema, const void* record)
{
    std::string sql = MakeInsertSql(schema);
    auto pstmt = std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter>(
        m_Impl->m_DBConnection->prepareStatement(sql));
    for (int i = 0; i < schema->fieldCount; ++i)
        BindField(pstmt.get(), i + 1, schema->fields[i], record);
    pstmt->executeUpdate();
}

void MariadbWrapper::BatchInsert(const TableSchema* schema, const void* const* records, int count)
{
    Exec("START TRANSACTION;");
    for (int i = 0; i < count; ++i)
        Insert(schema, records[i]);
    Exec("COMMIT;");
}

void MariadbWrapper::Update(const TableSchema* schema, const void* record)
{
    std::string sql = MakeUpdateSql(schema);
    auto pstmt = std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter>(
        m_Impl->m_DBConnection->prepareStatement(sql));
    int paramIndex = 1;
    for (int i = 0; i < schema->fieldCount; ++i)
        BindField(pstmt.get(), paramIndex++, schema->fields[i], record);
    for (int i = 0; i < schema->primaryKeyCount; ++i)
    {
        int idx = schema->primaryKeyIndices[i];
        BindField(pstmt.get(), paramIndex++, schema->fields[idx], record);
    }
    pstmt->executeUpdate();
}

void MariadbWrapper::Delete(const TableSchema* schema, const void* record,
                              const int* keyFieldIndices, int keyFieldCount)
{
    std::string sql = MakeDeleteSql(schema, keyFieldIndices, keyFieldCount);
    auto pstmt = std::unique_ptr<sql::PreparedStatement, SqlPreparedStatementDeleter>(
        m_Impl->m_DBConnection->prepareStatement(sql));
    for (int i = 0; i < keyFieldCount; ++i)
        BindField(pstmt.get(), i + 1, schema->fields[keyFieldIndices[i]], record);
    pstmt->executeUpdate();
}

void MariadbWrapper::SelectAll(const TableSchema* schema, void* recordsList,
                                 const RecordFactory& factory)
{
    std::string sql = "SELECT * FROM `";
    sql += schema->tableName;
    sql += "`;";

    if (!m_Impl->m_Statement)
    {
        m_Impl->m_Statement.reset(
            m_Impl->m_DBConnection->createStatement());
    }
    auto result = std::unique_ptr<sql::ResultSet>(
        m_Impl->m_Statement->executeQuery(sql));
    while (result->next())
    {
        void* record = factory.Allocate();
        ReadRow(result.get(), schema, record);
        factory.PushBack(recordsList, record);
    }
}

void MariadbWrapper::SelectWithSql(const char* sql, const TableSchema* schema,
                                     void* recordsList, const RecordFactory& factory)
{
    if (!m_Impl->m_Statement)
    {
        m_Impl->m_Statement.reset(
            m_Impl->m_DBConnection->createStatement());
    }
    auto result = std::unique_ptr<sql::ResultSet>(
        m_Impl->m_Statement->executeQuery(sql));
    while (result->next())
    {
        void* record = factory.Allocate();
        ReadRow(result.get(), schema, record);
        factory.PushBack(recordsList, record);
    }
}
