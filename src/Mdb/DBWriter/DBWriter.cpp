#include <Mdb/DBWriter/DBWriter.h>
#include "DBOperateImpl.h"
#include <PersonalLib/Core/Logger/Logger.h>
#include <cstring>
#include <vector>

using namespace std;


DBWriter::DBWriter(DB* db, SchemaRegistry* schemaRegistry)
	:ThreadBase("DBWriter"), m_DB(db), m_SchemaRegistry(schemaRegistry), m_DBSubscriber(nullptr)
{
}
DBWriter::~DBWriter()
{
	if (m_DB != nullptr)
	{
		delete m_DB;
		m_DB = nullptr;
	}
}
void DBWriter::Subscribe(DBSubscriber* dbSubscriber)
{
	m_DBSubscriber = dbSubscriber;
}
DB* DBWriter::GetDB()
{
	return m_DB;
}
bool DBWriter::Connect()
{
	if (m_DB->Connect())
	{
		m_Connected = true;
		if (m_DBSubscriber != nullptr)
		{
			m_DBSubscriber->OnDBConnected();
		}
		return true;
	}
	return false;
}
void DBWriter::DisConnect()
{
	m_Connected = false;
	if (m_DBSubscriber != nullptr)
	{
		m_DBSubscriber->OnDBDisConnected();
	}
	m_DB->DisConnect();
	lock_guard<mutex> guard(m_Mutex);
	for (auto item : m_DBOperates)
	{
		item->DeallocateRecord();
		item->Deallocate();
	}
	m_DBOperates.clear();
}

// ---- Generic MdbSubscriber overrides ----

void DBWriter::OnTableOp(DBOperateType op)
{
	DBOperate* dbOperate = AllocateDBOperate();
	dbOperate->Operate = op;
	dbOperate->TableID = 0;
	dbOperate->Record = nullptr;
	AddDBOperate(dbOperate);
}

void DBWriter::OnRecordInsert(unsigned int tableID, void* record)
{
	DBOperate* dbOperate = AllocateDBOperate();
	dbOperate->Operate = DBOperateType::Insert;
	dbOperate->TableID = tableID;
	dbOperate->Record = record;
	AddDBOperate(dbOperate);
}

void DBWriter::OnRecordBatchInsert(unsigned int tableID, std::vector<const void*>* records)
{
	DBOperate* dbOperate = AllocateDBOperate();
	dbOperate->Operate = DBOperateType::BatchInsert;
	dbOperate->TableID = tableID;
	dbOperate->Record = nullptr;

	auto& batch = static_cast<DBOperateImpl*>(dbOperate)->GetBatchData();
	batch.swap(*records);
	delete records;
	AddDBOperate(dbOperate);
}

void DBWriter::OnRecordErase(unsigned int tableID, void* record)
{
	DBOperate* dbOperate = AllocateDBOperate();
	dbOperate->Operate = DBOperateType::Delete;
	dbOperate->TableID = tableID;
	dbOperate->Record = record;
	AddDBOperate(dbOperate);
}

void DBWriter::OnRecordEraseByIndex(unsigned int tableID, unsigned int indexID, void* record)
{
	DBOperate* dbOperate = AllocateDBOperate();
	dbOperate->Operate = DBOperateType::DeleteByIndex;
	dbOperate->TableID = tableID;
	dbOperate->IndexID = indexID;
	dbOperate->Record = record;
	AddDBOperate(dbOperate);
}

void DBWriter::OnRecordUpdate(unsigned int tableID, void* record)
{
	DBOperate* dbOperate = AllocateDBOperate();
	dbOperate->Operate = DBOperateType::Update;
	dbOperate->TableID = tableID;
	dbOperate->Record = record;
	AddDBOperate(dbOperate);
}

void DBWriter::OnRecordTruncate(unsigned int tableID)
{
	DBOperate* dbOperate = AllocateDBOperate();
	dbOperate->Operate = DBOperateType::Truncate;
	dbOperate->TableID = tableID;
	dbOperate->Record = nullptr;
	AddDBOperate(dbOperate);
}


void DBWriter::Run()
{
	CheckConnect();
	CheckDBOperate();
	HandleDBOperate();
}
void DBWriter::CheckConnect()
{
	if (!m_Connected)
	{
		Connect();
	}
}
void DBWriter::CheckDBOperate()
{
	unique_lock<mutex> guard(m_Mutex);
	m_ConditionVariable.wait_for(guard, m_TimeOut, [&] {return !m_DBOperates.empty(); });
}
void DBWriter::HandleDBOperate()
{
	if (!m_Connected)
		return;
	DBOperate* dbOperate = nullptr;
	try
	{
		while ((dbOperate = GetDBOperate()) != nullptr)
		{
			switch (dbOperate->Operate)
			{
			case DBOperateType::CreateTables:		CreateTables(dbOperate); break;
			case DBOperateType::DropTables:			DropTables(dbOperate); break;
			case DBOperateType::TruncateTables:		TruncateTables(dbOperate); break;
			case DBOperateType::Insert:				InsertRecord(dbOperate); break;
			case DBOperateType::Delete:				DeleteRecord(dbOperate); break;
			case DBOperateType::DeleteByIndex:		DeleteRecordByIndex(dbOperate); break;
			case DBOperateType::Update:				UpdateRecord(dbOperate); break;
			case DBOperateType::BatchInsert:		BatchInsertRecords(dbOperate); break;
			case DBOperateType::Truncate:			TruncateTable(dbOperate); break;
			default:
				WriteLog(LogLevel::Warning, "Unknown DBOperateType:%d", dbOperate->Operate);
				break;
			}
			dbOperate->Deallocate();
		}
	}
	catch(exception e)
	{
		WriteLog(LogLevel::Warning, "HandleDBOperate Failed. TableID;0x%X, Operate:%d, Message:%s", dbOperate->TableID, (int)dbOperate->Operate, e.what());
		DisConnect();
		dbOperate->DeallocateRecord();
		dbOperate->Deallocate();
		this_thread::sleep_for(chrono::seconds(5));
	}
}
DBOperate* DBWriter::GetDBOperate()
{
	lock_guard<mutex> guard(m_Mutex);
	if (m_DBOperates.empty())
	{
		return nullptr;
	}
	auto item = m_DBOperates.front();
	m_DBOperates.pop_front();
	return item;
}


void DBWriter::AddDBOperate(DBOperate* dbOperate)
{
	{
		lock_guard<mutex> guard(m_Mutex);
		m_DBOperates.push_back(dbOperate);
	}
	m_ConditionVariable.notify_one();
}

DBOperate* DBWriter::AllocateDBOperate()
{
	DBOperate* op = DBOperate::Allocate();
	static_cast<DBOperateImpl*>(op)->SetSchemaRegistry(m_SchemaRegistry);
	return op;
}


void DBWriter::CreateTables(DBOperate* dbOperate)
{
	m_DB->CreateTables(m_SchemaRegistry->GetAllSchemas(), m_SchemaRegistry->GetTableCount());
}
void DBWriter::DropTables(DBOperate* dbOperate)
{
	m_DB->DropTables(m_SchemaRegistry->GetAllSchemas(), m_SchemaRegistry->GetTableCount());
}
void DBWriter::TruncateTables(DBOperate* dbOperate)
{
	m_DB->TruncateTables(m_SchemaRegistry->GetAllSchemas(), m_SchemaRegistry->GetTableCount());
}
void DBWriter::InsertRecord(DBOperate* dbOperate)
{
	const TableSchema* schema = m_SchemaRegistry->GetSchema(dbOperate->TableID);
	if (schema)
	{
		m_DB->Insert(schema, dbOperate->Record);
	}
}
void DBWriter::BatchInsertRecords(DBOperate* dbOperate)
{
	const TableSchema* schema = m_SchemaRegistry->GetSchema(dbOperate->TableID);
	if (!schema) return;

	auto& batch = static_cast<DBOperateImpl*>(dbOperate)->GetBatchData();
	if (!batch.empty())
	{
		m_DB->BatchInsert(schema, batch.data(), static_cast<int>(batch.size()));
	}
}
void DBWriter::DeleteRecord(DBOperate* dbOperate)
{
	const TableSchema* schema = m_SchemaRegistry->GetSchema(dbOperate->TableID);
	if (schema)
	{
		m_DB->Delete(schema, dbOperate->Record, schema->primaryKeyIndices, schema->primaryKeyCount);
		schema->DeallocateRecord(dbOperate->Record);
	}
}

void DBWriter::DeleteRecordByIndex(DBOperate* dbOperate)
{
	const TableSchema* schema = m_SchemaRegistry->GetSchema(dbOperate->TableID);
	if (!schema) return;

	for (int i = 0; i < schema->secondaryIndexCount; ++i)
	{
		if (schema->secondaryIndices[i].indexID == dbOperate->IndexID)
		{
			m_DB->Delete(schema, dbOperate->Record,
			             schema->secondaryIndices[i].fieldIndices,
			             schema->secondaryIndices[i].fieldCount);
			schema->DeallocateRecord(dbOperate->Record);
			return;
		}
	}
	WriteLog(LogLevel::Error, "Incorrect TableID/IndexID for DeleteRecordByIndex. TableID:0x%X, IndexID:%d", dbOperate->TableID, dbOperate->IndexID);
	schema->DeallocateRecord(dbOperate->Record);
}
void DBWriter::UpdateRecord(DBOperate* dbOperate)
{
	const TableSchema* schema = m_SchemaRegistry->GetSchema(dbOperate->TableID);
	if (schema)
	{
		m_DB->Update(schema, dbOperate->Record);
		schema->DeallocateRecord(dbOperate->Record);
	}
}
void DBWriter::TruncateTable(DBOperate* dbOperate)
{
	const TableSchema* schema = m_SchemaRegistry->GetSchema(dbOperate->TableID);
	if (schema)
	{
		m_DB->TruncateTable(schema->tableName);
	}
}
