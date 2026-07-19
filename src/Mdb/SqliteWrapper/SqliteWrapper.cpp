#include <Mdb/SqliteWrapper/SqliteWrapper.h>
#include <sqlite3.h>
#include <cstring>
#include <sstream>
#include <cstdio>


struct SqliteWrapper::Impl
{
	sqlite3* db = nullptr;
};

SqliteWrapper::SqliteWrapper(const std::string& dbName)
	: m_Impl(new Impl)
{
	int rc = sqlite3_open(dbName.c_str(), &m_Impl->db);
	if (rc != SQLITE_OK)
	{
		m_Impl->db = nullptr;
		return;
	}
	sqlite3_exec(m_Impl->db, "PRAGMA encoding = 'UTF-8';", nullptr, nullptr, nullptr);

	// Check actual database encoding
	sqlite3_stmt* stmt = nullptr;
	sqlite3_prepare_v2(m_Impl->db, "PRAGMA encoding;", -1, &stmt, nullptr);
	if (stmt)
	{
		if (sqlite3_step(stmt) == SQLITE_ROW)
		{
			const unsigned char* enc = sqlite3_column_text(stmt, 0);
			std::printf("[SqliteWrapper] Database encoding: %s\n", enc ? (const char*)enc : "NULL");
		}
		sqlite3_finalize(stmt);
	}
}
SqliteWrapper::~SqliteWrapper()
{
	DisConnect();
	delete m_Impl;
}

bool SqliteWrapper::Connect()
{
	return m_Impl->db != nullptr;
}
void SqliteWrapper::DisConnect()
{
	if (m_Impl->db)
	{
		sqlite3_close(m_Impl->db);
		m_Impl->db = nullptr;
	}
}
void SqliteWrapper::InitDB()
{
}
void SqliteWrapper::Exec(const char* sql)
{
	if (!m_Impl->db) return;
	char* errMsg = nullptr;
	sqlite3_exec(m_Impl->db, sql, nullptr, nullptr, &errMsg);
	if (errMsg)
	{
		sqlite3_free(errMsg);
	}
}

void SqliteWrapper::BindField(sqlite3_stmt* stmt, int index, const FieldDescriptor& field, const void* record)
{
	const char* data = static_cast<const char*>(record) + field.offset;
	switch (field.type)
	{
	case FieldType::Int:
		sqlite3_bind_int(stmt, index, *reinterpret_cast<const int*>(data));
		break;
	case FieldType::Int64:
		sqlite3_bind_int64(stmt, index, *reinterpret_cast<const long long*>(data));
		break;
	case FieldType::Double:
		sqlite3_bind_double(stmt, index, *reinterpret_cast<const double*>(data));
		break;
	case FieldType::Char:
		sqlite3_bind_text(stmt, index, data, -1, nullptr);
		break;
	case FieldType::Bool:
		sqlite3_bind_int(stmt, index, *reinterpret_cast<const bool*>(data) ? 1 : 0);
		break;
	}
}

void SqliteWrapper::BindFields(sqlite3_stmt* stmt, const TableSchema* schema, const void* record)
{
	for (int i = 0; i < schema->fieldCount; ++i)
	{
		BindField(stmt, i + 1, schema->fields[i], record);
	}
}

void SqliteWrapper::BindKeyFields(sqlite3_stmt* stmt, const TableSchema* schema,
                                   const void* record, const int* keyIndices, int keyCount)
{
	for (int i = 0; i < keyCount; ++i)
	{
		BindField(stmt, i + 1, schema->fields[keyIndices[i]], record);
	}
}

void SqliteWrapper::CreateTable(const TableSchema* schema)
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
			sql << schema->fields[schema->primaryKeyIndices[i]].name;
		}
		sql << ")";
	}
	sql << ");";
	Exec(sql.str().c_str());
}

void SqliteWrapper::DropTable(const char* tableName)
{
	std::string sql = "DROP TABLE IF EXISTS ";
	sql += tableName;
	sql += ";";
	Exec(sql.c_str());
}

void SqliteWrapper::TruncateTable(const char* tableName)
{
	std::string sql = "DELETE FROM ";
	sql += tableName;
	sql += ";";
	Exec(sql.c_str());
}

void SqliteWrapper::CreateTables(const TableSchema* const* schemas, int count)
{
	for (int i = 0; i < count; ++i)
	{
		CreateTable(schemas[i]);
	}
}

void SqliteWrapper::DropTables(const TableSchema* const* schemas, int count)
{
	for (int i = 0; i < count; ++i)
	{
		DropTable(schemas[i]->tableName);
	}
}

void SqliteWrapper::TruncateTables(const TableSchema* const* schemas, int count)
{
	for (int i = 0; i < count; ++i)
	{
		TruncateTable(schemas[i]->tableName);
	}
}

void SqliteWrapper::Insert(const TableSchema* schema, const void* record)
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

	sqlite3_stmt* stmt = nullptr;
	sqlite3_prepare_v2(m_Impl->db, sql.str().c_str(), -1, &stmt, nullptr);
	if (!stmt) return;
	BindFields(stmt, schema, record);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
}

void SqliteWrapper::BatchInsert(const TableSchema* schema, const void* const* records, int count)
{
	sqlite3_exec(m_Impl->db, "BEGIN;", nullptr, nullptr, nullptr);
	for (int i = 0; i < count; ++i)
	{
		Insert(schema, records[i]);
	}
	sqlite3_exec(m_Impl->db, "COMMIT;", nullptr, nullptr, nullptr);
}

void SqliteWrapper::Update(const TableSchema* schema, const void* record)
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

	sqlite3_stmt* stmt = nullptr;
	sqlite3_prepare_v2(m_Impl->db, sql.str().c_str(), -1, &stmt, nullptr);
	if (!stmt) return;
	for (int i = 0; i < schema->fieldCount; ++i)
	{
		BindField(stmt, i + 1, schema->fields[i], record);
	}
	int paramIndex = schema->fieldCount + 1;
	for (int i = 0; i < schema->primaryKeyCount; ++i)
	{
		int idx = schema->primaryKeyIndices[i];
		BindField(stmt, paramIndex, schema->fields[idx], record);
		paramIndex++;
	}
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
}

void SqliteWrapper::Delete(const TableSchema* schema, const void* record,
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

	sqlite3_stmt* stmt = nullptr;
	sqlite3_prepare_v2(m_Impl->db, sql.str().c_str(), -1, &stmt, nullptr);
	if (!stmt) return;
	BindKeyFields(stmt, schema, record, keyFieldIndices, keyFieldCount);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
}

void SqliteWrapper::ReadRow(sqlite3_stmt* stmt, const TableSchema* schema, void* record)
{
	char* data = static_cast<char*>(record);
	for (int i = 0; i < schema->fieldCount; ++i)
	{
		const auto& field = schema->fields[i];
		char* dest = data + field.offset;
		switch (field.type)
		{
		case FieldType::Int:
			*reinterpret_cast<int*>(dest) = sqlite3_column_int(stmt, i);
			break;
		case FieldType::Int64:
			*reinterpret_cast<long long*>(dest) = sqlite3_column_int64(stmt, i);
			break;
		case FieldType::Double:
			*reinterpret_cast<double*>(dest) = sqlite3_column_double(stmt, i);
			break;
		case FieldType::Char:
		{
			const unsigned char* text = sqlite3_column_text(stmt, i);
			if (text)
			{
				std::memcpy(dest, text, field.arraySize);
			}
			break;
		}
		case FieldType::Bool:
			*reinterpret_cast<bool*>(dest) = sqlite3_column_int(stmt, i) != 0;
			break;
		}
	}
}

void SqliteWrapper::SelectAll(const TableSchema* schema, void* recordsList,
                               const RecordFactory& factory)
{
	std::string sql = "SELECT * FROM ";
	sql += schema->tableName;
	sql += ";";

	sqlite3_stmt* stmt = nullptr;
	sqlite3_prepare_v2(m_Impl->db, sql.c_str(), -1, &stmt, nullptr);
	if (!stmt) return;

	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		void* record = factory.Allocate();
		ReadRow(stmt, schema, record);
		factory.PushBack(recordsList, record);
	}
	sqlite3_finalize(stmt);
}

void SqliteWrapper::SelectWithSql(const char* sql, const TableSchema* schema,
                                   void* recordsList, const RecordFactory& factory)
{
	sqlite3_stmt* stmt = nullptr;
	sqlite3_prepare_v2(m_Impl->db, sql, -1, &stmt, nullptr);
	if (!stmt) return;

	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		void* record = factory.Allocate();
		ReadRow(stmt, schema, record);
		factory.PushBack(recordsList, record);
	}
	sqlite3_finalize(stmt);
}
