// 本文件由 ../Templates/Cpp/Mdb/MdbTables.cpp.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#include "MdbTables.h"
#include <Spark/Core/Logger/Logger.h>
#include <string>
#include <cstring>
#include <set>
#include <vector>

using std::string;
using std::set;
using namespace Spark::Core;

namespace Mdb
{
	TradingDayTable::TradingDayTable()
	{
		mdbSubscriber_ = nullptr;
		PrimaryKey = new TradingDayPrimaryKey(this);
	}
	TradingDayTable::~TradingDayTable()
	{
		delete PrimaryKey;
		PrimaryKey = nullptr;
	}
	void TradingDayTable::Subscribe(MdbSubscriber* subscriber)
	{
		mdbSubscriber_ = subscriber;
	}
	void TradingDayTable::UnSubscribe()
	{
		mdbSubscriber_ = nullptr;
	}
	void TradingDayTable::LockShared()
	{
		SharedMutex.lock_shared();
	}
	void TradingDayTable::UnlockShared()
	{
		SharedMutex.unlock_shared();
	}
	void TradingDayTable::InitDB()
	{
		if (mdbSubscriber_ == nullptr)
		{
			dbInited = true;
			return;
		}
		mdbSubscriber_->OnRecordTruncate(TradingDay::TableID);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
			{
				auto record = TradingDay::Allocate();
				memcpy(record, *it, sizeof(TradingDay));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber_->OnRecordBatchInsert(TradingDay::TableID, records);
		}
		else
		{
			delete records;
		}
		dbInited = true;
	}
	bool TradingDayTable::Insert(TradingDay* record)
	{
		std::lock_guard guard(SharedMutex);
		if (!PrimaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for TradingDay:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		PrimaryKey->Insert(record);

		
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordInsert(TradingDay::TableID, record);
		}
		return true;
	}
	void TradingDayTable::BatchInsert(std::vector<mdb::TradingDay*>* records)
	{
		{
			std::lock_guard guard(SharedMutex);
			for (auto record : *records)
			{
				auto newRecord = TradingDay::Allocate();
				memcpy(newRecord, record, sizeof(TradingDay));
				PrimaryKey->Insert(newRecord);

			}
		}
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber_->OnRecordBatchInsert(TradingDay::TableID, dbRecords);
		}
		delete records;
	}
	void TradingDayTable::Erase(TradingDay* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordErase(TradingDay::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	bool TradingDayTable::Update(TradingDay* const oldRecord, TradingDay* const newRecord, bool updateDB)
	{
		std::lock_guard guard(SharedMutex);
		if (!PrimaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for TradingDay:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New TradingDay:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		::memcpy(oldRecord, newRecord, sizeof(TradingDay));

		if (updateDB && mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordUpdate(TradingDay::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void TradingDayTable::TruncateTables()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index.clear();
	}
	void TradingDayTable::TruncateTable()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index.clear();
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordTruncate(TradingDay::TableID);
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
		std::shared_lock guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
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
		PrimaryKey->Erase(record);
	}
	void TradingDayTable::EraseIndex(TradingDay* record)
	{
	}

	ExchangeTable::ExchangeTable()
	{
		mdbSubscriber_ = nullptr;
		PrimaryKey = new ExchangePrimaryKey(this);
	}
	ExchangeTable::~ExchangeTable()
	{
		delete PrimaryKey;
		PrimaryKey = nullptr;
	}
	void ExchangeTable::Subscribe(MdbSubscriber* subscriber)
	{
		mdbSubscriber_ = subscriber;
	}
	void ExchangeTable::UnSubscribe()
	{
		mdbSubscriber_ = nullptr;
	}
	void ExchangeTable::LockShared()
	{
		SharedMutex.lock_shared();
	}
	void ExchangeTable::UnlockShared()
	{
		SharedMutex.unlock_shared();
	}
	void ExchangeTable::InitDB()
	{
		if (mdbSubscriber_ == nullptr)
		{
			dbInited = true;
			return;
		}
		mdbSubscriber_->OnRecordTruncate(Exchange::TableID);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
			{
				auto record = Exchange::Allocate();
				memcpy(record, *it, sizeof(Exchange));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber_->OnRecordBatchInsert(Exchange::TableID, records);
		}
		else
		{
			delete records;
		}
		dbInited = true;
	}
	bool ExchangeTable::Insert(Exchange* record)
	{
		std::lock_guard guard(SharedMutex);
		if (!PrimaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for Exchange:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		PrimaryKey->Insert(record);

		
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordInsert(Exchange::TableID, record);
		}
		return true;
	}
	void ExchangeTable::BatchInsert(std::vector<mdb::Exchange*>* records)
	{
		{
			std::lock_guard guard(SharedMutex);
			for (auto record : *records)
			{
				auto newRecord = Exchange::Allocate();
				memcpy(newRecord, record, sizeof(Exchange));
				PrimaryKey->Insert(newRecord);

			}
		}
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber_->OnRecordBatchInsert(Exchange::TableID, dbRecords);
		}
		delete records;
	}
	void ExchangeTable::Erase(Exchange* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordErase(Exchange::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	bool ExchangeTable::Update(Exchange* const oldRecord, Exchange* const newRecord, bool updateDB)
	{
		std::lock_guard guard(SharedMutex);
		if (!PrimaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for Exchange:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New Exchange:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		::memcpy(oldRecord, newRecord, sizeof(Exchange));

		if (updateDB && mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordUpdate(Exchange::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void ExchangeTable::TruncateTables()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index.clear();
	}
	void ExchangeTable::TruncateTable()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index.clear();
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordTruncate(Exchange::TableID);
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

		fprintf(dumpFile, "ExchangeId,ExchangeName\n");
		char buff[4096] = { 0 };
		set<Exchange*, ExchangeLessForExchangePrimaryKey> records;
		std::shared_lock guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
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
		PrimaryKey->Erase(record);
	}
	void ExchangeTable::EraseIndex(Exchange* record)
	{
	}

	ProductTable::ProductTable()
	{
		mdbSubscriber_ = nullptr;
		PrimaryKey = new ProductPrimaryKey(this);
	}
	ProductTable::~ProductTable()
	{
		delete PrimaryKey;
		PrimaryKey = nullptr;
	}
	void ProductTable::Subscribe(MdbSubscriber* subscriber)
	{
		mdbSubscriber_ = subscriber;
	}
	void ProductTable::UnSubscribe()
	{
		mdbSubscriber_ = nullptr;
	}
	void ProductTable::LockShared()
	{
		SharedMutex.lock_shared();
	}
	void ProductTable::UnlockShared()
	{
		SharedMutex.unlock_shared();
	}
	void ProductTable::InitDB()
	{
		if (mdbSubscriber_ == nullptr)
		{
			dbInited = true;
			return;
		}
		mdbSubscriber_->OnRecordTruncate(Product::TableID);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
			{
				auto record = Product::Allocate();
				memcpy(record, *it, sizeof(Product));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber_->OnRecordBatchInsert(Product::TableID, records);
		}
		else
		{
			delete records;
		}
		dbInited = true;
	}
	bool ProductTable::Insert(Product* record)
	{
		std::lock_guard guard(SharedMutex);
		if (!PrimaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for Product:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		PrimaryKey->Insert(record);

		
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordInsert(Product::TableID, record);
		}
		return true;
	}
	void ProductTable::BatchInsert(std::vector<mdb::Product*>* records)
	{
		{
			std::lock_guard guard(SharedMutex);
			for (auto record : *records)
			{
				auto newRecord = Product::Allocate();
				memcpy(newRecord, record, sizeof(Product));
				PrimaryKey->Insert(newRecord);

			}
		}
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber_->OnRecordBatchInsert(Product::TableID, dbRecords);
		}
		delete records;
	}
	void ProductTable::Erase(Product* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordErase(Product::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	bool ProductTable::Update(Product* const oldRecord, Product* const newRecord, bool updateDB)
	{
		std::lock_guard guard(SharedMutex);
		if (!PrimaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for Product:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New Product:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		::memcpy(oldRecord, newRecord, sizeof(Product));

		if (updateDB && mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordUpdate(Product::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void ProductTable::TruncateTables()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index.clear();
	}
	void ProductTable::TruncateTable()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index.clear();
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordTruncate(Product::TableID);
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

		fprintf(dumpFile, "ExchangeId,ProductId,ProductName,ProductClass,VolumeMultiple,PriceTick,MaxMarketOrderVolume,MinMarketOrderVolume,MaxLimitOrderVolume,MinLimitOrderVolume,SessionName\n");
		char buff[4096] = { 0 };
		set<Product*, ProductLessForProductPrimaryKey> records;
		std::shared_lock guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
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
		PrimaryKey->Erase(record);
	}
	void ProductTable::EraseIndex(Product* record)
	{
	}

	InstrumentTable::InstrumentTable()
	{
		mdbSubscriber_ = nullptr;
		PrimaryKey = new InstrumentPrimaryKey(this);
	}
	InstrumentTable::~InstrumentTable()
	{
		delete PrimaryKey;
		PrimaryKey = nullptr;
	}
	void InstrumentTable::Subscribe(MdbSubscriber* subscriber)
	{
		mdbSubscriber_ = subscriber;
	}
	void InstrumentTable::UnSubscribe()
	{
		mdbSubscriber_ = nullptr;
	}
	void InstrumentTable::LockShared()
	{
		SharedMutex.lock_shared();
	}
	void InstrumentTable::UnlockShared()
	{
		SharedMutex.unlock_shared();
	}
	void InstrumentTable::InitDB()
	{
		if (mdbSubscriber_ == nullptr)
		{
			dbInited = true;
			return;
		}
		mdbSubscriber_->OnRecordTruncate(Instrument::TableID);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
			{
				auto record = Instrument::Allocate();
				memcpy(record, *it, sizeof(Instrument));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber_->OnRecordBatchInsert(Instrument::TableID, records);
		}
		else
		{
			delete records;
		}
		dbInited = true;
	}
	bool InstrumentTable::Insert(Instrument* record)
	{
		std::lock_guard guard(SharedMutex);
		if (!PrimaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for Instrument:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		PrimaryKey->Insert(record);

		
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordInsert(Instrument::TableID, record);
		}
		return true;
	}
	void InstrumentTable::BatchInsert(std::vector<mdb::Instrument*>* records)
	{
		{
			std::lock_guard guard(SharedMutex);
			for (auto record : *records)
			{
				auto newRecord = Instrument::Allocate();
				memcpy(newRecord, record, sizeof(Instrument));
				PrimaryKey->Insert(newRecord);

			}
		}
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber_->OnRecordBatchInsert(Instrument::TableID, dbRecords);
		}
		delete records;
	}
	void InstrumentTable::Erase(Instrument* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordErase(Instrument::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	bool InstrumentTable::Update(Instrument* const oldRecord, Instrument* const newRecord, bool updateDB)
	{
		std::lock_guard guard(SharedMutex);
		if (!PrimaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for Instrument:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New Instrument:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		::memcpy(oldRecord, newRecord, sizeof(Instrument));

		if (updateDB && mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordUpdate(Instrument::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void InstrumentTable::TruncateTables()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index.clear();
	}
	void InstrumentTable::TruncateTable()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index.clear();
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordTruncate(Instrument::TableID);
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

		fprintf(dumpFile, "ExchangeId,InstrumentId,ExchangeInstId,InstrumentName,ProductId,ProductClass,InstrumentClass,Rank,VolumeMultiple,PriceTick,MaxMarketOrderVolume,MinMarketOrderVolume,MaxLimitOrderVolume,MinLimitOrderVolume,SessionName\n");
		char buff[4096] = { 0 };
		set<Instrument*, InstrumentLessForInstrumentPrimaryKey> records;
		std::shared_lock guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
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
		PrimaryKey->Erase(record);
	}
	void InstrumentTable::EraseIndex(Instrument* record)
	{
	}

	PrimaryAccountTable::PrimaryAccountTable()
	{
		mdbSubscriber_ = nullptr;
		PrimaryKey = new PrimaryAccountPrimaryKey(this);
		OfferIdIndex = new PrimaryAccountIndexOfferId(this);
	}
	PrimaryAccountTable::~PrimaryAccountTable()
	{
		delete PrimaryKey;
		PrimaryKey = nullptr;
		delete OfferIdIndex;
		OfferIdIndex = nullptr;
	}
	void PrimaryAccountTable::Subscribe(MdbSubscriber* subscriber)
	{
		mdbSubscriber_ = subscriber;
	}
	void PrimaryAccountTable::UnSubscribe()
	{
		mdbSubscriber_ = nullptr;
	}
	void PrimaryAccountTable::LockShared()
	{
		SharedMutex.lock_shared();
	}
	void PrimaryAccountTable::UnlockShared()
	{
		SharedMutex.unlock_shared();
	}
	void PrimaryAccountTable::InitDB()
	{
		if (mdbSubscriber_ == nullptr)
		{
			dbInited = true;
			return;
		}
		mdbSubscriber_->OnRecordTruncate(PrimaryAccount::TableID);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
			{
				auto record = PrimaryAccount::Allocate();
				memcpy(record, *it, sizeof(PrimaryAccount));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber_->OnRecordBatchInsert(PrimaryAccount::TableID, records);
		}
		else
		{
			delete records;
		}
		dbInited = true;
	}
	bool PrimaryAccountTable::Insert(PrimaryAccount* record)
	{
		std::lock_guard guard(SharedMutex);
		if (!PrimaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for PrimaryAccount:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		PrimaryKey->Insert(record);

		OfferIdIndex->Insert(record);
		
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordInsert(PrimaryAccount::TableID, record);
		}
		return true;
	}
	void PrimaryAccountTable::BatchInsert(std::vector<mdb::PrimaryAccount*>* records)
	{
		{
			std::lock_guard guard(SharedMutex);
			for (auto record : *records)
			{
				auto newRecord = PrimaryAccount::Allocate();
				memcpy(newRecord, record, sizeof(PrimaryAccount));
				PrimaryKey->Insert(newRecord);

				OfferIdIndex->Insert(newRecord);
			}
		}
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber_->OnRecordBatchInsert(PrimaryAccount::TableID, dbRecords);
		}
		delete records;
	}
	void PrimaryAccountTable::Erase(PrimaryAccount* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordErase(PrimaryAccount::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	int PrimaryAccountTable::EraseByOfferIdIndex(const OfferIdType& OfferId)
	{
		OfferIdIndex->FillCompareRecord(OfferId);
		std::vector<PrimaryAccount*> records;
		std::lock_guard guard(SharedMutex);
		auto range = OfferIdIndex->index.equal_range(&ComparePrimaryAccount);
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
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			auto record = PrimaryAccount::Allocate();
			memcpy(record, &ComparePrimaryAccount, sizeof(PrimaryAccount));
			mdbSubscriber_->OnRecordEraseByIndex(PrimaryAccount::TableID, PrimaryAccountIndexOfferId::IndexID, record);
		}
		return static_cast<int>(records.size());
	}
	bool PrimaryAccountTable::Update(PrimaryAccount* const oldRecord, PrimaryAccount* const newRecord, bool updateDB)
	{
		std::lock_guard guard(SharedMutex);
		if (!PrimaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for PrimaryAccount:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New PrimaryAccount:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		bool OfferIdIndexUpdate = OfferIdIndex->NeedUpdate(oldRecord, newRecord);
		PrimaryAccountIndexOfferId::iterator itOfferId;
		if (OfferIdIndexUpdate)
		{
			itOfferId = OfferIdIndex->FindNode(oldRecord);
		}
		::memcpy(oldRecord, newRecord, sizeof(PrimaryAccount));
		if (OfferIdIndexUpdate)
		{
			OfferIdIndex->Update(itOfferId);
		}

		if (updateDB && mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordUpdate(PrimaryAccount::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void PrimaryAccountTable::TruncateTables()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index.clear();
		OfferIdIndex->index.clear();
	}
	void PrimaryAccountTable::TruncateTable()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index.clear();
		OfferIdIndex->index.clear();
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordTruncate(PrimaryAccount::TableID);
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

		fprintf(dumpFile, "PrimaryAccountId,PrimaryAccountName,AccountClass,BrokerPassword,OfferId,IsAllowLogin,IsSimulateAccount,LoginStatus,InitStatus\n");
		char buff[4096] = { 0 };
		set<PrimaryAccount*, PrimaryAccountLessForPrimaryAccountPrimaryKey> records;
		std::shared_lock guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
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
		PrimaryKey->Erase(record);
	}
	void PrimaryAccountTable::EraseIndex(PrimaryAccount* record)
	{
		OfferIdIndex->Erase(record);
	}

	AccountTable::AccountTable()
	{
		mdbSubscriber_ = nullptr;
		PrimaryKey = new AccountPrimaryKey(this);
	}
	AccountTable::~AccountTable()
	{
		delete PrimaryKey;
		PrimaryKey = nullptr;
	}
	void AccountTable::Subscribe(MdbSubscriber* subscriber)
	{
		mdbSubscriber_ = subscriber;
	}
	void AccountTable::UnSubscribe()
	{
		mdbSubscriber_ = nullptr;
	}
	void AccountTable::LockShared()
	{
		SharedMutex.lock_shared();
	}
	void AccountTable::UnlockShared()
	{
		SharedMutex.unlock_shared();
	}
	void AccountTable::InitDB()
	{
		if (mdbSubscriber_ == nullptr)
		{
			dbInited = true;
			return;
		}
		mdbSubscriber_->OnRecordTruncate(Account::TableID);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
			{
				auto record = Account::Allocate();
				memcpy(record, *it, sizeof(Account));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber_->OnRecordBatchInsert(Account::TableID, records);
		}
		else
		{
			delete records;
		}
		dbInited = true;
	}
	bool AccountTable::Insert(Account* record)
	{
		std::lock_guard guard(SharedMutex);
		if (!PrimaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for Account:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		PrimaryKey->Insert(record);

		
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordInsert(Account::TableID, record);
		}
		return true;
	}
	void AccountTable::BatchInsert(std::vector<mdb::Account*>* records)
	{
		{
			std::lock_guard guard(SharedMutex);
			for (auto record : *records)
			{
				auto newRecord = Account::Allocate();
				memcpy(newRecord, record, sizeof(Account));
				PrimaryKey->Insert(newRecord);

			}
		}
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber_->OnRecordBatchInsert(Account::TableID, dbRecords);
		}
		delete records;
	}
	void AccountTable::Erase(Account* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordErase(Account::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	bool AccountTable::Update(Account* const oldRecord, Account* const newRecord, bool updateDB)
	{
		std::lock_guard guard(SharedMutex);
		if (!PrimaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for Account:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New Account:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		::memcpy(oldRecord, newRecord, sizeof(Account));

		if (updateDB && mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordUpdate(Account::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void AccountTable::TruncateTables()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index.clear();
	}
	void AccountTable::TruncateTable()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index.clear();
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordTruncate(Account::TableID);
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

		fprintf(dumpFile, "AccountId,AccountName,AccountType,AccountStatus,Password,TradeGroupId,RiskGroupId,CommissionGroupId\n");
		char buff[4096] = { 0 };
		set<Account*, AccountLessForAccountPrimaryKey> records;
		std::shared_lock guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
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
		PrimaryKey->Erase(record);
	}
	void AccountTable::EraseIndex(Account* record)
	{
	}

	CapitalTable::CapitalTable()
	{
		mdbSubscriber_ = nullptr;
		PrimaryKey = new CapitalPrimaryKey(this);
		TradingDayIndex = new CapitalIndexTradingDay(this);
	}
	CapitalTable::~CapitalTable()
	{
		delete PrimaryKey;
		PrimaryKey = nullptr;
		delete TradingDayIndex;
		TradingDayIndex = nullptr;
	}
	void CapitalTable::Subscribe(MdbSubscriber* subscriber)
	{
		mdbSubscriber_ = subscriber;
	}
	void CapitalTable::UnSubscribe()
	{
		mdbSubscriber_ = nullptr;
	}
	void CapitalTable::LockShared()
	{
		SharedMutex.lock_shared();
	}
	void CapitalTable::UnlockShared()
	{
		SharedMutex.unlock_shared();
	}
	void CapitalTable::InitDB()
	{
		if (mdbSubscriber_ == nullptr)
		{
			dbInited = true;
			return;
		}
		mdbSubscriber_->OnRecordTruncate(Capital::TableID);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
			{
				auto record = Capital::Allocate();
				memcpy(record, *it, sizeof(Capital));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber_->OnRecordBatchInsert(Capital::TableID, records);
		}
		else
		{
			delete records;
		}
		dbInited = true;
	}
	bool CapitalTable::Insert(Capital* record)
	{
		std::lock_guard guard(SharedMutex);
		if (!PrimaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for Capital:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		PrimaryKey->Insert(record);

		TradingDayIndex->Insert(record);
		
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordInsert(Capital::TableID, record);
		}
		return true;
	}
	void CapitalTable::BatchInsert(std::vector<mdb::Capital*>* records)
	{
		{
			std::lock_guard guard(SharedMutex);
			for (auto record : *records)
			{
				auto newRecord = Capital::Allocate();
				memcpy(newRecord, record, sizeof(Capital));
				PrimaryKey->Insert(newRecord);

				TradingDayIndex->Insert(newRecord);
			}
		}
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber_->OnRecordBatchInsert(Capital::TableID, dbRecords);
		}
		delete records;
	}
	void CapitalTable::Erase(Capital* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordErase(Capital::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	int CapitalTable::EraseByTradingDayIndex(const DateType& TradingDay)
	{
		TradingDayIndex->FillCompareRecord(TradingDay);
		std::vector<Capital*> records;
		std::lock_guard guard(SharedMutex);
		auto range = TradingDayIndex->index.equal_range(&CompareCapital);
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
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			auto record = Capital::Allocate();
			memcpy(record, &CompareCapital, sizeof(Capital));
			mdbSubscriber_->OnRecordEraseByIndex(Capital::TableID, CapitalIndexTradingDay::IndexID, record);
		}
		return static_cast<int>(records.size());
	}
	bool CapitalTable::Update(Capital* const oldRecord, Capital* const newRecord, bool updateDB)
	{
		std::lock_guard guard(SharedMutex);
		if (!PrimaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for Capital:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New Capital:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		bool TradingDayIndexUpdate = TradingDayIndex->NeedUpdate(oldRecord, newRecord);
		CapitalIndexTradingDay::iterator itTradingDay;
		if (TradingDayIndexUpdate)
		{
			itTradingDay = TradingDayIndex->FindNode(oldRecord);
		}
		::memcpy(oldRecord, newRecord, sizeof(Capital));
		if (TradingDayIndexUpdate)
		{
			TradingDayIndex->Update(itTradingDay);
		}

		if (updateDB && mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordUpdate(Capital::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void CapitalTable::TruncateTables()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index.clear();
		TradingDayIndex->index.clear();
	}
	void CapitalTable::TruncateTable()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index.clear();
		TradingDayIndex->index.clear();
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordTruncate(Capital::TableID);
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

		fprintf(dumpFile, "TradingDay,AccountId,AccountType,Balance,PreBalance,Available,MarketValue,CashIn,CashOut,Margin,Commission,FrozenCash,FrozenMargin,FrozenCommission,CloseProfitByDate,CloseProfitByTrade,PositionProfitByDate,PositionProfitByTrade,Deposit,Withdraw\n");
		char buff[4096] = { 0 };
		set<Capital*, CapitalLessForCapitalPrimaryKey> records;
		std::shared_lock guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
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
		PrimaryKey->Erase(record);
	}
	void CapitalTable::EraseIndex(Capital* record)
	{
		TradingDayIndex->Erase(record);
	}

	PositionTable::PositionTable()
	{
		mdbSubscriber_ = nullptr;
		PrimaryKey = new PositionPrimaryKey(this);
		AccountIndex = new PositionIndexAccount(this);
		TradingDayIndex = new PositionIndexTradingDay(this);
	}
	PositionTable::~PositionTable()
	{
		delete PrimaryKey;
		PrimaryKey = nullptr;
		delete AccountIndex;
		AccountIndex = nullptr;
		delete TradingDayIndex;
		TradingDayIndex = nullptr;
	}
	void PositionTable::Subscribe(MdbSubscriber* subscriber)
	{
		mdbSubscriber_ = subscriber;
	}
	void PositionTable::UnSubscribe()
	{
		mdbSubscriber_ = nullptr;
	}
	void PositionTable::LockShared()
	{
		SharedMutex.lock_shared();
	}
	void PositionTable::UnlockShared()
	{
		SharedMutex.unlock_shared();
	}
	void PositionTable::InitDB()
	{
		if (mdbSubscriber_ == nullptr)
		{
			dbInited = true;
			return;
		}
		mdbSubscriber_->OnRecordTruncate(Position::TableID);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
			{
				auto record = Position::Allocate();
				memcpy(record, *it, sizeof(Position));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber_->OnRecordBatchInsert(Position::TableID, records);
		}
		else
		{
			delete records;
		}
		dbInited = true;
	}
	bool PositionTable::Insert(Position* record)
	{
		std::lock_guard guard(SharedMutex);
		if (!PrimaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for Position:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		PrimaryKey->Insert(record);

		AccountIndex->Insert(record);
		TradingDayIndex->Insert(record);
		
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordInsert(Position::TableID, record);
		}
		return true;
	}
	void PositionTable::BatchInsert(std::vector<mdb::Position*>* records)
	{
		{
			std::lock_guard guard(SharedMutex);
			for (auto record : *records)
			{
				auto newRecord = Position::Allocate();
				memcpy(newRecord, record, sizeof(Position));
				PrimaryKey->Insert(newRecord);

				AccountIndex->Insert(newRecord);
				TradingDayIndex->Insert(newRecord);
			}
		}
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber_->OnRecordBatchInsert(Position::TableID, dbRecords);
		}
		delete records;
	}
	void PositionTable::Erase(Position* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordErase(Position::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	int PositionTable::EraseByAccountIndex(const DateType& TradingDay, const AccountIdType& AccountId)
	{
		AccountIndex->FillCompareRecord(TradingDay, AccountId);
		std::vector<Position*> records;
		std::lock_guard guard(SharedMutex);
		auto range = AccountIndex->index.equal_range(&ComparePosition);
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
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			auto record = Position::Allocate();
			memcpy(record, &ComparePosition, sizeof(Position));
			mdbSubscriber_->OnRecordEraseByIndex(Position::TableID, PositionIndexAccount::IndexID, record);
		}
		return static_cast<int>(records.size());
	}
	int PositionTable::EraseByTradingDayIndex(const DateType& TradingDay)
	{
		TradingDayIndex->FillCompareRecord(TradingDay);
		std::vector<Position*> records;
		std::lock_guard guard(SharedMutex);
		auto range = TradingDayIndex->index.equal_range(&ComparePosition);
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
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			auto record = Position::Allocate();
			memcpy(record, &ComparePosition, sizeof(Position));
			mdbSubscriber_->OnRecordEraseByIndex(Position::TableID, PositionIndexTradingDay::IndexID, record);
		}
		return static_cast<int>(records.size());
	}
	bool PositionTable::Update(Position* const oldRecord, Position* const newRecord, bool updateDB)
	{
		std::lock_guard guard(SharedMutex);
		if (!PrimaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for Position:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New Position:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		bool AccountIndexUpdate = AccountIndex->NeedUpdate(oldRecord, newRecord);
		PositionIndexAccount::iterator itAccount;
		if (AccountIndexUpdate)
		{
			itAccount = AccountIndex->FindNode(oldRecord);
		}
		bool TradingDayIndexUpdate = TradingDayIndex->NeedUpdate(oldRecord, newRecord);
		PositionIndexTradingDay::iterator itTradingDay;
		if (TradingDayIndexUpdate)
		{
			itTradingDay = TradingDayIndex->FindNode(oldRecord);
		}
		::memcpy(oldRecord, newRecord, sizeof(Position));
		if (AccountIndexUpdate)
		{
			AccountIndex->Update(itAccount);
		}
		if (TradingDayIndexUpdate)
		{
			TradingDayIndex->Update(itTradingDay);
		}

		if (updateDB && mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordUpdate(Position::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void PositionTable::TruncateTables()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index.clear();
		AccountIndex->index.clear();
		TradingDayIndex->index.clear();
	}
	void PositionTable::TruncateTable()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index.clear();
		AccountIndex->index.clear();
		TradingDayIndex->index.clear();
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordTruncate(Position::TableID);
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

		fprintf(dumpFile, "TradingDay,AccountId,AccountType,ExchangeId,InstrumentId,ProductClass,PosiDirection,TotalPosition,PositionFrozen,TodayPosition,MarketValue,CashIn,CashOut,Margin,Commission,FrozenCash,FrozenMargin,FrozenCommission,VolumeMultiple,CloseProfitByDate,CloseProfitByTrade,PositionProfitByDate,PositionProfitByTrade,SettlementPrice,PreSettlementPrice\n");
		char buff[4096] = { 0 };
		set<Position*, PositionLessForPositionPrimaryKey> records;
		std::shared_lock guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
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
		PrimaryKey->Erase(record);
	}
	void PositionTable::EraseIndex(Position* record)
	{
		AccountIndex->Erase(record);
		TradingDayIndex->Erase(record);
	}

	PositionDetailTable::PositionDetailTable()
	{
		mdbSubscriber_ = nullptr;
		PrimaryKey = new PositionDetailPrimaryKey(this);
		TradeMatchIndex = new PositionDetailIndexTradeMatch(this);
		TradingDayIndex = new PositionDetailIndexTradingDay(this);
	}
	PositionDetailTable::~PositionDetailTable()
	{
		delete PrimaryKey;
		PrimaryKey = nullptr;
		delete TradeMatchIndex;
		TradeMatchIndex = nullptr;
		delete TradingDayIndex;
		TradingDayIndex = nullptr;
	}
	void PositionDetailTable::Subscribe(MdbSubscriber* subscriber)
	{
		mdbSubscriber_ = subscriber;
	}
	void PositionDetailTable::UnSubscribe()
	{
		mdbSubscriber_ = nullptr;
	}
	void PositionDetailTable::LockShared()
	{
		SharedMutex.lock_shared();
	}
	void PositionDetailTable::UnlockShared()
	{
		SharedMutex.unlock_shared();
	}
	void PositionDetailTable::InitDB()
	{
		if (mdbSubscriber_ == nullptr)
		{
			dbInited = true;
			return;
		}
		mdbSubscriber_->OnRecordTruncate(PositionDetail::TableID);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
			{
				auto record = PositionDetail::Allocate();
				memcpy(record, *it, sizeof(PositionDetail));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber_->OnRecordBatchInsert(PositionDetail::TableID, records);
		}
		else
		{
			delete records;
		}
		dbInited = true;
	}
	bool PositionDetailTable::Insert(PositionDetail* record)
	{
		std::lock_guard guard(SharedMutex);
		if (!PrimaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for PositionDetail:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		PrimaryKey->Insert(record);

		TradeMatchIndex->Insert(record);
		TradingDayIndex->Insert(record);
		
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordInsert(PositionDetail::TableID, record);
		}
		return true;
	}
	void PositionDetailTable::BatchInsert(std::vector<mdb::PositionDetail*>* records)
	{
		{
			std::lock_guard guard(SharedMutex);
			for (auto record : *records)
			{
				auto newRecord = PositionDetail::Allocate();
				memcpy(newRecord, record, sizeof(PositionDetail));
				PrimaryKey->Insert(newRecord);

				TradeMatchIndex->Insert(newRecord);
				TradingDayIndex->Insert(newRecord);
			}
		}
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber_->OnRecordBatchInsert(PositionDetail::TableID, dbRecords);
		}
		delete records;
	}
	void PositionDetailTable::Erase(PositionDetail* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordErase(PositionDetail::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	int PositionDetailTable::EraseByTradeMatchIndex(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const PosiDirectionType& PosiDirection)
	{
		TradeMatchIndex->FillCompareRecord(TradingDay, AccountId, ExchangeId, InstrumentId, PosiDirection);
		std::vector<PositionDetail*> records;
		std::lock_guard guard(SharedMutex);
		auto range = TradeMatchIndex->index.equal_range(&ComparePositionDetail);
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
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			auto record = PositionDetail::Allocate();
			memcpy(record, &ComparePositionDetail, sizeof(PositionDetail));
			mdbSubscriber_->OnRecordEraseByIndex(PositionDetail::TableID, PositionDetailIndexTradeMatch::IndexID, record);
		}
		return static_cast<int>(records.size());
	}
	int PositionDetailTable::EraseByTradingDayIndex(const DateType& TradingDay)
	{
		TradingDayIndex->FillCompareRecord(TradingDay);
		std::vector<PositionDetail*> records;
		std::lock_guard guard(SharedMutex);
		auto range = TradingDayIndex->index.equal_range(&ComparePositionDetail);
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
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			auto record = PositionDetail::Allocate();
			memcpy(record, &ComparePositionDetail, sizeof(PositionDetail));
			mdbSubscriber_->OnRecordEraseByIndex(PositionDetail::TableID, PositionDetailIndexTradingDay::IndexID, record);
		}
		return static_cast<int>(records.size());
	}
	bool PositionDetailTable::Update(PositionDetail* const oldRecord, PositionDetail* const newRecord, bool updateDB)
	{
		std::lock_guard guard(SharedMutex);
		if (!PrimaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for PositionDetail:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New PositionDetail:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		bool TradeMatchIndexUpdate = TradeMatchIndex->NeedUpdate(oldRecord, newRecord);
		PositionDetailIndexTradeMatch::iterator itTradeMatch;
		if (TradeMatchIndexUpdate)
		{
			itTradeMatch = TradeMatchIndex->FindNode(oldRecord);
		}
		bool TradingDayIndexUpdate = TradingDayIndex->NeedUpdate(oldRecord, newRecord);
		PositionDetailIndexTradingDay::iterator itTradingDay;
		if (TradingDayIndexUpdate)
		{
			itTradingDay = TradingDayIndex->FindNode(oldRecord);
		}
		::memcpy(oldRecord, newRecord, sizeof(PositionDetail));
		if (TradeMatchIndexUpdate)
		{
			TradeMatchIndex->Update(itTradeMatch);
		}
		if (TradingDayIndexUpdate)
		{
			TradingDayIndex->Update(itTradingDay);
		}

		if (updateDB && mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordUpdate(PositionDetail::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void PositionDetailTable::TruncateTables()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index.clear();
		TradeMatchIndex->index.clear();
		TradingDayIndex->index.clear();
	}
	void PositionDetailTable::TruncateTable()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index.clear();
		TradeMatchIndex->index.clear();
		TradingDayIndex->index.clear();
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordTruncate(PositionDetail::TableID);
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

		fprintf(dumpFile, "TradingDay,AccountId,AccountType,ExchangeId,InstrumentId,ProductClass,PosiDirection,OpenDate,TradeId,Volume,OpenPrice,MarketValue,CashIn,CashOut,Margin,Commission,VolumeMultiple,CloseProfitByDate,CloseProfitByTrade,PositionProfitByDate,PositionProfitByTrade,SettlementPrice,PreSettlementPrice,CloseVolume,CloseAmount\n");
		char buff[4096] = { 0 };
		set<PositionDetail*, PositionDetailLessForPositionDetailPrimaryKey> records;
		std::shared_lock guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
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
		PrimaryKey->Erase(record);
	}
	void PositionDetailTable::EraseIndex(PositionDetail* record)
	{
		TradeMatchIndex->Erase(record);
		TradingDayIndex->Erase(record);
	}

	OrderTable::OrderTable()
	{
		mdbSubscriber_ = nullptr;
		PrimaryKey = new OrderPrimaryKey(this);
		ClientOrderIdUniqueKey = new OrderUniqueKeyClientOrderId(this);
	}
	OrderTable::~OrderTable()
	{
		delete PrimaryKey;
		PrimaryKey = nullptr;
		delete ClientOrderIdUniqueKey;
		ClientOrderIdUniqueKey = nullptr;
	}
	void OrderTable::Subscribe(MdbSubscriber* subscriber)
	{
		mdbSubscriber_ = subscriber;
	}
	void OrderTable::UnSubscribe()
	{
		mdbSubscriber_ = nullptr;
	}
	void OrderTable::LockShared()
	{
		SharedMutex.lock_shared();
	}
	void OrderTable::UnlockShared()
	{
		SharedMutex.unlock_shared();
	}
	void OrderTable::InitDB()
	{
		if (mdbSubscriber_ == nullptr)
		{
			dbInited = true;
			return;
		}
		mdbSubscriber_->OnRecordTruncate(Order::TableID);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
			{
				auto record = Order::Allocate();
				memcpy(record, *it, sizeof(Order));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber_->OnRecordBatchInsert(Order::TableID, records);
		}
		else
		{
			delete records;
		}
		dbInited = true;
	}
	bool OrderTable::Insert(Order* record)
	{
		std::lock_guard guard(SharedMutex);
		if (!PrimaryKey->CheckInsert(record) || !(ClientOrderIdUniqueKey->CheckInsert(record)))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for Order:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		PrimaryKey->Insert(record);
		ClientOrderIdUniqueKey->Insert(record);

		
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordInsert(Order::TableID, record);
		}
		return true;
	}
	void OrderTable::BatchInsert(std::vector<mdb::Order*>* records)
	{
		{
			std::lock_guard guard(SharedMutex);
			for (auto record : *records)
			{
				auto newRecord = Order::Allocate();
				memcpy(newRecord, record, sizeof(Order));
				PrimaryKey->Insert(newRecord);
				ClientOrderIdUniqueKey->Insert(newRecord);

			}
		}
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber_->OnRecordBatchInsert(Order::TableID, dbRecords);
		}
		delete records;
	}
	void OrderTable::Erase(Order* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordErase(Order::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	bool OrderTable::Update(Order* const oldRecord, Order* const newRecord, bool updateDB)
	{
		std::lock_guard guard(SharedMutex);
		if (!PrimaryKey->CheckUpdate(oldRecord, newRecord) || !(ClientOrderIdUniqueKey->CheckUpdate(oldRecord, newRecord)))
		{
			WriteLog(LogLevel::Warning, "Update Failed for Order:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New Order:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		::memcpy(oldRecord, newRecord, sizeof(Order));

		if (updateDB && mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordUpdate(Order::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void OrderTable::TruncateTables()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index.clear();
		ClientOrderIdUniqueKey->index.clear();
	}
	void OrderTable::TruncateTable()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index.clear();
		ClientOrderIdUniqueKey->index.clear();
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordTruncate(Order::TableID);
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

		fprintf(dumpFile, "TradingDay,AccountId,AccountType,ExchangeId,InstrumentId,ProductClass,OrderId,OrderSysId,Direction,OffsetFlag,OrderPriceType,Price,Volume,VolumeTotal,VolumeTraded,VolumeMultiple,OrderStatus,OrderDate,OrderTime,CancelDate,CancelTime,SessionId,ClientOrderId,RequestId,OfferId,TradeGroupId,RiskGroupId,CommissionGroupId,FrozenCash,FrozenMargin,FrozenCommission,RebuildMark,IsForceClose\n");
		char buff[4096] = { 0 };
		set<Order*, OrderLessForOrderPrimaryKey> records;
		std::shared_lock guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
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
		PrimaryKey->Erase(record);
		ClientOrderIdUniqueKey->Erase(record);
	}
	void OrderTable::EraseIndex(Order* record)
	{
	}

	TradeTable::TradeTable()
	{
		mdbSubscriber_ = nullptr;
		PrimaryKey = new TradePrimaryKey(this);
	}
	TradeTable::~TradeTable()
	{
		delete PrimaryKey;
		PrimaryKey = nullptr;
	}
	void TradeTable::Subscribe(MdbSubscriber* subscriber)
	{
		mdbSubscriber_ = subscriber;
	}
	void TradeTable::UnSubscribe()
	{
		mdbSubscriber_ = nullptr;
	}
	void TradeTable::LockShared()
	{
		SharedMutex.lock_shared();
	}
	void TradeTable::UnlockShared()
	{
		SharedMutex.unlock_shared();
	}
	void TradeTable::InitDB()
	{
		if (mdbSubscriber_ == nullptr)
		{
			dbInited = true;
			return;
		}
		mdbSubscriber_->OnRecordTruncate(Trade::TableID);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
			{
				auto record = Trade::Allocate();
				memcpy(record, *it, sizeof(Trade));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber_->OnRecordBatchInsert(Trade::TableID, records);
		}
		else
		{
			delete records;
		}
		dbInited = true;
	}
	bool TradeTable::Insert(Trade* record)
	{
		std::lock_guard guard(SharedMutex);
		if (!PrimaryKey->CheckInsert(record))
		{
			WriteLog(LogLevel::Warning, "Insert Failed for Trade:[%s]", record->GetString());
			record->Deallocate();
			return false;
		}

		PrimaryKey->Insert(record);

		
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordInsert(Trade::TableID, record);
		}
		return true;
	}
	void TradeTable::BatchInsert(std::vector<mdb::Trade*>* records)
	{
		{
			std::lock_guard guard(SharedMutex);
			for (auto record : *records)
			{
				auto newRecord = Trade::Allocate();
				memcpy(newRecord, record, sizeof(Trade));
				PrimaryKey->Insert(newRecord);

			}
		}
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber_->OnRecordBatchInsert(Trade::TableID, dbRecords);
		}
		delete records;
	}
	void TradeTable::Erase(Trade* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordErase(Trade::TableID, record);
		}
		else
		{
			record->Deallocate();
		}
	}
	bool TradeTable::Update(Trade* const oldRecord, Trade* const newRecord, bool updateDB)
	{
		std::lock_guard guard(SharedMutex);
		if (!PrimaryKey->CheckUpdate(oldRecord, newRecord))
		{
			WriteLog(LogLevel::Warning, "Update Failed for Trade:[%s]", oldRecord->GetString());
			WriteLog(LogLevel::Warning, "              New Trade:[%s]", newRecord->GetString());
			newRecord->Deallocate();
			return false;
		}

		::memcpy(oldRecord, newRecord, sizeof(Trade));

		if (updateDB && mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordUpdate(Trade::TableID, newRecord);
		}
		else
		{
			newRecord->Deallocate();
		}
		return true;
	}
	void TradeTable::TruncateTables()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index.clear();
	}
	void TradeTable::TruncateTable()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index.clear();
		if (mdbSubscriber_ != nullptr && dbInited)
		{
			mdbSubscriber_->OnRecordTruncate(Trade::TableID);
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

		fprintf(dumpFile, "TradingDay,AccountId,AccountType,ExchangeId,InstrumentId,ProductClass,OrderId,OrderSysId,TradeId,Direction,OffsetFlag,Price,Volume,VolumeMultiple,TradeAmount,Commission,TradeDate,TradeTime\n");
		char buff[4096] = { 0 };
		set<Trade*, TradeLessForTradePrimaryKey> records;
		std::shared_lock guard(SharedMutex);
		for (auto it = PrimaryKey->index.begin(); it != PrimaryKey->index.end(); ++it)
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
		PrimaryKey->Erase(record);
	}
	void TradeTable::EraseIndex(Trade* record)
	{
	}

}

