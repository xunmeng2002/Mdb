// 本文件由 ../Templates/Cpp/Mdb/Rcu/PointerSwap/PointerSwapTables.cpp.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#include "Rcu/PointerSwap/PointerSwapTables.h"
#include <DbAdapters/DbInterface/RecordHandle.h>
#include <Spark/Core/Logger/Logger.h>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

using std::set;
using std::string;
using namespace Spark::Core;
using namespace DbAdapters;

namespace Mdb::Rcu::PointerSwap
{
	TradingDayTable::TradingDayTable()
	{
		PrimaryKey = new TradingDayTablePrimaryKey(this);
	}

	TradingDayTable::~TradingDayTable()
	{
		delete PrimaryKey;
		PrimaryKey = nullptr;
	}

	unsigned int TradingDayTable::GetTableId() const
	{
		return TradingDay::TableId;
	}

	void TradingDayTable::Subscribe(MdbSubscriber* subscriber)
	{
		if (subscriber == nullptr)
		{
			return;
		}
		subscribers_.push_back(subscriber);
	}

	void TradingDayTable::SubscribeReplica(MdbReplicaSubscriber* replica)
	{
		replicaSubscriber_ = replica;
	}

	void TradingDayTable::UnSubscribe()
	{
		subscribers_.clear();
		replicaSubscriber_ = nullptr;
	}

	void TradingDayTable::InitDb()
	{
		if (replicaSubscriber_ == nullptr)
		{
			return;
		}
		// 快照期间持有 SharedMutex 共享锁，写者与回收都被挡在外面
		std::lock_guard publishGuard(publishMutex_);
		std::shared_lock guard(SharedMutex);
		replicaSubscriber_->OnSnapshotBegin(TradingDay::TableId);
		std::vector<RowChangeEventPtr> events;
		PrimaryKey->ForEachRecord([this, &events](TradingDay* record)
		{
			events.push_back(MakeInsertEvent(record));
			return true;
		});
		if (!events.empty())
		{
			replicaSubscriber_->OnRowBatchChange(events);
		}
		replicaSubscriber_->OnSnapshotComplete(TradingDay::TableId);
	}

	bool TradingDayTable::Insert(TradingDay* record)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			if (!PrimaryKey->CheckInsert(record))
			{
				WriteLog(LogLevel::Warning, "Insert Failed for TradingDay:[%s]", record->GetString());
				record->Deallocate();
				return false;
			}
			PrimaryKey->Insert(record);
			event = BuildInsertEvent(record);
		}
		PublishRowChange(event);
		return true;
	}

	void TradingDayTable::BatchInsert(std::vector<TradingDay*>& records)
	{
		std::lock_guard publishGuard(publishMutex_);
		std::vector<RowChangeEventPtr> events;
		{
			std::lock_guard guard(SharedMutex);
			for (auto record : records)
			{
				[[maybe_unused]] const bool insertedIntoPrimaryKey = PrimaryKey->Insert(record);
				assert(insertedIntoPrimaryKey);
			}
			events = BuildBatchInsertEvents(records);
			records.clear();
		}
		PublishRowBatchChange(events);
	}

	bool TradingDayTable::Erase(const TradingDay& record)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			TradingDay* liveRecord = PrimaryKey->Find(&record);
			if (liveRecord == nullptr)
			{
				return false;
			}
			if (HasSubscribers())
			{
				// 离表的活记录要等读者走完才回收，事件拿到的是它的副本
				TradingDay* eventRecord = TradingDay::Allocate();
				std::memcpy(eventRecord, liveRecord, sizeof(TradingDay));
				event = BuildRowChangeEvent(RowChangeType::Erase, 0, true, AdoptRecord(eventRecord));
			}
			PrimaryKey->Erase(liveRecord);
		}
		PublishRowChange(event);
		return true;
	}

	bool TradingDayTable::Update(const TradingDay& newRecord, bool updateDB)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			TradingDay* liveRecord = PrimaryKey->Find(&newRecord);
			if (liveRecord == nullptr)
			{
				WriteLog(LogLevel::Warning, "Update Failed for TradingDay:[%s]", newRecord.GetString());
				return false;
			}
			PrimaryKey->Replace(liveRecord, newRecord);
			event = BuildUpdateEvent(newRecord, updateDB);
		}
		PublishRowChange(event);
		return true;
	}

	void TradingDayTable::TruncateTables()
	{
		std::lock_guard publishGuard(publishMutex_);
		std::lock_guard guard(SharedMutex);
		PrimaryKey->Clear();
	}

	void TradingDayTable::TruncateTable()
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			PrimaryKey->Clear();
			event = BuildRowChangeEvent(RowChangeType::Truncate, 0, true, RecordHandle());
		}
		PublishRowChange(event);
	}

	void TradingDayTable::Dump(const char* dir)
	{
		string fileName = string(dir) + "//TradingDay.csv";
		FILE* dumpFile = fopen(fileName.c_str(), "w");
		if (dumpFile == nullptr)
		{
			return;
		}

		fprintf(dumpFile, "Pk,CurrTradingDay,PreTradingDay\n");
		set<TradingDay*, TradingDayLessForTradingDayPrimaryKey> records;
		{
			std::shared_lock guard(SharedMutex);
			PrimaryKey->ForEachRecord([&records](TradingDay* record)
			{
				records.insert(record);
				return true;
			});
		}
		for (auto record : records)
		{
			fprintf(dumpFile, "%s\n", record->GetString());
		}
		records.clear();
		fclose(dumpFile);
	}

	bool TradingDayTable::HasSubscribers() const
	{
		return replicaSubscriber_ != nullptr || !subscribers_.empty();
	}

	RowChangeEventPtr TradingDayTable::MakeInsertEvent(const TradingDay* record)
	{
		auto eventRecord = TradingDay::Allocate();
		std::memcpy(eventRecord, record, sizeof(TradingDay));
		return std::make_shared<const RowChangeEvent>(TradingDay::TableId, RowChangeType::Insert, 0, ++sequence_, true, AdoptRecord(eventRecord));
	}

	RowChangeEventPtr TradingDayTable::BuildRowChangeEvent(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		return std::make_shared<const RowChangeEvent>(TradingDay::TableId, type, indexId, ++sequence_, persistDatabase, std::move(record));
	}

	RowChangeEventPtr TradingDayTable::BuildInsertEvent(const TradingDay* record)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		return MakeInsertEvent(record);
	}

	RowChangeEventPtr TradingDayTable::BuildUpdateEvent(const TradingDay& newRecord, bool updateDB)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		TradingDay* eventRecord = TradingDay::Allocate();
		std::memcpy(eventRecord, &newRecord, sizeof(TradingDay));
		// updateDB 只表示"这条变更不落库"，行变更照常广播，由副本订阅者自行忽略
		return std::make_shared<const RowChangeEvent>(
			TradingDay::TableId, RowChangeType::Update, 0, ++sequence_, updateDB, AdoptRecord(eventRecord));
	}

	std::vector<RowChangeEventPtr> TradingDayTable::BuildBatchInsertEvents(const std::vector<TradingDay*>& records)
	{
		std::vector<RowChangeEventPtr> events;
		if (!HasSubscribers())
		{
			return events;
		}
		events.reserve(records.size());
		for (const TradingDay* record : records)
		{
			events.push_back(MakeInsertEvent(record));
		}
		return events;
	}

	void TradingDayTable::PublishRowChange(const RowChangeEventPtr& event)
	{
		if (replicaSubscriber_ != nullptr)
		{
			replicaSubscriber_->OnRowChange(event);
		}
		for (MdbSubscriber* subscriber : subscribers_)
		{
			subscriber->OnRowChange(event);
		}
	}

	void TradingDayTable::PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events)
	{
		if (replicaSubscriber_ != nullptr)
		{
			replicaSubscriber_->OnRowBatchChange(events);
		}
		for (MdbSubscriber* subscriber : subscribers_)
		{
			subscriber->OnRowBatchChange(events);
		}
	}

	ExchangeTable::ExchangeTable()
	{
		PrimaryKey = new ExchangeTablePrimaryKey(this);
	}

	ExchangeTable::~ExchangeTable()
	{
		delete PrimaryKey;
		PrimaryKey = nullptr;
	}

	unsigned int ExchangeTable::GetTableId() const
	{
		return Exchange::TableId;
	}

	void ExchangeTable::Subscribe(MdbSubscriber* subscriber)
	{
		if (subscriber == nullptr)
		{
			return;
		}
		subscribers_.push_back(subscriber);
	}

	void ExchangeTable::SubscribeReplica(MdbReplicaSubscriber* replica)
	{
		replicaSubscriber_ = replica;
	}

	void ExchangeTable::UnSubscribe()
	{
		subscribers_.clear();
		replicaSubscriber_ = nullptr;
	}

	void ExchangeTable::InitDb()
	{
		if (replicaSubscriber_ == nullptr)
		{
			return;
		}
		// 快照期间持有 SharedMutex 共享锁，写者与回收都被挡在外面
		std::lock_guard publishGuard(publishMutex_);
		std::shared_lock guard(SharedMutex);
		replicaSubscriber_->OnSnapshotBegin(Exchange::TableId);
		std::vector<RowChangeEventPtr> events;
		PrimaryKey->ForEachRecord([this, &events](Exchange* record)
		{
			events.push_back(MakeInsertEvent(record));
			return true;
		});
		if (!events.empty())
		{
			replicaSubscriber_->OnRowBatchChange(events);
		}
		replicaSubscriber_->OnSnapshotComplete(Exchange::TableId);
	}

	bool ExchangeTable::Insert(Exchange* record)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			if (!PrimaryKey->CheckInsert(record))
			{
				WriteLog(LogLevel::Warning, "Insert Failed for Exchange:[%s]", record->GetString());
				record->Deallocate();
				return false;
			}
			PrimaryKey->Insert(record);
			event = BuildInsertEvent(record);
		}
		PublishRowChange(event);
		return true;
	}

	void ExchangeTable::BatchInsert(std::vector<Exchange*>& records)
	{
		std::lock_guard publishGuard(publishMutex_);
		std::vector<RowChangeEventPtr> events;
		{
			std::lock_guard guard(SharedMutex);
			for (auto record : records)
			{
				[[maybe_unused]] const bool insertedIntoPrimaryKey = PrimaryKey->Insert(record);
				assert(insertedIntoPrimaryKey);
			}
			events = BuildBatchInsertEvents(records);
			records.clear();
		}
		PublishRowBatchChange(events);
	}

	bool ExchangeTable::Erase(const Exchange& record)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			Exchange* liveRecord = PrimaryKey->Find(&record);
			if (liveRecord == nullptr)
			{
				return false;
			}
			if (HasSubscribers())
			{
				// 离表的活记录要等读者走完才回收，事件拿到的是它的副本
				Exchange* eventRecord = Exchange::Allocate();
				std::memcpy(eventRecord, liveRecord, sizeof(Exchange));
				event = BuildRowChangeEvent(RowChangeType::Erase, 0, true, AdoptRecord(eventRecord));
			}
			PrimaryKey->Erase(liveRecord);
		}
		PublishRowChange(event);
		return true;
	}

	bool ExchangeTable::Update(const Exchange& newRecord, bool updateDB)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			Exchange* liveRecord = PrimaryKey->Find(&newRecord);
			if (liveRecord == nullptr)
			{
				WriteLog(LogLevel::Warning, "Update Failed for Exchange:[%s]", newRecord.GetString());
				return false;
			}
			PrimaryKey->Replace(liveRecord, newRecord);
			event = BuildUpdateEvent(newRecord, updateDB);
		}
		PublishRowChange(event);
		return true;
	}

	void ExchangeTable::TruncateTables()
	{
		std::lock_guard publishGuard(publishMutex_);
		std::lock_guard guard(SharedMutex);
		PrimaryKey->Clear();
	}

	void ExchangeTable::TruncateTable()
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			PrimaryKey->Clear();
			event = BuildRowChangeEvent(RowChangeType::Truncate, 0, true, RecordHandle());
		}
		PublishRowChange(event);
	}

	void ExchangeTable::Dump(const char* dir)
	{
		string fileName = string(dir) + "//Exchange.csv";
		FILE* dumpFile = fopen(fileName.c_str(), "w");
		if (dumpFile == nullptr)
		{
			return;
		}

		fprintf(dumpFile, "ExchangeId,ExchangeName\n");
		set<Exchange*, ExchangeLessForExchangePrimaryKey> records;
		{
			std::shared_lock guard(SharedMutex);
			PrimaryKey->ForEachRecord([&records](Exchange* record)
			{
				records.insert(record);
				return true;
			});
		}
		for (auto record : records)
		{
			fprintf(dumpFile, "%s\n", record->GetString());
		}
		records.clear();
		fclose(dumpFile);
	}

	bool ExchangeTable::HasSubscribers() const
	{
		return replicaSubscriber_ != nullptr || !subscribers_.empty();
	}

	RowChangeEventPtr ExchangeTable::MakeInsertEvent(const Exchange* record)
	{
		auto eventRecord = Exchange::Allocate();
		std::memcpy(eventRecord, record, sizeof(Exchange));
		return std::make_shared<const RowChangeEvent>(Exchange::TableId, RowChangeType::Insert, 0, ++sequence_, true, AdoptRecord(eventRecord));
	}

	RowChangeEventPtr ExchangeTable::BuildRowChangeEvent(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		return std::make_shared<const RowChangeEvent>(Exchange::TableId, type, indexId, ++sequence_, persistDatabase, std::move(record));
	}

	RowChangeEventPtr ExchangeTable::BuildInsertEvent(const Exchange* record)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		return MakeInsertEvent(record);
	}

	RowChangeEventPtr ExchangeTable::BuildUpdateEvent(const Exchange& newRecord, bool updateDB)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		Exchange* eventRecord = Exchange::Allocate();
		std::memcpy(eventRecord, &newRecord, sizeof(Exchange));
		// updateDB 只表示"这条变更不落库"，行变更照常广播，由副本订阅者自行忽略
		return std::make_shared<const RowChangeEvent>(
			Exchange::TableId, RowChangeType::Update, 0, ++sequence_, updateDB, AdoptRecord(eventRecord));
	}

	std::vector<RowChangeEventPtr> ExchangeTable::BuildBatchInsertEvents(const std::vector<Exchange*>& records)
	{
		std::vector<RowChangeEventPtr> events;
		if (!HasSubscribers())
		{
			return events;
		}
		events.reserve(records.size());
		for (const Exchange* record : records)
		{
			events.push_back(MakeInsertEvent(record));
		}
		return events;
	}

	void ExchangeTable::PublishRowChange(const RowChangeEventPtr& event)
	{
		if (replicaSubscriber_ != nullptr)
		{
			replicaSubscriber_->OnRowChange(event);
		}
		for (MdbSubscriber* subscriber : subscribers_)
		{
			subscriber->OnRowChange(event);
		}
	}

	void ExchangeTable::PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events)
	{
		if (replicaSubscriber_ != nullptr)
		{
			replicaSubscriber_->OnRowBatchChange(events);
		}
		for (MdbSubscriber* subscriber : subscribers_)
		{
			subscriber->OnRowBatchChange(events);
		}
	}

	ProductTable::ProductTable()
	{
		PrimaryKey = new ProductTablePrimaryKey(this);
	}

	ProductTable::~ProductTable()
	{
		delete PrimaryKey;
		PrimaryKey = nullptr;
	}

	unsigned int ProductTable::GetTableId() const
	{
		return Product::TableId;
	}

	void ProductTable::Subscribe(MdbSubscriber* subscriber)
	{
		if (subscriber == nullptr)
		{
			return;
		}
		subscribers_.push_back(subscriber);
	}

	void ProductTable::SubscribeReplica(MdbReplicaSubscriber* replica)
	{
		replicaSubscriber_ = replica;
	}

	void ProductTable::UnSubscribe()
	{
		subscribers_.clear();
		replicaSubscriber_ = nullptr;
	}

	void ProductTable::InitDb()
	{
		if (replicaSubscriber_ == nullptr)
		{
			return;
		}
		// 快照期间持有 SharedMutex 共享锁，写者与回收都被挡在外面
		std::lock_guard publishGuard(publishMutex_);
		std::shared_lock guard(SharedMutex);
		replicaSubscriber_->OnSnapshotBegin(Product::TableId);
		std::vector<RowChangeEventPtr> events;
		PrimaryKey->ForEachRecord([this, &events](Product* record)
		{
			events.push_back(MakeInsertEvent(record));
			return true;
		});
		if (!events.empty())
		{
			replicaSubscriber_->OnRowBatchChange(events);
		}
		replicaSubscriber_->OnSnapshotComplete(Product::TableId);
	}

	bool ProductTable::Insert(Product* record)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			if (!PrimaryKey->CheckInsert(record))
			{
				WriteLog(LogLevel::Warning, "Insert Failed for Product:[%s]", record->GetString());
				record->Deallocate();
				return false;
			}
			PrimaryKey->Insert(record);
			event = BuildInsertEvent(record);
		}
		PublishRowChange(event);
		return true;
	}

	void ProductTable::BatchInsert(std::vector<Product*>& records)
	{
		std::lock_guard publishGuard(publishMutex_);
		std::vector<RowChangeEventPtr> events;
		{
			std::lock_guard guard(SharedMutex);
			for (auto record : records)
			{
				[[maybe_unused]] const bool insertedIntoPrimaryKey = PrimaryKey->Insert(record);
				assert(insertedIntoPrimaryKey);
			}
			events = BuildBatchInsertEvents(records);
			records.clear();
		}
		PublishRowBatchChange(events);
	}

	bool ProductTable::Erase(const Product& record)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			Product* liveRecord = PrimaryKey->Find(&record);
			if (liveRecord == nullptr)
			{
				return false;
			}
			if (HasSubscribers())
			{
				// 离表的活记录要等读者走完才回收，事件拿到的是它的副本
				Product* eventRecord = Product::Allocate();
				std::memcpy(eventRecord, liveRecord, sizeof(Product));
				event = BuildRowChangeEvent(RowChangeType::Erase, 0, true, AdoptRecord(eventRecord));
			}
			PrimaryKey->Erase(liveRecord);
		}
		PublishRowChange(event);
		return true;
	}

	bool ProductTable::Update(const Product& newRecord, bool updateDB)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			Product* liveRecord = PrimaryKey->Find(&newRecord);
			if (liveRecord == nullptr)
			{
				WriteLog(LogLevel::Warning, "Update Failed for Product:[%s]", newRecord.GetString());
				return false;
			}
			PrimaryKey->Replace(liveRecord, newRecord);
			event = BuildUpdateEvent(newRecord, updateDB);
		}
		PublishRowChange(event);
		return true;
	}

	void ProductTable::TruncateTables()
	{
		std::lock_guard publishGuard(publishMutex_);
		std::lock_guard guard(SharedMutex);
		PrimaryKey->Clear();
	}

	void ProductTable::TruncateTable()
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			PrimaryKey->Clear();
			event = BuildRowChangeEvent(RowChangeType::Truncate, 0, true, RecordHandle());
		}
		PublishRowChange(event);
	}

	void ProductTable::Dump(const char* dir)
	{
		string fileName = string(dir) + "//Product.csv";
		FILE* dumpFile = fopen(fileName.c_str(), "w");
		if (dumpFile == nullptr)
		{
			return;
		}

		fprintf(dumpFile, "ExchangeId,ProductId,ProductName,ProductClass,VolumeMultiple,PriceTick,MaxMarketOrderVolume,MinMarketOrderVolume,MaxLimitOrderVolume,MinLimitOrderVolume,SessionName\n");
		set<Product*, ProductLessForProductPrimaryKey> records;
		{
			std::shared_lock guard(SharedMutex);
			PrimaryKey->ForEachRecord([&records](Product* record)
			{
				records.insert(record);
				return true;
			});
		}
		for (auto record : records)
		{
			fprintf(dumpFile, "%s\n", record->GetString());
		}
		records.clear();
		fclose(dumpFile);
	}

	bool ProductTable::HasSubscribers() const
	{
		return replicaSubscriber_ != nullptr || !subscribers_.empty();
	}

	RowChangeEventPtr ProductTable::MakeInsertEvent(const Product* record)
	{
		auto eventRecord = Product::Allocate();
		std::memcpy(eventRecord, record, sizeof(Product));
		return std::make_shared<const RowChangeEvent>(Product::TableId, RowChangeType::Insert, 0, ++sequence_, true, AdoptRecord(eventRecord));
	}

	RowChangeEventPtr ProductTable::BuildRowChangeEvent(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		return std::make_shared<const RowChangeEvent>(Product::TableId, type, indexId, ++sequence_, persistDatabase, std::move(record));
	}

	RowChangeEventPtr ProductTable::BuildInsertEvent(const Product* record)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		return MakeInsertEvent(record);
	}

	RowChangeEventPtr ProductTable::BuildUpdateEvent(const Product& newRecord, bool updateDB)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		Product* eventRecord = Product::Allocate();
		std::memcpy(eventRecord, &newRecord, sizeof(Product));
		// updateDB 只表示"这条变更不落库"，行变更照常广播，由副本订阅者自行忽略
		return std::make_shared<const RowChangeEvent>(
			Product::TableId, RowChangeType::Update, 0, ++sequence_, updateDB, AdoptRecord(eventRecord));
	}

	std::vector<RowChangeEventPtr> ProductTable::BuildBatchInsertEvents(const std::vector<Product*>& records)
	{
		std::vector<RowChangeEventPtr> events;
		if (!HasSubscribers())
		{
			return events;
		}
		events.reserve(records.size());
		for (const Product* record : records)
		{
			events.push_back(MakeInsertEvent(record));
		}
		return events;
	}

	void ProductTable::PublishRowChange(const RowChangeEventPtr& event)
	{
		if (replicaSubscriber_ != nullptr)
		{
			replicaSubscriber_->OnRowChange(event);
		}
		for (MdbSubscriber* subscriber : subscribers_)
		{
			subscriber->OnRowChange(event);
		}
	}

	void ProductTable::PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events)
	{
		if (replicaSubscriber_ != nullptr)
		{
			replicaSubscriber_->OnRowBatchChange(events);
		}
		for (MdbSubscriber* subscriber : subscribers_)
		{
			subscriber->OnRowBatchChange(events);
		}
	}

	InstrumentTable::InstrumentTable()
	{
		PrimaryKey = new InstrumentTablePrimaryKey(this);
	}

	InstrumentTable::~InstrumentTable()
	{
		delete PrimaryKey;
		PrimaryKey = nullptr;
	}

	unsigned int InstrumentTable::GetTableId() const
	{
		return Instrument::TableId;
	}

	void InstrumentTable::Subscribe(MdbSubscriber* subscriber)
	{
		if (subscriber == nullptr)
		{
			return;
		}
		subscribers_.push_back(subscriber);
	}

	void InstrumentTable::SubscribeReplica(MdbReplicaSubscriber* replica)
	{
		replicaSubscriber_ = replica;
	}

	void InstrumentTable::UnSubscribe()
	{
		subscribers_.clear();
		replicaSubscriber_ = nullptr;
	}

	void InstrumentTable::InitDb()
	{
		if (replicaSubscriber_ == nullptr)
		{
			return;
		}
		// 快照期间持有 SharedMutex 共享锁，写者与回收都被挡在外面
		std::lock_guard publishGuard(publishMutex_);
		std::shared_lock guard(SharedMutex);
		replicaSubscriber_->OnSnapshotBegin(Instrument::TableId);
		std::vector<RowChangeEventPtr> events;
		PrimaryKey->ForEachRecord([this, &events](Instrument* record)
		{
			events.push_back(MakeInsertEvent(record));
			return true;
		});
		if (!events.empty())
		{
			replicaSubscriber_->OnRowBatchChange(events);
		}
		replicaSubscriber_->OnSnapshotComplete(Instrument::TableId);
	}

	bool InstrumentTable::Insert(Instrument* record)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			if (!PrimaryKey->CheckInsert(record))
			{
				WriteLog(LogLevel::Warning, "Insert Failed for Instrument:[%s]", record->GetString());
				record->Deallocate();
				return false;
			}
			PrimaryKey->Insert(record);
			event = BuildInsertEvent(record);
		}
		PublishRowChange(event);
		return true;
	}

	void InstrumentTable::BatchInsert(std::vector<Instrument*>& records)
	{
		std::lock_guard publishGuard(publishMutex_);
		std::vector<RowChangeEventPtr> events;
		{
			std::lock_guard guard(SharedMutex);
			for (auto record : records)
			{
				[[maybe_unused]] const bool insertedIntoPrimaryKey = PrimaryKey->Insert(record);
				assert(insertedIntoPrimaryKey);
			}
			events = BuildBatchInsertEvents(records);
			records.clear();
		}
		PublishRowBatchChange(events);
	}

	bool InstrumentTable::Erase(const Instrument& record)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			Instrument* liveRecord = PrimaryKey->Find(&record);
			if (liveRecord == nullptr)
			{
				return false;
			}
			if (HasSubscribers())
			{
				// 离表的活记录要等读者走完才回收，事件拿到的是它的副本
				Instrument* eventRecord = Instrument::Allocate();
				std::memcpy(eventRecord, liveRecord, sizeof(Instrument));
				event = BuildRowChangeEvent(RowChangeType::Erase, 0, true, AdoptRecord(eventRecord));
			}
			PrimaryKey->Erase(liveRecord);
		}
		PublishRowChange(event);
		return true;
	}

	bool InstrumentTable::Update(const Instrument& newRecord, bool updateDB)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			Instrument* liveRecord = PrimaryKey->Find(&newRecord);
			if (liveRecord == nullptr)
			{
				WriteLog(LogLevel::Warning, "Update Failed for Instrument:[%s]", newRecord.GetString());
				return false;
			}
			PrimaryKey->Replace(liveRecord, newRecord);
			event = BuildUpdateEvent(newRecord, updateDB);
		}
		PublishRowChange(event);
		return true;
	}

	void InstrumentTable::TruncateTables()
	{
		std::lock_guard publishGuard(publishMutex_);
		std::lock_guard guard(SharedMutex);
		PrimaryKey->Clear();
	}

	void InstrumentTable::TruncateTable()
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			PrimaryKey->Clear();
			event = BuildRowChangeEvent(RowChangeType::Truncate, 0, true, RecordHandle());
		}
		PublishRowChange(event);
	}

	void InstrumentTable::Dump(const char* dir)
	{
		string fileName = string(dir) + "//Instrument.csv";
		FILE* dumpFile = fopen(fileName.c_str(), "w");
		if (dumpFile == nullptr)
		{
			return;
		}

		fprintf(dumpFile, "ExchangeId,InstrumentId,ExchangeInstId,InstrumentName,ProductId,ProductClass,InstrumentClass,Rank,VolumeMultiple,PriceTick,MaxMarketOrderVolume,MinMarketOrderVolume,MaxLimitOrderVolume,MinLimitOrderVolume,SessionName\n");
		set<Instrument*, InstrumentLessForInstrumentPrimaryKey> records;
		{
			std::shared_lock guard(SharedMutex);
			PrimaryKey->ForEachRecord([&records](Instrument* record)
			{
				records.insert(record);
				return true;
			});
		}
		for (auto record : records)
		{
			fprintf(dumpFile, "%s\n", record->GetString());
		}
		records.clear();
		fclose(dumpFile);
	}

	bool InstrumentTable::HasSubscribers() const
	{
		return replicaSubscriber_ != nullptr || !subscribers_.empty();
	}

	RowChangeEventPtr InstrumentTable::MakeInsertEvent(const Instrument* record)
	{
		auto eventRecord = Instrument::Allocate();
		std::memcpy(eventRecord, record, sizeof(Instrument));
		return std::make_shared<const RowChangeEvent>(Instrument::TableId, RowChangeType::Insert, 0, ++sequence_, true, AdoptRecord(eventRecord));
	}

	RowChangeEventPtr InstrumentTable::BuildRowChangeEvent(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		return std::make_shared<const RowChangeEvent>(Instrument::TableId, type, indexId, ++sequence_, persistDatabase, std::move(record));
	}

	RowChangeEventPtr InstrumentTable::BuildInsertEvent(const Instrument* record)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		return MakeInsertEvent(record);
	}

	RowChangeEventPtr InstrumentTable::BuildUpdateEvent(const Instrument& newRecord, bool updateDB)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		Instrument* eventRecord = Instrument::Allocate();
		std::memcpy(eventRecord, &newRecord, sizeof(Instrument));
		// updateDB 只表示"这条变更不落库"，行变更照常广播，由副本订阅者自行忽略
		return std::make_shared<const RowChangeEvent>(
			Instrument::TableId, RowChangeType::Update, 0, ++sequence_, updateDB, AdoptRecord(eventRecord));
	}

	std::vector<RowChangeEventPtr> InstrumentTable::BuildBatchInsertEvents(const std::vector<Instrument*>& records)
	{
		std::vector<RowChangeEventPtr> events;
		if (!HasSubscribers())
		{
			return events;
		}
		events.reserve(records.size());
		for (const Instrument* record : records)
		{
			events.push_back(MakeInsertEvent(record));
		}
		return events;
	}

	void InstrumentTable::PublishRowChange(const RowChangeEventPtr& event)
	{
		if (replicaSubscriber_ != nullptr)
		{
			replicaSubscriber_->OnRowChange(event);
		}
		for (MdbSubscriber* subscriber : subscribers_)
		{
			subscriber->OnRowChange(event);
		}
	}

	void InstrumentTable::PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events)
	{
		if (replicaSubscriber_ != nullptr)
		{
			replicaSubscriber_->OnRowBatchChange(events);
		}
		for (MdbSubscriber* subscriber : subscribers_)
		{
			subscriber->OnRowBatchChange(events);
		}
	}

	PrimaryAccountTable::PrimaryAccountTable()
	{
		PrimaryKey = new PrimaryAccountTablePrimaryKey(this);
	}

	PrimaryAccountTable::~PrimaryAccountTable()
	{
		delete PrimaryKey;
		PrimaryKey = nullptr;
	}

	unsigned int PrimaryAccountTable::GetTableId() const
	{
		return PrimaryAccount::TableId;
	}

	void PrimaryAccountTable::Subscribe(MdbSubscriber* subscriber)
	{
		if (subscriber == nullptr)
		{
			return;
		}
		subscribers_.push_back(subscriber);
	}

	void PrimaryAccountTable::SubscribeReplica(MdbReplicaSubscriber* replica)
	{
		replicaSubscriber_ = replica;
	}

	void PrimaryAccountTable::UnSubscribe()
	{
		subscribers_.clear();
		replicaSubscriber_ = nullptr;
	}

	void PrimaryAccountTable::InitDb()
	{
		if (replicaSubscriber_ == nullptr)
		{
			return;
		}
		// 快照期间持有 SharedMutex 共享锁，写者与回收都被挡在外面
		std::lock_guard publishGuard(publishMutex_);
		std::shared_lock guard(SharedMutex);
		replicaSubscriber_->OnSnapshotBegin(PrimaryAccount::TableId);
		std::vector<RowChangeEventPtr> events;
		PrimaryKey->ForEachRecord([this, &events](PrimaryAccount* record)
		{
			events.push_back(MakeInsertEvent(record));
			return true;
		});
		if (!events.empty())
		{
			replicaSubscriber_->OnRowBatchChange(events);
		}
		replicaSubscriber_->OnSnapshotComplete(PrimaryAccount::TableId);
	}

	bool PrimaryAccountTable::Insert(PrimaryAccount* record)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			if (!PrimaryKey->CheckInsert(record))
			{
				WriteLog(LogLevel::Warning, "Insert Failed for PrimaryAccount:[%s]", record->GetString());
				record->Deallocate();
				return false;
			}
			PrimaryKey->Insert(record);
			event = BuildInsertEvent(record);
		}
		PublishRowChange(event);
		return true;
	}

	void PrimaryAccountTable::BatchInsert(std::vector<PrimaryAccount*>& records)
	{
		std::lock_guard publishGuard(publishMutex_);
		std::vector<RowChangeEventPtr> events;
		{
			std::lock_guard guard(SharedMutex);
			for (auto record : records)
			{
				[[maybe_unused]] const bool insertedIntoPrimaryKey = PrimaryKey->Insert(record);
				assert(insertedIntoPrimaryKey);
			}
			events = BuildBatchInsertEvents(records);
			records.clear();
		}
		PublishRowBatchChange(events);
	}

	bool PrimaryAccountTable::Erase(const PrimaryAccount& record)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			PrimaryAccount* liveRecord = PrimaryKey->Find(&record);
			if (liveRecord == nullptr)
			{
				return false;
			}
			if (HasSubscribers())
			{
				// 离表的活记录要等读者走完才回收，事件拿到的是它的副本
				PrimaryAccount* eventRecord = PrimaryAccount::Allocate();
				std::memcpy(eventRecord, liveRecord, sizeof(PrimaryAccount));
				event = BuildRowChangeEvent(RowChangeType::Erase, 0, true, AdoptRecord(eventRecord));
			}
			PrimaryKey->Erase(liveRecord);
		}
		PublishRowChange(event);
		return true;
	}

	bool PrimaryAccountTable::Update(const PrimaryAccount& newRecord, bool updateDB)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			PrimaryAccount* liveRecord = PrimaryKey->Find(&newRecord);
			if (liveRecord == nullptr)
			{
				WriteLog(LogLevel::Warning, "Update Failed for PrimaryAccount:[%s]", newRecord.GetString());
				return false;
			}
			PrimaryKey->Replace(liveRecord, newRecord);
			event = BuildUpdateEvent(newRecord, updateDB);
		}
		PublishRowChange(event);
		return true;
	}

	void PrimaryAccountTable::TruncateTables()
	{
		std::lock_guard publishGuard(publishMutex_);
		std::lock_guard guard(SharedMutex);
		PrimaryKey->Clear();
	}

	void PrimaryAccountTable::TruncateTable()
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			PrimaryKey->Clear();
			event = BuildRowChangeEvent(RowChangeType::Truncate, 0, true, RecordHandle());
		}
		PublishRowChange(event);
	}

	void PrimaryAccountTable::Dump(const char* dir)
	{
		string fileName = string(dir) + "//PrimaryAccount.csv";
		FILE* dumpFile = fopen(fileName.c_str(), "w");
		if (dumpFile == nullptr)
		{
			return;
		}

		fprintf(dumpFile, "PrimaryAccountId,PrimaryAccountName,AccountClass,BrokerPassword,OfferId,IsAllowLogin,IsSimulateAccount,LoginStatus,InitStatus\n");
		set<PrimaryAccount*, PrimaryAccountLessForPrimaryAccountPrimaryKey> records;
		{
			std::shared_lock guard(SharedMutex);
			PrimaryKey->ForEachRecord([&records](PrimaryAccount* record)
			{
				records.insert(record);
				return true;
			});
		}
		for (auto record : records)
		{
			fprintf(dumpFile, "%s\n", record->GetString());
		}
		records.clear();
		fclose(dumpFile);
	}

	bool PrimaryAccountTable::HasSubscribers() const
	{
		return replicaSubscriber_ != nullptr || !subscribers_.empty();
	}

	RowChangeEventPtr PrimaryAccountTable::MakeInsertEvent(const PrimaryAccount* record)
	{
		auto eventRecord = PrimaryAccount::Allocate();
		std::memcpy(eventRecord, record, sizeof(PrimaryAccount));
		return std::make_shared<const RowChangeEvent>(PrimaryAccount::TableId, RowChangeType::Insert, 0, ++sequence_, true, AdoptRecord(eventRecord));
	}

	RowChangeEventPtr PrimaryAccountTable::BuildRowChangeEvent(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		return std::make_shared<const RowChangeEvent>(PrimaryAccount::TableId, type, indexId, ++sequence_, persistDatabase, std::move(record));
	}

	RowChangeEventPtr PrimaryAccountTable::BuildInsertEvent(const PrimaryAccount* record)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		return MakeInsertEvent(record);
	}

	RowChangeEventPtr PrimaryAccountTable::BuildUpdateEvent(const PrimaryAccount& newRecord, bool updateDB)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		PrimaryAccount* eventRecord = PrimaryAccount::Allocate();
		std::memcpy(eventRecord, &newRecord, sizeof(PrimaryAccount));
		// updateDB 只表示"这条变更不落库"，行变更照常广播，由副本订阅者自行忽略
		return std::make_shared<const RowChangeEvent>(
			PrimaryAccount::TableId, RowChangeType::Update, 0, ++sequence_, updateDB, AdoptRecord(eventRecord));
	}

	std::vector<RowChangeEventPtr> PrimaryAccountTable::BuildBatchInsertEvents(const std::vector<PrimaryAccount*>& records)
	{
		std::vector<RowChangeEventPtr> events;
		if (!HasSubscribers())
		{
			return events;
		}
		events.reserve(records.size());
		for (const PrimaryAccount* record : records)
		{
			events.push_back(MakeInsertEvent(record));
		}
		return events;
	}

	void PrimaryAccountTable::PublishRowChange(const RowChangeEventPtr& event)
	{
		if (replicaSubscriber_ != nullptr)
		{
			replicaSubscriber_->OnRowChange(event);
		}
		for (MdbSubscriber* subscriber : subscribers_)
		{
			subscriber->OnRowChange(event);
		}
	}

	void PrimaryAccountTable::PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events)
	{
		if (replicaSubscriber_ != nullptr)
		{
			replicaSubscriber_->OnRowBatchChange(events);
		}
		for (MdbSubscriber* subscriber : subscribers_)
		{
			subscriber->OnRowBatchChange(events);
		}
	}

	AccountTable::AccountTable()
	{
		PrimaryKey = new AccountTablePrimaryKey(this);
	}

	AccountTable::~AccountTable()
	{
		delete PrimaryKey;
		PrimaryKey = nullptr;
	}

	unsigned int AccountTable::GetTableId() const
	{
		return Account::TableId;
	}

	void AccountTable::Subscribe(MdbSubscriber* subscriber)
	{
		if (subscriber == nullptr)
		{
			return;
		}
		subscribers_.push_back(subscriber);
	}

	void AccountTable::SubscribeReplica(MdbReplicaSubscriber* replica)
	{
		replicaSubscriber_ = replica;
	}

	void AccountTable::UnSubscribe()
	{
		subscribers_.clear();
		replicaSubscriber_ = nullptr;
	}

	void AccountTable::InitDb()
	{
		if (replicaSubscriber_ == nullptr)
		{
			return;
		}
		// 快照期间持有 SharedMutex 共享锁，写者与回收都被挡在外面
		std::lock_guard publishGuard(publishMutex_);
		std::shared_lock guard(SharedMutex);
		replicaSubscriber_->OnSnapshotBegin(Account::TableId);
		std::vector<RowChangeEventPtr> events;
		PrimaryKey->ForEachRecord([this, &events](Account* record)
		{
			events.push_back(MakeInsertEvent(record));
			return true;
		});
		if (!events.empty())
		{
			replicaSubscriber_->OnRowBatchChange(events);
		}
		replicaSubscriber_->OnSnapshotComplete(Account::TableId);
	}

	bool AccountTable::Insert(Account* record)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			if (!PrimaryKey->CheckInsert(record))
			{
				WriteLog(LogLevel::Warning, "Insert Failed for Account:[%s]", record->GetString());
				record->Deallocate();
				return false;
			}
			PrimaryKey->Insert(record);
			event = BuildInsertEvent(record);
		}
		PublishRowChange(event);
		return true;
	}

	void AccountTable::BatchInsert(std::vector<Account*>& records)
	{
		std::lock_guard publishGuard(publishMutex_);
		std::vector<RowChangeEventPtr> events;
		{
			std::lock_guard guard(SharedMutex);
			for (auto record : records)
			{
				[[maybe_unused]] const bool insertedIntoPrimaryKey = PrimaryKey->Insert(record);
				assert(insertedIntoPrimaryKey);
			}
			events = BuildBatchInsertEvents(records);
			records.clear();
		}
		PublishRowBatchChange(events);
	}

	bool AccountTable::Erase(const Account& record)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			Account* liveRecord = PrimaryKey->Find(&record);
			if (liveRecord == nullptr)
			{
				return false;
			}
			if (HasSubscribers())
			{
				// 离表的活记录要等读者走完才回收，事件拿到的是它的副本
				Account* eventRecord = Account::Allocate();
				std::memcpy(eventRecord, liveRecord, sizeof(Account));
				event = BuildRowChangeEvent(RowChangeType::Erase, 0, true, AdoptRecord(eventRecord));
			}
			PrimaryKey->Erase(liveRecord);
		}
		PublishRowChange(event);
		return true;
	}

	bool AccountTable::Update(const Account& newRecord, bool updateDB)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			Account* liveRecord = PrimaryKey->Find(&newRecord);
			if (liveRecord == nullptr)
			{
				WriteLog(LogLevel::Warning, "Update Failed for Account:[%s]", newRecord.GetString());
				return false;
			}
			PrimaryKey->Replace(liveRecord, newRecord);
			event = BuildUpdateEvent(newRecord, updateDB);
		}
		PublishRowChange(event);
		return true;
	}

	void AccountTable::TruncateTables()
	{
		std::lock_guard publishGuard(publishMutex_);
		std::lock_guard guard(SharedMutex);
		PrimaryKey->Clear();
	}

	void AccountTable::TruncateTable()
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			PrimaryKey->Clear();
			event = BuildRowChangeEvent(RowChangeType::Truncate, 0, true, RecordHandle());
		}
		PublishRowChange(event);
	}

	void AccountTable::Dump(const char* dir)
	{
		string fileName = string(dir) + "//Account.csv";
		FILE* dumpFile = fopen(fileName.c_str(), "w");
		if (dumpFile == nullptr)
		{
			return;
		}

		fprintf(dumpFile, "AccountId,AccountName,AccountType,AccountStatus,Password,TradeGroupId,RiskGroupId,CommissionGroupId\n");
		set<Account*, AccountLessForAccountPrimaryKey> records;
		{
			std::shared_lock guard(SharedMutex);
			PrimaryKey->ForEachRecord([&records](Account* record)
			{
				records.insert(record);
				return true;
			});
		}
		for (auto record : records)
		{
			fprintf(dumpFile, "%s\n", record->GetString());
		}
		records.clear();
		fclose(dumpFile);
	}

	bool AccountTable::HasSubscribers() const
	{
		return replicaSubscriber_ != nullptr || !subscribers_.empty();
	}

	RowChangeEventPtr AccountTable::MakeInsertEvent(const Account* record)
	{
		auto eventRecord = Account::Allocate();
		std::memcpy(eventRecord, record, sizeof(Account));
		return std::make_shared<const RowChangeEvent>(Account::TableId, RowChangeType::Insert, 0, ++sequence_, true, AdoptRecord(eventRecord));
	}

	RowChangeEventPtr AccountTable::BuildRowChangeEvent(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		return std::make_shared<const RowChangeEvent>(Account::TableId, type, indexId, ++sequence_, persistDatabase, std::move(record));
	}

	RowChangeEventPtr AccountTable::BuildInsertEvent(const Account* record)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		return MakeInsertEvent(record);
	}

	RowChangeEventPtr AccountTable::BuildUpdateEvent(const Account& newRecord, bool updateDB)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		Account* eventRecord = Account::Allocate();
		std::memcpy(eventRecord, &newRecord, sizeof(Account));
		// updateDB 只表示"这条变更不落库"，行变更照常广播，由副本订阅者自行忽略
		return std::make_shared<const RowChangeEvent>(
			Account::TableId, RowChangeType::Update, 0, ++sequence_, updateDB, AdoptRecord(eventRecord));
	}

	std::vector<RowChangeEventPtr> AccountTable::BuildBatchInsertEvents(const std::vector<Account*>& records)
	{
		std::vector<RowChangeEventPtr> events;
		if (!HasSubscribers())
		{
			return events;
		}
		events.reserve(records.size());
		for (const Account* record : records)
		{
			events.push_back(MakeInsertEvent(record));
		}
		return events;
	}

	void AccountTable::PublishRowChange(const RowChangeEventPtr& event)
	{
		if (replicaSubscriber_ != nullptr)
		{
			replicaSubscriber_->OnRowChange(event);
		}
		for (MdbSubscriber* subscriber : subscribers_)
		{
			subscriber->OnRowChange(event);
		}
	}

	void AccountTable::PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events)
	{
		if (replicaSubscriber_ != nullptr)
		{
			replicaSubscriber_->OnRowBatchChange(events);
		}
		for (MdbSubscriber* subscriber : subscribers_)
		{
			subscriber->OnRowBatchChange(events);
		}
	}

	CapitalTable::CapitalTable()
	{
		PrimaryKey = new CapitalTablePrimaryKey(this);
	}

	CapitalTable::~CapitalTable()
	{
		delete PrimaryKey;
		PrimaryKey = nullptr;
	}

	unsigned int CapitalTable::GetTableId() const
	{
		return Capital::TableId;
	}

	void CapitalTable::Subscribe(MdbSubscriber* subscriber)
	{
		if (subscriber == nullptr)
		{
			return;
		}
		subscribers_.push_back(subscriber);
	}

	void CapitalTable::SubscribeReplica(MdbReplicaSubscriber* replica)
	{
		replicaSubscriber_ = replica;
	}

	void CapitalTable::UnSubscribe()
	{
		subscribers_.clear();
		replicaSubscriber_ = nullptr;
	}

	void CapitalTable::InitDb()
	{
		if (replicaSubscriber_ == nullptr)
		{
			return;
		}
		// 快照期间持有 SharedMutex 共享锁，写者与回收都被挡在外面
		std::lock_guard publishGuard(publishMutex_);
		std::shared_lock guard(SharedMutex);
		replicaSubscriber_->OnSnapshotBegin(Capital::TableId);
		std::vector<RowChangeEventPtr> events;
		PrimaryKey->ForEachRecord([this, &events](Capital* record)
		{
			events.push_back(MakeInsertEvent(record));
			return true;
		});
		if (!events.empty())
		{
			replicaSubscriber_->OnRowBatchChange(events);
		}
		replicaSubscriber_->OnSnapshotComplete(Capital::TableId);
	}

	bool CapitalTable::Insert(Capital* record)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			if (!PrimaryKey->CheckInsert(record))
			{
				WriteLog(LogLevel::Warning, "Insert Failed for Capital:[%s]", record->GetString());
				record->Deallocate();
				return false;
			}
			PrimaryKey->Insert(record);
			event = BuildInsertEvent(record);
		}
		PublishRowChange(event);
		return true;
	}

	void CapitalTable::BatchInsert(std::vector<Capital*>& records)
	{
		std::lock_guard publishGuard(publishMutex_);
		std::vector<RowChangeEventPtr> events;
		{
			std::lock_guard guard(SharedMutex);
			for (auto record : records)
			{
				[[maybe_unused]] const bool insertedIntoPrimaryKey = PrimaryKey->Insert(record);
				assert(insertedIntoPrimaryKey);
			}
			events = BuildBatchInsertEvents(records);
			records.clear();
		}
		PublishRowBatchChange(events);
	}

	bool CapitalTable::Erase(const Capital& record)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			Capital* liveRecord = PrimaryKey->Find(&record);
			if (liveRecord == nullptr)
			{
				return false;
			}
			if (HasSubscribers())
			{
				// 离表的活记录要等读者走完才回收，事件拿到的是它的副本
				Capital* eventRecord = Capital::Allocate();
				std::memcpy(eventRecord, liveRecord, sizeof(Capital));
				event = BuildRowChangeEvent(RowChangeType::Erase, 0, true, AdoptRecord(eventRecord));
			}
			PrimaryKey->Erase(liveRecord);
		}
		PublishRowChange(event);
		return true;
	}

	bool CapitalTable::Update(const Capital& newRecord, bool updateDB)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			Capital* liveRecord = PrimaryKey->Find(&newRecord);
			if (liveRecord == nullptr)
			{
				WriteLog(LogLevel::Warning, "Update Failed for Capital:[%s]", newRecord.GetString());
				return false;
			}
			PrimaryKey->Replace(liveRecord, newRecord);
			event = BuildUpdateEvent(newRecord, updateDB);
		}
		PublishRowChange(event);
		return true;
	}

	void CapitalTable::TruncateTables()
	{
		std::lock_guard publishGuard(publishMutex_);
		std::lock_guard guard(SharedMutex);
		PrimaryKey->Clear();
	}

	void CapitalTable::TruncateTable()
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			PrimaryKey->Clear();
			event = BuildRowChangeEvent(RowChangeType::Truncate, 0, true, RecordHandle());
		}
		PublishRowChange(event);
	}

	void CapitalTable::Dump(const char* dir)
	{
		string fileName = string(dir) + "//Capital.csv";
		FILE* dumpFile = fopen(fileName.c_str(), "w");
		if (dumpFile == nullptr)
		{
			return;
		}

		fprintf(dumpFile, "TradingDay,AccountId,AccountType,Balance,PreBalance,Available,MarketValue,CashIn,CashOut,Margin,Commission,FrozenCash,FrozenMargin,FrozenCommission,CloseProfitByDate,CloseProfitByTrade,PositionProfitByDate,PositionProfitByTrade,Deposit,Withdraw\n");
		set<Capital*, CapitalLessForCapitalPrimaryKey> records;
		{
			std::shared_lock guard(SharedMutex);
			PrimaryKey->ForEachRecord([&records](Capital* record)
			{
				records.insert(record);
				return true;
			});
		}
		for (auto record : records)
		{
			fprintf(dumpFile, "%s\n", record->GetString());
		}
		records.clear();
		fclose(dumpFile);
	}

	bool CapitalTable::HasSubscribers() const
	{
		return replicaSubscriber_ != nullptr || !subscribers_.empty();
	}

	RowChangeEventPtr CapitalTable::MakeInsertEvent(const Capital* record)
	{
		auto eventRecord = Capital::Allocate();
		std::memcpy(eventRecord, record, sizeof(Capital));
		return std::make_shared<const RowChangeEvent>(Capital::TableId, RowChangeType::Insert, 0, ++sequence_, true, AdoptRecord(eventRecord));
	}

	RowChangeEventPtr CapitalTable::BuildRowChangeEvent(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		return std::make_shared<const RowChangeEvent>(Capital::TableId, type, indexId, ++sequence_, persistDatabase, std::move(record));
	}

	RowChangeEventPtr CapitalTable::BuildInsertEvent(const Capital* record)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		return MakeInsertEvent(record);
	}

	RowChangeEventPtr CapitalTable::BuildUpdateEvent(const Capital& newRecord, bool updateDB)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		Capital* eventRecord = Capital::Allocate();
		std::memcpy(eventRecord, &newRecord, sizeof(Capital));
		// updateDB 只表示"这条变更不落库"，行变更照常广播，由副本订阅者自行忽略
		return std::make_shared<const RowChangeEvent>(
			Capital::TableId, RowChangeType::Update, 0, ++sequence_, updateDB, AdoptRecord(eventRecord));
	}

	std::vector<RowChangeEventPtr> CapitalTable::BuildBatchInsertEvents(const std::vector<Capital*>& records)
	{
		std::vector<RowChangeEventPtr> events;
		if (!HasSubscribers())
		{
			return events;
		}
		events.reserve(records.size());
		for (const Capital* record : records)
		{
			events.push_back(MakeInsertEvent(record));
		}
		return events;
	}

	void CapitalTable::PublishRowChange(const RowChangeEventPtr& event)
	{
		if (replicaSubscriber_ != nullptr)
		{
			replicaSubscriber_->OnRowChange(event);
		}
		for (MdbSubscriber* subscriber : subscribers_)
		{
			subscriber->OnRowChange(event);
		}
	}

	void CapitalTable::PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events)
	{
		if (replicaSubscriber_ != nullptr)
		{
			replicaSubscriber_->OnRowBatchChange(events);
		}
		for (MdbSubscriber* subscriber : subscribers_)
		{
			subscriber->OnRowBatchChange(events);
		}
	}

	PositionTable::PositionTable()
	{
		PrimaryKey = new PositionTablePrimaryKey(this);
	}

	PositionTable::~PositionTable()
	{
		delete PrimaryKey;
		PrimaryKey = nullptr;
	}

	unsigned int PositionTable::GetTableId() const
	{
		return Position::TableId;
	}

	void PositionTable::Subscribe(MdbSubscriber* subscriber)
	{
		if (subscriber == nullptr)
		{
			return;
		}
		subscribers_.push_back(subscriber);
	}

	void PositionTable::SubscribeReplica(MdbReplicaSubscriber* replica)
	{
		replicaSubscriber_ = replica;
	}

	void PositionTable::UnSubscribe()
	{
		subscribers_.clear();
		replicaSubscriber_ = nullptr;
	}

	void PositionTable::InitDb()
	{
		if (replicaSubscriber_ == nullptr)
		{
			return;
		}
		// 快照期间持有 SharedMutex 共享锁，写者与回收都被挡在外面
		std::lock_guard publishGuard(publishMutex_);
		std::shared_lock guard(SharedMutex);
		replicaSubscriber_->OnSnapshotBegin(Position::TableId);
		std::vector<RowChangeEventPtr> events;
		PrimaryKey->ForEachRecord([this, &events](Position* record)
		{
			events.push_back(MakeInsertEvent(record));
			return true;
		});
		if (!events.empty())
		{
			replicaSubscriber_->OnRowBatchChange(events);
		}
		replicaSubscriber_->OnSnapshotComplete(Position::TableId);
	}

	bool PositionTable::Insert(Position* record)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			if (!PrimaryKey->CheckInsert(record))
			{
				WriteLog(LogLevel::Warning, "Insert Failed for Position:[%s]", record->GetString());
				record->Deallocate();
				return false;
			}
			PrimaryKey->Insert(record);
			event = BuildInsertEvent(record);
		}
		PublishRowChange(event);
		return true;
	}

	void PositionTable::BatchInsert(std::vector<Position*>& records)
	{
		std::lock_guard publishGuard(publishMutex_);
		std::vector<RowChangeEventPtr> events;
		{
			std::lock_guard guard(SharedMutex);
			for (auto record : records)
			{
				[[maybe_unused]] const bool insertedIntoPrimaryKey = PrimaryKey->Insert(record);
				assert(insertedIntoPrimaryKey);
			}
			events = BuildBatchInsertEvents(records);
			records.clear();
		}
		PublishRowBatchChange(events);
	}

	bool PositionTable::Erase(const Position& record)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			Position* liveRecord = PrimaryKey->Find(&record);
			if (liveRecord == nullptr)
			{
				return false;
			}
			if (HasSubscribers())
			{
				// 离表的活记录要等读者走完才回收，事件拿到的是它的副本
				Position* eventRecord = Position::Allocate();
				std::memcpy(eventRecord, liveRecord, sizeof(Position));
				event = BuildRowChangeEvent(RowChangeType::Erase, 0, true, AdoptRecord(eventRecord));
			}
			PrimaryKey->Erase(liveRecord);
		}
		PublishRowChange(event);
		return true;
	}

	bool PositionTable::Update(const Position& newRecord, bool updateDB)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			Position* liveRecord = PrimaryKey->Find(&newRecord);
			if (liveRecord == nullptr)
			{
				WriteLog(LogLevel::Warning, "Update Failed for Position:[%s]", newRecord.GetString());
				return false;
			}
			PrimaryKey->Replace(liveRecord, newRecord);
			event = BuildUpdateEvent(newRecord, updateDB);
		}
		PublishRowChange(event);
		return true;
	}

	void PositionTable::TruncateTables()
	{
		std::lock_guard publishGuard(publishMutex_);
		std::lock_guard guard(SharedMutex);
		PrimaryKey->Clear();
	}

	void PositionTable::TruncateTable()
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			PrimaryKey->Clear();
			event = BuildRowChangeEvent(RowChangeType::Truncate, 0, true, RecordHandle());
		}
		PublishRowChange(event);
	}

	void PositionTable::Dump(const char* dir)
	{
		string fileName = string(dir) + "//Position.csv";
		FILE* dumpFile = fopen(fileName.c_str(), "w");
		if (dumpFile == nullptr)
		{
			return;
		}

		fprintf(dumpFile, "TradingDay,AccountId,AccountType,ExchangeId,InstrumentId,ProductClass,PosiDirection,TotalPosition,PositionFrozen,TodayPosition,MarketValue,CashIn,CashOut,Margin,Commission,FrozenCash,FrozenMargin,FrozenCommission,VolumeMultiple,CloseProfitByDate,CloseProfitByTrade,PositionProfitByDate,PositionProfitByTrade,SettlementPrice,PreSettlementPrice\n");
		set<Position*, PositionLessForPositionPrimaryKey> records;
		{
			std::shared_lock guard(SharedMutex);
			PrimaryKey->ForEachRecord([&records](Position* record)
			{
				records.insert(record);
				return true;
			});
		}
		for (auto record : records)
		{
			fprintf(dumpFile, "%s\n", record->GetString());
		}
		records.clear();
		fclose(dumpFile);
	}

	bool PositionTable::HasSubscribers() const
	{
		return replicaSubscriber_ != nullptr || !subscribers_.empty();
	}

	RowChangeEventPtr PositionTable::MakeInsertEvent(const Position* record)
	{
		auto eventRecord = Position::Allocate();
		std::memcpy(eventRecord, record, sizeof(Position));
		return std::make_shared<const RowChangeEvent>(Position::TableId, RowChangeType::Insert, 0, ++sequence_, true, AdoptRecord(eventRecord));
	}

	RowChangeEventPtr PositionTable::BuildRowChangeEvent(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		return std::make_shared<const RowChangeEvent>(Position::TableId, type, indexId, ++sequence_, persistDatabase, std::move(record));
	}

	RowChangeEventPtr PositionTable::BuildInsertEvent(const Position* record)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		return MakeInsertEvent(record);
	}

	RowChangeEventPtr PositionTable::BuildUpdateEvent(const Position& newRecord, bool updateDB)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		Position* eventRecord = Position::Allocate();
		std::memcpy(eventRecord, &newRecord, sizeof(Position));
		// updateDB 只表示"这条变更不落库"，行变更照常广播，由副本订阅者自行忽略
		return std::make_shared<const RowChangeEvent>(
			Position::TableId, RowChangeType::Update, 0, ++sequence_, updateDB, AdoptRecord(eventRecord));
	}

	std::vector<RowChangeEventPtr> PositionTable::BuildBatchInsertEvents(const std::vector<Position*>& records)
	{
		std::vector<RowChangeEventPtr> events;
		if (!HasSubscribers())
		{
			return events;
		}
		events.reserve(records.size());
		for (const Position* record : records)
		{
			events.push_back(MakeInsertEvent(record));
		}
		return events;
	}

	void PositionTable::PublishRowChange(const RowChangeEventPtr& event)
	{
		if (replicaSubscriber_ != nullptr)
		{
			replicaSubscriber_->OnRowChange(event);
		}
		for (MdbSubscriber* subscriber : subscribers_)
		{
			subscriber->OnRowChange(event);
		}
	}

	void PositionTable::PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events)
	{
		if (replicaSubscriber_ != nullptr)
		{
			replicaSubscriber_->OnRowBatchChange(events);
		}
		for (MdbSubscriber* subscriber : subscribers_)
		{
			subscriber->OnRowBatchChange(events);
		}
	}

	PositionDetailTable::PositionDetailTable()
	{
		PrimaryKey = new PositionDetailTablePrimaryKey(this);
	}

	PositionDetailTable::~PositionDetailTable()
	{
		delete PrimaryKey;
		PrimaryKey = nullptr;
	}

	unsigned int PositionDetailTable::GetTableId() const
	{
		return PositionDetail::TableId;
	}

	void PositionDetailTable::Subscribe(MdbSubscriber* subscriber)
	{
		if (subscriber == nullptr)
		{
			return;
		}
		subscribers_.push_back(subscriber);
	}

	void PositionDetailTable::SubscribeReplica(MdbReplicaSubscriber* replica)
	{
		replicaSubscriber_ = replica;
	}

	void PositionDetailTable::UnSubscribe()
	{
		subscribers_.clear();
		replicaSubscriber_ = nullptr;
	}

	void PositionDetailTable::InitDb()
	{
		if (replicaSubscriber_ == nullptr)
		{
			return;
		}
		// 快照期间持有 SharedMutex 共享锁，写者与回收都被挡在外面
		std::lock_guard publishGuard(publishMutex_);
		std::shared_lock guard(SharedMutex);
		replicaSubscriber_->OnSnapshotBegin(PositionDetail::TableId);
		std::vector<RowChangeEventPtr> events;
		PrimaryKey->ForEachRecord([this, &events](PositionDetail* record)
		{
			events.push_back(MakeInsertEvent(record));
			return true;
		});
		if (!events.empty())
		{
			replicaSubscriber_->OnRowBatchChange(events);
		}
		replicaSubscriber_->OnSnapshotComplete(PositionDetail::TableId);
	}

	bool PositionDetailTable::Insert(PositionDetail* record)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			if (!PrimaryKey->CheckInsert(record))
			{
				WriteLog(LogLevel::Warning, "Insert Failed for PositionDetail:[%s]", record->GetString());
				record->Deallocate();
				return false;
			}
			PrimaryKey->Insert(record);
			event = BuildInsertEvent(record);
		}
		PublishRowChange(event);
		return true;
	}

	void PositionDetailTable::BatchInsert(std::vector<PositionDetail*>& records)
	{
		std::lock_guard publishGuard(publishMutex_);
		std::vector<RowChangeEventPtr> events;
		{
			std::lock_guard guard(SharedMutex);
			for (auto record : records)
			{
				[[maybe_unused]] const bool insertedIntoPrimaryKey = PrimaryKey->Insert(record);
				assert(insertedIntoPrimaryKey);
			}
			events = BuildBatchInsertEvents(records);
			records.clear();
		}
		PublishRowBatchChange(events);
	}

	bool PositionDetailTable::Erase(const PositionDetail& record)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			PositionDetail* liveRecord = PrimaryKey->Find(&record);
			if (liveRecord == nullptr)
			{
				return false;
			}
			if (HasSubscribers())
			{
				// 离表的活记录要等读者走完才回收，事件拿到的是它的副本
				PositionDetail* eventRecord = PositionDetail::Allocate();
				std::memcpy(eventRecord, liveRecord, sizeof(PositionDetail));
				event = BuildRowChangeEvent(RowChangeType::Erase, 0, true, AdoptRecord(eventRecord));
			}
			PrimaryKey->Erase(liveRecord);
		}
		PublishRowChange(event);
		return true;
	}

	bool PositionDetailTable::Update(const PositionDetail& newRecord, bool updateDB)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			PositionDetail* liveRecord = PrimaryKey->Find(&newRecord);
			if (liveRecord == nullptr)
			{
				WriteLog(LogLevel::Warning, "Update Failed for PositionDetail:[%s]", newRecord.GetString());
				return false;
			}
			PrimaryKey->Replace(liveRecord, newRecord);
			event = BuildUpdateEvent(newRecord, updateDB);
		}
		PublishRowChange(event);
		return true;
	}

	void PositionDetailTable::TruncateTables()
	{
		std::lock_guard publishGuard(publishMutex_);
		std::lock_guard guard(SharedMutex);
		PrimaryKey->Clear();
	}

	void PositionDetailTable::TruncateTable()
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			PrimaryKey->Clear();
			event = BuildRowChangeEvent(RowChangeType::Truncate, 0, true, RecordHandle());
		}
		PublishRowChange(event);
	}

	void PositionDetailTable::Dump(const char* dir)
	{
		string fileName = string(dir) + "//PositionDetail.csv";
		FILE* dumpFile = fopen(fileName.c_str(), "w");
		if (dumpFile == nullptr)
		{
			return;
		}

		fprintf(dumpFile, "TradingDay,AccountId,AccountType,ExchangeId,InstrumentId,ProductClass,PosiDirection,OpenDate,TradeId,Volume,OpenPrice,MarketValue,CashIn,CashOut,Margin,Commission,VolumeMultiple,CloseProfitByDate,CloseProfitByTrade,PositionProfitByDate,PositionProfitByTrade,SettlementPrice,PreSettlementPrice,CloseVolume,CloseAmount\n");
		set<PositionDetail*, PositionDetailLessForPositionDetailPrimaryKey> records;
		{
			std::shared_lock guard(SharedMutex);
			PrimaryKey->ForEachRecord([&records](PositionDetail* record)
			{
				records.insert(record);
				return true;
			});
		}
		for (auto record : records)
		{
			fprintf(dumpFile, "%s\n", record->GetString());
		}
		records.clear();
		fclose(dumpFile);
	}

	bool PositionDetailTable::HasSubscribers() const
	{
		return replicaSubscriber_ != nullptr || !subscribers_.empty();
	}

	RowChangeEventPtr PositionDetailTable::MakeInsertEvent(const PositionDetail* record)
	{
		auto eventRecord = PositionDetail::Allocate();
		std::memcpy(eventRecord, record, sizeof(PositionDetail));
		return std::make_shared<const RowChangeEvent>(PositionDetail::TableId, RowChangeType::Insert, 0, ++sequence_, true, AdoptRecord(eventRecord));
	}

	RowChangeEventPtr PositionDetailTable::BuildRowChangeEvent(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		return std::make_shared<const RowChangeEvent>(PositionDetail::TableId, type, indexId, ++sequence_, persistDatabase, std::move(record));
	}

	RowChangeEventPtr PositionDetailTable::BuildInsertEvent(const PositionDetail* record)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		return MakeInsertEvent(record);
	}

	RowChangeEventPtr PositionDetailTable::BuildUpdateEvent(const PositionDetail& newRecord, bool updateDB)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		PositionDetail* eventRecord = PositionDetail::Allocate();
		std::memcpy(eventRecord, &newRecord, sizeof(PositionDetail));
		// updateDB 只表示"这条变更不落库"，行变更照常广播，由副本订阅者自行忽略
		return std::make_shared<const RowChangeEvent>(
			PositionDetail::TableId, RowChangeType::Update, 0, ++sequence_, updateDB, AdoptRecord(eventRecord));
	}

	std::vector<RowChangeEventPtr> PositionDetailTable::BuildBatchInsertEvents(const std::vector<PositionDetail*>& records)
	{
		std::vector<RowChangeEventPtr> events;
		if (!HasSubscribers())
		{
			return events;
		}
		events.reserve(records.size());
		for (const PositionDetail* record : records)
		{
			events.push_back(MakeInsertEvent(record));
		}
		return events;
	}

	void PositionDetailTable::PublishRowChange(const RowChangeEventPtr& event)
	{
		if (replicaSubscriber_ != nullptr)
		{
			replicaSubscriber_->OnRowChange(event);
		}
		for (MdbSubscriber* subscriber : subscribers_)
		{
			subscriber->OnRowChange(event);
		}
	}

	void PositionDetailTable::PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events)
	{
		if (replicaSubscriber_ != nullptr)
		{
			replicaSubscriber_->OnRowBatchChange(events);
		}
		for (MdbSubscriber* subscriber : subscribers_)
		{
			subscriber->OnRowBatchChange(events);
		}
	}

	OrderTable::OrderTable()
	{
		PrimaryKey = new OrderTablePrimaryKey(this);
	}

	OrderTable::~OrderTable()
	{
		delete PrimaryKey;
		PrimaryKey = nullptr;
	}

	unsigned int OrderTable::GetTableId() const
	{
		return Order::TableId;
	}

	void OrderTable::Subscribe(MdbSubscriber* subscriber)
	{
		if (subscriber == nullptr)
		{
			return;
		}
		subscribers_.push_back(subscriber);
	}

	void OrderTable::SubscribeReplica(MdbReplicaSubscriber* replica)
	{
		replicaSubscriber_ = replica;
	}

	void OrderTable::UnSubscribe()
	{
		subscribers_.clear();
		replicaSubscriber_ = nullptr;
	}

	void OrderTable::InitDb()
	{
		if (replicaSubscriber_ == nullptr)
		{
			return;
		}
		// 快照期间持有 SharedMutex 共享锁，写者与回收都被挡在外面
		std::lock_guard publishGuard(publishMutex_);
		std::shared_lock guard(SharedMutex);
		replicaSubscriber_->OnSnapshotBegin(Order::TableId);
		std::vector<RowChangeEventPtr> events;
		PrimaryKey->ForEachRecord([this, &events](Order* record)
		{
			events.push_back(MakeInsertEvent(record));
			return true;
		});
		if (!events.empty())
		{
			replicaSubscriber_->OnRowBatchChange(events);
		}
		replicaSubscriber_->OnSnapshotComplete(Order::TableId);
	}

	bool OrderTable::Insert(Order* record)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			if (!PrimaryKey->CheckInsert(record))
			{
				WriteLog(LogLevel::Warning, "Insert Failed for Order:[%s]", record->GetString());
				record->Deallocate();
				return false;
			}
			PrimaryKey->Insert(record);
			event = BuildInsertEvent(record);
		}
		PublishRowChange(event);
		return true;
	}

	void OrderTable::BatchInsert(std::vector<Order*>& records)
	{
		std::lock_guard publishGuard(publishMutex_);
		std::vector<RowChangeEventPtr> events;
		{
			std::lock_guard guard(SharedMutex);
			for (auto record : records)
			{
				[[maybe_unused]] const bool insertedIntoPrimaryKey = PrimaryKey->Insert(record);
				assert(insertedIntoPrimaryKey);
			}
			events = BuildBatchInsertEvents(records);
			records.clear();
		}
		PublishRowBatchChange(events);
	}

	bool OrderTable::Erase(const Order& record)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			Order* liveRecord = PrimaryKey->Find(&record);
			if (liveRecord == nullptr)
			{
				return false;
			}
			if (HasSubscribers())
			{
				// 离表的活记录要等读者走完才回收，事件拿到的是它的副本
				Order* eventRecord = Order::Allocate();
				std::memcpy(eventRecord, liveRecord, sizeof(Order));
				event = BuildRowChangeEvent(RowChangeType::Erase, 0, true, AdoptRecord(eventRecord));
			}
			PrimaryKey->Erase(liveRecord);
		}
		PublishRowChange(event);
		return true;
	}

	bool OrderTable::Update(const Order& newRecord, bool updateDB)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			Order* liveRecord = PrimaryKey->Find(&newRecord);
			if (liveRecord == nullptr)
			{
				WriteLog(LogLevel::Warning, "Update Failed for Order:[%s]", newRecord.GetString());
				return false;
			}
			PrimaryKey->Replace(liveRecord, newRecord);
			event = BuildUpdateEvent(newRecord, updateDB);
		}
		PublishRowChange(event);
		return true;
	}

	void OrderTable::TruncateTables()
	{
		std::lock_guard publishGuard(publishMutex_);
		std::lock_guard guard(SharedMutex);
		PrimaryKey->Clear();
	}

	void OrderTable::TruncateTable()
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			PrimaryKey->Clear();
			event = BuildRowChangeEvent(RowChangeType::Truncate, 0, true, RecordHandle());
		}
		PublishRowChange(event);
	}

	void OrderTable::Dump(const char* dir)
	{
		string fileName = string(dir) + "//Order.csv";
		FILE* dumpFile = fopen(fileName.c_str(), "w");
		if (dumpFile == nullptr)
		{
			return;
		}

		fprintf(dumpFile, "TradingDay,AccountId,AccountType,ExchangeId,InstrumentId,ProductClass,OrderId,OrderSysId,Direction,OffsetFlag,OrderPriceType,Price,Volume,VolumeTotal,VolumeTraded,VolumeMultiple,OrderStatus,OrderDate,OrderTime,CancelDate,CancelTime,SessionId,ClientOrderId,RequestId,OfferId,TradeGroupId,RiskGroupId,CommissionGroupId,FrozenCash,FrozenMargin,FrozenCommission,RebuildMark,IsForceClose\n");
		set<Order*, OrderLessForOrderPrimaryKey> records;
		{
			std::shared_lock guard(SharedMutex);
			PrimaryKey->ForEachRecord([&records](Order* record)
			{
				records.insert(record);
				return true;
			});
		}
		for (auto record : records)
		{
			fprintf(dumpFile, "%s\n", record->GetString());
		}
		records.clear();
		fclose(dumpFile);
	}

	bool OrderTable::HasSubscribers() const
	{
		return replicaSubscriber_ != nullptr || !subscribers_.empty();
	}

	RowChangeEventPtr OrderTable::MakeInsertEvent(const Order* record)
	{
		auto eventRecord = Order::Allocate();
		std::memcpy(eventRecord, record, sizeof(Order));
		return std::make_shared<const RowChangeEvent>(Order::TableId, RowChangeType::Insert, 0, ++sequence_, true, AdoptRecord(eventRecord));
	}

	RowChangeEventPtr OrderTable::BuildRowChangeEvent(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		return std::make_shared<const RowChangeEvent>(Order::TableId, type, indexId, ++sequence_, persistDatabase, std::move(record));
	}

	RowChangeEventPtr OrderTable::BuildInsertEvent(const Order* record)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		return MakeInsertEvent(record);
	}

	RowChangeEventPtr OrderTable::BuildUpdateEvent(const Order& newRecord, bool updateDB)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		Order* eventRecord = Order::Allocate();
		std::memcpy(eventRecord, &newRecord, sizeof(Order));
		// updateDB 只表示"这条变更不落库"，行变更照常广播，由副本订阅者自行忽略
		return std::make_shared<const RowChangeEvent>(
			Order::TableId, RowChangeType::Update, 0, ++sequence_, updateDB, AdoptRecord(eventRecord));
	}

	std::vector<RowChangeEventPtr> OrderTable::BuildBatchInsertEvents(const std::vector<Order*>& records)
	{
		std::vector<RowChangeEventPtr> events;
		if (!HasSubscribers())
		{
			return events;
		}
		events.reserve(records.size());
		for (const Order* record : records)
		{
			events.push_back(MakeInsertEvent(record));
		}
		return events;
	}

	void OrderTable::PublishRowChange(const RowChangeEventPtr& event)
	{
		if (replicaSubscriber_ != nullptr)
		{
			replicaSubscriber_->OnRowChange(event);
		}
		for (MdbSubscriber* subscriber : subscribers_)
		{
			subscriber->OnRowChange(event);
		}
	}

	void OrderTable::PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events)
	{
		if (replicaSubscriber_ != nullptr)
		{
			replicaSubscriber_->OnRowBatchChange(events);
		}
		for (MdbSubscriber* subscriber : subscribers_)
		{
			subscriber->OnRowBatchChange(events);
		}
	}

	TradeTable::TradeTable()
	{
		PrimaryKey = new TradeTablePrimaryKey(this);
	}

	TradeTable::~TradeTable()
	{
		delete PrimaryKey;
		PrimaryKey = nullptr;
	}

	unsigned int TradeTable::GetTableId() const
	{
		return Trade::TableId;
	}

	void TradeTable::Subscribe(MdbSubscriber* subscriber)
	{
		if (subscriber == nullptr)
		{
			return;
		}
		subscribers_.push_back(subscriber);
	}

	void TradeTable::SubscribeReplica(MdbReplicaSubscriber* replica)
	{
		replicaSubscriber_ = replica;
	}

	void TradeTable::UnSubscribe()
	{
		subscribers_.clear();
		replicaSubscriber_ = nullptr;
	}

	void TradeTable::InitDb()
	{
		if (replicaSubscriber_ == nullptr)
		{
			return;
		}
		// 快照期间持有 SharedMutex 共享锁，写者与回收都被挡在外面
		std::lock_guard publishGuard(publishMutex_);
		std::shared_lock guard(SharedMutex);
		replicaSubscriber_->OnSnapshotBegin(Trade::TableId);
		std::vector<RowChangeEventPtr> events;
		PrimaryKey->ForEachRecord([this, &events](Trade* record)
		{
			events.push_back(MakeInsertEvent(record));
			return true;
		});
		if (!events.empty())
		{
			replicaSubscriber_->OnRowBatchChange(events);
		}
		replicaSubscriber_->OnSnapshotComplete(Trade::TableId);
	}

	bool TradeTable::Insert(Trade* record)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			if (!PrimaryKey->CheckInsert(record))
			{
				WriteLog(LogLevel::Warning, "Insert Failed for Trade:[%s]", record->GetString());
				record->Deallocate();
				return false;
			}
			PrimaryKey->Insert(record);
			event = BuildInsertEvent(record);
		}
		PublishRowChange(event);
		return true;
	}

	void TradeTable::BatchInsert(std::vector<Trade*>& records)
	{
		std::lock_guard publishGuard(publishMutex_);
		std::vector<RowChangeEventPtr> events;
		{
			std::lock_guard guard(SharedMutex);
			for (auto record : records)
			{
				[[maybe_unused]] const bool insertedIntoPrimaryKey = PrimaryKey->Insert(record);
				assert(insertedIntoPrimaryKey);
			}
			events = BuildBatchInsertEvents(records);
			records.clear();
		}
		PublishRowBatchChange(events);
	}

	bool TradeTable::Erase(const Trade& record)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			Trade* liveRecord = PrimaryKey->Find(&record);
			if (liveRecord == nullptr)
			{
				return false;
			}
			if (HasSubscribers())
			{
				// 离表的活记录要等读者走完才回收，事件拿到的是它的副本
				Trade* eventRecord = Trade::Allocate();
				std::memcpy(eventRecord, liveRecord, sizeof(Trade));
				event = BuildRowChangeEvent(RowChangeType::Erase, 0, true, AdoptRecord(eventRecord));
			}
			PrimaryKey->Erase(liveRecord);
		}
		PublishRowChange(event);
		return true;
	}

	bool TradeTable::Update(const Trade& newRecord, bool updateDB)
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			Trade* liveRecord = PrimaryKey->Find(&newRecord);
			if (liveRecord == nullptr)
			{
				WriteLog(LogLevel::Warning, "Update Failed for Trade:[%s]", newRecord.GetString());
				return false;
			}
			PrimaryKey->Replace(liveRecord, newRecord);
			event = BuildUpdateEvent(newRecord, updateDB);
		}
		PublishRowChange(event);
		return true;
	}

	void TradeTable::TruncateTables()
	{
		std::lock_guard publishGuard(publishMutex_);
		std::lock_guard guard(SharedMutex);
		PrimaryKey->Clear();
	}

	void TradeTable::TruncateTable()
	{
		std::lock_guard publishGuard(publishMutex_);
		RowChangeEventPtr event;
		{
			std::lock_guard guard(SharedMutex);
			PrimaryKey->Clear();
			event = BuildRowChangeEvent(RowChangeType::Truncate, 0, true, RecordHandle());
		}
		PublishRowChange(event);
	}

	void TradeTable::Dump(const char* dir)
	{
		string fileName = string(dir) + "//Trade.csv";
		FILE* dumpFile = fopen(fileName.c_str(), "w");
		if (dumpFile == nullptr)
		{
			return;
		}

		fprintf(dumpFile, "TradingDay,AccountId,AccountType,ExchangeId,InstrumentId,ProductClass,OrderId,OrderSysId,TradeId,Direction,OffsetFlag,Price,Volume,VolumeMultiple,TradeAmount,Commission,TradeDate,TradeTime\n");
		set<Trade*, TradeLessForTradePrimaryKey> records;
		{
			std::shared_lock guard(SharedMutex);
			PrimaryKey->ForEachRecord([&records](Trade* record)
			{
				records.insert(record);
				return true;
			});
		}
		for (auto record : records)
		{
			fprintf(dumpFile, "%s\n", record->GetString());
		}
		records.clear();
		fclose(dumpFile);
	}

	bool TradeTable::HasSubscribers() const
	{
		return replicaSubscriber_ != nullptr || !subscribers_.empty();
	}

	RowChangeEventPtr TradeTable::MakeInsertEvent(const Trade* record)
	{
		auto eventRecord = Trade::Allocate();
		std::memcpy(eventRecord, record, sizeof(Trade));
		return std::make_shared<const RowChangeEvent>(Trade::TableId, RowChangeType::Insert, 0, ++sequence_, true, AdoptRecord(eventRecord));
	}

	RowChangeEventPtr TradeTable::BuildRowChangeEvent(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		return std::make_shared<const RowChangeEvent>(Trade::TableId, type, indexId, ++sequence_, persistDatabase, std::move(record));
	}

	RowChangeEventPtr TradeTable::BuildInsertEvent(const Trade* record)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		return MakeInsertEvent(record);
	}

	RowChangeEventPtr TradeTable::BuildUpdateEvent(const Trade& newRecord, bool updateDB)
	{
		if (!HasSubscribers())
		{
			return nullptr;
		}
		Trade* eventRecord = Trade::Allocate();
		std::memcpy(eventRecord, &newRecord, sizeof(Trade));
		// updateDB 只表示"这条变更不落库"，行变更照常广播，由副本订阅者自行忽略
		return std::make_shared<const RowChangeEvent>(
			Trade::TableId, RowChangeType::Update, 0, ++sequence_, updateDB, AdoptRecord(eventRecord));
	}

	std::vector<RowChangeEventPtr> TradeTable::BuildBatchInsertEvents(const std::vector<Trade*>& records)
	{
		std::vector<RowChangeEventPtr> events;
		if (!HasSubscribers())
		{
			return events;
		}
		events.reserve(records.size());
		for (const Trade* record : records)
		{
			events.push_back(MakeInsertEvent(record));
		}
		return events;
	}

	void TradeTable::PublishRowChange(const RowChangeEventPtr& event)
	{
		if (replicaSubscriber_ != nullptr)
		{
			replicaSubscriber_->OnRowChange(event);
		}
		for (MdbSubscriber* subscriber : subscribers_)
		{
			subscriber->OnRowChange(event);
		}
	}

	void TradeTable::PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events)
	{
		if (replicaSubscriber_ != nullptr)
		{
			replicaSubscriber_->OnRowBatchChange(events);
		}
		for (MdbSubscriber* subscriber : subscribers_)
		{
			subscriber->OnRowBatchChange(events);
		}
	}

}
