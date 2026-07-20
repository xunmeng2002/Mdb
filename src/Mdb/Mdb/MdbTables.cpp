#include <Mdb/Mdb/MdbTables.h>
#include <Mdb/Mdb/Mdb.h>
#include <PersonalLib/Core/Logger/Logger.h>
#include <string>
#include <cstring>
#include <set>
#include <vector>

using std::string;
using std::set;

namespace mdb
{
	TradingDayTable::TradingDayTable(Mdb* mdb)
		:m_Mdb(mdb)
	{
		m_MdbSubscriber = nullptr;
		m_PrimaryKey = new TradingDayPrimaryKey(this);
	}
	TradingDayTable::~TradingDayTable()
	{
		delete m_PrimaryKey;
		m_PrimaryKey = nullptr;
	}
	void TradingDayTable::Subscribe(MdbSubscriber* mdbSubscriber)
	{
		m_MdbSubscriber = mdbSubscriber;
	}
	void TradingDayTable::UnSubscribe()
	{
		m_MdbSubscriber = nullptr;
	}
	void TradingDayTable::LockShared()
	{
		m_SharedMutex.lock_shared();
	}
	void TradingDayTable::UnlockShared()
	{
		m_SharedMutex.unlock_shared();
	}
	void TradingDayTable::InitDB()
	{
		m_MdbSubscriber->OnRecordTruncate(TradingDay::TableID);
		
		auto records = new std::vector<const void*>();
		std::shared_lock guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			records->push_back(new TradingDay(**it));
		}
		if (records->empty())
		{
			delete records;
		}
		else
		{
			m_MdbSubscriber->OnRecordBatchInsert(TradingDay::TableID, records);
		}
		m_DBInited = true;
	}
	bool TradingDayTable::Insert(TradingDay* record)
	{
		std::lock_guard guard(m_SharedMutex);
		if (!m_PrimaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for TradingDay:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		m_PrimaryKey->Insert(record);

		
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordInsert(TradingDay::TableID, record);
		}
		return true;
	}
	void TradingDayTable::BatchInsert(std::vector<mdb::TradingDay*>* records)
	{
		{
			std::lock_guard guard(m_SharedMutex);
			for (auto record : *records)
			{
				auto newRecord = TradingDay::Allocate();
				memcpy(newRecord, record, sizeof(TradingDay));
				m_PrimaryKey->Insert(newRecord);

			}
		}
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			m_MdbSubscriber->OnRecordBatchInsert(TradingDay::TableID, dbRecords);
		}
		delete records;
	}
	void TradingDayTable::Erase(TradingDay* record)
	{
		std::lock_guard guard(m_SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordErase(TradingDay::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	bool TradingDayTable::Update(TradingDay* const oldRecord, TradingDay* const newRecord, bool updateDB)
	{
		std::lock_guard guard(m_SharedMutex);
		if (!m_PrimaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for TradingDay:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New TradingDay:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		::memcpy((void*)oldRecord, newRecord, sizeof(TradingDay));

		if (updateDB && m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordUpdate(TradingDay::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void TradingDayTable::TruncateTables()
	{
		std::lock_guard guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		m_PrimaryKey->m_Index.clear();
	}
	void TradingDayTable::TruncateTable()
	{
		std::lock_guard guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		m_PrimaryKey->m_Index.clear();
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordTruncate(TradingDay::TableID);
		}
	}
	void TradingDayTable::Dump(const char* dir)
	{
		string fileName = string(dir) + "//t_TradingDay.csv";
		FILE* dumpFile = fopen(fileName.c_str(), "w");
		if (dumpFile == nullptr)
		{
			return;
		}

		fprintf(dumpFile, "PK,CurrTradingDay,PreTradingDay\n");
		char buff[4096] = { 0 };
		set<TradingDay*, TradingDayLessForTradingDayPrimaryKey> records;
		std::shared_lock guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			records.insert(*it);
		}
		for (auto record : records)
		{
			fprintf(dumpFile, "%s\n", record->GetString());
		}
		records.clear();
		fclose(dumpFile);
	}
	void TradingDayTable::EraseUniqueKey(TradingDay* record)
	{
		m_PrimaryKey->Erase(record);
	}
	void TradingDayTable::EraseIndex(TradingDay* record)
	{
	}

	ExchangeTable::ExchangeTable(Mdb* mdb)
		:m_Mdb(mdb)
	{
		m_MdbSubscriber = nullptr;
		m_PrimaryKey = new ExchangePrimaryKey(this);
	}
	ExchangeTable::~ExchangeTable()
	{
		delete m_PrimaryKey;
		m_PrimaryKey = nullptr;
	}
	void ExchangeTable::Subscribe(MdbSubscriber* mdbSubscriber)
	{
		m_MdbSubscriber = mdbSubscriber;
	}
	void ExchangeTable::UnSubscribe()
	{
		m_MdbSubscriber = nullptr;
	}
	void ExchangeTable::LockShared()
	{
		m_SharedMutex.lock_shared();
	}
	void ExchangeTable::UnlockShared()
	{
		m_SharedMutex.unlock_shared();
	}
	void ExchangeTable::InitDB()
	{
		m_MdbSubscriber->OnRecordTruncate(Exchange::TableID);
		
		auto records = new std::vector<const void*>();
		std::shared_lock guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			records->push_back(new Exchange(**it));
		}
		if (records->empty())
		{
			delete records;
		}
		else
		{
			m_MdbSubscriber->OnRecordBatchInsert(Exchange::TableID, records);
		}
		m_DBInited = true;
	}
	bool ExchangeTable::Insert(Exchange* record)
	{
		std::lock_guard guard(m_SharedMutex);
		if (!m_PrimaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for Exchange:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		m_PrimaryKey->Insert(record);

		
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordInsert(Exchange::TableID, record);
		}
		return true;
	}
	void ExchangeTable::BatchInsert(std::vector<mdb::Exchange*>* records)
	{
		{
			std::lock_guard guard(m_SharedMutex);
			for (auto record : *records)
			{
				auto newRecord = Exchange::Allocate();
				memcpy(newRecord, record, sizeof(Exchange));
				m_PrimaryKey->Insert(newRecord);

			}
		}
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			m_MdbSubscriber->OnRecordBatchInsert(Exchange::TableID, dbRecords);
		}
		delete records;
	}
	void ExchangeTable::Erase(Exchange* record)
	{
		std::lock_guard guard(m_SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordErase(Exchange::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	bool ExchangeTable::Update(Exchange* const oldRecord, Exchange* const newRecord, bool updateDB)
	{
		std::lock_guard guard(m_SharedMutex);
		if (!m_PrimaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for Exchange:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New Exchange:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		::memcpy((void*)oldRecord, newRecord, sizeof(Exchange));

		if (updateDB && m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordUpdate(Exchange::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void ExchangeTable::TruncateTables()
	{
		std::lock_guard guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		m_PrimaryKey->m_Index.clear();
	}
	void ExchangeTable::TruncateTable()
	{
		std::lock_guard guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		m_PrimaryKey->m_Index.clear();
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordTruncate(Exchange::TableID);
		}
	}
	void ExchangeTable::Dump(const char* dir)
	{
		string fileName = string(dir) + "//t_Exchange.csv";
		FILE* dumpFile = fopen(fileName.c_str(), "w");
		if (dumpFile == nullptr)
		{
			return;
		}

		fprintf(dumpFile, "ExchangeID,ExchangeName\n");
		char buff[4096] = { 0 };
		set<Exchange*, ExchangeLessForExchangePrimaryKey> records;
		std::shared_lock guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			records.insert(*it);
		}
		for (auto record : records)
		{
			fprintf(dumpFile, "%s\n", record->GetString());
		}
		records.clear();
		fclose(dumpFile);
	}
	void ExchangeTable::EraseUniqueKey(Exchange* record)
	{
		m_PrimaryKey->Erase(record);
	}
	void ExchangeTable::EraseIndex(Exchange* record)
	{
	}

	ProductTable::ProductTable(Mdb* mdb)
		:m_Mdb(mdb)
	{
		m_MdbSubscriber = nullptr;
		m_PrimaryKey = new ProductPrimaryKey(this);
	}
	ProductTable::~ProductTable()
	{
		delete m_PrimaryKey;
		m_PrimaryKey = nullptr;
	}
	void ProductTable::Subscribe(MdbSubscriber* mdbSubscriber)
	{
		m_MdbSubscriber = mdbSubscriber;
	}
	void ProductTable::UnSubscribe()
	{
		m_MdbSubscriber = nullptr;
	}
	void ProductTable::LockShared()
	{
		m_SharedMutex.lock_shared();
	}
	void ProductTable::UnlockShared()
	{
		m_SharedMutex.unlock_shared();
	}
	void ProductTable::InitDB()
	{
		m_MdbSubscriber->OnRecordTruncate(Product::TableID);
		
		auto records = new std::vector<const void*>();
		std::shared_lock guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			records->push_back(new Product(**it));
		}
		if (records->empty())
		{
			delete records;
		}
		else
		{
			m_MdbSubscriber->OnRecordBatchInsert(Product::TableID, records);
		}
		m_DBInited = true;
	}
	bool ProductTable::Insert(Product* record)
	{
		std::lock_guard guard(m_SharedMutex);
		if (!m_PrimaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for Product:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		m_PrimaryKey->Insert(record);

		
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordInsert(Product::TableID, record);
		}
		return true;
	}
	void ProductTable::BatchInsert(std::vector<mdb::Product*>* records)
	{
		{
			std::lock_guard guard(m_SharedMutex);
			for (auto record : *records)
			{
				auto newRecord = Product::Allocate();
				memcpy(newRecord, record, sizeof(Product));
				m_PrimaryKey->Insert(newRecord);

			}
		}
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			m_MdbSubscriber->OnRecordBatchInsert(Product::TableID, dbRecords);
		}
		delete records;
	}
	void ProductTable::Erase(Product* record)
	{
		std::lock_guard guard(m_SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordErase(Product::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	bool ProductTable::Update(Product* const oldRecord, Product* const newRecord, bool updateDB)
	{
		std::lock_guard guard(m_SharedMutex);
		if (!m_PrimaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for Product:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New Product:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		::memcpy((void*)oldRecord, newRecord, sizeof(Product));

		if (updateDB && m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordUpdate(Product::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void ProductTable::TruncateTables()
	{
		std::lock_guard guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		m_PrimaryKey->m_Index.clear();
	}
	void ProductTable::TruncateTable()
	{
		std::lock_guard guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		m_PrimaryKey->m_Index.clear();
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordTruncate(Product::TableID);
		}
	}
	void ProductTable::Dump(const char* dir)
	{
		string fileName = string(dir) + "//t_Product.csv";
		FILE* dumpFile = fopen(fileName.c_str(), "w");
		if (dumpFile == nullptr)
		{
			return;
		}

		fprintf(dumpFile, "ExchangeID,ProductID,ProductName,ProductClass,VolumeMultiple,PriceTick,MaxMarketOrderVolume,MinMarketOrderVolume,MaxLimitOrderVolume,MinLimitOrderVolume,SessionName\n");
		char buff[4096] = { 0 };
		set<Product*, ProductLessForProductPrimaryKey> records;
		std::shared_lock guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			records.insert(*it);
		}
		for (auto record : records)
		{
			fprintf(dumpFile, "%s\n", record->GetString());
		}
		records.clear();
		fclose(dumpFile);
	}
	void ProductTable::EraseUniqueKey(Product* record)
	{
		m_PrimaryKey->Erase(record);
	}
	void ProductTable::EraseIndex(Product* record)
	{
	}

	InstrumentTable::InstrumentTable(Mdb* mdb)
		:m_Mdb(mdb)
	{
		m_MdbSubscriber = nullptr;
		m_PrimaryKey = new InstrumentPrimaryKey(this);
	}
	InstrumentTable::~InstrumentTable()
	{
		delete m_PrimaryKey;
		m_PrimaryKey = nullptr;
	}
	void InstrumentTable::Subscribe(MdbSubscriber* mdbSubscriber)
	{
		m_MdbSubscriber = mdbSubscriber;
	}
	void InstrumentTable::UnSubscribe()
	{
		m_MdbSubscriber = nullptr;
	}
	void InstrumentTable::LockShared()
	{
		m_SharedMutex.lock_shared();
	}
	void InstrumentTable::UnlockShared()
	{
		m_SharedMutex.unlock_shared();
	}
	void InstrumentTable::InitDB()
	{
		m_MdbSubscriber->OnRecordTruncate(Instrument::TableID);
		
		auto records = new std::vector<const void*>();
		std::shared_lock guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			records->push_back(new Instrument(**it));
		}
		if (records->empty())
		{
			delete records;
		}
		else
		{
			m_MdbSubscriber->OnRecordBatchInsert(Instrument::TableID, records);
		}
		m_DBInited = true;
	}
	bool InstrumentTable::Insert(Instrument* record)
	{
		std::lock_guard guard(m_SharedMutex);
		if (!m_PrimaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for Instrument:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		m_PrimaryKey->Insert(record);

		
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordInsert(Instrument::TableID, record);
		}
		return true;
	}
	void InstrumentTable::BatchInsert(std::vector<mdb::Instrument*>* records)
	{
		{
			std::lock_guard guard(m_SharedMutex);
			for (auto record : *records)
			{
				auto newRecord = Instrument::Allocate();
				memcpy(newRecord, record, sizeof(Instrument));
				m_PrimaryKey->Insert(newRecord);

			}
		}
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			m_MdbSubscriber->OnRecordBatchInsert(Instrument::TableID, dbRecords);
		}
		delete records;
	}
	void InstrumentTable::Erase(Instrument* record)
	{
		std::lock_guard guard(m_SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordErase(Instrument::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	bool InstrumentTable::Update(Instrument* const oldRecord, Instrument* const newRecord, bool updateDB)
	{
		std::lock_guard guard(m_SharedMutex);
		if (!m_PrimaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for Instrument:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New Instrument:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		::memcpy((void*)oldRecord, newRecord, sizeof(Instrument));

		if (updateDB && m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordUpdate(Instrument::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void InstrumentTable::TruncateTables()
	{
		std::lock_guard guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		m_PrimaryKey->m_Index.clear();
	}
	void InstrumentTable::TruncateTable()
	{
		std::lock_guard guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		m_PrimaryKey->m_Index.clear();
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordTruncate(Instrument::TableID);
		}
	}
	void InstrumentTable::Dump(const char* dir)
	{
		string fileName = string(dir) + "//t_Instrument.csv";
		FILE* dumpFile = fopen(fileName.c_str(), "w");
		if (dumpFile == nullptr)
		{
			return;
		}

		fprintf(dumpFile, "ExchangeID,InstrumentID,ExchangeInstID,InstrumentName,ProductID,ProductClass,InstrumentClass,Rank,VolumeMultiple,PriceTick,MaxMarketOrderVolume,MinMarketOrderVolume,MaxLimitOrderVolume,MinLimitOrderVolume,SessionName\n");
		char buff[4096] = { 0 };
		set<Instrument*, InstrumentLessForInstrumentPrimaryKey> records;
		std::shared_lock guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			records.insert(*it);
		}
		for (auto record : records)
		{
			fprintf(dumpFile, "%s\n", record->GetString());
		}
		records.clear();
		fclose(dumpFile);
	}
	void InstrumentTable::EraseUniqueKey(Instrument* record)
	{
		m_PrimaryKey->Erase(record);
	}
	void InstrumentTable::EraseIndex(Instrument* record)
	{
	}

	PrimaryAccountTable::PrimaryAccountTable(Mdb* mdb)
		:m_Mdb(mdb)
	{
		m_MdbSubscriber = nullptr;
		m_PrimaryKey = new PrimaryAccountPrimaryKey(this);
		m_OfferIDIndex = new PrimaryAccountIndexOfferID(this);
	}
	PrimaryAccountTable::~PrimaryAccountTable()
	{
		delete m_PrimaryKey;
		m_PrimaryKey = nullptr;
		delete m_OfferIDIndex;
		m_OfferIDIndex = nullptr;
	}
	void PrimaryAccountTable::Subscribe(MdbSubscriber* mdbSubscriber)
	{
		m_MdbSubscriber = mdbSubscriber;
	}
	void PrimaryAccountTable::UnSubscribe()
	{
		m_MdbSubscriber = nullptr;
	}
	void PrimaryAccountTable::LockShared()
	{
		m_SharedMutex.lock_shared();
	}
	void PrimaryAccountTable::UnlockShared()
	{
		m_SharedMutex.unlock_shared();
	}
	void PrimaryAccountTable::InitDB()
	{
		m_MdbSubscriber->OnRecordTruncate(PrimaryAccount::TableID);
		
		auto records = new std::vector<const void*>();
		std::shared_lock guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			records->push_back(new PrimaryAccount(**it));
		}
		if (records->empty())
		{
			delete records;
		}
		else
		{
			m_MdbSubscriber->OnRecordBatchInsert(PrimaryAccount::TableID, records);
		}
		m_DBInited = true;
	}
	bool PrimaryAccountTable::Insert(PrimaryAccount* record)
	{
		std::lock_guard guard(m_SharedMutex);
		if (!m_PrimaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for PrimaryAccount:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		m_PrimaryKey->Insert(record);

		m_OfferIDIndex->Insert(record);
		
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordInsert(PrimaryAccount::TableID, record);
		}
		return true;
	}
	void PrimaryAccountTable::BatchInsert(std::vector<mdb::PrimaryAccount*>* records)
	{
		{
			std::lock_guard guard(m_SharedMutex);
			for (auto record : *records)
			{
				auto newRecord = PrimaryAccount::Allocate();
				memcpy(newRecord, record, sizeof(PrimaryAccount));
				m_PrimaryKey->Insert(newRecord);

				m_OfferIDIndex->Insert(newRecord);
			}
		}
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			m_MdbSubscriber->OnRecordBatchInsert(PrimaryAccount::TableID, dbRecords);
		}
		delete records;
	}
	void PrimaryAccountTable::Erase(PrimaryAccount* record)
	{
		std::lock_guard guard(m_SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordEraseByIndex(PrimaryAccount::TableID, PrimaryAccountIndexOfferID::IndexID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	int PrimaryAccountTable::EraseByOfferIDIndex(const OfferIDType& OfferID)
	{
		m_OfferIDIndex->FillCompareRecord(OfferID);
		std::vector<PrimaryAccount*> records;
		std::lock_guard guard(m_SharedMutex);
		auto range = m_OfferIDIndex->m_Index.equal_range(&t_ComparePrimaryAccount);
		for (auto& it = range.first; it != range.second; ++it)
		{
			records.push_back(*it);
		}
		for (auto record : records)
		{
			EraseUniqueKey(record);
			EraseIndex(record);
			record->Deallocate();
		}
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			auto record = PrimaryAccount::Allocate();
			memcpy(record, &t_ComparePrimaryAccount, sizeof(PrimaryAccount));
			m_MdbSubscriber->OnRecordEraseByIndex(PrimaryAccount::TableID, PrimaryAccountIndexOfferID::IndexID, record);
		}
		return (int)records.size();
	}
	bool PrimaryAccountTable::Update(PrimaryAccount* const oldRecord, PrimaryAccount* const newRecord, bool updateDB)
	{
		std::lock_guard guard(m_SharedMutex);
		if (!m_PrimaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for PrimaryAccount:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New PrimaryAccount:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		bool OfferIDIndexUpdate = m_OfferIDIndex->NeedUpdate(oldRecord, newRecord);
		PrimaryAccountIndexOfferID::iterator itOfferID;
		if (OfferIDIndexUpdate)
		{
			itOfferID = m_OfferIDIndex->FindNode(oldRecord);
		}
		::memcpy((void*)oldRecord, newRecord, sizeof(PrimaryAccount));
		if (OfferIDIndexUpdate)
		{
			m_OfferIDIndex->Update(itOfferID);
		}

		if (updateDB && m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordUpdate(PrimaryAccount::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void PrimaryAccountTable::TruncateTables()
	{
		std::lock_guard guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		m_PrimaryKey->m_Index.clear();
		m_OfferIDIndex->m_Index.clear();
	}
	void PrimaryAccountTable::TruncateTable()
	{
		std::lock_guard guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		m_PrimaryKey->m_Index.clear();
		m_OfferIDIndex->m_Index.clear();
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordTruncate(PrimaryAccount::TableID);
		}
	}
	void PrimaryAccountTable::Dump(const char* dir)
	{
		string fileName = string(dir) + "//t_PrimaryAccount.csv";
		FILE* dumpFile = fopen(fileName.c_str(), "w");
		if (dumpFile == nullptr)
		{
			return;
		}

		fprintf(dumpFile, "PrimaryAccountID,PrimaryAccountName,AccountClass,BrokerPassword,OfferID,IsAllowLogin,IsSimulateAccount,LoginStatus,InitStatus\n");
		char buff[4096] = { 0 };
		set<PrimaryAccount*, PrimaryAccountLessForPrimaryAccountPrimaryKey> records;
		std::shared_lock guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			records.insert(*it);
		}
		for (auto record : records)
		{
			fprintf(dumpFile, "%s\n", record->GetString());
		}
		records.clear();
		fclose(dumpFile);
	}
	void PrimaryAccountTable::EraseUniqueKey(PrimaryAccount* record)
	{
		m_PrimaryKey->Erase(record);
	}
	void PrimaryAccountTable::EraseIndex(PrimaryAccount* record)
	{
		m_OfferIDIndex->Erase(record);
	}

	AccountTable::AccountTable(Mdb* mdb)
		:m_Mdb(mdb)
	{
		m_MdbSubscriber = nullptr;
		m_PrimaryKey = new AccountPrimaryKey(this);
	}
	AccountTable::~AccountTable()
	{
		delete m_PrimaryKey;
		m_PrimaryKey = nullptr;
	}
	void AccountTable::Subscribe(MdbSubscriber* mdbSubscriber)
	{
		m_MdbSubscriber = mdbSubscriber;
	}
	void AccountTable::UnSubscribe()
	{
		m_MdbSubscriber = nullptr;
	}
	void AccountTable::LockShared()
	{
		m_SharedMutex.lock_shared();
	}
	void AccountTable::UnlockShared()
	{
		m_SharedMutex.unlock_shared();
	}
	void AccountTable::InitDB()
	{
		m_MdbSubscriber->OnRecordTruncate(Account::TableID);
		
		auto records = new std::vector<const void*>();
		std::shared_lock guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			records->push_back(new Account(**it));
		}
		if (records->empty())
		{
			delete records;
		}
		else
		{
			m_MdbSubscriber->OnRecordBatchInsert(Account::TableID, records);
		}
		m_DBInited = true;
	}
	bool AccountTable::Insert(Account* record)
	{
		std::lock_guard guard(m_SharedMutex);
		if (!m_PrimaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for Account:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		m_PrimaryKey->Insert(record);

		
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordInsert(Account::TableID, record);
		}
		return true;
	}
	void AccountTable::BatchInsert(std::vector<mdb::Account*>* records)
	{
		{
			std::lock_guard guard(m_SharedMutex);
			for (auto record : *records)
			{
				auto newRecord = Account::Allocate();
				memcpy(newRecord, record, sizeof(Account));
				m_PrimaryKey->Insert(newRecord);

			}
		}
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			m_MdbSubscriber->OnRecordBatchInsert(Account::TableID, dbRecords);
		}
		delete records;
	}
	void AccountTable::Erase(Account* record)
	{
		std::lock_guard guard(m_SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordErase(Account::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	bool AccountTable::Update(Account* const oldRecord, Account* const newRecord, bool updateDB)
	{
		std::lock_guard guard(m_SharedMutex);
		if (!m_PrimaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for Account:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New Account:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		::memcpy((void*)oldRecord, newRecord, sizeof(Account));

		if (updateDB && m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordUpdate(Account::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void AccountTable::TruncateTables()
	{
		std::lock_guard guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		m_PrimaryKey->m_Index.clear();
	}
	void AccountTable::TruncateTable()
	{
		std::lock_guard guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		m_PrimaryKey->m_Index.clear();
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordTruncate(Account::TableID);
		}
	}
	void AccountTable::Dump(const char* dir)
	{
		string fileName = string(dir) + "//t_Account.csv";
		FILE* dumpFile = fopen(fileName.c_str(), "w");
		if (dumpFile == nullptr)
		{
			return;
		}

		fprintf(dumpFile, "AccountID,AccountName,AccountType,AccountStatus,Password,TradeGroupID,RiskGroupID,CommissionGroupID\n");
		char buff[4096] = { 0 };
		set<Account*, AccountLessForAccountPrimaryKey> records;
		std::shared_lock guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			records.insert(*it);
		}
		for (auto record : records)
		{
			fprintf(dumpFile, "%s\n", record->GetString());
		}
		records.clear();
		fclose(dumpFile);
	}
	void AccountTable::EraseUniqueKey(Account* record)
	{
		m_PrimaryKey->Erase(record);
	}
	void AccountTable::EraseIndex(Account* record)
	{
	}

	CapitalTable::CapitalTable(Mdb* mdb)
		:m_Mdb(mdb)
	{
		m_MdbSubscriber = nullptr;
		m_PrimaryKey = new CapitalPrimaryKey(this);
		m_TradingDayIndex = new CapitalIndexTradingDay(this);
	}
	CapitalTable::~CapitalTable()
	{
		delete m_PrimaryKey;
		m_PrimaryKey = nullptr;
		delete m_TradingDayIndex;
		m_TradingDayIndex = nullptr;
	}
	void CapitalTable::Subscribe(MdbSubscriber* mdbSubscriber)
	{
		m_MdbSubscriber = mdbSubscriber;
	}
	void CapitalTable::UnSubscribe()
	{
		m_MdbSubscriber = nullptr;
	}
	void CapitalTable::LockShared()
	{
		m_SharedMutex.lock_shared();
	}
	void CapitalTable::UnlockShared()
	{
		m_SharedMutex.unlock_shared();
	}
	void CapitalTable::InitDB()
	{
		m_MdbSubscriber->OnRecordTruncate(Capital::TableID);
		
		auto records = new std::vector<const void*>();
		std::shared_lock guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			records->push_back(new Capital(**it));
		}
		if (records->empty())
		{
			delete records;
		}
		else
		{
			m_MdbSubscriber->OnRecordBatchInsert(Capital::TableID, records);
		}
		m_DBInited = true;
	}
	bool CapitalTable::Insert(Capital* record)
	{
		std::lock_guard guard(m_SharedMutex);
		if (!m_PrimaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for Capital:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		m_PrimaryKey->Insert(record);

		m_TradingDayIndex->Insert(record);
		
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordInsert(Capital::TableID, record);
		}
		return true;
	}
	void CapitalTable::BatchInsert(std::vector<mdb::Capital*>* records)
	{
		{
			std::lock_guard guard(m_SharedMutex);
			for (auto record : *records)
			{
				auto newRecord = Capital::Allocate();
				memcpy(newRecord, record, sizeof(Capital));
				m_PrimaryKey->Insert(newRecord);

				m_TradingDayIndex->Insert(newRecord);
			}
		}
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			m_MdbSubscriber->OnRecordBatchInsert(Capital::TableID, dbRecords);
		}
		delete records;
	}
	void CapitalTable::Erase(Capital* record)
	{
		std::lock_guard guard(m_SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordEraseByIndex(Capital::TableID, CapitalIndexTradingDay::IndexID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	int CapitalTable::EraseByTradingDayIndex(const DateType& TradingDay)
	{
		m_TradingDayIndex->FillCompareRecord(TradingDay);
		std::vector<Capital*> records;
		std::lock_guard guard(m_SharedMutex);
		auto range = m_TradingDayIndex->m_Index.equal_range(&t_CompareCapital);
		for (auto& it = range.first; it != range.second; ++it)
		{
			records.push_back(*it);
		}
		for (auto record : records)
		{
			EraseUniqueKey(record);
			EraseIndex(record);
			record->Deallocate();
		}
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			auto record = Capital::Allocate();
			memcpy(record, &t_CompareCapital, sizeof(Capital));
			m_MdbSubscriber->OnRecordEraseByIndex(Capital::TableID, CapitalIndexTradingDay::IndexID, record);
		}
		return (int)records.size();
	}
	bool CapitalTable::Update(Capital* const oldRecord, Capital* const newRecord, bool updateDB)
	{
		std::lock_guard guard(m_SharedMutex);
		if (!m_PrimaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for Capital:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New Capital:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		bool TradingDayIndexUpdate = m_TradingDayIndex->NeedUpdate(oldRecord, newRecord);
		CapitalIndexTradingDay::iterator itTradingDay;
		if (TradingDayIndexUpdate)
		{
			itTradingDay = m_TradingDayIndex->FindNode(oldRecord);
		}
		::memcpy((void*)oldRecord, newRecord, sizeof(Capital));
		if (TradingDayIndexUpdate)
		{
			m_TradingDayIndex->Update(itTradingDay);
		}

		if (updateDB && m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordUpdate(Capital::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void CapitalTable::TruncateTables()
	{
		std::lock_guard guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		m_PrimaryKey->m_Index.clear();
		m_TradingDayIndex->m_Index.clear();
	}
	void CapitalTable::TruncateTable()
	{
		std::lock_guard guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		m_PrimaryKey->m_Index.clear();
		m_TradingDayIndex->m_Index.clear();
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordTruncate(Capital::TableID);
		}
	}
	void CapitalTable::Dump(const char* dir)
	{
		string fileName = string(dir) + "//t_Capital.csv";
		FILE* dumpFile = fopen(fileName.c_str(), "w");
		if (dumpFile == nullptr)
		{
			return;
		}

		fprintf(dumpFile, "TradingDay,AccountID,AccountType,Balance,PreBalance,Available,MarketValue,CashIn,CashOut,Margin,Commission,FrozenCash,FrozenMargin,FrozenCommission,CloseProfitByDate,CloseProfitByTrade,PositionProfitByDate,PositionProfitByTrade,Deposit,Withdraw\n");
		char buff[4096] = { 0 };
		set<Capital*, CapitalLessForCapitalPrimaryKey> records;
		std::shared_lock guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			records.insert(*it);
		}
		for (auto record : records)
		{
			fprintf(dumpFile, "%s\n", record->GetString());
		}
		records.clear();
		fclose(dumpFile);
	}
	void CapitalTable::EraseUniqueKey(Capital* record)
	{
		m_PrimaryKey->Erase(record);
	}
	void CapitalTable::EraseIndex(Capital* record)
	{
		m_TradingDayIndex->Erase(record);
	}

	PositionTable::PositionTable(Mdb* mdb)
		:m_Mdb(mdb)
	{
		m_MdbSubscriber = nullptr;
		m_PrimaryKey = new PositionPrimaryKey(this);
		m_AccountIndex = new PositionIndexAccount(this);
		m_TradingDayIndex = new PositionIndexTradingDay(this);
	}
	PositionTable::~PositionTable()
	{
		delete m_PrimaryKey;
		m_PrimaryKey = nullptr;
		delete m_AccountIndex;
		m_AccountIndex = nullptr;
		delete m_TradingDayIndex;
		m_TradingDayIndex = nullptr;
	}
	void PositionTable::Subscribe(MdbSubscriber* mdbSubscriber)
	{
		m_MdbSubscriber = mdbSubscriber;
	}
	void PositionTable::UnSubscribe()
	{
		m_MdbSubscriber = nullptr;
	}
	void PositionTable::LockShared()
	{
		m_SharedMutex.lock_shared();
	}
	void PositionTable::UnlockShared()
	{
		m_SharedMutex.unlock_shared();
	}
	void PositionTable::InitDB()
	{
		m_MdbSubscriber->OnRecordTruncate(Position::TableID);
		
		auto records = new std::vector<const void*>();
		std::shared_lock guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			records->push_back(new Position(**it));
		}
		if (records->empty())
		{
			delete records;
		}
		else
		{
			m_MdbSubscriber->OnRecordBatchInsert(Position::TableID, records);
		}
		m_DBInited = true;
	}
	bool PositionTable::Insert(Position* record)
	{
		std::lock_guard guard(m_SharedMutex);
		if (!m_PrimaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for Position:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		m_PrimaryKey->Insert(record);

		m_AccountIndex->Insert(record);
		m_TradingDayIndex->Insert(record);
		
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordInsert(Position::TableID, record);
		}
		return true;
	}
	void PositionTable::BatchInsert(std::vector<mdb::Position*>* records)
	{
		{
			std::lock_guard guard(m_SharedMutex);
			for (auto record : *records)
			{
				auto newRecord = Position::Allocate();
				memcpy(newRecord, record, sizeof(Position));
				m_PrimaryKey->Insert(newRecord);

				m_AccountIndex->Insert(newRecord);
				m_TradingDayIndex->Insert(newRecord);
			}
		}
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			m_MdbSubscriber->OnRecordBatchInsert(Position::TableID, dbRecords);
		}
		delete records;
	}
	void PositionTable::Erase(Position* record)
	{
		std::lock_guard guard(m_SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordEraseByIndex(Position::TableID, PositionIndexAccount::IndexID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	int PositionTable::EraseByAccountIndex(const DateType& TradingDay, const AccountIDType& AccountID)
	{
		m_AccountIndex->FillCompareRecord(TradingDay, AccountID);
		std::vector<Position*> records;
		std::lock_guard guard(m_SharedMutex);
		auto range = m_AccountIndex->m_Index.equal_range(&t_ComparePosition);
		for (auto& it = range.first; it != range.second; ++it)
		{
			records.push_back(*it);
		}
		for (auto record : records)
		{
			EraseUniqueKey(record);
			EraseIndex(record);
			record->Deallocate();
		}
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			auto record = Position::Allocate();
			memcpy(record, &t_ComparePosition, sizeof(Position));
			m_MdbSubscriber->OnRecordEraseByIndex(Position::TableID, PositionIndexTradingDay::IndexID, record);
		}
		return (int)records.size();
	}
	int PositionTable::EraseByTradingDayIndex(const DateType& TradingDay)
	{
		m_TradingDayIndex->FillCompareRecord(TradingDay);
		std::vector<Position*> records;
		std::lock_guard guard(m_SharedMutex);
		auto range = m_TradingDayIndex->m_Index.equal_range(&t_ComparePosition);
		for (auto& it = range.first; it != range.second; ++it)
		{
			records.push_back(*it);
		}
		for (auto record : records)
		{
			EraseUniqueKey(record);
			EraseIndex(record);
			record->Deallocate();
		}
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			auto record = Position::Allocate();
			memcpy(record, &t_ComparePosition, sizeof(Position));
			m_MdbSubscriber->OnRecordErase(Position::TableID, record);
		}
		return (int)records.size();
	}
	bool PositionTable::Update(Position* const oldRecord, Position* const newRecord, bool updateDB)
	{
		std::lock_guard guard(m_SharedMutex);
		if (!m_PrimaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for Position:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New Position:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		bool AccountIndexUpdate = m_AccountIndex->NeedUpdate(oldRecord, newRecord);
		PositionIndexAccount::iterator itAccount;
		if (AccountIndexUpdate)
		{
			itAccount = m_AccountIndex->FindNode(oldRecord);
		}
		bool TradingDayIndexUpdate = m_TradingDayIndex->NeedUpdate(oldRecord, newRecord);
		PositionIndexTradingDay::iterator itTradingDay;
		if (TradingDayIndexUpdate)
		{
			itTradingDay = m_TradingDayIndex->FindNode(oldRecord);
		}
		::memcpy((void*)oldRecord, newRecord, sizeof(Position));
		if (AccountIndexUpdate)
		{
			m_AccountIndex->Update(itAccount);
		}
		if (TradingDayIndexUpdate)
		{
			m_TradingDayIndex->Update(itTradingDay);
		}

		if (updateDB && m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordUpdate(Position::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void PositionTable::TruncateTables()
	{
		std::lock_guard guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		m_PrimaryKey->m_Index.clear();
		m_AccountIndex->m_Index.clear();
		m_TradingDayIndex->m_Index.clear();
	}
	void PositionTable::TruncateTable()
	{
		std::lock_guard guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		m_PrimaryKey->m_Index.clear();
		m_AccountIndex->m_Index.clear();
		m_TradingDayIndex->m_Index.clear();
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordTruncate(Position::TableID);
		}
	}
	void PositionTable::Dump(const char* dir)
	{
		string fileName = string(dir) + "//t_Position.csv";
		FILE* dumpFile = fopen(fileName.c_str(), "w");
		if (dumpFile == nullptr)
		{
			return;
		}

		fprintf(dumpFile, "TradingDay,AccountID,AccountType,ExchangeID,InstrumentID,ProductClass,PosiDirection,TotalPosition,PositionFrozen,TodayPosition,MarketValue,CashIn,CashOut,Margin,Commission,FrozenCash,FrozenMargin,FrozenCommission,VolumeMultiple,CloseProfitByDate,CloseProfitByTrade,PositionProfitByDate,PositionProfitByTrade,SettlementPrice,PreSettlementPrice\n");
		char buff[4096] = { 0 };
		set<Position*, PositionLessForPositionPrimaryKey> records;
		std::shared_lock guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			records.insert(*it);
		}
		for (auto record : records)
		{
			fprintf(dumpFile, "%s\n", record->GetString());
		}
		records.clear();
		fclose(dumpFile);
	}
	void PositionTable::EraseUniqueKey(Position* record)
	{
		m_PrimaryKey->Erase(record);
	}
	void PositionTable::EraseIndex(Position* record)
	{
		m_AccountIndex->Erase(record);
		m_TradingDayIndex->Erase(record);
	}

	PositionDetailTable::PositionDetailTable(Mdb* mdb)
		:m_Mdb(mdb)
	{
		m_MdbSubscriber = nullptr;
		m_PrimaryKey = new PositionDetailPrimaryKey(this);
		m_TradeMatchIndex = new PositionDetailIndexTradeMatch(this);
		m_TradingDayIndex = new PositionDetailIndexTradingDay(this);
	}
	PositionDetailTable::~PositionDetailTable()
	{
		delete m_PrimaryKey;
		m_PrimaryKey = nullptr;
		delete m_TradeMatchIndex;
		m_TradeMatchIndex = nullptr;
		delete m_TradingDayIndex;
		m_TradingDayIndex = nullptr;
	}
	void PositionDetailTable::Subscribe(MdbSubscriber* mdbSubscriber)
	{
		m_MdbSubscriber = mdbSubscriber;
	}
	void PositionDetailTable::UnSubscribe()
	{
		m_MdbSubscriber = nullptr;
	}
	void PositionDetailTable::LockShared()
	{
		m_SharedMutex.lock_shared();
	}
	void PositionDetailTable::UnlockShared()
	{
		m_SharedMutex.unlock_shared();
	}
	void PositionDetailTable::InitDB()
	{
		m_MdbSubscriber->OnRecordTruncate(PositionDetail::TableID);
		
		auto records = new std::vector<const void*>();
		std::shared_lock guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			records->push_back(new PositionDetail(**it));
		}
		if (records->empty())
		{
			delete records;
		}
		else
		{
			m_MdbSubscriber->OnRecordBatchInsert(PositionDetail::TableID, records);
		}
		m_DBInited = true;
	}
	bool PositionDetailTable::Insert(PositionDetail* record)
	{
		std::lock_guard guard(m_SharedMutex);
		if (!m_PrimaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for PositionDetail:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		m_PrimaryKey->Insert(record);

		m_TradeMatchIndex->Insert(record);
		m_TradingDayIndex->Insert(record);
		
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordInsert(PositionDetail::TableID, record);
		}
		return true;
	}
	void PositionDetailTable::BatchInsert(std::vector<mdb::PositionDetail*>* records)
	{
		{
			std::lock_guard guard(m_SharedMutex);
			for (auto record : *records)
			{
				auto newRecord = PositionDetail::Allocate();
				memcpy(newRecord, record, sizeof(PositionDetail));
				m_PrimaryKey->Insert(newRecord);

				m_TradeMatchIndex->Insert(newRecord);
				m_TradingDayIndex->Insert(newRecord);
			}
		}
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			m_MdbSubscriber->OnRecordBatchInsert(PositionDetail::TableID, dbRecords);
		}
		delete records;
	}
	void PositionDetailTable::Erase(PositionDetail* record)
	{
		std::lock_guard guard(m_SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordEraseByIndex(PositionDetail::TableID, PositionDetailIndexTradeMatch::IndexID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	int PositionDetailTable::EraseByTradeMatchIndex(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection)
	{
		m_TradeMatchIndex->FillCompareRecord(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection);
		std::vector<PositionDetail*> records;
		std::lock_guard guard(m_SharedMutex);
		auto range = m_TradeMatchIndex->m_Index.equal_range(&t_ComparePositionDetail);
		for (auto& it = range.first; it != range.second; ++it)
		{
			records.push_back(*it);
		}
		for (auto record : records)
		{
			EraseUniqueKey(record);
			EraseIndex(record);
			record->Deallocate();
		}
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			auto record = PositionDetail::Allocate();
			memcpy(record, &t_ComparePositionDetail, sizeof(PositionDetail));
			m_MdbSubscriber->OnRecordEraseByIndex(PositionDetail::TableID, PositionDetailIndexTradingDay::IndexID, record);
		}
		return (int)records.size();
	}
	int PositionDetailTable::EraseByTradingDayIndex(const DateType& TradingDay)
	{
		m_TradingDayIndex->FillCompareRecord(TradingDay);
		std::vector<PositionDetail*> records;
		std::lock_guard guard(m_SharedMutex);
		auto range = m_TradingDayIndex->m_Index.equal_range(&t_ComparePositionDetail);
		for (auto& it = range.first; it != range.second; ++it)
		{
			records.push_back(*it);
		}
		for (auto record : records)
		{
			EraseUniqueKey(record);
			EraseIndex(record);
			record->Deallocate();
		}
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			auto record = PositionDetail::Allocate();
			memcpy(record, &t_ComparePositionDetail, sizeof(PositionDetail));
			m_MdbSubscriber->OnRecordErase(PositionDetail::TableID, record);
		}
		return (int)records.size();
	}
	bool PositionDetailTable::Update(PositionDetail* const oldRecord, PositionDetail* const newRecord, bool updateDB)
	{
		std::lock_guard guard(m_SharedMutex);
		if (!m_PrimaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for PositionDetail:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New PositionDetail:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		bool TradeMatchIndexUpdate = m_TradeMatchIndex->NeedUpdate(oldRecord, newRecord);
		PositionDetailIndexTradeMatch::iterator itTradeMatch;
		if (TradeMatchIndexUpdate)
		{
			itTradeMatch = m_TradeMatchIndex->FindNode(oldRecord);
		}
		bool TradingDayIndexUpdate = m_TradingDayIndex->NeedUpdate(oldRecord, newRecord);
		PositionDetailIndexTradingDay::iterator itTradingDay;
		if (TradingDayIndexUpdate)
		{
			itTradingDay = m_TradingDayIndex->FindNode(oldRecord);
		}
		::memcpy((void*)oldRecord, newRecord, sizeof(PositionDetail));
		if (TradeMatchIndexUpdate)
		{
			m_TradeMatchIndex->Update(itTradeMatch);
		}
		if (TradingDayIndexUpdate)
		{
			m_TradingDayIndex->Update(itTradingDay);
		}

		if (updateDB && m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordUpdate(PositionDetail::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void PositionDetailTable::TruncateTables()
	{
		std::lock_guard guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		m_PrimaryKey->m_Index.clear();
		m_TradeMatchIndex->m_Index.clear();
		m_TradingDayIndex->m_Index.clear();
	}
	void PositionDetailTable::TruncateTable()
	{
		std::lock_guard guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		m_PrimaryKey->m_Index.clear();
		m_TradeMatchIndex->m_Index.clear();
		m_TradingDayIndex->m_Index.clear();
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordTruncate(PositionDetail::TableID);
		}
	}
	void PositionDetailTable::Dump(const char* dir)
	{
		string fileName = string(dir) + "//t_PositionDetail.csv";
		FILE* dumpFile = fopen(fileName.c_str(), "w");
		if (dumpFile == nullptr)
		{
			return;
		}

		fprintf(dumpFile, "TradingDay,AccountID,AccountType,ExchangeID,InstrumentID,ProductClass,PosiDirection,OpenDate,TradeID,Volume,OpenPrice,MarketValue,CashIn,CashOut,Margin,Commission,VolumeMultiple,CloseProfitByDate,CloseProfitByTrade,PositionProfitByDate,PositionProfitByTrade,SettlementPrice,PreSettlementPrice,CloseVolume,CloseAmount\n");
		char buff[4096] = { 0 };
		set<PositionDetail*, PositionDetailLessForPositionDetailPrimaryKey> records;
		std::shared_lock guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			records.insert(*it);
		}
		for (auto record : records)
		{
			fprintf(dumpFile, "%s\n", record->GetString());
		}
		records.clear();
		fclose(dumpFile);
	}
	void PositionDetailTable::EraseUniqueKey(PositionDetail* record)
	{
		m_PrimaryKey->Erase(record);
	}
	void PositionDetailTable::EraseIndex(PositionDetail* record)
	{
		m_TradeMatchIndex->Erase(record);
		m_TradingDayIndex->Erase(record);
	}

	OrderTable::OrderTable(Mdb* mdb)
		:m_Mdb(mdb)
	{
		m_MdbSubscriber = nullptr;
		m_PrimaryKey = new OrderPrimaryKey(this);
		m_ClientOrderIDUniqueKey = new OrderUniqueKeyClientOrderID(this);
	}
	OrderTable::~OrderTable()
	{
		delete m_PrimaryKey;
		m_PrimaryKey = nullptr;
		delete m_ClientOrderIDUniqueKey;
		m_ClientOrderIDUniqueKey = nullptr;
	}
	void OrderTable::Subscribe(MdbSubscriber* mdbSubscriber)
	{
		m_MdbSubscriber = mdbSubscriber;
	}
	void OrderTable::UnSubscribe()
	{
		m_MdbSubscriber = nullptr;
	}
	void OrderTable::LockShared()
	{
		m_SharedMutex.lock_shared();
	}
	void OrderTable::UnlockShared()
	{
		m_SharedMutex.unlock_shared();
	}
	void OrderTable::InitDB()
	{
		m_MdbSubscriber->OnRecordTruncate(Order::TableID);
		
		auto records = new std::vector<const void*>();
		std::shared_lock guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			records->push_back(new Order(**it));
		}
		if (records->empty())
		{
			delete records;
		}
		else
		{
			m_MdbSubscriber->OnRecordBatchInsert(Order::TableID, records);
		}
		m_DBInited = true;
	}
	bool OrderTable::Insert(Order* record)
	{
		std::lock_guard guard(m_SharedMutex);
		if (!m_PrimaryKey->CheckInsert(record) || !m_ClientOrderIDUniqueKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for Order:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		m_PrimaryKey->Insert(record);
		m_ClientOrderIDUniqueKey->Insert(record);

		
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordInsert(Order::TableID, record);
		}
		return true;
	}
	void OrderTable::BatchInsert(std::vector<mdb::Order*>* records)
	{
		{
			std::lock_guard guard(m_SharedMutex);
			for (auto record : *records)
			{
				auto newRecord = Order::Allocate();
				memcpy(newRecord, record, sizeof(Order));
				m_PrimaryKey->Insert(newRecord);
				m_ClientOrderIDUniqueKey->Insert(newRecord);

			}
		}
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			m_MdbSubscriber->OnRecordBatchInsert(Order::TableID, dbRecords);
		}
		delete records;
	}
	void OrderTable::Erase(Order* record)
	{
		std::lock_guard guard(m_SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordErase(Order::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	bool OrderTable::Update(Order* const oldRecord, Order* const newRecord, bool updateDB)
	{
		std::lock_guard guard(m_SharedMutex);
		if (!m_PrimaryKey->CheckUpdate(oldRecord, newRecord) || !m_ClientOrderIDUniqueKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for Order:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New Order:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		::memcpy((void*)oldRecord, newRecord, sizeof(Order));

		if (updateDB && m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordUpdate(Order::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void OrderTable::TruncateTables()
	{
		std::lock_guard guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		m_PrimaryKey->m_Index.clear();
		m_ClientOrderIDUniqueKey->m_Index.clear();
	}
	void OrderTable::TruncateTable()
	{
		std::lock_guard guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		m_PrimaryKey->m_Index.clear();
		m_ClientOrderIDUniqueKey->m_Index.clear();
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordTruncate(Order::TableID);
		}
	}
	void OrderTable::Dump(const char* dir)
	{
		string fileName = string(dir) + "//t_Order.csv";
		FILE* dumpFile = fopen(fileName.c_str(), "w");
		if (dumpFile == nullptr)
		{
			return;
		}

		fprintf(dumpFile, "TradingDay,AccountID,AccountType,ExchangeID,InstrumentID,ProductClass,OrderID,OrderSysID,Direction,OffsetFlag,OrderPriceType,Price,Volume,VolumeTotal,VolumeTraded,VolumeMultiple,OrderStatus,OrderDate,OrderTime,CancelDate,CancelTime,SessionID,ClientOrderID,RequestID,OfferID,TradeGroupID,RiskGroupID,CommissionGroupID,FrozenCash,FrozenMargin,FrozenCommission,RebuildMark,IsForceClose\n");
		char buff[4096] = { 0 };
		set<Order*, OrderLessForOrderPrimaryKey> records;
		std::shared_lock guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			records.insert(*it);
		}
		for (auto record : records)
		{
			fprintf(dumpFile, "%s\n", record->GetString());
		}
		records.clear();
		fclose(dumpFile);
	}
	void OrderTable::EraseUniqueKey(Order* record)
	{
		m_PrimaryKey->Erase(record);
		m_ClientOrderIDUniqueKey->Erase(record);
	}
	void OrderTable::EraseIndex(Order* record)
	{
	}

	TradeTable::TradeTable(Mdb* mdb)
		:m_Mdb(mdb)
	{
		m_MdbSubscriber = nullptr;
		m_PrimaryKey = new TradePrimaryKey(this);
	}
	TradeTable::~TradeTable()
	{
		delete m_PrimaryKey;
		m_PrimaryKey = nullptr;
	}
	void TradeTable::Subscribe(MdbSubscriber* mdbSubscriber)
	{
		m_MdbSubscriber = mdbSubscriber;
	}
	void TradeTable::UnSubscribe()
	{
		m_MdbSubscriber = nullptr;
	}
	void TradeTable::LockShared()
	{
		m_SharedMutex.lock_shared();
	}
	void TradeTable::UnlockShared()
	{
		m_SharedMutex.unlock_shared();
	}
	void TradeTable::InitDB()
	{
		m_MdbSubscriber->OnRecordTruncate(Trade::TableID);
		
		auto records = new std::vector<const void*>();
		std::shared_lock guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			records->push_back(new Trade(**it));
		}
		if (records->empty())
		{
			delete records;
		}
		else
		{
			m_MdbSubscriber->OnRecordBatchInsert(Trade::TableID, records);
		}
		m_DBInited = true;
	}
	bool TradeTable::Insert(Trade* record)
	{
		std::lock_guard guard(m_SharedMutex);
		if (!m_PrimaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for Trade:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		m_PrimaryKey->Insert(record);

		
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordInsert(Trade::TableID, record);
		}
		return true;
	}
	void TradeTable::BatchInsert(std::vector<mdb::Trade*>* records)
	{
		{
			std::lock_guard guard(m_SharedMutex);
			for (auto record : *records)
			{
				auto newRecord = Trade::Allocate();
				memcpy(newRecord, record, sizeof(Trade));
				m_PrimaryKey->Insert(newRecord);

			}
		}
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			m_MdbSubscriber->OnRecordBatchInsert(Trade::TableID, dbRecords);
		}
		delete records;
	}
	void TradeTable::Erase(Trade* record)
	{
		std::lock_guard guard(m_SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordErase(Trade::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	bool TradeTable::Update(Trade* const oldRecord, Trade* const newRecord, bool updateDB)
	{
		std::lock_guard guard(m_SharedMutex);
		if (!m_PrimaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for Trade:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New Trade:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		::memcpy((void*)oldRecord, newRecord, sizeof(Trade));

		if (updateDB && m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordUpdate(Trade::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void TradeTable::TruncateTables()
	{
		std::lock_guard guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		m_PrimaryKey->m_Index.clear();
	}
	void TradeTable::TruncateTable()
	{
		std::lock_guard guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		m_PrimaryKey->m_Index.clear();
		if (m_MdbSubscriber != nullptr && m_DBInited)
		{
			m_MdbSubscriber->OnRecordTruncate(Trade::TableID);
		}
	}
	void TradeTable::Dump(const char* dir)
	{
		string fileName = string(dir) + "//t_Trade.csv";
		FILE* dumpFile = fopen(fileName.c_str(), "w");
		if (dumpFile == nullptr)
		{
			return;
		}

		fprintf(dumpFile, "TradingDay,AccountID,AccountType,ExchangeID,InstrumentID,ProductClass,OrderID,OrderSysID,TradeID,Direction,OffsetFlag,Price,Volume,VolumeMultiple,TradeAmount,Commission,TradeDate,TradeTime\n");
		char buff[4096] = { 0 };
		set<Trade*, TradeLessForTradePrimaryKey> records;
		std::shared_lock guard(m_SharedMutex);
		for (auto it = m_PrimaryKey->m_Index.begin(); it != m_PrimaryKey->m_Index.end(); ++it)
		{
			records.insert(*it);
		}
		for (auto record : records)
		{
			fprintf(dumpFile, "%s\n", record->GetString());
		}
		records.clear();
		fclose(dumpFile);
	}
	void TradeTable::EraseUniqueKey(Trade* record)
	{
		m_PrimaryKey->Erase(record);
	}
	void TradeTable::EraseIndex(Trade* record)
	{
	}

}

