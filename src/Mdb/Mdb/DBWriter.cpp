#include <Mdb/Mdb/DBWriter.h>
#include <Mdb/Mdb/MdbIndexes.h>
#include "MdbTableRegistry.h"
#include <PersonalLib/Core/Logger/Logger.h>
#include <cstring>
#include <vector>

using namespace std;
using namespace mdb;


DBWriter::DBWriter(DB* db)
	:ThreadBase("DBWriter"), m_DB(db), m_DBSubscriber(nullptr)
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

void DBWriter::OnCreateTables()
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::CreateTables;
	dbOperate->TableID = 0;
	dbOperate->Record = nullptr;
	AddDBOperate(dbOperate);
}
void DBWriter::OnDropTables()
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::DropTables;
	dbOperate->TableID = 0;
	dbOperate->Record = nullptr;
	AddDBOperate(dbOperate);
}
void DBWriter::OnTruncateTables()
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::TruncateTables;
	dbOperate->TableID = 0;
	dbOperate->Record = nullptr;
	AddDBOperate(dbOperate);
}

void DBWriter::OnTradingDayInsert(mdb::TradingDay* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Insert;
	dbOperate->TableID = TradingDay::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnTradingDayBatchInsert(std::list<mdb::TradingDay*>* records)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::BatchInsert;
	dbOperate->TableID = TradingDay::TableID;
	dbOperate->Record = records;

	AddDBOperate(dbOperate);
}
void DBWriter::OnTradingDayErase(mdb::TradingDay* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Delete;
	dbOperate->TableID = TradingDay::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnTradingDayUpdate(mdb::TradingDay* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Update;
	dbOperate->TableID = TradingDay::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnTradingDayTruncate()
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Truncate;
	dbOperate->TableID = TradingDay::TableID;
	dbOperate->Record = nullptr;

	AddDBOperate(dbOperate);
}

void DBWriter::OnExchangeInsert(mdb::Exchange* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Insert;
	dbOperate->TableID = Exchange::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnExchangeBatchInsert(std::list<mdb::Exchange*>* records)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::BatchInsert;
	dbOperate->TableID = Exchange::TableID;
	dbOperate->Record = records;

	AddDBOperate(dbOperate);
}
void DBWriter::OnExchangeErase(mdb::Exchange* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Delete;
	dbOperate->TableID = Exchange::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnExchangeUpdate(mdb::Exchange* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Update;
	dbOperate->TableID = Exchange::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnExchangeTruncate()
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Truncate;
	dbOperate->TableID = Exchange::TableID;
	dbOperate->Record = nullptr;

	AddDBOperate(dbOperate);
}

void DBWriter::OnProductInsert(mdb::Product* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Insert;
	dbOperate->TableID = Product::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnProductBatchInsert(std::list<mdb::Product*>* records)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::BatchInsert;
	dbOperate->TableID = Product::TableID;
	dbOperate->Record = records;

	AddDBOperate(dbOperate);
}
void DBWriter::OnProductErase(mdb::Product* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Delete;
	dbOperate->TableID = Product::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnProductUpdate(mdb::Product* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Update;
	dbOperate->TableID = Product::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnProductTruncate()
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Truncate;
	dbOperate->TableID = Product::TableID;
	dbOperate->Record = nullptr;

	AddDBOperate(dbOperate);
}

void DBWriter::OnInstrumentInsert(mdb::Instrument* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Insert;
	dbOperate->TableID = Instrument::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnInstrumentBatchInsert(std::list<mdb::Instrument*>* records)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::BatchInsert;
	dbOperate->TableID = Instrument::TableID;
	dbOperate->Record = records;

	AddDBOperate(dbOperate);
}
void DBWriter::OnInstrumentErase(mdb::Instrument* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Delete;
	dbOperate->TableID = Instrument::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnInstrumentUpdate(mdb::Instrument* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Update;
	dbOperate->TableID = Instrument::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnInstrumentTruncate()
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Truncate;
	dbOperate->TableID = Instrument::TableID;
	dbOperate->Record = nullptr;

	AddDBOperate(dbOperate);
}

void DBWriter::OnPrimaryAccountInsert(mdb::PrimaryAccount* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Insert;
	dbOperate->TableID = PrimaryAccount::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnPrimaryAccountBatchInsert(std::list<mdb::PrimaryAccount*>* records)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::BatchInsert;
	dbOperate->TableID = PrimaryAccount::TableID;
	dbOperate->Record = records;

	AddDBOperate(dbOperate);
}
void DBWriter::OnPrimaryAccountErase(mdb::PrimaryAccount* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Delete;
	dbOperate->TableID = PrimaryAccount::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnPrimaryAccountEraseByOfferIDIndex(mdb::PrimaryAccount* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::DeleteByIndex;
	dbOperate->TableID = PrimaryAccount::TableID;
	dbOperate->IndexID = PrimaryAccountIndexOfferID::IndexID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnPrimaryAccountUpdate(mdb::PrimaryAccount* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Update;
	dbOperate->TableID = PrimaryAccount::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnPrimaryAccountTruncate()
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Truncate;
	dbOperate->TableID = PrimaryAccount::TableID;
	dbOperate->Record = nullptr;

	AddDBOperate(dbOperate);
}

void DBWriter::OnAccountInsert(mdb::Account* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Insert;
	dbOperate->TableID = Account::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnAccountBatchInsert(std::list<mdb::Account*>* records)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::BatchInsert;
	dbOperate->TableID = Account::TableID;
	dbOperate->Record = records;

	AddDBOperate(dbOperate);
}
void DBWriter::OnAccountErase(mdb::Account* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Delete;
	dbOperate->TableID = Account::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnAccountUpdate(mdb::Account* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Update;
	dbOperate->TableID = Account::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnAccountTruncate()
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Truncate;
	dbOperate->TableID = Account::TableID;
	dbOperate->Record = nullptr;

	AddDBOperate(dbOperate);
}

void DBWriter::OnCapitalInsert(mdb::Capital* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Insert;
	dbOperate->TableID = Capital::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnCapitalBatchInsert(std::list<mdb::Capital*>* records)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::BatchInsert;
	dbOperate->TableID = Capital::TableID;
	dbOperate->Record = records;

	AddDBOperate(dbOperate);
}
void DBWriter::OnCapitalErase(mdb::Capital* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Delete;
	dbOperate->TableID = Capital::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnCapitalEraseByTradingDayIndex(mdb::Capital* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::DeleteByIndex;
	dbOperate->TableID = Capital::TableID;
	dbOperate->IndexID = CapitalIndexTradingDay::IndexID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnCapitalUpdate(mdb::Capital* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Update;
	dbOperate->TableID = Capital::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnCapitalTruncate()
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Truncate;
	dbOperate->TableID = Capital::TableID;
	dbOperate->Record = nullptr;

	AddDBOperate(dbOperate);
}

void DBWriter::OnPositionInsert(mdb::Position* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Insert;
	dbOperate->TableID = Position::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnPositionBatchInsert(std::list<mdb::Position*>* records)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::BatchInsert;
	dbOperate->TableID = Position::TableID;
	dbOperate->Record = records;

	AddDBOperate(dbOperate);
}
void DBWriter::OnPositionErase(mdb::Position* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Delete;
	dbOperate->TableID = Position::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnPositionEraseByAccountIndex(mdb::Position* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::DeleteByIndex;
	dbOperate->TableID = Position::TableID;
	dbOperate->IndexID = PositionIndexAccount::IndexID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnPositionEraseByTradingDayIndex(mdb::Position* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::DeleteByIndex;
	dbOperate->TableID = Position::TableID;
	dbOperate->IndexID = PositionIndexTradingDay::IndexID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnPositionUpdate(mdb::Position* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Update;
	dbOperate->TableID = Position::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnPositionTruncate()
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Truncate;
	dbOperate->TableID = Position::TableID;
	dbOperate->Record = nullptr;

	AddDBOperate(dbOperate);
}

void DBWriter::OnPositionDetailInsert(mdb::PositionDetail* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Insert;
	dbOperate->TableID = PositionDetail::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnPositionDetailBatchInsert(std::list<mdb::PositionDetail*>* records)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::BatchInsert;
	dbOperate->TableID = PositionDetail::TableID;
	dbOperate->Record = records;

	AddDBOperate(dbOperate);
}
void DBWriter::OnPositionDetailErase(mdb::PositionDetail* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Delete;
	dbOperate->TableID = PositionDetail::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnPositionDetailEraseByTradeMatchIndex(mdb::PositionDetail* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::DeleteByIndex;
	dbOperate->TableID = PositionDetail::TableID;
	dbOperate->IndexID = PositionDetailIndexTradeMatch::IndexID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnPositionDetailEraseByTradingDayIndex(mdb::PositionDetail* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::DeleteByIndex;
	dbOperate->TableID = PositionDetail::TableID;
	dbOperate->IndexID = PositionDetailIndexTradingDay::IndexID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnPositionDetailUpdate(mdb::PositionDetail* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Update;
	dbOperate->TableID = PositionDetail::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnPositionDetailTruncate()
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Truncate;
	dbOperate->TableID = PositionDetail::TableID;
	dbOperate->Record = nullptr;

	AddDBOperate(dbOperate);
}

void DBWriter::OnOrderInsert(mdb::Order* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Insert;
	dbOperate->TableID = Order::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnOrderBatchInsert(std::list<mdb::Order*>* records)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::BatchInsert;
	dbOperate->TableID = Order::TableID;
	dbOperate->Record = records;

	AddDBOperate(dbOperate);
}
void DBWriter::OnOrderErase(mdb::Order* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Delete;
	dbOperate->TableID = Order::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnOrderUpdate(mdb::Order* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Update;
	dbOperate->TableID = Order::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnOrderTruncate()
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Truncate;
	dbOperate->TableID = Order::TableID;
	dbOperate->Record = nullptr;

	AddDBOperate(dbOperate);
}

void DBWriter::OnTradeInsert(mdb::Trade* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Insert;
	dbOperate->TableID = Trade::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnTradeBatchInsert(std::list<mdb::Trade*>* records)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::BatchInsert;
	dbOperate->TableID = Trade::TableID;
	dbOperate->Record = records;

	AddDBOperate(dbOperate);
}
void DBWriter::OnTradeErase(mdb::Trade* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Delete;
	dbOperate->TableID = Trade::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnTradeUpdate(mdb::Trade* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Update;
	dbOperate->TableID = Trade::TableID;
	dbOperate->Record = record;

	AddDBOperate(dbOperate);
}
void DBWriter::OnTradeTruncate()
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Truncate;
	dbOperate->TableID = Trade::TableID;
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
			case DBOperateType::CreateTables:
			{
				CreateTables(dbOperate);
				break;
			}
			case DBOperateType::DropTables:
			{
				DropTables(dbOperate);
				break;
			}
			case DBOperateType::TruncateTables:
			{
				TruncateTables(dbOperate);
				break;
			}
			case DBOperateType::Insert:
			{
				InsertRecord(dbOperate);
				break;
			}
			case DBOperateType::Delete:
			{
				DeleteRecord(dbOperate);
				break;
			}
			case DBOperateType::DeleteByIndex:
			{
				DeleteRecordByIndex(dbOperate);
				break;
			}
			case DBOperateType::Update:
			{
				UpdateRecord(dbOperate);
				break;
			}
			case DBOperateType::BatchInsert:
			{
				BatchInsertRecords(dbOperate);
				break;
			}
			case DBOperateType::Truncate:
			{
				TruncateTable(dbOperate);
				break;
			}
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


void DBWriter::CreateTables(DBOperate* dbOperate)
{
	m_DB->CreateTables(kAllSchemas, kTableCount);
}
void DBWriter::DropTables(DBOperate* dbOperate)
{
	m_DB->DropTables(kAllSchemas, kTableCount);
}
void DBWriter::TruncateTables(DBOperate* dbOperate)
{
	m_DB->TruncateTables(kAllSchemas, kTableCount);
}
void DBWriter::InsertRecord(DBOperate* dbOperate)
{
	const TableSchema* schema = GetSchemaByTableID(dbOperate->TableID);
	if (schema)
	{
		m_DB->Insert(schema, dbOperate->Record);
	}
}
void DBWriter::BatchInsertRecords(DBOperate* dbOperate)
{
	const TableSchema* schema = GetSchemaByTableID(dbOperate->TableID);
	if (!schema) return;

	std::vector<const void*> ptrs;
	switch (dbOperate->TableID)
	{
	case mdb::TradingDay::TableID:
	{
		auto records = (std::list<mdb::TradingDay*>*)dbOperate->Record;
		ptrs.reserve(records->size());
		for (auto& r : *records) ptrs.push_back(r);
		records->clear();
		delete records;
		break;
	}
	case mdb::Exchange::TableID:
	{
		auto records = (std::list<mdb::Exchange*>*)dbOperate->Record;
		ptrs.reserve(records->size());
		for (auto& r : *records) ptrs.push_back(r);
		records->clear();
		delete records;
		break;
	}
	case mdb::Product::TableID:
	{
		auto records = (std::list<mdb::Product*>*)dbOperate->Record;
		ptrs.reserve(records->size());
		for (auto& r : *records) ptrs.push_back(r);
		records->clear();
		delete records;
		break;
	}
	case mdb::Instrument::TableID:
	{
		auto records = (std::list<mdb::Instrument*>*)dbOperate->Record;
		ptrs.reserve(records->size());
		for (auto& r : *records) ptrs.push_back(r);
		records->clear();
		delete records;
		break;
	}
	case mdb::PrimaryAccount::TableID:
	{
		auto records = (std::list<mdb::PrimaryAccount*>*)dbOperate->Record;
		ptrs.reserve(records->size());
		for (auto& r : *records) ptrs.push_back(r);
		records->clear();
		delete records;
		break;
	}
	case mdb::Account::TableID:
	{
		auto records = (std::list<mdb::Account*>*)dbOperate->Record;
		ptrs.reserve(records->size());
		for (auto& r : *records) ptrs.push_back(r);
		records->clear();
		delete records;
		break;
	}
	case mdb::Capital::TableID:
	{
		auto records = (std::list<mdb::Capital*>*)dbOperate->Record;
		ptrs.reserve(records->size());
		for (auto& r : *records) ptrs.push_back(r);
		records->clear();
		delete records;
		break;
	}
	case mdb::Position::TableID:
	{
		auto records = (std::list<mdb::Position*>*)dbOperate->Record;
		ptrs.reserve(records->size());
		for (auto& r : *records) ptrs.push_back(r);
		records->clear();
		delete records;
		break;
	}
	case mdb::PositionDetail::TableID:
	{
		auto records = (std::list<mdb::PositionDetail*>*)dbOperate->Record;
		ptrs.reserve(records->size());
		for (auto& r : *records) ptrs.push_back(r);
		records->clear();
		delete records;
		break;
	}
	case mdb::Order::TableID:
	{
		auto records = (std::list<mdb::Order*>*)dbOperate->Record;
		ptrs.reserve(records->size());
		for (auto& r : *records) ptrs.push_back(r);
		records->clear();
		delete records;
		break;
	}
	case mdb::Trade::TableID:
	{
		auto records = (std::list<mdb::Trade*>*)dbOperate->Record;
		ptrs.reserve(records->size());
		for (auto& r : *records) ptrs.push_back(r);
		records->clear();
		delete records;
		break;
	}
	default:
		WriteLog(LogLevel::Error, "Unexpected BatchInsert TableID:0x%X", dbOperate->TableID);
		return;
	}
	if (!ptrs.empty())
	{
		m_DB->BatchInsert(schema, ptrs.data(), (int)ptrs.size());
	}
}
void DBWriter::DeleteRecord(DBOperate* dbOperate)
{
	const TableSchema* schema = GetSchemaByTableID(dbOperate->TableID);
	if (schema)
	{
		m_DB->Delete(schema, dbOperate->Record, schema->primaryKeyIndices, schema->primaryKeyCount);
		schema->DeallocateRecord(dbOperate->Record);
	}
}
void DBWriter::DeleteRecordByIndex(DBOperate* dbOperate)
{
	const TableSchema* schema = GetSchemaByTableID(dbOperate->TableID);
	if (!schema) return;

	switch (dbOperate->TableID)
	{
	case mdb::PrimaryAccount::TableID:
	{
		static const int idxFields[] = {4};
		m_DB->Delete(schema, dbOperate->Record, idxFields, 1);
		break;
	}
	case mdb::Capital::TableID:
	{
		static const int idxFields[] = {0};
		m_DB->Delete(schema, dbOperate->Record, idxFields, 1);
		break;
	}
	case mdb::Position::TableID:
	{
		switch (dbOperate->IndexID)
		{
		case mdb::PositionIndexAccount::IndexID:
		{
			static const int idxFields[] = {0, 1};
			m_DB->Delete(schema, dbOperate->Record, idxFields, 2);
			break;
		}
		case mdb::PositionIndexTradingDay::IndexID:
		{
			static const int idxFields[] = {0};
			m_DB->Delete(schema, dbOperate->Record, idxFields, 1);
			break;
		}
		default:
			WriteLog(LogLevel::Error, "Incorrect IndexID for DeleteRecordByIndex. TableID:0x%X, IndexID:%d", dbOperate->TableID, dbOperate->IndexID);
			break;
		}
		break;
	}
	case mdb::PositionDetail::TableID:
	{
		switch (dbOperate->IndexID)
		{
		case mdb::PositionDetailIndexTradeMatch::IndexID:
		{
			static const int idxFields[] = {0, 1, 2, 3, 6};
			m_DB->Delete(schema, dbOperate->Record, idxFields, 5);
			break;
		}
		case mdb::PositionDetailIndexTradingDay::IndexID:
		{
			static const int idxFields[] = {0};
			m_DB->Delete(schema, dbOperate->Record, idxFields, 1);
			break;
		}
		default:
			WriteLog(LogLevel::Error, "Incorrect IndexID for DeleteRecordByIndex. TableID:0x%X, IndexID:%d", dbOperate->TableID, dbOperate->IndexID);
			break;
		}
		break;
	}
	default:
		WriteLog(LogLevel::Error, "Incorrect TableID for DeleteRecordByIndex. TableID:0x%X", dbOperate->TableID);
		break;
	}
	schema->DeallocateRecord(dbOperate->Record);
}
void DBWriter::UpdateRecord(DBOperate* dbOperate)
{
	const TableSchema* schema = GetSchemaByTableID(dbOperate->TableID);
	if (schema)
	{
		m_DB->Update(schema, dbOperate->Record);
		schema->DeallocateRecord(dbOperate->Record);
	}
}
void DBWriter::TruncateTable(DBOperate* dbOperate)
{
	const TableSchema* schema = GetSchemaByTableID(dbOperate->TableID);
	if (schema)
	{
		m_DB->TruncateTable(schema->tableName);
	}
}
