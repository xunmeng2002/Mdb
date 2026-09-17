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
	void TradingDayTable::InitDb()
	{
		if (mdbSubscriber_ == nullptr)
		{
			DbInited = true;
			return;
		}
		mdbSubscriber_->OnRecordTruncate(TradingDay::TableId);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				auto record = TradingDay::Allocate();
				memcpy(record, *it, sizeof(TradingDay));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber_->OnRecordBatchInsert(TradingDay::TableId, records);
		}
		else
		{
			delete records;
		}
		DbInited = true;
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

		
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordInsert(TradingDay::TableId, record);
		}
		return true;
	}
	void TradingDayTable::BatchInsert(std::vector<TradingDay*>* records)
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
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber_->OnRecordBatchInsert(TradingDay::TableId, dbRecords);
		}
		delete records;
	}
	void TradingDayTable::Erase(TradingDay* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordErase(TradingDay::TableId, record);
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

		if (updateDB && mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordUpdate(TradingDay::TableId, newRecord);
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
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index_.clear();
	}
	void TradingDayTable::TruncateTable()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index_.clear();
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordTruncate(TradingDay::TableId);
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

		fprintf(dumpFile, "Pk,CurrTradingDay,PreTradingDay\n");
		char buff[4096] = { 0 };
		set<TradingDay*, TradingDayLessForTradingDayPrimaryKey> records;
		std::shared_lock guard(SharedMutex);
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
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
	void ExchangeTable::InitDb()
	{
		if (mdbSubscriber_ == nullptr)
		{
			DbInited = true;
			return;
		}
		mdbSubscriber_->OnRecordTruncate(Exchange::TableId);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				auto record = Exchange::Allocate();
				memcpy(record, *it, sizeof(Exchange));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber_->OnRecordBatchInsert(Exchange::TableId, records);
		}
		else
		{
			delete records;
		}
		DbInited = true;
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

		
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordInsert(Exchange::TableId, record);
		}
		return true;
	}
	void ExchangeTable::BatchInsert(std::vector<Exchange*>* records)
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
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber_->OnRecordBatchInsert(Exchange::TableId, dbRecords);
		}
		delete records;
	}
	void ExchangeTable::Erase(Exchange* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordErase(Exchange::TableId, record);
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

		if (updateDB && mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordUpdate(Exchange::TableId, newRecord);
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
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index_.clear();
	}
	void ExchangeTable::TruncateTable()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index_.clear();
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordTruncate(Exchange::TableId);
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
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
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
	void ProductTable::InitDb()
	{
		if (mdbSubscriber_ == nullptr)
		{
			DbInited = true;
			return;
		}
		mdbSubscriber_->OnRecordTruncate(Product::TableId);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				auto record = Product::Allocate();
				memcpy(record, *it, sizeof(Product));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber_->OnRecordBatchInsert(Product::TableId, records);
		}
		else
		{
			delete records;
		}
		DbInited = true;
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

		
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordInsert(Product::TableId, record);
		}
		return true;
	}
	void ProductTable::BatchInsert(std::vector<Product*>* records)
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
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber_->OnRecordBatchInsert(Product::TableId, dbRecords);
		}
		delete records;
	}
	void ProductTable::Erase(Product* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordErase(Product::TableId, record);
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

		if (updateDB && mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordUpdate(Product::TableId, newRecord);
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
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index_.clear();
	}
	void ProductTable::TruncateTable()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index_.clear();
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordTruncate(Product::TableId);
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
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
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
	void InstrumentTable::InitDb()
	{
		if (mdbSubscriber_ == nullptr)
		{
			DbInited = true;
			return;
		}
		mdbSubscriber_->OnRecordTruncate(Instrument::TableId);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				auto record = Instrument::Allocate();
				memcpy(record, *it, sizeof(Instrument));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber_->OnRecordBatchInsert(Instrument::TableId, records);
		}
		else
		{
			delete records;
		}
		DbInited = true;
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

		
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordInsert(Instrument::TableId, record);
		}
		return true;
	}
	void InstrumentTable::BatchInsert(std::vector<Instrument*>* records)
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
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber_->OnRecordBatchInsert(Instrument::TableId, dbRecords);
		}
		delete records;
	}
	void InstrumentTable::Erase(Instrument* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordErase(Instrument::TableId, record);
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

		if (updateDB && mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordUpdate(Instrument::TableId, newRecord);
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
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index_.clear();
	}
	void InstrumentTable::TruncateTable()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index_.clear();
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordTruncate(Instrument::TableId);
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
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
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
	void PrimaryAccountTable::InitDb()
	{
		if (mdbSubscriber_ == nullptr)
		{
			DbInited = true;
			return;
		}
		mdbSubscriber_->OnRecordTruncate(PrimaryAccount::TableId);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				auto record = PrimaryAccount::Allocate();
				memcpy(record, *it, sizeof(PrimaryAccount));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber_->OnRecordBatchInsert(PrimaryAccount::TableId, records);
		}
		else
		{
			delete records;
		}
		DbInited = true;
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
		
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordInsert(PrimaryAccount::TableId, record);
		}
		return true;
	}
	void PrimaryAccountTable::BatchInsert(std::vector<PrimaryAccount*>* records)
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
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber_->OnRecordBatchInsert(PrimaryAccount::TableId, dbRecords);
		}
		delete records;
	}
	void PrimaryAccountTable::Erase(PrimaryAccount* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordErase(PrimaryAccount::TableId, record);
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
		auto range = OfferIdIndex->index_.equal_range(&ComparePrimaryAccount);
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
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			auto record = PrimaryAccount::Allocate();
			memcpy(record, &ComparePrimaryAccount, sizeof(PrimaryAccount));
			mdbSubscriber_->OnRecordEraseByIndex(PrimaryAccount::TableId, PrimaryAccountIndexOfferId::IndexID, record);
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

		if (updateDB && mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordUpdate(PrimaryAccount::TableId, newRecord);
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
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index_.clear();
		OfferIdIndex->index_.clear();
	}
	void PrimaryAccountTable::TruncateTable()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index_.clear();
		OfferIdIndex->index_.clear();
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordTruncate(PrimaryAccount::TableId);
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
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
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
	void AccountTable::InitDb()
	{
		if (mdbSubscriber_ == nullptr)
		{
			DbInited = true;
			return;
		}
		mdbSubscriber_->OnRecordTruncate(Account::TableId);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				auto record = Account::Allocate();
				memcpy(record, *it, sizeof(Account));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber_->OnRecordBatchInsert(Account::TableId, records);
		}
		else
		{
			delete records;
		}
		DbInited = true;
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

		
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordInsert(Account::TableId, record);
		}
		return true;
	}
	void AccountTable::BatchInsert(std::vector<Account*>* records)
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
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber_->OnRecordBatchInsert(Account::TableId, dbRecords);
		}
		delete records;
	}
	void AccountTable::Erase(Account* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordErase(Account::TableId, record);
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

		if (updateDB && mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordUpdate(Account::TableId, newRecord);
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
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index_.clear();
	}
	void AccountTable::TruncateTable()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index_.clear();
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordTruncate(Account::TableId);
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
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
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
	void CapitalTable::InitDb()
	{
		if (mdbSubscriber_ == nullptr)
		{
			DbInited = true;
			return;
		}
		mdbSubscriber_->OnRecordTruncate(Capital::TableId);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				auto record = Capital::Allocate();
				memcpy(record, *it, sizeof(Capital));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber_->OnRecordBatchInsert(Capital::TableId, records);
		}
		else
		{
			delete records;
		}
		DbInited = true;
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
		
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordInsert(Capital::TableId, record);
		}
		return true;
	}
	void CapitalTable::BatchInsert(std::vector<Capital*>* records)
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
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber_->OnRecordBatchInsert(Capital::TableId, dbRecords);
		}
		delete records;
	}
	void CapitalTable::Erase(Capital* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordErase(Capital::TableId, record);
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
		auto range = TradingDayIndex->index_.equal_range(&CompareCapital);
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
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			auto record = Capital::Allocate();
			memcpy(record, &CompareCapital, sizeof(Capital));
			mdbSubscriber_->OnRecordEraseByIndex(Capital::TableId, CapitalIndexTradingDay::IndexID, record);
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

		if (updateDB && mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordUpdate(Capital::TableId, newRecord);
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
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index_.clear();
		TradingDayIndex->index_.clear();
	}
	void CapitalTable::TruncateTable()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index_.clear();
		TradingDayIndex->index_.clear();
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordTruncate(Capital::TableId);
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
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
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
	void PositionTable::InitDb()
	{
		if (mdbSubscriber_ == nullptr)
		{
			DbInited = true;
			return;
		}
		mdbSubscriber_->OnRecordTruncate(Position::TableId);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				auto record = Position::Allocate();
				memcpy(record, *it, sizeof(Position));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber_->OnRecordBatchInsert(Position::TableId, records);
		}
		else
		{
			delete records;
		}
		DbInited = true;
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
		
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordInsert(Position::TableId, record);
		}
		return true;
	}
	void PositionTable::BatchInsert(std::vector<Position*>* records)
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
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber_->OnRecordBatchInsert(Position::TableId, dbRecords);
		}
		delete records;
	}
	void PositionTable::Erase(Position* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordErase(Position::TableId, record);
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
		auto range = AccountIndex->index_.equal_range(&ComparePosition);
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
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			auto record = Position::Allocate();
			memcpy(record, &ComparePosition, sizeof(Position));
			mdbSubscriber_->OnRecordEraseByIndex(Position::TableId, PositionIndexAccount::IndexID, record);
		}
		return static_cast<int>(records.size());
	}
	int PositionTable::EraseByTradingDayIndex(const DateType& TradingDay)
	{
		TradingDayIndex->FillCompareRecord(TradingDay);
		std::vector<Position*> records;
		std::lock_guard guard(SharedMutex);
		auto range = TradingDayIndex->index_.equal_range(&ComparePosition);
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
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			auto record = Position::Allocate();
			memcpy(record, &ComparePosition, sizeof(Position));
			mdbSubscriber_->OnRecordEraseByIndex(Position::TableId, PositionIndexTradingDay::IndexID, record);
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

		if (updateDB && mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordUpdate(Position::TableId, newRecord);
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
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index_.clear();
		AccountIndex->index_.clear();
		TradingDayIndex->index_.clear();
	}
	void PositionTable::TruncateTable()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index_.clear();
		AccountIndex->index_.clear();
		TradingDayIndex->index_.clear();
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordTruncate(Position::TableId);
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
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
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
	void PositionDetailTable::InitDb()
	{
		if (mdbSubscriber_ == nullptr)
		{
			DbInited = true;
			return;
		}
		mdbSubscriber_->OnRecordTruncate(PositionDetail::TableId);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				auto record = PositionDetail::Allocate();
				memcpy(record, *it, sizeof(PositionDetail));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber_->OnRecordBatchInsert(PositionDetail::TableId, records);
		}
		else
		{
			delete records;
		}
		DbInited = true;
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
		
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordInsert(PositionDetail::TableId, record);
		}
		return true;
	}
	void PositionDetailTable::BatchInsert(std::vector<PositionDetail*>* records)
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
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber_->OnRecordBatchInsert(PositionDetail::TableId, dbRecords);
		}
		delete records;
	}
	void PositionDetailTable::Erase(PositionDetail* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordErase(PositionDetail::TableId, record);
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
		auto range = TradeMatchIndex->index_.equal_range(&ComparePositionDetail);
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
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			auto record = PositionDetail::Allocate();
			memcpy(record, &ComparePositionDetail, sizeof(PositionDetail));
			mdbSubscriber_->OnRecordEraseByIndex(PositionDetail::TableId, PositionDetailIndexTradeMatch::IndexID, record);
		}
		return static_cast<int>(records.size());
	}
	int PositionDetailTable::EraseByTradingDayIndex(const DateType& TradingDay)
	{
		TradingDayIndex->FillCompareRecord(TradingDay);
		std::vector<PositionDetail*> records;
		std::lock_guard guard(SharedMutex);
		auto range = TradingDayIndex->index_.equal_range(&ComparePositionDetail);
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
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			auto record = PositionDetail::Allocate();
			memcpy(record, &ComparePositionDetail, sizeof(PositionDetail));
			mdbSubscriber_->OnRecordEraseByIndex(PositionDetail::TableId, PositionDetailIndexTradingDay::IndexID, record);
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

		if (updateDB && mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordUpdate(PositionDetail::TableId, newRecord);
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
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index_.clear();
		TradeMatchIndex->index_.clear();
		TradingDayIndex->index_.clear();
	}
	void PositionDetailTable::TruncateTable()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index_.clear();
		TradeMatchIndex->index_.clear();
		TradingDayIndex->index_.clear();
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordTruncate(PositionDetail::TableId);
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
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
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
	void OrderTable::InitDb()
	{
		if (mdbSubscriber_ == nullptr)
		{
			DbInited = true;
			return;
		}
		mdbSubscriber_->OnRecordTruncate(Order::TableId);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				auto record = Order::Allocate();
				memcpy(record, *it, sizeof(Order));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber_->OnRecordBatchInsert(Order::TableId, records);
		}
		else
		{
			delete records;
		}
		DbInited = true;
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

		
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordInsert(Order::TableId, record);
		}
		return true;
	}
	void OrderTable::BatchInsert(std::vector<Order*>* records)
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
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber_->OnRecordBatchInsert(Order::TableId, dbRecords);
		}
		delete records;
	}
	void OrderTable::Erase(Order* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordErase(Order::TableId, record);
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

		if (updateDB && mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordUpdate(Order::TableId, newRecord);
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
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index_.clear();
		ClientOrderIdUniqueKey->index_.clear();
	}
	void OrderTable::TruncateTable()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index_.clear();
		ClientOrderIdUniqueKey->index_.clear();
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordTruncate(Order::TableId);
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
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
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
	void TradeTable::InitDb()
	{
		if (mdbSubscriber_ == nullptr)
		{
			DbInited = true;
			return;
		}
		mdbSubscriber_->OnRecordTruncate(Trade::TableId);

		auto records = new std::vector<const void*>();
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				auto record = Trade::Allocate();
				memcpy(record, *it, sizeof(Trade));
				records->push_back(record);
			}
		}
		if (!records->empty())
		{
			mdbSubscriber_->OnRecordBatchInsert(Trade::TableId, records);
		}
		else
		{
			delete records;
		}
		DbInited = true;
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

		
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordInsert(Trade::TableId, record);
		}
		return true;
	}
	void TradeTable::BatchInsert(std::vector<Trade*>* records)
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
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			auto dbRecords = new std::vector<const void*>();
			dbRecords->reserve(records->size());
			for (auto* r : *records) dbRecords->push_back(r);
			mdbSubscriber_->OnRecordBatchInsert(Trade::TableId, dbRecords);
		}
		delete records;
	}
	void TradeTable::Erase(Trade* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordErase(Trade::TableId, record);
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

		if (updateDB && mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordUpdate(Trade::TableId, newRecord);
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
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index_.clear();
	}
	void TradeTable::TruncateTable()
	{
		std::lock_guard guard(SharedMutex);
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
		{
			(*it)->Deallocate();
		}
		PrimaryKey->index_.clear();
		if (mdbSubscriber_ != nullptr && DbInited)
		{
			mdbSubscriber_->OnRecordTruncate(Trade::TableId);
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
		for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
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

