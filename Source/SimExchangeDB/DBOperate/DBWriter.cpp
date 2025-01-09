#include "DBWriter.h"
#include "Logger.h"
#include "MdbIndexes.h"
#include <cstring>
#include <mutex>

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
		item->FreeRecord();
		item->Free();
	}
	m_DBOperates.clear();
}

void DBWriter::TruncateTables()
{
	m_DB->TruncateTables();
}
void DBWriter::OnTradingDayInsert(mdb::TradingDay* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Insert;
	dbOperate->TableID = TradingDay::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnTradingDayBatchInsert(std::list<mdb::TradingDay*>* records)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::BatchInsert;
	dbOperate->TableID = TradingDay::TableID;
	dbOperate->Record = records;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnTradingDayErase(mdb::TradingDay* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Delete;
	dbOperate->TableID = TradingDay::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnTradingDayUpdate(mdb::TradingDay* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Update;
	dbOperate->TableID = TradingDay::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnTradingDayTruncate()
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Truncate;
	dbOperate->TableID = TradingDay::TableID;
	dbOperate->Record = nullptr;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}

void DBWriter::OnExchangeInsert(mdb::Exchange* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Insert;
	dbOperate->TableID = Exchange::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnExchangeBatchInsert(std::list<mdb::Exchange*>* records)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::BatchInsert;
	dbOperate->TableID = Exchange::TableID;
	dbOperate->Record = records;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnExchangeErase(mdb::Exchange* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Delete;
	dbOperate->TableID = Exchange::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnExchangeUpdate(mdb::Exchange* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Update;
	dbOperate->TableID = Exchange::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnExchangeTruncate()
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Truncate;
	dbOperate->TableID = Exchange::TableID;
	dbOperate->Record = nullptr;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}

void DBWriter::OnProductInsert(mdb::Product* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Insert;
	dbOperate->TableID = Product::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnProductBatchInsert(std::list<mdb::Product*>* records)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::BatchInsert;
	dbOperate->TableID = Product::TableID;
	dbOperate->Record = records;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnProductErase(mdb::Product* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Delete;
	dbOperate->TableID = Product::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnProductUpdate(mdb::Product* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Update;
	dbOperate->TableID = Product::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnProductTruncate()
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Truncate;
	dbOperate->TableID = Product::TableID;
	dbOperate->Record = nullptr;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}

void DBWriter::OnInstrumentInsert(mdb::Instrument* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Insert;
	dbOperate->TableID = Instrument::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnInstrumentBatchInsert(std::list<mdb::Instrument*>* records)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::BatchInsert;
	dbOperate->TableID = Instrument::TableID;
	dbOperate->Record = records;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnInstrumentErase(mdb::Instrument* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Delete;
	dbOperate->TableID = Instrument::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnInstrumentUpdate(mdb::Instrument* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Update;
	dbOperate->TableID = Instrument::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnInstrumentTruncate()
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Truncate;
	dbOperate->TableID = Instrument::TableID;
	dbOperate->Record = nullptr;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}

void DBWriter::OnAccountInsert(mdb::Account* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Insert;
	dbOperate->TableID = Account::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnAccountBatchInsert(std::list<mdb::Account*>* records)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::BatchInsert;
	dbOperate->TableID = Account::TableID;
	dbOperate->Record = records;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnAccountErase(mdb::Account* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Delete;
	dbOperate->TableID = Account::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnAccountUpdate(mdb::Account* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Update;
	dbOperate->TableID = Account::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnAccountTruncate()
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Truncate;
	dbOperate->TableID = Account::TableID;
	dbOperate->Record = nullptr;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}

void DBWriter::OnPositionInsert(mdb::Position* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Insert;
	dbOperate->TableID = Position::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnPositionBatchInsert(std::list<mdb::Position*>* records)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::BatchInsert;
	dbOperate->TableID = Position::TableID;
	dbOperate->Record = records;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnPositionErase(mdb::Position* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Delete;
	dbOperate->TableID = Position::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnPositionEraseByAccountIndex(mdb::Position* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::DeleteByIndex;
	dbOperate->TableID = Position::TableID;
	dbOperate->IndexID = PositionIndexAccount::IndexID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnPositionUpdate(mdb::Position* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Update;
	dbOperate->TableID = Position::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnPositionTruncate()
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Truncate;
	dbOperate->TableID = Position::TableID;
	dbOperate->Record = nullptr;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}

void DBWriter::OnOrderInsert(mdb::Order* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Insert;
	dbOperate->TableID = Order::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnOrderBatchInsert(std::list<mdb::Order*>* records)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::BatchInsert;
	dbOperate->TableID = Order::TableID;
	dbOperate->Record = records;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnOrderErase(mdb::Order* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Delete;
	dbOperate->TableID = Order::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnOrderUpdate(mdb::Order* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Update;
	dbOperate->TableID = Order::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnOrderTruncate()
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Truncate;
	dbOperate->TableID = Order::TableID;
	dbOperate->Record = nullptr;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}

void DBWriter::OnTradeInsert(mdb::Trade* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Insert;
	dbOperate->TableID = Trade::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnTradeBatchInsert(std::list<mdb::Trade*>* records)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::BatchInsert;
	dbOperate->TableID = Trade::TableID;
	dbOperate->Record = records;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnTradeErase(mdb::Trade* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Delete;
	dbOperate->TableID = Trade::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnTradeUpdate(mdb::Trade* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Update;
	dbOperate->TableID = Trade::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnTradeTruncate()
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Truncate;
	dbOperate->TableID = Trade::TableID;
	dbOperate->Record = nullptr;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}

void DBWriter::OnMdTickInsert(mdb::MdTick* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Insert;
	dbOperate->TableID = MdTick::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnMdTickBatchInsert(std::list<mdb::MdTick*>* records)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::BatchInsert;
	dbOperate->TableID = MdTick::TableID;
	dbOperate->Record = records;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnMdTickErase(mdb::MdTick* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Delete;
	dbOperate->TableID = MdTick::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnMdTickUpdate(mdb::MdTick* record)
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Update;
	dbOperate->TableID = MdTick::TableID;
	dbOperate->Record = record;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
}
void DBWriter::OnMdTickTruncate()
{
	DBOperate* dbOperate = DBOperate::Allocate();
	dbOperate->Operate = DBOperateType::Truncate;
	dbOperate->TableID = MdTick::TableID;
	dbOperate->Record = nullptr;

	lock_guard<mutex> guard(m_Mutex);
	m_DBOperates.push_back(dbOperate);
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
	m_ThreadConditionVariable.wait_for(guard, m_TimeOut, [&] {return !m_DBOperates.empty(); });
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
			dbOperate->Free();
		}
	}
	catch(exception e)
	{
		WriteLog(LogLevel::Warning, "HandleDBOperate Failed. TableID;0x%X, Operate:%d, Message:%s", dbOperate->TableID, (int)dbOperate->Operate, e.what());
		DisConnect();
		dbOperate->FreeRecord();
		dbOperate->Free();
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
	static int i = 0;
	if (++i % 100 == 0)
	{
		WriteLog(LogLevel::Info, "TotalOperate:%d, m_DBOperate.size:%d", i, m_DBOperates.size());
	}
	auto item = m_DBOperates.front();
	m_DBOperates.pop_front();
	return item;
}

void DBWriter::InsertRecord(DBOperate* dbOperate)
{
	switch (dbOperate->TableID)
	{
	case TradingDay::TableID:
		m_DB->InsertTradingDay((TradingDay*)dbOperate->Record);
		break;
	case Exchange::TableID:
		m_DB->InsertExchange((Exchange*)dbOperate->Record);
		break;
	case Product::TableID:
		m_DB->InsertProduct((Product*)dbOperate->Record);
		break;
	case Instrument::TableID:
		m_DB->InsertInstrument((Instrument*)dbOperate->Record);
		break;
	case Account::TableID:
		m_DB->InsertAccount((Account*)dbOperate->Record);
		break;
	case Position::TableID:
		m_DB->InsertPosition((Position*)dbOperate->Record);
		break;
	case Order::TableID:
		m_DB->InsertOrder((Order*)dbOperate->Record);
		break;
	case Trade::TableID:
		m_DB->InsertTrade((Trade*)dbOperate->Record);
		break;
	case MdTick::TableID:
		m_DB->InsertMdTick((MdTick*)dbOperate->Record);
		break;
	default:
		break;
	}
}
void DBWriter::BatchInsertRecords(DBOperate* dbOperate)
{
	switch (dbOperate->TableID)
	{
	case TradingDay::TableID:
	{
		auto records = (std::list<TradingDay*>*)dbOperate->Record;
		m_DB->BatchInsertTradingDay(records);
		records->clear();
		delete records;
		break;
	}
	case Exchange::TableID:
	{
		auto records = (std::list<Exchange*>*)dbOperate->Record;
		m_DB->BatchInsertExchange(records);
		records->clear();
		delete records;
		break;
	}
	case Product::TableID:
	{
		auto records = (std::list<Product*>*)dbOperate->Record;
		m_DB->BatchInsertProduct(records);
		records->clear();
		delete records;
		break;
	}
	case Instrument::TableID:
	{
		auto records = (std::list<Instrument*>*)dbOperate->Record;
		m_DB->BatchInsertInstrument(records);
		records->clear();
		delete records;
		break;
	}
	case Account::TableID:
	{
		auto records = (std::list<Account*>*)dbOperate->Record;
		m_DB->BatchInsertAccount(records);
		records->clear();
		delete records;
		break;
	}
	case Position::TableID:
	{
		auto records = (std::list<Position*>*)dbOperate->Record;
		m_DB->BatchInsertPosition(records);
		records->clear();
		delete records;
		break;
	}
	case Order::TableID:
	{
		auto records = (std::list<Order*>*)dbOperate->Record;
		m_DB->BatchInsertOrder(records);
		records->clear();
		delete records;
		break;
	}
	case Trade::TableID:
	{
		auto records = (std::list<Trade*>*)dbOperate->Record;
		m_DB->BatchInsertTrade(records);
		records->clear();
		delete records;
		break;
	}
	case MdTick::TableID:
	{
		auto records = (std::list<MdTick*>*)dbOperate->Record;
		m_DB->BatchInsertMdTick(records);
		records->clear();
		delete records;
		break;
	}
	default:
		WriteLog(LogLevel::Error, "Unexpected BatchInsert TableID:0x%X", dbOperate->TableID);
		break;
	}
}
void DBWriter::DeleteRecord(DBOperate* dbOperate)
{
	switch (dbOperate->TableID)
	{
	case TradingDay::TableID:
		m_DB->DeleteTradingDay((TradingDay*)dbOperate->Record);
		((TradingDay*)dbOperate->Record)->Free();
		break;
	case Exchange::TableID:
		m_DB->DeleteExchange((Exchange*)dbOperate->Record);
		((Exchange*)dbOperate->Record)->Free();
		break;
	case Product::TableID:
		m_DB->DeleteProduct((Product*)dbOperate->Record);
		((Product*)dbOperate->Record)->Free();
		break;
	case Instrument::TableID:
		m_DB->DeleteInstrument((Instrument*)dbOperate->Record);
		((Instrument*)dbOperate->Record)->Free();
		break;
	case Account::TableID:
		m_DB->DeleteAccount((Account*)dbOperate->Record);
		((Account*)dbOperate->Record)->Free();
		break;
	case Position::TableID:
		m_DB->DeletePosition((Position*)dbOperate->Record);
		((Position*)dbOperate->Record)->Free();
		break;
	case Order::TableID:
		m_DB->DeleteOrder((Order*)dbOperate->Record);
		((Order*)dbOperate->Record)->Free();
		break;
	case Trade::TableID:
		m_DB->DeleteTrade((Trade*)dbOperate->Record);
		((Trade*)dbOperate->Record)->Free();
		break;
	case MdTick::TableID:
		m_DB->DeleteMdTick((MdTick*)dbOperate->Record);
		((MdTick*)dbOperate->Record)->Free();
		break;
	default:
		break;
	}
}
void DBWriter::DeleteRecordByIndex(DBOperate* dbOperate)
{
	switch (dbOperate->TableID)
	{
	case Position::TableID:
	{
		switch (dbOperate->IndexID)
		{
		case PositionIndexAccount::IndexID:
		{
			m_DB->DeletePositionByAccountIndex((Position*)dbOperate->Record);
			break;
		}
		default:
			WriteLog(LogLevel::Error, "Incorrect IndexID for DeleteRecordByIndex. TableID:0x%X, IndexID:%d", dbOperate->TableID, dbOperate->IndexID);
			break;
		}
		((Position*)dbOperate->Record)->Free();
		break;
	}
	default:
		WriteLog(LogLevel::Error, "Incorrect TableID for DeleteRecordByIndex. TableID:0x%X", dbOperate->TableID);
		break;
	}
}
void DBWriter::UpdateRecord(DBOperate* dbOperate)
{
	switch (dbOperate->TableID)
	{
	case TradingDay::TableID:
		m_DB->UpdateTradingDay((TradingDay*)dbOperate->Record);
		((TradingDay*)dbOperate->Record)->Free();
		break;
	case Exchange::TableID:
		m_DB->UpdateExchange((Exchange*)dbOperate->Record);
		((Exchange*)dbOperate->Record)->Free();
		break;
	case Product::TableID:
		m_DB->UpdateProduct((Product*)dbOperate->Record);
		((Product*)dbOperate->Record)->Free();
		break;
	case Instrument::TableID:
		m_DB->UpdateInstrument((Instrument*)dbOperate->Record);
		((Instrument*)dbOperate->Record)->Free();
		break;
	case Account::TableID:
		m_DB->UpdateAccount((Account*)dbOperate->Record);
		((Account*)dbOperate->Record)->Free();
		break;
	case Position::TableID:
		m_DB->UpdatePosition((Position*)dbOperate->Record);
		((Position*)dbOperate->Record)->Free();
		break;
	case Order::TableID:
		m_DB->UpdateOrder((Order*)dbOperate->Record);
		((Order*)dbOperate->Record)->Free();
		break;
	case Trade::TableID:
		m_DB->UpdateTrade((Trade*)dbOperate->Record);
		((Trade*)dbOperate->Record)->Free();
		break;
	case MdTick::TableID:
		m_DB->UpdateMdTick((MdTick*)dbOperate->Record);
		((MdTick*)dbOperate->Record)->Free();
		break;
	default:
		break;
	}
}

void DBWriter::TruncateTable(DBOperate* dbOperate)
{
	switch (dbOperate->TableID)
	{
	case TradingDay::TableID:
		m_DB->TruncateTradingDay();
		break;
	case Exchange::TableID:
		m_DB->TruncateExchange();
		break;
	case Product::TableID:
		m_DB->TruncateProduct();
		break;
	case Instrument::TableID:
		m_DB->TruncateInstrument();
		break;
	case Account::TableID:
		m_DB->TruncateAccount();
		break;
	case Position::TableID:
		m_DB->TruncatePosition();
		break;
	case Order::TableID:
		m_DB->TruncateOrder();
		break;
	case Trade::TableID:
		m_DB->TruncateTrade();
		break;
	case MdTick::TableID:
		m_DB->TruncateMdTick();
		break;
	default:
		break;
	}
}

