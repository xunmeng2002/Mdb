// 本文件由 ../Templates/Cpp/Mdb/MdbTables.cpp.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#include "MdbTables.h"
#include <Spark/Core/Logger/Logger.h>
#include <string>
#include <cstring>
#include <set>
#include <vector>

using std::string;
using std::set;
using namespace spark::core;

namespace mdb
{
	TradingDayTable::TradingDayTable()
	{
		mdbSubscriber = nullptr;
		primaryKey = new TradingDayPrimaryKey(this);
	}
	TradingDayTable::~TradingDayTable()
	{
		delete primaryKey;
		primaryKey = nullptr;
	}
	void TradingDayTable::Subscribe(MdbSubscriber* subscriber)
	{
		mdbSubscriber = subscriber;
	}
	void TradingDayTable::UnSubscribe()
	{
		mdbSubscriber = nullptr;
	}
	void TradingDayTable::LockShared()
	{
		sharedMutex.lock_shared();
	}
	void TradingDayTable::UnlockShared()
	{
		sharedMutex.unlock_shared();
	}
	void TradingDayTable::InitDB()
	{
		if (mdbSubscriber == nullptr)
		{
			dbInited = true;
			return;
		}
		mdbSubscriber->OnRecordTruncate(TradingDay::TableID);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(sharedMutex);
			for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
			{
				auto record = TradingDay::Allocate();
				memcpy(record, *it, sizeof(TradingDay));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber->OnRecordBatchInsert(TradingDay::TableID, records);
		}
		else
		{
			delete records;
		}
		dbInited = true;
	}
	bool TradingDayTable::Insert(TradingDay* record)
	{
		std::lock_guard guard(sharedMutex);
		if (!primaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for TradingDay:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		primaryKey->Insert(record);

		
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordInsert(TradingDay::TableID, record);
		}
		return true;
	}
	void TradingDayTable::BatchInsert(std::vector<mdb::TradingDay*>* records)
	{
		{
			std::lock_guard guard(sharedMutex);
			for (auto record : *records)
			{
				auto newRecord = TradingDay::Allocate();
				memcpy(newRecord, record, sizeof(TradingDay));
				primaryKey->Insert(newRecord);

			}
		}
		if (mdbSubscriber != nullptr && dbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber->OnRecordBatchInsert(TradingDay::TableID, dbRecords);
		}
		delete records;
	}
	void TradingDayTable::Erase(TradingDay* record)
	{
		std::lock_guard guard(sharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordErase(TradingDay::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	bool TradingDayTable::Update(TradingDay* const oldRecord, TradingDay* const newRecord, bool updateDB)
	{
		std::lock_guard guard(sharedMutex);
		if (!primaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for TradingDay:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New TradingDay:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		::memcpy(oldRecord, newRecord, sizeof(TradingDay));

		if (updateDB && mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordUpdate(TradingDay::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void TradingDayTable::TruncateTables()
	{
		std::lock_guard guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		primaryKey->index.clear();
	}
	void TradingDayTable::TruncateTable()
	{
		std::lock_guard guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		primaryKey->index.clear();
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordTruncate(TradingDay::TableID);
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
		std::shared_lock guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
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
		primaryKey->Erase(record);
	}
	void TradingDayTable::EraseIndex(TradingDay* record)
	{
	}

	ExchangeTable::ExchangeTable()
	{
		mdbSubscriber = nullptr;
		primaryKey = new ExchangePrimaryKey(this);
	}
	ExchangeTable::~ExchangeTable()
	{
		delete primaryKey;
		primaryKey = nullptr;
	}
	void ExchangeTable::Subscribe(MdbSubscriber* subscriber)
	{
		mdbSubscriber = subscriber;
	}
	void ExchangeTable::UnSubscribe()
	{
		mdbSubscriber = nullptr;
	}
	void ExchangeTable::LockShared()
	{
		sharedMutex.lock_shared();
	}
	void ExchangeTable::UnlockShared()
	{
		sharedMutex.unlock_shared();
	}
	void ExchangeTable::InitDB()
	{
		if (mdbSubscriber == nullptr)
		{
			dbInited = true;
			return;
		}
		mdbSubscriber->OnRecordTruncate(Exchange::TableID);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(sharedMutex);
			for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
			{
				auto record = Exchange::Allocate();
				memcpy(record, *it, sizeof(Exchange));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber->OnRecordBatchInsert(Exchange::TableID, records);
		}
		else
		{
			delete records;
		}
		dbInited = true;
	}
	bool ExchangeTable::Insert(Exchange* record)
	{
		std::lock_guard guard(sharedMutex);
		if (!primaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for Exchange:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		primaryKey->Insert(record);

		
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordInsert(Exchange::TableID, record);
		}
		return true;
	}
	void ExchangeTable::BatchInsert(std::vector<mdb::Exchange*>* records)
	{
		{
			std::lock_guard guard(sharedMutex);
			for (auto record : *records)
			{
				auto newRecord = Exchange::Allocate();
				memcpy(newRecord, record, sizeof(Exchange));
				primaryKey->Insert(newRecord);

			}
		}
		if (mdbSubscriber != nullptr && dbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber->OnRecordBatchInsert(Exchange::TableID, dbRecords);
		}
		delete records;
	}
	void ExchangeTable::Erase(Exchange* record)
	{
		std::lock_guard guard(sharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordErase(Exchange::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	bool ExchangeTable::Update(Exchange* const oldRecord, Exchange* const newRecord, bool updateDB)
	{
		std::lock_guard guard(sharedMutex);
		if (!primaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for Exchange:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New Exchange:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		::memcpy(oldRecord, newRecord, sizeof(Exchange));

		if (updateDB && mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordUpdate(Exchange::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void ExchangeTable::TruncateTables()
	{
		std::lock_guard guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		primaryKey->index.clear();
	}
	void ExchangeTable::TruncateTable()
	{
		std::lock_guard guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		primaryKey->index.clear();
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordTruncate(Exchange::TableID);
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
		std::shared_lock guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
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
		primaryKey->Erase(record);
	}
	void ExchangeTable::EraseIndex(Exchange* record)
	{
	}

	ProductTable::ProductTable()
	{
		mdbSubscriber = nullptr;
		primaryKey = new ProductPrimaryKey(this);
	}
	ProductTable::~ProductTable()
	{
		delete primaryKey;
		primaryKey = nullptr;
	}
	void ProductTable::Subscribe(MdbSubscriber* subscriber)
	{
		mdbSubscriber = subscriber;
	}
	void ProductTable::UnSubscribe()
	{
		mdbSubscriber = nullptr;
	}
	void ProductTable::LockShared()
	{
		sharedMutex.lock_shared();
	}
	void ProductTable::UnlockShared()
	{
		sharedMutex.unlock_shared();
	}
	void ProductTable::InitDB()
	{
		if (mdbSubscriber == nullptr)
		{
			dbInited = true;
			return;
		}
		mdbSubscriber->OnRecordTruncate(Product::TableID);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(sharedMutex);
			for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
			{
				auto record = Product::Allocate();
				memcpy(record, *it, sizeof(Product));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber->OnRecordBatchInsert(Product::TableID, records);
		}
		else
		{
			delete records;
		}
		dbInited = true;
	}
	bool ProductTable::Insert(Product* record)
	{
		std::lock_guard guard(sharedMutex);
		if (!primaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for Product:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		primaryKey->Insert(record);

		
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordInsert(Product::TableID, record);
		}
		return true;
	}
	void ProductTable::BatchInsert(std::vector<mdb::Product*>* records)
	{
		{
			std::lock_guard guard(sharedMutex);
			for (auto record : *records)
			{
				auto newRecord = Product::Allocate();
				memcpy(newRecord, record, sizeof(Product));
				primaryKey->Insert(newRecord);

			}
		}
		if (mdbSubscriber != nullptr && dbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber->OnRecordBatchInsert(Product::TableID, dbRecords);
		}
		delete records;
	}
	void ProductTable::Erase(Product* record)
	{
		std::lock_guard guard(sharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordErase(Product::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	bool ProductTable::Update(Product* const oldRecord, Product* const newRecord, bool updateDB)
	{
		std::lock_guard guard(sharedMutex);
		if (!primaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for Product:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New Product:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		::memcpy(oldRecord, newRecord, sizeof(Product));

		if (updateDB && mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordUpdate(Product::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void ProductTable::TruncateTables()
	{
		std::lock_guard guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		primaryKey->index.clear();
	}
	void ProductTable::TruncateTable()
	{
		std::lock_guard guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		primaryKey->index.clear();
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordTruncate(Product::TableID);
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
		std::shared_lock guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
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
		primaryKey->Erase(record);
	}
	void ProductTable::EraseIndex(Product* record)
	{
	}

	InstrumentTable::InstrumentTable()
	{
		mdbSubscriber = nullptr;
		primaryKey = new InstrumentPrimaryKey(this);
	}
	InstrumentTable::~InstrumentTable()
	{
		delete primaryKey;
		primaryKey = nullptr;
	}
	void InstrumentTable::Subscribe(MdbSubscriber* subscriber)
	{
		mdbSubscriber = subscriber;
	}
	void InstrumentTable::UnSubscribe()
	{
		mdbSubscriber = nullptr;
	}
	void InstrumentTable::LockShared()
	{
		sharedMutex.lock_shared();
	}
	void InstrumentTable::UnlockShared()
	{
		sharedMutex.unlock_shared();
	}
	void InstrumentTable::InitDB()
	{
		if (mdbSubscriber == nullptr)
		{
			dbInited = true;
			return;
		}
		mdbSubscriber->OnRecordTruncate(Instrument::TableID);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(sharedMutex);
			for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
			{
				auto record = Instrument::Allocate();
				memcpy(record, *it, sizeof(Instrument));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber->OnRecordBatchInsert(Instrument::TableID, records);
		}
		else
		{
			delete records;
		}
		dbInited = true;
	}
	bool InstrumentTable::Insert(Instrument* record)
	{
		std::lock_guard guard(sharedMutex);
		if (!primaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for Instrument:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		primaryKey->Insert(record);

		
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordInsert(Instrument::TableID, record);
		}
		return true;
	}
	void InstrumentTable::BatchInsert(std::vector<mdb::Instrument*>* records)
	{
		{
			std::lock_guard guard(sharedMutex);
			for (auto record : *records)
			{
				auto newRecord = Instrument::Allocate();
				memcpy(newRecord, record, sizeof(Instrument));
				primaryKey->Insert(newRecord);

			}
		}
		if (mdbSubscriber != nullptr && dbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber->OnRecordBatchInsert(Instrument::TableID, dbRecords);
		}
		delete records;
	}
	void InstrumentTable::Erase(Instrument* record)
	{
		std::lock_guard guard(sharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordErase(Instrument::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	bool InstrumentTable::Update(Instrument* const oldRecord, Instrument* const newRecord, bool updateDB)
	{
		std::lock_guard guard(sharedMutex);
		if (!primaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for Instrument:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New Instrument:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		::memcpy(oldRecord, newRecord, sizeof(Instrument));

		if (updateDB && mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordUpdate(Instrument::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void InstrumentTable::TruncateTables()
	{
		std::lock_guard guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		primaryKey->index.clear();
	}
	void InstrumentTable::TruncateTable()
	{
		std::lock_guard guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		primaryKey->index.clear();
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordTruncate(Instrument::TableID);
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
		std::shared_lock guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
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
		primaryKey->Erase(record);
	}
	void InstrumentTable::EraseIndex(Instrument* record)
	{
	}

	PrimaryAccountTable::PrimaryAccountTable()
	{
		mdbSubscriber = nullptr;
		primaryKey = new PrimaryAccountPrimaryKey(this);
		offerIDIndex = new PrimaryAccountIndexOfferID(this);
	}
	PrimaryAccountTable::~PrimaryAccountTable()
	{
		delete primaryKey;
		primaryKey = nullptr;
		delete offerIDIndex;
		offerIDIndex = nullptr;
	}
	void PrimaryAccountTable::Subscribe(MdbSubscriber* subscriber)
	{
		mdbSubscriber = subscriber;
	}
	void PrimaryAccountTable::UnSubscribe()
	{
		mdbSubscriber = nullptr;
	}
	void PrimaryAccountTable::LockShared()
	{
		sharedMutex.lock_shared();
	}
	void PrimaryAccountTable::UnlockShared()
	{
		sharedMutex.unlock_shared();
	}
	void PrimaryAccountTable::InitDB()
	{
		if (mdbSubscriber == nullptr)
		{
			dbInited = true;
			return;
		}
		mdbSubscriber->OnRecordTruncate(PrimaryAccount::TableID);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(sharedMutex);
			for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
			{
				auto record = PrimaryAccount::Allocate();
				memcpy(record, *it, sizeof(PrimaryAccount));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber->OnRecordBatchInsert(PrimaryAccount::TableID, records);
		}
		else
		{
			delete records;
		}
		dbInited = true;
	}
	bool PrimaryAccountTable::Insert(PrimaryAccount* record)
	{
		std::lock_guard guard(sharedMutex);
		if (!primaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for PrimaryAccount:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		primaryKey->Insert(record);

		offerIDIndex->Insert(record);
		
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordInsert(PrimaryAccount::TableID, record);
		}
		return true;
	}
	void PrimaryAccountTable::BatchInsert(std::vector<mdb::PrimaryAccount*>* records)
	{
		{
			std::lock_guard guard(sharedMutex);
			for (auto record : *records)
			{
				auto newRecord = PrimaryAccount::Allocate();
				memcpy(newRecord, record, sizeof(PrimaryAccount));
				primaryKey->Insert(newRecord);

				offerIDIndex->Insert(newRecord);
			}
		}
		if (mdbSubscriber != nullptr && dbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber->OnRecordBatchInsert(PrimaryAccount::TableID, dbRecords);
		}
		delete records;
	}
	void PrimaryAccountTable::Erase(PrimaryAccount* record)
	{
		std::lock_guard guard(sharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordErase(PrimaryAccount::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	int PrimaryAccountTable::EraseByOfferIDIndex(const OfferIDType& OfferID)
	{
		offerIDIndex->FillCompareRecord(OfferID);
		std::vector<PrimaryAccount*> records;
		std::lock_guard guard(sharedMutex);
		auto range = offerIDIndex->index.equal_range(&ComparePrimaryAccount);
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
		if (mdbSubscriber != nullptr && dbInited)
		{
			auto record = PrimaryAccount::Allocate();
			memcpy(record, &ComparePrimaryAccount, sizeof(PrimaryAccount));
			mdbSubscriber->OnRecordEraseByIndex(PrimaryAccount::TableID, PrimaryAccountIndexOfferID::IndexID, record);
		}
		return static_cast<int>(records.size());
	}
	bool PrimaryAccountTable::Update(PrimaryAccount* const oldRecord, PrimaryAccount* const newRecord, bool updateDB)
	{
		std::lock_guard guard(sharedMutex);
		if (!primaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for PrimaryAccount:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New PrimaryAccount:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		bool OfferIDIndexUpdate = offerIDIndex->NeedUpdate(oldRecord, newRecord);
		PrimaryAccountIndexOfferID::iterator itOfferID;
		if (OfferIDIndexUpdate)
		{
			itOfferID = offerIDIndex->FindNode(oldRecord);
		}
		::memcpy(oldRecord, newRecord, sizeof(PrimaryAccount));
		if (OfferIDIndexUpdate)
		{
			offerIDIndex->Update(itOfferID);
		}

		if (updateDB && mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordUpdate(PrimaryAccount::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void PrimaryAccountTable::TruncateTables()
	{
		std::lock_guard guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		primaryKey->index.clear();
		offerIDIndex->index.clear();
	}
	void PrimaryAccountTable::TruncateTable()
	{
		std::lock_guard guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		primaryKey->index.clear();
		offerIDIndex->index.clear();
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordTruncate(PrimaryAccount::TableID);
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
		std::shared_lock guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
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
		primaryKey->Erase(record);
	}
	void PrimaryAccountTable::EraseIndex(PrimaryAccount* record)
	{
		offerIDIndex->Erase(record);
	}

	AccountTable::AccountTable()
	{
		mdbSubscriber = nullptr;
		primaryKey = new AccountPrimaryKey(this);
	}
	AccountTable::~AccountTable()
	{
		delete primaryKey;
		primaryKey = nullptr;
	}
	void AccountTable::Subscribe(MdbSubscriber* subscriber)
	{
		mdbSubscriber = subscriber;
	}
	void AccountTable::UnSubscribe()
	{
		mdbSubscriber = nullptr;
	}
	void AccountTable::LockShared()
	{
		sharedMutex.lock_shared();
	}
	void AccountTable::UnlockShared()
	{
		sharedMutex.unlock_shared();
	}
	void AccountTable::InitDB()
	{
		if (mdbSubscriber == nullptr)
		{
			dbInited = true;
			return;
		}
		mdbSubscriber->OnRecordTruncate(Account::TableID);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(sharedMutex);
			for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
			{
				auto record = Account::Allocate();
				memcpy(record, *it, sizeof(Account));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber->OnRecordBatchInsert(Account::TableID, records);
		}
		else
		{
			delete records;
		}
		dbInited = true;
	}
	bool AccountTable::Insert(Account* record)
	{
		std::lock_guard guard(sharedMutex);
		if (!primaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for Account:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		primaryKey->Insert(record);

		
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordInsert(Account::TableID, record);
		}
		return true;
	}
	void AccountTable::BatchInsert(std::vector<mdb::Account*>* records)
	{
		{
			std::lock_guard guard(sharedMutex);
			for (auto record : *records)
			{
				auto newRecord = Account::Allocate();
				memcpy(newRecord, record, sizeof(Account));
				primaryKey->Insert(newRecord);

			}
		}
		if (mdbSubscriber != nullptr && dbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber->OnRecordBatchInsert(Account::TableID, dbRecords);
		}
		delete records;
	}
	void AccountTable::Erase(Account* record)
	{
		std::lock_guard guard(sharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordErase(Account::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	bool AccountTable::Update(Account* const oldRecord, Account* const newRecord, bool updateDB)
	{
		std::lock_guard guard(sharedMutex);
		if (!primaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for Account:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New Account:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		::memcpy(oldRecord, newRecord, sizeof(Account));

		if (updateDB && mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordUpdate(Account::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void AccountTable::TruncateTables()
	{
		std::lock_guard guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		primaryKey->index.clear();
	}
	void AccountTable::TruncateTable()
	{
		std::lock_guard guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		primaryKey->index.clear();
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordTruncate(Account::TableID);
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
		std::shared_lock guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
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
		primaryKey->Erase(record);
	}
	void AccountTable::EraseIndex(Account* record)
	{
	}

	CapitalTable::CapitalTable()
	{
		mdbSubscriber = nullptr;
		primaryKey = new CapitalPrimaryKey(this);
		tradingDayIndex = new CapitalIndexTradingDay(this);
	}
	CapitalTable::~CapitalTable()
	{
		delete primaryKey;
		primaryKey = nullptr;
		delete tradingDayIndex;
		tradingDayIndex = nullptr;
	}
	void CapitalTable::Subscribe(MdbSubscriber* subscriber)
	{
		mdbSubscriber = subscriber;
	}
	void CapitalTable::UnSubscribe()
	{
		mdbSubscriber = nullptr;
	}
	void CapitalTable::LockShared()
	{
		sharedMutex.lock_shared();
	}
	void CapitalTable::UnlockShared()
	{
		sharedMutex.unlock_shared();
	}
	void CapitalTable::InitDB()
	{
		if (mdbSubscriber == nullptr)
		{
			dbInited = true;
			return;
		}
		mdbSubscriber->OnRecordTruncate(Capital::TableID);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(sharedMutex);
			for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
			{
				auto record = Capital::Allocate();
				memcpy(record, *it, sizeof(Capital));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber->OnRecordBatchInsert(Capital::TableID, records);
		}
		else
		{
			delete records;
		}
		dbInited = true;
	}
	bool CapitalTable::Insert(Capital* record)
	{
		std::lock_guard guard(sharedMutex);
		if (!primaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for Capital:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		primaryKey->Insert(record);

		tradingDayIndex->Insert(record);
		
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordInsert(Capital::TableID, record);
		}
		return true;
	}
	void CapitalTable::BatchInsert(std::vector<mdb::Capital*>* records)
	{
		{
			std::lock_guard guard(sharedMutex);
			for (auto record : *records)
			{
				auto newRecord = Capital::Allocate();
				memcpy(newRecord, record, sizeof(Capital));
				primaryKey->Insert(newRecord);

				tradingDayIndex->Insert(newRecord);
			}
		}
		if (mdbSubscriber != nullptr && dbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber->OnRecordBatchInsert(Capital::TableID, dbRecords);
		}
		delete records;
	}
	void CapitalTable::Erase(Capital* record)
	{
		std::lock_guard guard(sharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordErase(Capital::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	int CapitalTable::EraseByTradingDayIndex(const DateType& TradingDay)
	{
		tradingDayIndex->FillCompareRecord(TradingDay);
		std::vector<Capital*> records;
		std::lock_guard guard(sharedMutex);
		auto range = tradingDayIndex->index.equal_range(&CompareCapital);
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
		if (mdbSubscriber != nullptr && dbInited)
		{
			auto record = Capital::Allocate();
			memcpy(record, &CompareCapital, sizeof(Capital));
			mdbSubscriber->OnRecordEraseByIndex(Capital::TableID, CapitalIndexTradingDay::IndexID, record);
		}
		return static_cast<int>(records.size());
	}
	bool CapitalTable::Update(Capital* const oldRecord, Capital* const newRecord, bool updateDB)
	{
		std::lock_guard guard(sharedMutex);
		if (!primaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for Capital:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New Capital:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		bool TradingDayIndexUpdate = tradingDayIndex->NeedUpdate(oldRecord, newRecord);
		CapitalIndexTradingDay::iterator itTradingDay;
		if (TradingDayIndexUpdate)
		{
			itTradingDay = tradingDayIndex->FindNode(oldRecord);
		}
		::memcpy(oldRecord, newRecord, sizeof(Capital));
		if (TradingDayIndexUpdate)
		{
			tradingDayIndex->Update(itTradingDay);
		}

		if (updateDB && mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordUpdate(Capital::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void CapitalTable::TruncateTables()
	{
		std::lock_guard guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		primaryKey->index.clear();
		tradingDayIndex->index.clear();
	}
	void CapitalTable::TruncateTable()
	{
		std::lock_guard guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		primaryKey->index.clear();
		tradingDayIndex->index.clear();
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordTruncate(Capital::TableID);
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
		std::shared_lock guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
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
		primaryKey->Erase(record);
	}
	void CapitalTable::EraseIndex(Capital* record)
	{
		tradingDayIndex->Erase(record);
	}

	PositionTable::PositionTable()
	{
		mdbSubscriber = nullptr;
		primaryKey = new PositionPrimaryKey(this);
		accountIndex = new PositionIndexAccount(this);
		tradingDayIndex = new PositionIndexTradingDay(this);
	}
	PositionTable::~PositionTable()
	{
		delete primaryKey;
		primaryKey = nullptr;
		delete accountIndex;
		accountIndex = nullptr;
		delete tradingDayIndex;
		tradingDayIndex = nullptr;
	}
	void PositionTable::Subscribe(MdbSubscriber* subscriber)
	{
		mdbSubscriber = subscriber;
	}
	void PositionTable::UnSubscribe()
	{
		mdbSubscriber = nullptr;
	}
	void PositionTable::LockShared()
	{
		sharedMutex.lock_shared();
	}
	void PositionTable::UnlockShared()
	{
		sharedMutex.unlock_shared();
	}
	void PositionTable::InitDB()
	{
		if (mdbSubscriber == nullptr)
		{
			dbInited = true;
			return;
		}
		mdbSubscriber->OnRecordTruncate(Position::TableID);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(sharedMutex);
			for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
			{
				auto record = Position::Allocate();
				memcpy(record, *it, sizeof(Position));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber->OnRecordBatchInsert(Position::TableID, records);
		}
		else
		{
			delete records;
		}
		dbInited = true;
	}
	bool PositionTable::Insert(Position* record)
	{
		std::lock_guard guard(sharedMutex);
		if (!primaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for Position:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		primaryKey->Insert(record);

		accountIndex->Insert(record);
		tradingDayIndex->Insert(record);
		
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordInsert(Position::TableID, record);
		}
		return true;
	}
	void PositionTable::BatchInsert(std::vector<mdb::Position*>* records)
	{
		{
			std::lock_guard guard(sharedMutex);
			for (auto record : *records)
			{
				auto newRecord = Position::Allocate();
				memcpy(newRecord, record, sizeof(Position));
				primaryKey->Insert(newRecord);

				accountIndex->Insert(newRecord);
				tradingDayIndex->Insert(newRecord);
			}
		}
		if (mdbSubscriber != nullptr && dbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber->OnRecordBatchInsert(Position::TableID, dbRecords);
		}
		delete records;
	}
	void PositionTable::Erase(Position* record)
	{
		std::lock_guard guard(sharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordErase(Position::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	int PositionTable::EraseByAccountIndex(const DateType& TradingDay, const AccountIDType& AccountID)
	{
		accountIndex->FillCompareRecord(TradingDay, AccountID);
		std::vector<Position*> records;
		std::lock_guard guard(sharedMutex);
		auto range = accountIndex->index.equal_range(&ComparePosition);
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
		if (mdbSubscriber != nullptr && dbInited)
		{
			auto record = Position::Allocate();
			memcpy(record, &ComparePosition, sizeof(Position));
			mdbSubscriber->OnRecordEraseByIndex(Position::TableID, PositionIndexAccount::IndexID, record);
		}
		return static_cast<int>(records.size());
	}
	int PositionTable::EraseByTradingDayIndex(const DateType& TradingDay)
	{
		tradingDayIndex->FillCompareRecord(TradingDay);
		std::vector<Position*> records;
		std::lock_guard guard(sharedMutex);
		auto range = tradingDayIndex->index.equal_range(&ComparePosition);
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
		if (mdbSubscriber != nullptr && dbInited)
		{
			auto record = Position::Allocate();
			memcpy(record, &ComparePosition, sizeof(Position));
			mdbSubscriber->OnRecordEraseByIndex(Position::TableID, PositionIndexTradingDay::IndexID, record);
		}
		return static_cast<int>(records.size());
	}
	bool PositionTable::Update(Position* const oldRecord, Position* const newRecord, bool updateDB)
	{
		std::lock_guard guard(sharedMutex);
		if (!primaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for Position:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New Position:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		bool AccountIndexUpdate = accountIndex->NeedUpdate(oldRecord, newRecord);
		PositionIndexAccount::iterator itAccount;
		if (AccountIndexUpdate)
		{
			itAccount = accountIndex->FindNode(oldRecord);
		}
		bool TradingDayIndexUpdate = tradingDayIndex->NeedUpdate(oldRecord, newRecord);
		PositionIndexTradingDay::iterator itTradingDay;
		if (TradingDayIndexUpdate)
		{
			itTradingDay = tradingDayIndex->FindNode(oldRecord);
		}
		::memcpy(oldRecord, newRecord, sizeof(Position));
		if (AccountIndexUpdate)
		{
			accountIndex->Update(itAccount);
		}
		if (TradingDayIndexUpdate)
		{
			tradingDayIndex->Update(itTradingDay);
		}

		if (updateDB && mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordUpdate(Position::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void PositionTable::TruncateTables()
	{
		std::lock_guard guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		primaryKey->index.clear();
		accountIndex->index.clear();
		tradingDayIndex->index.clear();
	}
	void PositionTable::TruncateTable()
	{
		std::lock_guard guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		primaryKey->index.clear();
		accountIndex->index.clear();
		tradingDayIndex->index.clear();
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordTruncate(Position::TableID);
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
		std::shared_lock guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
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
		primaryKey->Erase(record);
	}
	void PositionTable::EraseIndex(Position* record)
	{
		accountIndex->Erase(record);
		tradingDayIndex->Erase(record);
	}

	PositionDetailTable::PositionDetailTable()
	{
		mdbSubscriber = nullptr;
		primaryKey = new PositionDetailPrimaryKey(this);
		tradeMatchIndex = new PositionDetailIndexTradeMatch(this);
		tradingDayIndex = new PositionDetailIndexTradingDay(this);
	}
	PositionDetailTable::~PositionDetailTable()
	{
		delete primaryKey;
		primaryKey = nullptr;
		delete tradeMatchIndex;
		tradeMatchIndex = nullptr;
		delete tradingDayIndex;
		tradingDayIndex = nullptr;
	}
	void PositionDetailTable::Subscribe(MdbSubscriber* subscriber)
	{
		mdbSubscriber = subscriber;
	}
	void PositionDetailTable::UnSubscribe()
	{
		mdbSubscriber = nullptr;
	}
	void PositionDetailTable::LockShared()
	{
		sharedMutex.lock_shared();
	}
	void PositionDetailTable::UnlockShared()
	{
		sharedMutex.unlock_shared();
	}
	void PositionDetailTable::InitDB()
	{
		if (mdbSubscriber == nullptr)
		{
			dbInited = true;
			return;
		}
		mdbSubscriber->OnRecordTruncate(PositionDetail::TableID);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(sharedMutex);
			for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
			{
				auto record = PositionDetail::Allocate();
				memcpy(record, *it, sizeof(PositionDetail));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber->OnRecordBatchInsert(PositionDetail::TableID, records);
		}
		else
		{
			delete records;
		}
		dbInited = true;
	}
	bool PositionDetailTable::Insert(PositionDetail* record)
	{
		std::lock_guard guard(sharedMutex);
		if (!primaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for PositionDetail:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		primaryKey->Insert(record);

		tradeMatchIndex->Insert(record);
		tradingDayIndex->Insert(record);
		
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordInsert(PositionDetail::TableID, record);
		}
		return true;
	}
	void PositionDetailTable::BatchInsert(std::vector<mdb::PositionDetail*>* records)
	{
		{
			std::lock_guard guard(sharedMutex);
			for (auto record : *records)
			{
				auto newRecord = PositionDetail::Allocate();
				memcpy(newRecord, record, sizeof(PositionDetail));
				primaryKey->Insert(newRecord);

				tradeMatchIndex->Insert(newRecord);
				tradingDayIndex->Insert(newRecord);
			}
		}
		if (mdbSubscriber != nullptr && dbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber->OnRecordBatchInsert(PositionDetail::TableID, dbRecords);
		}
		delete records;
	}
	void PositionDetailTable::Erase(PositionDetail* record)
	{
		std::lock_guard guard(sharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordErase(PositionDetail::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	int PositionDetailTable::EraseByTradeMatchIndex(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection)
	{
		tradeMatchIndex->FillCompareRecord(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection);
		std::vector<PositionDetail*> records;
		std::lock_guard guard(sharedMutex);
		auto range = tradeMatchIndex->index.equal_range(&ComparePositionDetail);
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
		if (mdbSubscriber != nullptr && dbInited)
		{
			auto record = PositionDetail::Allocate();
			memcpy(record, &ComparePositionDetail, sizeof(PositionDetail));
			mdbSubscriber->OnRecordEraseByIndex(PositionDetail::TableID, PositionDetailIndexTradeMatch::IndexID, record);
		}
		return static_cast<int>(records.size());
	}
	int PositionDetailTable::EraseByTradingDayIndex(const DateType& TradingDay)
	{
		tradingDayIndex->FillCompareRecord(TradingDay);
		std::vector<PositionDetail*> records;
		std::lock_guard guard(sharedMutex);
		auto range = tradingDayIndex->index.equal_range(&ComparePositionDetail);
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
		if (mdbSubscriber != nullptr && dbInited)
		{
			auto record = PositionDetail::Allocate();
			memcpy(record, &ComparePositionDetail, sizeof(PositionDetail));
			mdbSubscriber->OnRecordEraseByIndex(PositionDetail::TableID, PositionDetailIndexTradingDay::IndexID, record);
		}
		return static_cast<int>(records.size());
	}
	bool PositionDetailTable::Update(PositionDetail* const oldRecord, PositionDetail* const newRecord, bool updateDB)
	{
		std::lock_guard guard(sharedMutex);
		if (!primaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for PositionDetail:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New PositionDetail:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		bool TradeMatchIndexUpdate = tradeMatchIndex->NeedUpdate(oldRecord, newRecord);
		PositionDetailIndexTradeMatch::iterator itTradeMatch;
		if (TradeMatchIndexUpdate)
		{
			itTradeMatch = tradeMatchIndex->FindNode(oldRecord);
		}
		bool TradingDayIndexUpdate = tradingDayIndex->NeedUpdate(oldRecord, newRecord);
		PositionDetailIndexTradingDay::iterator itTradingDay;
		if (TradingDayIndexUpdate)
		{
			itTradingDay = tradingDayIndex->FindNode(oldRecord);
		}
		::memcpy(oldRecord, newRecord, sizeof(PositionDetail));
		if (TradeMatchIndexUpdate)
		{
			tradeMatchIndex->Update(itTradeMatch);
		}
		if (TradingDayIndexUpdate)
		{
			tradingDayIndex->Update(itTradingDay);
		}

		if (updateDB && mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordUpdate(PositionDetail::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void PositionDetailTable::TruncateTables()
	{
		std::lock_guard guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		primaryKey->index.clear();
		tradeMatchIndex->index.clear();
		tradingDayIndex->index.clear();
	}
	void PositionDetailTable::TruncateTable()
	{
		std::lock_guard guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		primaryKey->index.clear();
		tradeMatchIndex->index.clear();
		tradingDayIndex->index.clear();
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordTruncate(PositionDetail::TableID);
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
		std::shared_lock guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
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
		primaryKey->Erase(record);
	}
	void PositionDetailTable::EraseIndex(PositionDetail* record)
	{
		tradeMatchIndex->Erase(record);
		tradingDayIndex->Erase(record);
	}

	OrderTable::OrderTable()
	{
		mdbSubscriber = nullptr;
		primaryKey = new OrderPrimaryKey(this);
		clientOrderIDUniqueKey = new OrderUniqueKeyClientOrderID(this);
	}
	OrderTable::~OrderTable()
	{
		delete primaryKey;
		primaryKey = nullptr;
		delete clientOrderIDUniqueKey;
		clientOrderIDUniqueKey = nullptr;
	}
	void OrderTable::Subscribe(MdbSubscriber* subscriber)
	{
		mdbSubscriber = subscriber;
	}
	void OrderTable::UnSubscribe()
	{
		mdbSubscriber = nullptr;
	}
	void OrderTable::LockShared()
	{
		sharedMutex.lock_shared();
	}
	void OrderTable::UnlockShared()
	{
		sharedMutex.unlock_shared();
	}
	void OrderTable::InitDB()
	{
		if (mdbSubscriber == nullptr)
		{
			dbInited = true;
			return;
		}
		mdbSubscriber->OnRecordTruncate(Order::TableID);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(sharedMutex);
			for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
			{
				auto record = Order::Allocate();
				memcpy(record, *it, sizeof(Order));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber->OnRecordBatchInsert(Order::TableID, records);
		}
		else
		{
			delete records;
		}
		dbInited = true;
	}
	bool OrderTable::Insert(Order* record)
	{
		std::lock_guard guard(sharedMutex);
		if (!primaryKey->CheckInsert(record) || !(clientOrderIDUniqueKey->CheckInsert(record)))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for Order:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		primaryKey->Insert(record);
		clientOrderIDUniqueKey->Insert(record);

		
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordInsert(Order::TableID, record);
		}
		return true;
	}
	void OrderTable::BatchInsert(std::vector<mdb::Order*>* records)
	{
		{
			std::lock_guard guard(sharedMutex);
			for (auto record : *records)
			{
				auto newRecord = Order::Allocate();
				memcpy(newRecord, record, sizeof(Order));
				primaryKey->Insert(newRecord);
				clientOrderIDUniqueKey->Insert(newRecord);

			}
		}
		if (mdbSubscriber != nullptr && dbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber->OnRecordBatchInsert(Order::TableID, dbRecords);
		}
		delete records;
	}
	void OrderTable::Erase(Order* record)
	{
		std::lock_guard guard(sharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordErase(Order::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	bool OrderTable::Update(Order* const oldRecord, Order* const newRecord, bool updateDB)
	{
		std::lock_guard guard(sharedMutex);
		if (!primaryKey->CheckUpdate(oldRecord, newRecord) || !(clientOrderIDUniqueKey->CheckUpdate(oldRecord, newRecord)))
		{
			WriteLog(LogLevel::Warning, "Update Failed for Order:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New Order:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		::memcpy(oldRecord, newRecord, sizeof(Order));

		if (updateDB && mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordUpdate(Order::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void OrderTable::TruncateTables()
	{
		std::lock_guard guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		primaryKey->index.clear();
		clientOrderIDUniqueKey->index.clear();
	}
	void OrderTable::TruncateTable()
	{
		std::lock_guard guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		primaryKey->index.clear();
		clientOrderIDUniqueKey->index.clear();
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordTruncate(Order::TableID);
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
		std::shared_lock guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
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
		primaryKey->Erase(record);
		clientOrderIDUniqueKey->Erase(record);
	}
	void OrderTable::EraseIndex(Order* record)
	{
	}

	TradeTable::TradeTable()
	{
		mdbSubscriber = nullptr;
		primaryKey = new TradePrimaryKey(this);
	}
	TradeTable::~TradeTable()
	{
		delete primaryKey;
		primaryKey = nullptr;
	}
	void TradeTable::Subscribe(MdbSubscriber* subscriber)
	{
		mdbSubscriber = subscriber;
	}
	void TradeTable::UnSubscribe()
	{
		mdbSubscriber = nullptr;
	}
	void TradeTable::LockShared()
	{
		sharedMutex.lock_shared();
	}
	void TradeTable::UnlockShared()
	{
		sharedMutex.unlock_shared();
	}
	void TradeTable::InitDB()
	{
		if (mdbSubscriber == nullptr)
		{
			dbInited = true;
			return;
		}
		mdbSubscriber->OnRecordTruncate(Trade::TableID);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(sharedMutex);
			for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
			{
				auto record = Trade::Allocate();
				memcpy(record, *it, sizeof(Trade));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber->OnRecordBatchInsert(Trade::TableID, records);
		}
		else
		{
			delete records;
		}
		dbInited = true;
	}
	bool TradeTable::Insert(Trade* record)
	{
		std::lock_guard guard(sharedMutex);
		if (!primaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for Trade:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		primaryKey->Insert(record);

		
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordInsert(Trade::TableID, record);
		}
		return true;
	}
	void TradeTable::BatchInsert(std::vector<mdb::Trade*>* records)
	{
		{
			std::lock_guard guard(sharedMutex);
			for (auto record : *records)
			{
				auto newRecord = Trade::Allocate();
				memcpy(newRecord, record, sizeof(Trade));
				primaryKey->Insert(newRecord);

			}
		}
		if (mdbSubscriber != nullptr && dbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber->OnRecordBatchInsert(Trade::TableID, dbRecords);
		}
		delete records;
	}
	void TradeTable::Erase(Trade* record)
	{
		std::lock_guard guard(sharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordErase(Trade::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	bool TradeTable::Update(Trade* const oldRecord, Trade* const newRecord, bool updateDB)
	{
		std::lock_guard guard(sharedMutex);
		if (!primaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for Trade:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New Trade:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		::memcpy(oldRecord, newRecord, sizeof(Trade));

		if (updateDB && mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordUpdate(Trade::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void TradeTable::TruncateTables()
	{
		std::lock_guard guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		primaryKey->index.clear();
	}
	void TradeTable::TruncateTable()
	{
		std::lock_guard guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		primaryKey->index.clear();
		if (mdbSubscriber != nullptr && dbInited)
		{
			mdbSubscriber->OnRecordTruncate(Trade::TableID);
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
		std::shared_lock guard(sharedMutex);
		for (auto it = primaryKey->index.begin(); it != primaryKey->index.end(); ++it)
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
		primaryKey->Erase(record);
	}
	void TradeTable::EraseIndex(Trade* record)
	{
	}

}

