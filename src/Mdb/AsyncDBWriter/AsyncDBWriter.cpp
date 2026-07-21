#include <Mdb/AsyncDBWriter/AsyncDBWriter.h>
#include "DBOperateImpl.h"
#include <PersonalLib/Core/Logger/Logger.h>
#include <cstring>
#include <vector>

using namespace std;


AsyncDBWriter::AsyncDBWriter(DB* db, SchemaRegistry* schemaRegistry)
	:ThreadBase("AsyncDBWriter"), m_DB(db), m_SchemaRegistry(schemaRegistry), m_DBSubscriber(nullptr)
{
}
AsyncDBWriter::~AsyncDBWriter()
{
	if (m_DB != nullptr)
	{
		delete m_DB;
		m_DB = nullptr;
	}
}
void AsyncDBWriter::Subscribe(DBSubscriber* dbSubscriber)
{
	m_DBSubscriber = dbSubscriber;
}
DB* AsyncDBWriter::GetDB()
{
	return m_DB;
}
bool AsyncDBWriter::Connect()
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
void AsyncDBWriter::DisConnect()
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

void AsyncDBWriter::OnTableOp(DBOperateType op)
{
	DBOperate* dbOperate = AllocateDBOperate();
	dbOperate->Operate = op;
	dbOperate->TableID = 0;
	dbOperate->Record = nullptr;
	AddDBOperate(dbOperate);
}

void AsyncDBWriter::OnRecordInsert(unsigned int tableID, void* record)
{
	DBOperate* dbOperate = AllocateDBOperate();
	dbOperate->Operate = DBOperateType::Insert;
	dbOperate->TableID = tableID;
	dbOperate->Record = record;
	AddDBOperate(dbOperate);
}

void AsyncDBWriter::OnRecordBatchInsert(unsigned int tableID, std::vector<const void*>* records)
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

void AsyncDBWriter::OnRecordErase(unsigned int tableID, void* record)
{
	DBOperate* dbOperate = AllocateDBOperate();
	dbOperate->Operate = DBOperateType::Delete;
	dbOperate->TableID = tableID;
	dbOperate->Record = record;
	AddDBOperate(dbOperate);
}

void AsyncDBWriter::OnRecordEraseByIndex(unsigned int tableID, unsigned int indexID, void* record)
{
	DBOperate* dbOperate = AllocateDBOperate();
	dbOperate->Operate = DBOperateType::DeleteByIndex;
	dbOperate->TableID = tableID;
	dbOperate->IndexID = indexID;
	dbOperate->Record = record;
	AddDBOperate(dbOperate);
}

void AsyncDBWriter::OnRecordUpdate(unsigned int tableID, void* record)
{
	DBOperate* dbOperate = AllocateDBOperate();
	dbOperate->Operate = DBOperateType::Update;
	dbOperate->TableID = tableID;
	dbOperate->Record = record;
	AddDBOperate(dbOperate);
}

void AsyncDBWriter::OnRecordTruncate(unsigned int tableID)
{
	DBOperate* dbOperate = AllocateDBOperate();
	dbOperate->Operate = DBOperateType::Truncate;
	dbOperate->TableID = tableID;
	dbOperate->Record = nullptr;
	AddDBOperate(dbOperate);
}


void AsyncDBWriter::Run()
{
	CheckConnect();
	CheckDBOperate();
	HandleDBOperate();
}
void AsyncDBWriter::CheckConnect()
{
	if (!m_Connected)
	{
		Connect();
	}
}
void AsyncDBWriter::CheckDBOperate()
{
	unique_lock<mutex> guard(m_Mutex);
	m_ConditionVariable.wait_for(guard, m_TimeOut, [&] {return !m_DBOperates.empty(); });
}
void AsyncDBWriter::HandleDBOperate()
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
DBOperate* AsyncDBWriter::GetDBOperate()
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


void AsyncDBWriter::AddDBOperate(DBOperate* dbOperate)
{
	{
		lock_guard<mutex> guard(m_Mutex);
		m_DBOperates.push_back(dbOperate);
	}
	m_ConditionVariable.notify_one();
}

DBOperate* AsyncDBWriter::AllocateDBOperate()
{
	DBOperate* op = DBOperate::Allocate();
	static_cast<DBOperateImpl*>(op)->SetSchemaRegistry(m_SchemaRegistry);
	return op;
}


void AsyncDBWriter::CreateTables(DBOperate* dbOperate)
{
	m_DB->CreateTables(m_SchemaRegistry->GetAllSchemas(), m_SchemaRegistry->GetTableCount());
}
void AsyncDBWriter::DropTables(DBOperate* dbOperate)
{
	m_DB->DropTables(m_SchemaRegistry->GetAllSchemas(), m_SchemaRegistry->GetTableCount());
}
void AsyncDBWriter::TruncateTables(DBOperate* dbOperate)
{
	m_DB->TruncateTables(m_SchemaRegistry->GetAllSchemas(), m_SchemaRegistry->GetTableCount());
}
void AsyncDBWriter::InsertRecord(DBOperate* dbOperate)
{
	const TableSchema* schema = m_SchemaRegistry->GetSchema(dbOperate->TableID);
	if (schema)
	{
		m_DB->Insert(schema, dbOperate->Record);
	}
}
void AsyncDBWriter::BatchInsertRecords(DBOperate* dbOperate)
{
	const TableSchema* schema = m_SchemaRegistry->GetSchema(dbOperate->TableID);
	if (!schema) return;

	auto& batch = static_cast<DBOperateImpl*>(dbOperate)->GetBatchData();
	if (!batch.empty())
	{
		m_DB->BatchInsert(schema, batch.data(), static_cast<int>(batch.size()));
	}
}
void AsyncDBWriter::DeleteRecord(DBOperate* dbOperate)
{
	const TableSchema* schema = m_SchemaRegistry->GetSchema(dbOperate->TableID);
	if (schema)
	{
		m_DB->Delete(schema, dbOperate->Record, schema->primaryKeyIndices, schema->primaryKeyCount);
		schema->DeallocateRecord(dbOperate->Record);
	}
}

void AsyncDBWriter::DeleteRecordByIndex(DBOperate* dbOperate)
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
void AsyncDBWriter::UpdateRecord(DBOperate* dbOperate)
{
	const TableSchema* schema = m_SchemaRegistry->GetSchema(dbOperate->TableID);
	if (schema)
	{
		m_DB->Update(schema, dbOperate->Record);
		schema->DeallocateRecord(dbOperate->Record);
	}
}
void AsyncDBWriter::TruncateTable(DBOperate* dbOperate)
{
	const TableSchema* schema = m_SchemaRegistry->GetSchema(dbOperate->TableID);
	if (schema)
	{
		m_DB->TruncateTable(schema->tableName);
	}
}
