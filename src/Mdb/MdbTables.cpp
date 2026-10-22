// 本文件由 ../Templates/Cpp/Mdb/MdbTables.cpp.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#include "MdbTables.h"
#include <DbAdapters/DbInterface/RecordHandle.h>
#include <Spark/Core/Logger/Logger.h>
#include <cassert>
#include <string>
#include <cstring>
#include <memory>
#include <set>
#include <utility>
#include <vector>

using std::string;
using std::set;
using namespace Spark::Core;
using namespace DbAdapters;

namespace Mdb
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
		if (replicaSubscriber_ == nullptr)
		{
			return;
		}
		// 清表、拷贝、批量、就绪置位全部在一次读租约内完成：
		// 这期间的写入被阻塞，释放租约后到达的增量必然排在批量之后
		std::shared_lock guard(SharedMutex);
		replicaSubscriber_->OnSnapshotBegin(TradingDay::TableId);
		if (!PrimaryKey->index_.empty())
		{
			std::vector<RowChangeEventPtr> events;
			events.reserve(PrimaryKey->index_.size());
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				events.push_back(MakeInsertEvent(*it));
			}
			replicaSubscriber_->OnRowBatchChange(events);
		}
		replicaSubscriber_->OnSnapshotComplete(TradingDay::TableId);
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

		
		EmitRowInsert(record);
		return true;
	}
	void TradingDayTable::BatchInsert(std::vector<TradingDay*>& records)
	{
		std::lock_guard guard(SharedMutex);
		for (auto record : records)
		{
			[[maybe_unused]] const bool insertedIntoPrimaryKey = PrimaryKey->Insert(record);
			assert(insertedIntoPrimaryKey);

		}
		// 通知在写锁内发出：放到解锁后会让并发 Update 的事件先于本批到达订阅者
		EmitRowBatchInsert(records);
		records.clear();
	}
	void TradingDayTable::Erase(TradingDay* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		EmitRowChange(RowChangeType::Erase, 0, true, AdoptRecord(record));
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

		// updateDB 只表示"这条变更不落库"，行变更照常广播，由副本订阅者自行忽略
		EmitRowChange(RowChangeType::Update, 0, updateDB, AdoptRecord(newRecord));
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
		EmitRowChange(RowChangeType::Truncate, 0, true, RecordHandle());
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
	bool TradingDayTable::HasSubscribers() const
	{
		return replicaSubscriber_ != nullptr || !subscribers_.empty();
	}
	RowChangeEventPtr TradingDayTable::MakeInsertEvent(const TradingDay* record)
	{
		auto eventRecord = TradingDay::Allocate();
		memcpy(eventRecord, record, sizeof(TradingDay));
		return std::make_shared<const RowChangeEvent>(TradingDay::TableId, RowChangeType::Insert, 0, ++sequence_, true, AdoptRecord(eventRecord));
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
	void TradingDayTable::EmitRowChange(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record)
	{
		if (!HasSubscribers())
		{
			return;
		}
		PublishRowChange(std::make_shared<const RowChangeEvent>(TradingDay::TableId, type, indexId, ++sequence_, persistDatabase, std::move(record)));
	}
	void TradingDayTable::EmitRowInsert(const TradingDay* record)
	{
		if (!HasSubscribers())
		{
			return;
		}
		PublishRowChange(MakeInsertEvent(record));
	}
	void TradingDayTable::EmitRowBatchInsert(const std::vector<TradingDay*>& records)
	{
		if (!HasSubscribers())
		{
			return;
		}
		std::vector<RowChangeEventPtr> events;
		events.reserve(records.size());
		for (const TradingDay* record : records)
		{
			events.push_back(MakeInsertEvent(record));
		}
		PublishRowBatchChange(events);
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
		if (replicaSubscriber_ == nullptr)
		{
			return;
		}
		// 清表、拷贝、批量、就绪置位全部在一次读租约内完成：
		// 这期间的写入被阻塞，释放租约后到达的增量必然排在批量之后
		std::shared_lock guard(SharedMutex);
		replicaSubscriber_->OnSnapshotBegin(Exchange::TableId);
		if (!PrimaryKey->index_.empty())
		{
			std::vector<RowChangeEventPtr> events;
			events.reserve(PrimaryKey->index_.size());
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				events.push_back(MakeInsertEvent(*it));
			}
			replicaSubscriber_->OnRowBatchChange(events);
		}
		replicaSubscriber_->OnSnapshotComplete(Exchange::TableId);
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

		
		EmitRowInsert(record);
		return true;
	}
	void ExchangeTable::BatchInsert(std::vector<Exchange*>& records)
	{
		std::lock_guard guard(SharedMutex);
		for (auto record : records)
		{
			[[maybe_unused]] const bool insertedIntoPrimaryKey = PrimaryKey->Insert(record);
			assert(insertedIntoPrimaryKey);

		}
		// 通知在写锁内发出：放到解锁后会让并发 Update 的事件先于本批到达订阅者
		EmitRowBatchInsert(records);
		records.clear();
	}
	void ExchangeTable::Erase(Exchange* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		EmitRowChange(RowChangeType::Erase, 0, true, AdoptRecord(record));
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

		// updateDB 只表示"这条变更不落库"，行变更照常广播，由副本订阅者自行忽略
		EmitRowChange(RowChangeType::Update, 0, updateDB, AdoptRecord(newRecord));
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
		EmitRowChange(RowChangeType::Truncate, 0, true, RecordHandle());
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
	bool ExchangeTable::HasSubscribers() const
	{
		return replicaSubscriber_ != nullptr || !subscribers_.empty();
	}
	RowChangeEventPtr ExchangeTable::MakeInsertEvent(const Exchange* record)
	{
		auto eventRecord = Exchange::Allocate();
		memcpy(eventRecord, record, sizeof(Exchange));
		return std::make_shared<const RowChangeEvent>(Exchange::TableId, RowChangeType::Insert, 0, ++sequence_, true, AdoptRecord(eventRecord));
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
	void ExchangeTable::EmitRowChange(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record)
	{
		if (!HasSubscribers())
		{
			return;
		}
		PublishRowChange(std::make_shared<const RowChangeEvent>(Exchange::TableId, type, indexId, ++sequence_, persistDatabase, std::move(record)));
	}
	void ExchangeTable::EmitRowInsert(const Exchange* record)
	{
		if (!HasSubscribers())
		{
			return;
		}
		PublishRowChange(MakeInsertEvent(record));
	}
	void ExchangeTable::EmitRowBatchInsert(const std::vector<Exchange*>& records)
	{
		if (!HasSubscribers())
		{
			return;
		}
		std::vector<RowChangeEventPtr> events;
		events.reserve(records.size());
		for (const Exchange* record : records)
		{
			events.push_back(MakeInsertEvent(record));
		}
		PublishRowBatchChange(events);
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
		if (replicaSubscriber_ == nullptr)
		{
			return;
		}
		// 清表、拷贝、批量、就绪置位全部在一次读租约内完成：
		// 这期间的写入被阻塞，释放租约后到达的增量必然排在批量之后
		std::shared_lock guard(SharedMutex);
		replicaSubscriber_->OnSnapshotBegin(Product::TableId);
		if (!PrimaryKey->index_.empty())
		{
			std::vector<RowChangeEventPtr> events;
			events.reserve(PrimaryKey->index_.size());
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				events.push_back(MakeInsertEvent(*it));
			}
			replicaSubscriber_->OnRowBatchChange(events);
		}
		replicaSubscriber_->OnSnapshotComplete(Product::TableId);
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

		
		EmitRowInsert(record);
		return true;
	}
	void ProductTable::BatchInsert(std::vector<Product*>& records)
	{
		std::lock_guard guard(SharedMutex);
		for (auto record : records)
		{
			[[maybe_unused]] const bool insertedIntoPrimaryKey = PrimaryKey->Insert(record);
			assert(insertedIntoPrimaryKey);

		}
		// 通知在写锁内发出：放到解锁后会让并发 Update 的事件先于本批到达订阅者
		EmitRowBatchInsert(records);
		records.clear();
	}
	void ProductTable::Erase(Product* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		EmitRowChange(RowChangeType::Erase, 0, true, AdoptRecord(record));
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

		// updateDB 只表示"这条变更不落库"，行变更照常广播，由副本订阅者自行忽略
		EmitRowChange(RowChangeType::Update, 0, updateDB, AdoptRecord(newRecord));
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
		EmitRowChange(RowChangeType::Truncate, 0, true, RecordHandle());
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
	bool ProductTable::HasSubscribers() const
	{
		return replicaSubscriber_ != nullptr || !subscribers_.empty();
	}
	RowChangeEventPtr ProductTable::MakeInsertEvent(const Product* record)
	{
		auto eventRecord = Product::Allocate();
		memcpy(eventRecord, record, sizeof(Product));
		return std::make_shared<const RowChangeEvent>(Product::TableId, RowChangeType::Insert, 0, ++sequence_, true, AdoptRecord(eventRecord));
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
	void ProductTable::EmitRowChange(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record)
	{
		if (!HasSubscribers())
		{
			return;
		}
		PublishRowChange(std::make_shared<const RowChangeEvent>(Product::TableId, type, indexId, ++sequence_, persistDatabase, std::move(record)));
	}
	void ProductTable::EmitRowInsert(const Product* record)
	{
		if (!HasSubscribers())
		{
			return;
		}
		PublishRowChange(MakeInsertEvent(record));
	}
	void ProductTable::EmitRowBatchInsert(const std::vector<Product*>& records)
	{
		if (!HasSubscribers())
		{
			return;
		}
		std::vector<RowChangeEventPtr> events;
		events.reserve(records.size());
		for (const Product* record : records)
		{
			events.push_back(MakeInsertEvent(record));
		}
		PublishRowBatchChange(events);
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
		if (replicaSubscriber_ == nullptr)
		{
			return;
		}
		// 清表、拷贝、批量、就绪置位全部在一次读租约内完成：
		// 这期间的写入被阻塞，释放租约后到达的增量必然排在批量之后
		std::shared_lock guard(SharedMutex);
		replicaSubscriber_->OnSnapshotBegin(Instrument::TableId);
		if (!PrimaryKey->index_.empty())
		{
			std::vector<RowChangeEventPtr> events;
			events.reserve(PrimaryKey->index_.size());
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				events.push_back(MakeInsertEvent(*it));
			}
			replicaSubscriber_->OnRowBatchChange(events);
		}
		replicaSubscriber_->OnSnapshotComplete(Instrument::TableId);
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

		
		EmitRowInsert(record);
		return true;
	}
	void InstrumentTable::BatchInsert(std::vector<Instrument*>& records)
	{
		std::lock_guard guard(SharedMutex);
		for (auto record : records)
		{
			[[maybe_unused]] const bool insertedIntoPrimaryKey = PrimaryKey->Insert(record);
			assert(insertedIntoPrimaryKey);

		}
		// 通知在写锁内发出：放到解锁后会让并发 Update 的事件先于本批到达订阅者
		EmitRowBatchInsert(records);
		records.clear();
	}
	void InstrumentTable::Erase(Instrument* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		EmitRowChange(RowChangeType::Erase, 0, true, AdoptRecord(record));
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

		// updateDB 只表示"这条变更不落库"，行变更照常广播，由副本订阅者自行忽略
		EmitRowChange(RowChangeType::Update, 0, updateDB, AdoptRecord(newRecord));
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
		EmitRowChange(RowChangeType::Truncate, 0, true, RecordHandle());
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
	bool InstrumentTable::HasSubscribers() const
	{
		return replicaSubscriber_ != nullptr || !subscribers_.empty();
	}
	RowChangeEventPtr InstrumentTable::MakeInsertEvent(const Instrument* record)
	{
		auto eventRecord = Instrument::Allocate();
		memcpy(eventRecord, record, sizeof(Instrument));
		return std::make_shared<const RowChangeEvent>(Instrument::TableId, RowChangeType::Insert, 0, ++sequence_, true, AdoptRecord(eventRecord));
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
	void InstrumentTable::EmitRowChange(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record)
	{
		if (!HasSubscribers())
		{
			return;
		}
		PublishRowChange(std::make_shared<const RowChangeEvent>(Instrument::TableId, type, indexId, ++sequence_, persistDatabase, std::move(record)));
	}
	void InstrumentTable::EmitRowInsert(const Instrument* record)
	{
		if (!HasSubscribers())
		{
			return;
		}
		PublishRowChange(MakeInsertEvent(record));
	}
	void InstrumentTable::EmitRowBatchInsert(const std::vector<Instrument*>& records)
	{
		if (!HasSubscribers())
		{
			return;
		}
		std::vector<RowChangeEventPtr> events;
		events.reserve(records.size());
		for (const Instrument* record : records)
		{
			events.push_back(MakeInsertEvent(record));
		}
		PublishRowBatchChange(events);
	}

	PrimaryAccountTable::PrimaryAccountTable()
	{
		PrimaryKey = new PrimaryAccountTablePrimaryKey(this);
		OfferIdIndex = new PrimaryAccountTableIndexOfferId(this);
	}
	PrimaryAccountTable::~PrimaryAccountTable()
	{
		delete PrimaryKey;
		PrimaryKey = nullptr;
		delete OfferIdIndex;
		OfferIdIndex = nullptr;
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
		if (replicaSubscriber_ == nullptr)
		{
			return;
		}
		// 清表、拷贝、批量、就绪置位全部在一次读租约内完成：
		// 这期间的写入被阻塞，释放租约后到达的增量必然排在批量之后
		std::shared_lock guard(SharedMutex);
		replicaSubscriber_->OnSnapshotBegin(PrimaryAccount::TableId);
		if (!PrimaryKey->index_.empty())
		{
			std::vector<RowChangeEventPtr> events;
			events.reserve(PrimaryKey->index_.size());
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				events.push_back(MakeInsertEvent(*it));
			}
			replicaSubscriber_->OnRowBatchChange(events);
		}
		replicaSubscriber_->OnSnapshotComplete(PrimaryAccount::TableId);
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
		
		EmitRowInsert(record);
		return true;
	}
	void PrimaryAccountTable::BatchInsert(std::vector<PrimaryAccount*>& records)
	{
		std::lock_guard guard(SharedMutex);
		for (auto record : records)
		{
			[[maybe_unused]] const bool insertedIntoPrimaryKey = PrimaryKey->Insert(record);
			assert(insertedIntoPrimaryKey);

			OfferIdIndex->Insert(record);
		}
		// 通知在写锁内发出：放到解锁后会让并发 Update 的事件先于本批到达订阅者
		EmitRowBatchInsert(records);
		records.clear();
	}
	void PrimaryAccountTable::Erase(PrimaryAccount* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		EmitRowChange(RowChangeType::Erase, 0, true, AdoptRecord(record));
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
		if (HasSubscribers())
		{
			auto compareRecord = PrimaryAccount::Allocate();
			memcpy(compareRecord, &ComparePrimaryAccount, sizeof(PrimaryAccount));
			EmitRowChange(RowChangeType::EraseByIndex, PrimaryAccountTableIndexOfferId::IndexID, true, AdoptRecord(compareRecord));
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
		PrimaryAccountTableIndexOfferId::iterator itOfferId;
		if (OfferIdIndexUpdate)
		{
			itOfferId = OfferIdIndex->FindNode(oldRecord);
		}
		::memcpy(oldRecord, newRecord, sizeof(PrimaryAccount));
		if (OfferIdIndexUpdate)
		{
			OfferIdIndex->Update(itOfferId);
		}

		// updateDB 只表示"这条变更不落库"，行变更照常广播，由副本订阅者自行忽略
		EmitRowChange(RowChangeType::Update, 0, updateDB, AdoptRecord(newRecord));
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
		EmitRowChange(RowChangeType::Truncate, 0, true, RecordHandle());
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
	bool PrimaryAccountTable::HasSubscribers() const
	{
		return replicaSubscriber_ != nullptr || !subscribers_.empty();
	}
	RowChangeEventPtr PrimaryAccountTable::MakeInsertEvent(const PrimaryAccount* record)
	{
		auto eventRecord = PrimaryAccount::Allocate();
		memcpy(eventRecord, record, sizeof(PrimaryAccount));
		return std::make_shared<const RowChangeEvent>(PrimaryAccount::TableId, RowChangeType::Insert, 0, ++sequence_, true, AdoptRecord(eventRecord));
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
	void PrimaryAccountTable::EmitRowChange(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record)
	{
		if (!HasSubscribers())
		{
			return;
		}
		PublishRowChange(std::make_shared<const RowChangeEvent>(PrimaryAccount::TableId, type, indexId, ++sequence_, persistDatabase, std::move(record)));
	}
	void PrimaryAccountTable::EmitRowInsert(const PrimaryAccount* record)
	{
		if (!HasSubscribers())
		{
			return;
		}
		PublishRowChange(MakeInsertEvent(record));
	}
	void PrimaryAccountTable::EmitRowBatchInsert(const std::vector<PrimaryAccount*>& records)
	{
		if (!HasSubscribers())
		{
			return;
		}
		std::vector<RowChangeEventPtr> events;
		events.reserve(records.size());
		for (const PrimaryAccount* record : records)
		{
			events.push_back(MakeInsertEvent(record));
		}
		PublishRowBatchChange(events);
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
		if (replicaSubscriber_ == nullptr)
		{
			return;
		}
		// 清表、拷贝、批量、就绪置位全部在一次读租约内完成：
		// 这期间的写入被阻塞，释放租约后到达的增量必然排在批量之后
		std::shared_lock guard(SharedMutex);
		replicaSubscriber_->OnSnapshotBegin(Account::TableId);
		if (!PrimaryKey->index_.empty())
		{
			std::vector<RowChangeEventPtr> events;
			events.reserve(PrimaryKey->index_.size());
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				events.push_back(MakeInsertEvent(*it));
			}
			replicaSubscriber_->OnRowBatchChange(events);
		}
		replicaSubscriber_->OnSnapshotComplete(Account::TableId);
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

		
		EmitRowInsert(record);
		return true;
	}
	void AccountTable::BatchInsert(std::vector<Account*>& records)
	{
		std::lock_guard guard(SharedMutex);
		for (auto record : records)
		{
			[[maybe_unused]] const bool insertedIntoPrimaryKey = PrimaryKey->Insert(record);
			assert(insertedIntoPrimaryKey);

		}
		// 通知在写锁内发出：放到解锁后会让并发 Update 的事件先于本批到达订阅者
		EmitRowBatchInsert(records);
		records.clear();
	}
	void AccountTable::Erase(Account* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		EmitRowChange(RowChangeType::Erase, 0, true, AdoptRecord(record));
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

		// updateDB 只表示"这条变更不落库"，行变更照常广播，由副本订阅者自行忽略
		EmitRowChange(RowChangeType::Update, 0, updateDB, AdoptRecord(newRecord));
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
		EmitRowChange(RowChangeType::Truncate, 0, true, RecordHandle());
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
	bool AccountTable::HasSubscribers() const
	{
		return replicaSubscriber_ != nullptr || !subscribers_.empty();
	}
	RowChangeEventPtr AccountTable::MakeInsertEvent(const Account* record)
	{
		auto eventRecord = Account::Allocate();
		memcpy(eventRecord, record, sizeof(Account));
		return std::make_shared<const RowChangeEvent>(Account::TableId, RowChangeType::Insert, 0, ++sequence_, true, AdoptRecord(eventRecord));
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
	void AccountTable::EmitRowChange(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record)
	{
		if (!HasSubscribers())
		{
			return;
		}
		PublishRowChange(std::make_shared<const RowChangeEvent>(Account::TableId, type, indexId, ++sequence_, persistDatabase, std::move(record)));
	}
	void AccountTable::EmitRowInsert(const Account* record)
	{
		if (!HasSubscribers())
		{
			return;
		}
		PublishRowChange(MakeInsertEvent(record));
	}
	void AccountTable::EmitRowBatchInsert(const std::vector<Account*>& records)
	{
		if (!HasSubscribers())
		{
			return;
		}
		std::vector<RowChangeEventPtr> events;
		events.reserve(records.size());
		for (const Account* record : records)
		{
			events.push_back(MakeInsertEvent(record));
		}
		PublishRowBatchChange(events);
	}

	CapitalTable::CapitalTable()
	{
		PrimaryKey = new CapitalTablePrimaryKey(this);
		TradingDayIndex = new CapitalTableIndexTradingDay(this);
	}
	CapitalTable::~CapitalTable()
	{
		delete PrimaryKey;
		PrimaryKey = nullptr;
		delete TradingDayIndex;
		TradingDayIndex = nullptr;
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
		if (replicaSubscriber_ == nullptr)
		{
			return;
		}
		// 清表、拷贝、批量、就绪置位全部在一次读租约内完成：
		// 这期间的写入被阻塞，释放租约后到达的增量必然排在批量之后
		std::shared_lock guard(SharedMutex);
		replicaSubscriber_->OnSnapshotBegin(Capital::TableId);
		if (!PrimaryKey->index_.empty())
		{
			std::vector<RowChangeEventPtr> events;
			events.reserve(PrimaryKey->index_.size());
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				events.push_back(MakeInsertEvent(*it));
			}
			replicaSubscriber_->OnRowBatchChange(events);
		}
		replicaSubscriber_->OnSnapshotComplete(Capital::TableId);
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
		
		EmitRowInsert(record);
		return true;
	}
	void CapitalTable::BatchInsert(std::vector<Capital*>& records)
	{
		std::lock_guard guard(SharedMutex);
		for (auto record : records)
		{
			[[maybe_unused]] const bool insertedIntoPrimaryKey = PrimaryKey->Insert(record);
			assert(insertedIntoPrimaryKey);

			TradingDayIndex->Insert(record);
		}
		// 通知在写锁内发出：放到解锁后会让并发 Update 的事件先于本批到达订阅者
		EmitRowBatchInsert(records);
		records.clear();
	}
	void CapitalTable::Erase(Capital* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		EmitRowChange(RowChangeType::Erase, 0, true, AdoptRecord(record));
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
		if (HasSubscribers())
		{
			auto compareRecord = Capital::Allocate();
			memcpy(compareRecord, &CompareCapital, sizeof(Capital));
			EmitRowChange(RowChangeType::EraseByIndex, CapitalTableIndexTradingDay::IndexID, true, AdoptRecord(compareRecord));
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
		CapitalTableIndexTradingDay::iterator itTradingDay;
		if (TradingDayIndexUpdate)
		{
			itTradingDay = TradingDayIndex->FindNode(oldRecord);
		}
		::memcpy(oldRecord, newRecord, sizeof(Capital));
		if (TradingDayIndexUpdate)
		{
			TradingDayIndex->Update(itTradingDay);
		}

		// updateDB 只表示"这条变更不落库"，行变更照常广播，由副本订阅者自行忽略
		EmitRowChange(RowChangeType::Update, 0, updateDB, AdoptRecord(newRecord));
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
		EmitRowChange(RowChangeType::Truncate, 0, true, RecordHandle());
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
	bool CapitalTable::HasSubscribers() const
	{
		return replicaSubscriber_ != nullptr || !subscribers_.empty();
	}
	RowChangeEventPtr CapitalTable::MakeInsertEvent(const Capital* record)
	{
		auto eventRecord = Capital::Allocate();
		memcpy(eventRecord, record, sizeof(Capital));
		return std::make_shared<const RowChangeEvent>(Capital::TableId, RowChangeType::Insert, 0, ++sequence_, true, AdoptRecord(eventRecord));
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
	void CapitalTable::EmitRowChange(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record)
	{
		if (!HasSubscribers())
		{
			return;
		}
		PublishRowChange(std::make_shared<const RowChangeEvent>(Capital::TableId, type, indexId, ++sequence_, persistDatabase, std::move(record)));
	}
	void CapitalTable::EmitRowInsert(const Capital* record)
	{
		if (!HasSubscribers())
		{
			return;
		}
		PublishRowChange(MakeInsertEvent(record));
	}
	void CapitalTable::EmitRowBatchInsert(const std::vector<Capital*>& records)
	{
		if (!HasSubscribers())
		{
			return;
		}
		std::vector<RowChangeEventPtr> events;
		events.reserve(records.size());
		for (const Capital* record : records)
		{
			events.push_back(MakeInsertEvent(record));
		}
		PublishRowBatchChange(events);
	}

	PositionTable::PositionTable()
	{
		PrimaryKey = new PositionTablePrimaryKey(this);
		AccountIndex = new PositionTableIndexAccount(this);
		TradingDayIndex = new PositionTableIndexTradingDay(this);
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
		if (replicaSubscriber_ == nullptr)
		{
			return;
		}
		// 清表、拷贝、批量、就绪置位全部在一次读租约内完成：
		// 这期间的写入被阻塞，释放租约后到达的增量必然排在批量之后
		std::shared_lock guard(SharedMutex);
		replicaSubscriber_->OnSnapshotBegin(Position::TableId);
		if (!PrimaryKey->index_.empty())
		{
			std::vector<RowChangeEventPtr> events;
			events.reserve(PrimaryKey->index_.size());
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				events.push_back(MakeInsertEvent(*it));
			}
			replicaSubscriber_->OnRowBatchChange(events);
		}
		replicaSubscriber_->OnSnapshotComplete(Position::TableId);
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
		
		EmitRowInsert(record);
		return true;
	}
	void PositionTable::BatchInsert(std::vector<Position*>& records)
	{
		std::lock_guard guard(SharedMutex);
		for (auto record : records)
		{
			[[maybe_unused]] const bool insertedIntoPrimaryKey = PrimaryKey->Insert(record);
			assert(insertedIntoPrimaryKey);

			AccountIndex->Insert(record);
			TradingDayIndex->Insert(record);
		}
		// 通知在写锁内发出：放到解锁后会让并发 Update 的事件先于本批到达订阅者
		EmitRowBatchInsert(records);
		records.clear();
	}
	void PositionTable::Erase(Position* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		EmitRowChange(RowChangeType::Erase, 0, true, AdoptRecord(record));
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
		if (HasSubscribers())
		{
			auto compareRecord = Position::Allocate();
			memcpy(compareRecord, &ComparePosition, sizeof(Position));
			EmitRowChange(RowChangeType::EraseByIndex, PositionTableIndexAccount::IndexID, true, AdoptRecord(compareRecord));
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
		if (HasSubscribers())
		{
			auto compareRecord = Position::Allocate();
			memcpy(compareRecord, &ComparePosition, sizeof(Position));
			EmitRowChange(RowChangeType::EraseByIndex, PositionTableIndexTradingDay::IndexID, true, AdoptRecord(compareRecord));
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
		PositionTableIndexAccount::iterator itAccount;
		if (AccountIndexUpdate)
		{
			itAccount = AccountIndex->FindNode(oldRecord);
		}
		bool TradingDayIndexUpdate = TradingDayIndex->NeedUpdate(oldRecord, newRecord);
		PositionTableIndexTradingDay::iterator itTradingDay;
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

		// updateDB 只表示"这条变更不落库"，行变更照常广播，由副本订阅者自行忽略
		EmitRowChange(RowChangeType::Update, 0, updateDB, AdoptRecord(newRecord));
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
		EmitRowChange(RowChangeType::Truncate, 0, true, RecordHandle());
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
	bool PositionTable::HasSubscribers() const
	{
		return replicaSubscriber_ != nullptr || !subscribers_.empty();
	}
	RowChangeEventPtr PositionTable::MakeInsertEvent(const Position* record)
	{
		auto eventRecord = Position::Allocate();
		memcpy(eventRecord, record, sizeof(Position));
		return std::make_shared<const RowChangeEvent>(Position::TableId, RowChangeType::Insert, 0, ++sequence_, true, AdoptRecord(eventRecord));
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
	void PositionTable::EmitRowChange(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record)
	{
		if (!HasSubscribers())
		{
			return;
		}
		PublishRowChange(std::make_shared<const RowChangeEvent>(Position::TableId, type, indexId, ++sequence_, persistDatabase, std::move(record)));
	}
	void PositionTable::EmitRowInsert(const Position* record)
	{
		if (!HasSubscribers())
		{
			return;
		}
		PublishRowChange(MakeInsertEvent(record));
	}
	void PositionTable::EmitRowBatchInsert(const std::vector<Position*>& records)
	{
		if (!HasSubscribers())
		{
			return;
		}
		std::vector<RowChangeEventPtr> events;
		events.reserve(records.size());
		for (const Position* record : records)
		{
			events.push_back(MakeInsertEvent(record));
		}
		PublishRowBatchChange(events);
	}

	PositionDetailTable::PositionDetailTable()
	{
		PrimaryKey = new PositionDetailTablePrimaryKey(this);
		TradeMatchIndex = new PositionDetailTableIndexTradeMatch(this);
		TradingDayIndex = new PositionDetailTableIndexTradingDay(this);
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
		if (replicaSubscriber_ == nullptr)
		{
			return;
		}
		// 清表、拷贝、批量、就绪置位全部在一次读租约内完成：
		// 这期间的写入被阻塞，释放租约后到达的增量必然排在批量之后
		std::shared_lock guard(SharedMutex);
		replicaSubscriber_->OnSnapshotBegin(PositionDetail::TableId);
		if (!PrimaryKey->index_.empty())
		{
			std::vector<RowChangeEventPtr> events;
			events.reserve(PrimaryKey->index_.size());
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				events.push_back(MakeInsertEvent(*it));
			}
			replicaSubscriber_->OnRowBatchChange(events);
		}
		replicaSubscriber_->OnSnapshotComplete(PositionDetail::TableId);
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
		
		EmitRowInsert(record);
		return true;
	}
	void PositionDetailTable::BatchInsert(std::vector<PositionDetail*>& records)
	{
		std::lock_guard guard(SharedMutex);
		for (auto record : records)
		{
			[[maybe_unused]] const bool insertedIntoPrimaryKey = PrimaryKey->Insert(record);
			assert(insertedIntoPrimaryKey);

			TradeMatchIndex->Insert(record);
			TradingDayIndex->Insert(record);
		}
		// 通知在写锁内发出：放到解锁后会让并发 Update 的事件先于本批到达订阅者
		EmitRowBatchInsert(records);
		records.clear();
	}
	void PositionDetailTable::Erase(PositionDetail* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		EmitRowChange(RowChangeType::Erase, 0, true, AdoptRecord(record));
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
		if (HasSubscribers())
		{
			auto compareRecord = PositionDetail::Allocate();
			memcpy(compareRecord, &ComparePositionDetail, sizeof(PositionDetail));
			EmitRowChange(RowChangeType::EraseByIndex, PositionDetailTableIndexTradeMatch::IndexID, true, AdoptRecord(compareRecord));
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
		if (HasSubscribers())
		{
			auto compareRecord = PositionDetail::Allocate();
			memcpy(compareRecord, &ComparePositionDetail, sizeof(PositionDetail));
			EmitRowChange(RowChangeType::EraseByIndex, PositionDetailTableIndexTradingDay::IndexID, true, AdoptRecord(compareRecord));
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
		PositionDetailTableIndexTradeMatch::iterator itTradeMatch;
		if (TradeMatchIndexUpdate)
		{
			itTradeMatch = TradeMatchIndex->FindNode(oldRecord);
		}
		bool TradingDayIndexUpdate = TradingDayIndex->NeedUpdate(oldRecord, newRecord);
		PositionDetailTableIndexTradingDay::iterator itTradingDay;
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

		// updateDB 只表示"这条变更不落库"，行变更照常广播，由副本订阅者自行忽略
		EmitRowChange(RowChangeType::Update, 0, updateDB, AdoptRecord(newRecord));
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
		EmitRowChange(RowChangeType::Truncate, 0, true, RecordHandle());
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
	bool PositionDetailTable::HasSubscribers() const
	{
		return replicaSubscriber_ != nullptr || !subscribers_.empty();
	}
	RowChangeEventPtr PositionDetailTable::MakeInsertEvent(const PositionDetail* record)
	{
		auto eventRecord = PositionDetail::Allocate();
		memcpy(eventRecord, record, sizeof(PositionDetail));
		return std::make_shared<const RowChangeEvent>(PositionDetail::TableId, RowChangeType::Insert, 0, ++sequence_, true, AdoptRecord(eventRecord));
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
	void PositionDetailTable::EmitRowChange(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record)
	{
		if (!HasSubscribers())
		{
			return;
		}
		PublishRowChange(std::make_shared<const RowChangeEvent>(PositionDetail::TableId, type, indexId, ++sequence_, persistDatabase, std::move(record)));
	}
	void PositionDetailTable::EmitRowInsert(const PositionDetail* record)
	{
		if (!HasSubscribers())
		{
			return;
		}
		PublishRowChange(MakeInsertEvent(record));
	}
	void PositionDetailTable::EmitRowBatchInsert(const std::vector<PositionDetail*>& records)
	{
		if (!HasSubscribers())
		{
			return;
		}
		std::vector<RowChangeEventPtr> events;
		events.reserve(records.size());
		for (const PositionDetail* record : records)
		{
			events.push_back(MakeInsertEvent(record));
		}
		PublishRowBatchChange(events);
	}

	OrderTable::OrderTable()
	{
		PrimaryKey = new OrderTablePrimaryKey(this);
		ClientOrderIdUniqueKey = new OrderTableUniqueKeyClientOrderId(this);
	}
	OrderTable::~OrderTable()
	{
		delete PrimaryKey;
		PrimaryKey = nullptr;
		delete ClientOrderIdUniqueKey;
		ClientOrderIdUniqueKey = nullptr;
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
		if (replicaSubscriber_ == nullptr)
		{
			return;
		}
		// 清表、拷贝、批量、就绪置位全部在一次读租约内完成：
		// 这期间的写入被阻塞，释放租约后到达的增量必然排在批量之后
		std::shared_lock guard(SharedMutex);
		replicaSubscriber_->OnSnapshotBegin(Order::TableId);
		if (!PrimaryKey->index_.empty())
		{
			std::vector<RowChangeEventPtr> events;
			events.reserve(PrimaryKey->index_.size());
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				events.push_back(MakeInsertEvent(*it));
			}
			replicaSubscriber_->OnRowBatchChange(events);
		}
		replicaSubscriber_->OnSnapshotComplete(Order::TableId);
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

		
		EmitRowInsert(record);
		return true;
	}
	void OrderTable::BatchInsert(std::vector<Order*>& records)
	{
		std::lock_guard guard(SharedMutex);
		for (auto record : records)
		{
			[[maybe_unused]] const bool insertedIntoPrimaryKey = PrimaryKey->Insert(record);
			assert(insertedIntoPrimaryKey);
			[[maybe_unused]] const bool insertedIntoClientOrderIdUniqueKey = ClientOrderIdUniqueKey->Insert(record);
			assert(insertedIntoClientOrderIdUniqueKey);

		}
		// 通知在写锁内发出：放到解锁后会让并发 Update 的事件先于本批到达订阅者
		EmitRowBatchInsert(records);
		records.clear();
	}
	void OrderTable::Erase(Order* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		EmitRowChange(RowChangeType::Erase, 0, true, AdoptRecord(record));
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

		// updateDB 只表示"这条变更不落库"，行变更照常广播，由副本订阅者自行忽略
		EmitRowChange(RowChangeType::Update, 0, updateDB, AdoptRecord(newRecord));
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
		EmitRowChange(RowChangeType::Truncate, 0, true, RecordHandle());
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
	bool OrderTable::HasSubscribers() const
	{
		return replicaSubscriber_ != nullptr || !subscribers_.empty();
	}
	RowChangeEventPtr OrderTable::MakeInsertEvent(const Order* record)
	{
		auto eventRecord = Order::Allocate();
		memcpy(eventRecord, record, sizeof(Order));
		return std::make_shared<const RowChangeEvent>(Order::TableId, RowChangeType::Insert, 0, ++sequence_, true, AdoptRecord(eventRecord));
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
	void OrderTable::EmitRowChange(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record)
	{
		if (!HasSubscribers())
		{
			return;
		}
		PublishRowChange(std::make_shared<const RowChangeEvent>(Order::TableId, type, indexId, ++sequence_, persistDatabase, std::move(record)));
	}
	void OrderTable::EmitRowInsert(const Order* record)
	{
		if (!HasSubscribers())
		{
			return;
		}
		PublishRowChange(MakeInsertEvent(record));
	}
	void OrderTable::EmitRowBatchInsert(const std::vector<Order*>& records)
	{
		if (!HasSubscribers())
		{
			return;
		}
		std::vector<RowChangeEventPtr> events;
		events.reserve(records.size());
		for (const Order* record : records)
		{
			events.push_back(MakeInsertEvent(record));
		}
		PublishRowBatchChange(events);
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
		if (replicaSubscriber_ == nullptr)
		{
			return;
		}
		// 清表、拷贝、批量、就绪置位全部在一次读租约内完成：
		// 这期间的写入被阻塞，释放租约后到达的增量必然排在批量之后
		std::shared_lock guard(SharedMutex);
		replicaSubscriber_->OnSnapshotBegin(Trade::TableId);
		if (!PrimaryKey->index_.empty())
		{
			std::vector<RowChangeEventPtr> events;
			events.reserve(PrimaryKey->index_.size());
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				events.push_back(MakeInsertEvent(*it));
			}
			replicaSubscriber_->OnRowBatchChange(events);
		}
		replicaSubscriber_->OnSnapshotComplete(Trade::TableId);
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

		
		EmitRowInsert(record);
		return true;
	}
	void TradeTable::BatchInsert(std::vector<Trade*>& records)
	{
		std::lock_guard guard(SharedMutex);
		for (auto record : records)
		{
			[[maybe_unused]] const bool insertedIntoPrimaryKey = PrimaryKey->Insert(record);
			assert(insertedIntoPrimaryKey);

		}
		// 通知在写锁内发出：放到解锁后会让并发 Update 的事件先于本批到达订阅者
		EmitRowBatchInsert(records);
		records.clear();
	}
	void TradeTable::Erase(Trade* record)
	{
		std::lock_guard guard(SharedMutex);
		EraseUniqueKey(record);
		EraseIndex(record);
		EmitRowChange(RowChangeType::Erase, 0, true, AdoptRecord(record));
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

		// updateDB 只表示"这条变更不落库"，行变更照常广播，由副本订阅者自行忽略
		EmitRowChange(RowChangeType::Update, 0, updateDB, AdoptRecord(newRecord));
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
		EmitRowChange(RowChangeType::Truncate, 0, true, RecordHandle());
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
	bool TradeTable::HasSubscribers() const
	{
		return replicaSubscriber_ != nullptr || !subscribers_.empty();
	}
	RowChangeEventPtr TradeTable::MakeInsertEvent(const Trade* record)
	{
		auto eventRecord = Trade::Allocate();
		memcpy(eventRecord, record, sizeof(Trade));
		return std::make_shared<const RowChangeEvent>(Trade::TableId, RowChangeType::Insert, 0, ++sequence_, true, AdoptRecord(eventRecord));
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
	void TradeTable::EmitRowChange(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record)
	{
		if (!HasSubscribers())
		{
			return;
		}
		PublishRowChange(std::make_shared<const RowChangeEvent>(Trade::TableId, type, indexId, ++sequence_, persistDatabase, std::move(record)));
	}
	void TradeTable::EmitRowInsert(const Trade* record)
	{
		if (!HasSubscribers())
		{
			return;
		}
		PublishRowChange(MakeInsertEvent(record));
	}
	void TradeTable::EmitRowBatchInsert(const std::vector<Trade*>& records)
	{
		if (!HasSubscribers())
		{
			return;
		}
		std::vector<RowChangeEventPtr> events;
		events.reserve(records.size());
		for (const Trade* record : records)
		{
			events.push_back(MakeInsertEvent(record));
		}
		PublishRowBatchChange(events);
	}

}

