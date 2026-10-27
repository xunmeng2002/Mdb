// 本文件由 ../Templates/Cpp/Mdb/MdbTables.h.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#pragma once
#include "MdbStructs.h"
#include "MdbTableBase.h"
#include <DbAdapters/DbInterface/MdbSubscriber.h>
#include <DbAdapters/DbInterface/RowChangeEvent.h>
#include "MdbPrimaryKeys.h"
#include "MdbIndexes.h"
#include <cstdint>
#include <mutex>
#include <shared_mutex>
#include <vector>

using DbAdapters::MdbSubscriber;
using DbAdapters::MdbReplicaSubscriber;
using DbAdapters::RecordHandle;
using DbAdapters::RowChangeEventPtr;
using DbAdapters::RowChangeType;

namespace Mdb
{
	class TradingDayTable : public MdbTableBase
	{
	public:
		TradingDayTable();
		~TradingDayTable() override;
		virtual unsigned int GetTableId() const override;
		virtual void Subscribe(MdbSubscriber* subscriber) override;
		virtual void SubscribeReplica(MdbReplicaSubscriber* replica) override;
		virtual void UnSubscribe() override;
		virtual void InitDb() override;
		template <typename Visitor>
		void ForEach(Visitor&& visitor)
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				if (!visitor(*it))
				{
					break;
				}
			}
		}
		bool Insert(TradingDay* record);
		void BatchInsert(std::vector<TradingDay*>& records);
		// 按主键定位表内活记录删除，record 只提供主键
		bool Erase(const TradingDay& record);
		// 按主键定位表内活记录并整行覆盖，newRecord 只提供新值
		bool Update(const TradingDay& newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

		std::shared_mutex SharedMutex;
		TradingDayTablePrimaryKey* PrimaryKey;
	private:
		bool HasSubscribers() const;
		RowChangeEventPtr MakeInsertEvent(const TradingDay* record);
		// 单写者前提：Build* 在锁内按写入顺序取号，Publish* 在锁外调用订阅者；订阅者不得回写本表
		// BuildRowChangeEvent 接管 record：无订阅者时记录随句柄回池。Build* 调用方必须持有 SharedMutex。
		RowChangeEventPtr BuildRowChangeEvent(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record);
		RowChangeEventPtr BuildInsertEvent(const TradingDay* record);
		RowChangeEventPtr BuildUpdateEvent(const TradingDay& newRecord, bool updateDB);
		std::vector<RowChangeEventPtr> BuildBatchInsertEvents(const std::vector<TradingDay*>& records);
		void PublishRowChange(const RowChangeEventPtr& event);
		void PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events);
		void EraseUniqueKey(TradingDay* record);
		void EraseIndex(TradingDay* record);

		// 发布屏障：写事务与快照按交付顺序串行，读者不取此锁
		std::mutex publishMutex_;
		// 只在启动期注册，之后列表不可变，热路径无锁读
		std::vector<MdbSubscriber*> subscribers_;
		MdbReplicaSubscriber* replicaSubscriber_ = nullptr;
		// 只在持 publishMutex_ 时递增
		std::uint64_t sequence_ = 0;
	};

	class ExchangeTable : public MdbTableBase
	{
	public:
		ExchangeTable();
		~ExchangeTable() override;
		virtual unsigned int GetTableId() const override;
		virtual void Subscribe(MdbSubscriber* subscriber) override;
		virtual void SubscribeReplica(MdbReplicaSubscriber* replica) override;
		virtual void UnSubscribe() override;
		virtual void InitDb() override;
		template <typename Visitor>
		void ForEach(Visitor&& visitor)
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				if (!visitor(*it))
				{
					break;
				}
			}
		}
		bool Insert(Exchange* record);
		void BatchInsert(std::vector<Exchange*>& records);
		// 按主键定位表内活记录删除，record 只提供主键
		bool Erase(const Exchange& record);
		// 按主键定位表内活记录并整行覆盖，newRecord 只提供新值
		bool Update(const Exchange& newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

		std::shared_mutex SharedMutex;
		ExchangeTablePrimaryKey* PrimaryKey;
	private:
		bool HasSubscribers() const;
		RowChangeEventPtr MakeInsertEvent(const Exchange* record);
		// 单写者前提：Build* 在锁内按写入顺序取号，Publish* 在锁外调用订阅者；订阅者不得回写本表
		// BuildRowChangeEvent 接管 record：无订阅者时记录随句柄回池。Build* 调用方必须持有 SharedMutex。
		RowChangeEventPtr BuildRowChangeEvent(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record);
		RowChangeEventPtr BuildInsertEvent(const Exchange* record);
		RowChangeEventPtr BuildUpdateEvent(const Exchange& newRecord, bool updateDB);
		std::vector<RowChangeEventPtr> BuildBatchInsertEvents(const std::vector<Exchange*>& records);
		void PublishRowChange(const RowChangeEventPtr& event);
		void PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events);
		void EraseUniqueKey(Exchange* record);
		void EraseIndex(Exchange* record);

		// 发布屏障：写事务与快照按交付顺序串行，读者不取此锁
		std::mutex publishMutex_;
		// 只在启动期注册，之后列表不可变，热路径无锁读
		std::vector<MdbSubscriber*> subscribers_;
		MdbReplicaSubscriber* replicaSubscriber_ = nullptr;
		// 只在持 publishMutex_ 时递增
		std::uint64_t sequence_ = 0;
	};

	class ProductTable : public MdbTableBase
	{
	public:
		ProductTable();
		~ProductTable() override;
		virtual unsigned int GetTableId() const override;
		virtual void Subscribe(MdbSubscriber* subscriber) override;
		virtual void SubscribeReplica(MdbReplicaSubscriber* replica) override;
		virtual void UnSubscribe() override;
		virtual void InitDb() override;
		template <typename Visitor>
		void ForEach(Visitor&& visitor)
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				if (!visitor(*it))
				{
					break;
				}
			}
		}
		bool Insert(Product* record);
		void BatchInsert(std::vector<Product*>& records);
		// 按主键定位表内活记录删除，record 只提供主键
		bool Erase(const Product& record);
		// 按主键定位表内活记录并整行覆盖，newRecord 只提供新值
		bool Update(const Product& newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

		std::shared_mutex SharedMutex;
		ProductTablePrimaryKey* PrimaryKey;
	private:
		bool HasSubscribers() const;
		RowChangeEventPtr MakeInsertEvent(const Product* record);
		// 单写者前提：Build* 在锁内按写入顺序取号，Publish* 在锁外调用订阅者；订阅者不得回写本表
		// BuildRowChangeEvent 接管 record：无订阅者时记录随句柄回池。Build* 调用方必须持有 SharedMutex。
		RowChangeEventPtr BuildRowChangeEvent(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record);
		RowChangeEventPtr BuildInsertEvent(const Product* record);
		RowChangeEventPtr BuildUpdateEvent(const Product& newRecord, bool updateDB);
		std::vector<RowChangeEventPtr> BuildBatchInsertEvents(const std::vector<Product*>& records);
		void PublishRowChange(const RowChangeEventPtr& event);
		void PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events);
		void EraseUniqueKey(Product* record);
		void EraseIndex(Product* record);

		// 发布屏障：写事务与快照按交付顺序串行，读者不取此锁
		std::mutex publishMutex_;
		// 只在启动期注册，之后列表不可变，热路径无锁读
		std::vector<MdbSubscriber*> subscribers_;
		MdbReplicaSubscriber* replicaSubscriber_ = nullptr;
		// 只在持 publishMutex_ 时递增
		std::uint64_t sequence_ = 0;
	};

	class InstrumentTable : public MdbTableBase
	{
	public:
		InstrumentTable();
		~InstrumentTable() override;
		virtual unsigned int GetTableId() const override;
		virtual void Subscribe(MdbSubscriber* subscriber) override;
		virtual void SubscribeReplica(MdbReplicaSubscriber* replica) override;
		virtual void UnSubscribe() override;
		virtual void InitDb() override;
		template <typename Visitor>
		void ForEach(Visitor&& visitor)
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				if (!visitor(*it))
				{
					break;
				}
			}
		}
		bool Insert(Instrument* record);
		void BatchInsert(std::vector<Instrument*>& records);
		// 按主键定位表内活记录删除，record 只提供主键
		bool Erase(const Instrument& record);
		// 按主键定位表内活记录并整行覆盖，newRecord 只提供新值
		bool Update(const Instrument& newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

		std::shared_mutex SharedMutex;
		InstrumentTablePrimaryKey* PrimaryKey;
	private:
		bool HasSubscribers() const;
		RowChangeEventPtr MakeInsertEvent(const Instrument* record);
		// 单写者前提：Build* 在锁内按写入顺序取号，Publish* 在锁外调用订阅者；订阅者不得回写本表
		// BuildRowChangeEvent 接管 record：无订阅者时记录随句柄回池。Build* 调用方必须持有 SharedMutex。
		RowChangeEventPtr BuildRowChangeEvent(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record);
		RowChangeEventPtr BuildInsertEvent(const Instrument* record);
		RowChangeEventPtr BuildUpdateEvent(const Instrument& newRecord, bool updateDB);
		std::vector<RowChangeEventPtr> BuildBatchInsertEvents(const std::vector<Instrument*>& records);
		void PublishRowChange(const RowChangeEventPtr& event);
		void PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events);
		void EraseUniqueKey(Instrument* record);
		void EraseIndex(Instrument* record);

		// 发布屏障：写事务与快照按交付顺序串行，读者不取此锁
		std::mutex publishMutex_;
		// 只在启动期注册，之后列表不可变，热路径无锁读
		std::vector<MdbSubscriber*> subscribers_;
		MdbReplicaSubscriber* replicaSubscriber_ = nullptr;
		// 只在持 publishMutex_ 时递增
		std::uint64_t sequence_ = 0;
	};

	class PrimaryAccountTable : public MdbTableBase
	{
	public:
		PrimaryAccountTable();
		~PrimaryAccountTable() override;
		virtual unsigned int GetTableId() const override;
		virtual void Subscribe(MdbSubscriber* subscriber) override;
		virtual void SubscribeReplica(MdbReplicaSubscriber* replica) override;
		virtual void UnSubscribe() override;
		virtual void InitDb() override;
		template <typename Visitor>
		void ForEach(Visitor&& visitor)
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				if (!visitor(*it))
				{
					break;
				}
			}
		}
		bool Insert(PrimaryAccount* record);
		void BatchInsert(std::vector<PrimaryAccount*>& records);
		// 按主键定位表内活记录删除，record 只提供主键
		bool Erase(const PrimaryAccount& record);
		int EraseByOfferIdIndex(const OfferIdType& OfferId);
		// 按主键定位表内活记录并整行覆盖，newRecord 只提供新值
		bool Update(const PrimaryAccount& newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

		std::shared_mutex SharedMutex;
		PrimaryAccountTablePrimaryKey* PrimaryKey;
		PrimaryAccountTableIndexOfferId* OfferIdIndex;
	private:
		bool HasSubscribers() const;
		RowChangeEventPtr MakeInsertEvent(const PrimaryAccount* record);
		// 单写者前提：Build* 在锁内按写入顺序取号，Publish* 在锁外调用订阅者；订阅者不得回写本表
		// BuildRowChangeEvent 接管 record：无订阅者时记录随句柄回池。Build* 调用方必须持有 SharedMutex。
		RowChangeEventPtr BuildRowChangeEvent(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record);
		RowChangeEventPtr BuildInsertEvent(const PrimaryAccount* record);
		RowChangeEventPtr BuildUpdateEvent(const PrimaryAccount& newRecord, bool updateDB);
		std::vector<RowChangeEventPtr> BuildBatchInsertEvents(const std::vector<PrimaryAccount*>& records);
		void PublishRowChange(const RowChangeEventPtr& event);
		void PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events);
		void EraseUniqueKey(PrimaryAccount* record);
		void EraseIndex(PrimaryAccount* record);

		// 发布屏障：写事务与快照按交付顺序串行，读者不取此锁
		std::mutex publishMutex_;
		// 只在启动期注册，之后列表不可变，热路径无锁读
		std::vector<MdbSubscriber*> subscribers_;
		MdbReplicaSubscriber* replicaSubscriber_ = nullptr;
		// 只在持 publishMutex_ 时递增
		std::uint64_t sequence_ = 0;
	};

	class AccountTable : public MdbTableBase
	{
	public:
		AccountTable();
		~AccountTable() override;
		virtual unsigned int GetTableId() const override;
		virtual void Subscribe(MdbSubscriber* subscriber) override;
		virtual void SubscribeReplica(MdbReplicaSubscriber* replica) override;
		virtual void UnSubscribe() override;
		virtual void InitDb() override;
		template <typename Visitor>
		void ForEach(Visitor&& visitor)
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				if (!visitor(*it))
				{
					break;
				}
			}
		}
		bool Insert(Account* record);
		void BatchInsert(std::vector<Account*>& records);
		// 按主键定位表内活记录删除，record 只提供主键
		bool Erase(const Account& record);
		// 按主键定位表内活记录并整行覆盖，newRecord 只提供新值
		bool Update(const Account& newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

		std::shared_mutex SharedMutex;
		AccountTablePrimaryKey* PrimaryKey;
	private:
		bool HasSubscribers() const;
		RowChangeEventPtr MakeInsertEvent(const Account* record);
		// 单写者前提：Build* 在锁内按写入顺序取号，Publish* 在锁外调用订阅者；订阅者不得回写本表
		// BuildRowChangeEvent 接管 record：无订阅者时记录随句柄回池。Build* 调用方必须持有 SharedMutex。
		RowChangeEventPtr BuildRowChangeEvent(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record);
		RowChangeEventPtr BuildInsertEvent(const Account* record);
		RowChangeEventPtr BuildUpdateEvent(const Account& newRecord, bool updateDB);
		std::vector<RowChangeEventPtr> BuildBatchInsertEvents(const std::vector<Account*>& records);
		void PublishRowChange(const RowChangeEventPtr& event);
		void PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events);
		void EraseUniqueKey(Account* record);
		void EraseIndex(Account* record);

		// 发布屏障：写事务与快照按交付顺序串行，读者不取此锁
		std::mutex publishMutex_;
		// 只在启动期注册，之后列表不可变，热路径无锁读
		std::vector<MdbSubscriber*> subscribers_;
		MdbReplicaSubscriber* replicaSubscriber_ = nullptr;
		// 只在持 publishMutex_ 时递增
		std::uint64_t sequence_ = 0;
	};

	class CapitalTable : public MdbTableBase
	{
	public:
		CapitalTable();
		~CapitalTable() override;
		virtual unsigned int GetTableId() const override;
		virtual void Subscribe(MdbSubscriber* subscriber) override;
		virtual void SubscribeReplica(MdbReplicaSubscriber* replica) override;
		virtual void UnSubscribe() override;
		virtual void InitDb() override;
		template <typename Visitor>
		void ForEach(Visitor&& visitor)
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				if (!visitor(*it))
				{
					break;
				}
			}
		}
		bool Insert(Capital* record);
		void BatchInsert(std::vector<Capital*>& records);
		// 按主键定位表内活记录删除，record 只提供主键
		bool Erase(const Capital& record);
		int EraseByTradingDayIndex(const DateType& TradingDay);
		// 按主键定位表内活记录并整行覆盖，newRecord 只提供新值
		bool Update(const Capital& newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

		std::shared_mutex SharedMutex;
		CapitalTablePrimaryKey* PrimaryKey;
		CapitalTableIndexTradingDay* TradingDayIndex;
	private:
		bool HasSubscribers() const;
		RowChangeEventPtr MakeInsertEvent(const Capital* record);
		// 单写者前提：Build* 在锁内按写入顺序取号，Publish* 在锁外调用订阅者；订阅者不得回写本表
		// BuildRowChangeEvent 接管 record：无订阅者时记录随句柄回池。Build* 调用方必须持有 SharedMutex。
		RowChangeEventPtr BuildRowChangeEvent(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record);
		RowChangeEventPtr BuildInsertEvent(const Capital* record);
		RowChangeEventPtr BuildUpdateEvent(const Capital& newRecord, bool updateDB);
		std::vector<RowChangeEventPtr> BuildBatchInsertEvents(const std::vector<Capital*>& records);
		void PublishRowChange(const RowChangeEventPtr& event);
		void PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events);
		void EraseUniqueKey(Capital* record);
		void EraseIndex(Capital* record);

		// 发布屏障：写事务与快照按交付顺序串行，读者不取此锁
		std::mutex publishMutex_;
		// 只在启动期注册，之后列表不可变，热路径无锁读
		std::vector<MdbSubscriber*> subscribers_;
		MdbReplicaSubscriber* replicaSubscriber_ = nullptr;
		// 只在持 publishMutex_ 时递增
		std::uint64_t sequence_ = 0;
	};

	class PositionTable : public MdbTableBase
	{
	public:
		PositionTable();
		~PositionTable() override;
		virtual unsigned int GetTableId() const override;
		virtual void Subscribe(MdbSubscriber* subscriber) override;
		virtual void SubscribeReplica(MdbReplicaSubscriber* replica) override;
		virtual void UnSubscribe() override;
		virtual void InitDb() override;
		template <typename Visitor>
		void ForEach(Visitor&& visitor)
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				if (!visitor(*it))
				{
					break;
				}
			}
		}
		bool Insert(Position* record);
		void BatchInsert(std::vector<Position*>& records);
		// 按主键定位表内活记录删除，record 只提供主键
		bool Erase(const Position& record);
		int EraseByAccountIndex(const DateType& TradingDay, const AccountIdType& AccountId);
		int EraseByTradingDayIndex(const DateType& TradingDay);
		// 按主键定位表内活记录并整行覆盖，newRecord 只提供新值
		bool Update(const Position& newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

		std::shared_mutex SharedMutex;
		PositionTablePrimaryKey* PrimaryKey;
		PositionTableIndexAccount* AccountIndex;
		PositionTableIndexTradingDay* TradingDayIndex;
	private:
		bool HasSubscribers() const;
		RowChangeEventPtr MakeInsertEvent(const Position* record);
		// 单写者前提：Build* 在锁内按写入顺序取号，Publish* 在锁外调用订阅者；订阅者不得回写本表
		// BuildRowChangeEvent 接管 record：无订阅者时记录随句柄回池。Build* 调用方必须持有 SharedMutex。
		RowChangeEventPtr BuildRowChangeEvent(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record);
		RowChangeEventPtr BuildInsertEvent(const Position* record);
		RowChangeEventPtr BuildUpdateEvent(const Position& newRecord, bool updateDB);
		std::vector<RowChangeEventPtr> BuildBatchInsertEvents(const std::vector<Position*>& records);
		void PublishRowChange(const RowChangeEventPtr& event);
		void PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events);
		void EraseUniqueKey(Position* record);
		void EraseIndex(Position* record);

		// 发布屏障：写事务与快照按交付顺序串行，读者不取此锁
		std::mutex publishMutex_;
		// 只在启动期注册，之后列表不可变，热路径无锁读
		std::vector<MdbSubscriber*> subscribers_;
		MdbReplicaSubscriber* replicaSubscriber_ = nullptr;
		// 只在持 publishMutex_ 时递增
		std::uint64_t sequence_ = 0;
	};

	class PositionDetailTable : public MdbTableBase
	{
	public:
		PositionDetailTable();
		~PositionDetailTable() override;
		virtual unsigned int GetTableId() const override;
		virtual void Subscribe(MdbSubscriber* subscriber) override;
		virtual void SubscribeReplica(MdbReplicaSubscriber* replica) override;
		virtual void UnSubscribe() override;
		virtual void InitDb() override;
		template <typename Visitor>
		void ForEach(Visitor&& visitor)
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				if (!visitor(*it))
				{
					break;
				}
			}
		}
		bool Insert(PositionDetail* record);
		void BatchInsert(std::vector<PositionDetail*>& records);
		// 按主键定位表内活记录删除，record 只提供主键
		bool Erase(const PositionDetail& record);
		int EraseByTradeMatchIndex(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const PosiDirectionType& PosiDirection);
		int EraseByTradingDayIndex(const DateType& TradingDay);
		// 按主键定位表内活记录并整行覆盖，newRecord 只提供新值
		bool Update(const PositionDetail& newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

		std::shared_mutex SharedMutex;
		PositionDetailTablePrimaryKey* PrimaryKey;
		PositionDetailTableIndexTradeMatch* TradeMatchIndex;
		PositionDetailTableIndexTradingDay* TradingDayIndex;
	private:
		bool HasSubscribers() const;
		RowChangeEventPtr MakeInsertEvent(const PositionDetail* record);
		// 单写者前提：Build* 在锁内按写入顺序取号，Publish* 在锁外调用订阅者；订阅者不得回写本表
		// BuildRowChangeEvent 接管 record：无订阅者时记录随句柄回池。Build* 调用方必须持有 SharedMutex。
		RowChangeEventPtr BuildRowChangeEvent(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record);
		RowChangeEventPtr BuildInsertEvent(const PositionDetail* record);
		RowChangeEventPtr BuildUpdateEvent(const PositionDetail& newRecord, bool updateDB);
		std::vector<RowChangeEventPtr> BuildBatchInsertEvents(const std::vector<PositionDetail*>& records);
		void PublishRowChange(const RowChangeEventPtr& event);
		void PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events);
		void EraseUniqueKey(PositionDetail* record);
		void EraseIndex(PositionDetail* record);

		// 发布屏障：写事务与快照按交付顺序串行，读者不取此锁
		std::mutex publishMutex_;
		// 只在启动期注册，之后列表不可变，热路径无锁读
		std::vector<MdbSubscriber*> subscribers_;
		MdbReplicaSubscriber* replicaSubscriber_ = nullptr;
		// 只在持 publishMutex_ 时递增
		std::uint64_t sequence_ = 0;
	};

	class OrderTable : public MdbTableBase
	{
	public:
		OrderTable();
		~OrderTable() override;
		virtual unsigned int GetTableId() const override;
		virtual void Subscribe(MdbSubscriber* subscriber) override;
		virtual void SubscribeReplica(MdbReplicaSubscriber* replica) override;
		virtual void UnSubscribe() override;
		virtual void InitDb() override;
		template <typename Visitor>
		void ForEach(Visitor&& visitor)
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				if (!visitor(*it))
				{
					break;
				}
			}
		}
		bool Insert(Order* record);
		void BatchInsert(std::vector<Order*>& records);
		// 按主键定位表内活记录删除，record 只提供主键
		bool Erase(const Order& record);
		// 按主键定位表内活记录并整行覆盖，newRecord 只提供新值
		bool Update(const Order& newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

		std::shared_mutex SharedMutex;
		OrderTablePrimaryKey* PrimaryKey;
		OrderTableUniqueKeyClientOrderId* ClientOrderIdUniqueKey;
	private:
		bool HasSubscribers() const;
		RowChangeEventPtr MakeInsertEvent(const Order* record);
		// 单写者前提：Build* 在锁内按写入顺序取号，Publish* 在锁外调用订阅者；订阅者不得回写本表
		// BuildRowChangeEvent 接管 record：无订阅者时记录随句柄回池。Build* 调用方必须持有 SharedMutex。
		RowChangeEventPtr BuildRowChangeEvent(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record);
		RowChangeEventPtr BuildInsertEvent(const Order* record);
		RowChangeEventPtr BuildUpdateEvent(const Order& newRecord, bool updateDB);
		std::vector<RowChangeEventPtr> BuildBatchInsertEvents(const std::vector<Order*>& records);
		void PublishRowChange(const RowChangeEventPtr& event);
		void PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events);
		void EraseUniqueKey(Order* record);
		void EraseIndex(Order* record);

		// 发布屏障：写事务与快照按交付顺序串行，读者不取此锁
		std::mutex publishMutex_;
		// 只在启动期注册，之后列表不可变，热路径无锁读
		std::vector<MdbSubscriber*> subscribers_;
		MdbReplicaSubscriber* replicaSubscriber_ = nullptr;
		// 只在持 publishMutex_ 时递增
		std::uint64_t sequence_ = 0;
	};

	class TradeTable : public MdbTableBase
	{
	public:
		TradeTable();
		~TradeTable() override;
		virtual unsigned int GetTableId() const override;
		virtual void Subscribe(MdbSubscriber* subscriber) override;
		virtual void SubscribeReplica(MdbReplicaSubscriber* replica) override;
		virtual void UnSubscribe() override;
		virtual void InitDb() override;
		template <typename Visitor>
		void ForEach(Visitor&& visitor)
		{
			std::shared_lock guard(SharedMutex);
			for (auto it = PrimaryKey->index_.begin(); it != PrimaryKey->index_.end(); ++it)
			{
				if (!visitor(*it))
				{
					break;
				}
			}
		}
		bool Insert(Trade* record);
		void BatchInsert(std::vector<Trade*>& records);
		// 按主键定位表内活记录删除，record 只提供主键
		bool Erase(const Trade& record);
		// 按主键定位表内活记录并整行覆盖，newRecord 只提供新值
		bool Update(const Trade& newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

		std::shared_mutex SharedMutex;
		TradeTablePrimaryKey* PrimaryKey;
	private:
		bool HasSubscribers() const;
		RowChangeEventPtr MakeInsertEvent(const Trade* record);
		// 单写者前提：Build* 在锁内按写入顺序取号，Publish* 在锁外调用订阅者；订阅者不得回写本表
		// BuildRowChangeEvent 接管 record：无订阅者时记录随句柄回池。Build* 调用方必须持有 SharedMutex。
		RowChangeEventPtr BuildRowChangeEvent(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record);
		RowChangeEventPtr BuildInsertEvent(const Trade* record);
		RowChangeEventPtr BuildUpdateEvent(const Trade& newRecord, bool updateDB);
		std::vector<RowChangeEventPtr> BuildBatchInsertEvents(const std::vector<Trade*>& records);
		void PublishRowChange(const RowChangeEventPtr& event);
		void PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events);
		void EraseUniqueKey(Trade* record);
		void EraseIndex(Trade* record);

		// 发布屏障：写事务与快照按交付顺序串行，读者不取此锁
		std::mutex publishMutex_;
		// 只在启动期注册，之后列表不可变，热路径无锁读
		std::vector<MdbSubscriber*> subscribers_;
		MdbReplicaSubscriber* replicaSubscriber_ = nullptr;
		// 只在持 publishMutex_ 时递增
		std::uint64_t sequence_ = 0;
	};

}
