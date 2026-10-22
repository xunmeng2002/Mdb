// 本文件由 ../Templates/Cpp/Mdb/MdbTables.h.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#pragma once
#include "MdbStructs.h"
#include "MdbTableBase.h"
#include <DbAdapters/DbInterface/MdbSubscriber.h>
#include <DbAdapters/DbInterface/RowChangeEvent.h>
#include "MdbPrimaryKeys.h"
#include "MdbIndexes.h"
#include <cstdint>
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
		void LockShared();
		void UnlockShared();
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
		void Erase(TradingDay* record);
		bool Update(TradingDay* const oldRecord, TradingDay* const newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

		std::shared_mutex SharedMutex;
		TradingDayTablePrimaryKey* PrimaryKey;
	private:
		bool HasSubscribers() const;
		RowChangeEventPtr MakeInsertEvent(const TradingDay* record);
		void PublishRowChange(const RowChangeEventPtr& event);
		void PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events);
		// 接管 record：有订阅者则发布事件，否则记录随句柄回池。调用方必须持有 SharedMutex。
		void EmitRowChange(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record);
		void EmitRowInsert(const TradingDay* record);
		void EmitRowBatchInsert(const std::vector<TradingDay*>& records);
		void EraseUniqueKey(TradingDay* record);
		void EraseIndex(TradingDay* record);

		// 只在启动期注册，之后列表不可变，热路径无锁读
		std::vector<MdbSubscriber*> subscribers_;
		MdbReplicaSubscriber* replicaSubscriber_ = nullptr;
		// 受 SharedMutex 保护
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
		void LockShared();
		void UnlockShared();
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
		void Erase(Exchange* record);
		bool Update(Exchange* const oldRecord, Exchange* const newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

		std::shared_mutex SharedMutex;
		ExchangeTablePrimaryKey* PrimaryKey;
	private:
		bool HasSubscribers() const;
		RowChangeEventPtr MakeInsertEvent(const Exchange* record);
		void PublishRowChange(const RowChangeEventPtr& event);
		void PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events);
		// 接管 record：有订阅者则发布事件，否则记录随句柄回池。调用方必须持有 SharedMutex。
		void EmitRowChange(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record);
		void EmitRowInsert(const Exchange* record);
		void EmitRowBatchInsert(const std::vector<Exchange*>& records);
		void EraseUniqueKey(Exchange* record);
		void EraseIndex(Exchange* record);

		// 只在启动期注册，之后列表不可变，热路径无锁读
		std::vector<MdbSubscriber*> subscribers_;
		MdbReplicaSubscriber* replicaSubscriber_ = nullptr;
		// 受 SharedMutex 保护
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
		void LockShared();
		void UnlockShared();
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
		void Erase(Product* record);
		bool Update(Product* const oldRecord, Product* const newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

		std::shared_mutex SharedMutex;
		ProductTablePrimaryKey* PrimaryKey;
	private:
		bool HasSubscribers() const;
		RowChangeEventPtr MakeInsertEvent(const Product* record);
		void PublishRowChange(const RowChangeEventPtr& event);
		void PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events);
		// 接管 record：有订阅者则发布事件，否则记录随句柄回池。调用方必须持有 SharedMutex。
		void EmitRowChange(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record);
		void EmitRowInsert(const Product* record);
		void EmitRowBatchInsert(const std::vector<Product*>& records);
		void EraseUniqueKey(Product* record);
		void EraseIndex(Product* record);

		// 只在启动期注册，之后列表不可变，热路径无锁读
		std::vector<MdbSubscriber*> subscribers_;
		MdbReplicaSubscriber* replicaSubscriber_ = nullptr;
		// 受 SharedMutex 保护
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
		void LockShared();
		void UnlockShared();
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
		void Erase(Instrument* record);
		bool Update(Instrument* const oldRecord, Instrument* const newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

		std::shared_mutex SharedMutex;
		InstrumentTablePrimaryKey* PrimaryKey;
	private:
		bool HasSubscribers() const;
		RowChangeEventPtr MakeInsertEvent(const Instrument* record);
		void PublishRowChange(const RowChangeEventPtr& event);
		void PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events);
		// 接管 record：有订阅者则发布事件，否则记录随句柄回池。调用方必须持有 SharedMutex。
		void EmitRowChange(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record);
		void EmitRowInsert(const Instrument* record);
		void EmitRowBatchInsert(const std::vector<Instrument*>& records);
		void EraseUniqueKey(Instrument* record);
		void EraseIndex(Instrument* record);

		// 只在启动期注册，之后列表不可变，热路径无锁读
		std::vector<MdbSubscriber*> subscribers_;
		MdbReplicaSubscriber* replicaSubscriber_ = nullptr;
		// 受 SharedMutex 保护
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
		void LockShared();
		void UnlockShared();
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
		void Erase(PrimaryAccount* record);
		int EraseByOfferIdIndex(const OfferIdType& OfferId);
		bool Update(PrimaryAccount* const oldRecord, PrimaryAccount* const newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

		std::shared_mutex SharedMutex;
		PrimaryAccountTablePrimaryKey* PrimaryKey;
		PrimaryAccountTableIndexOfferId* OfferIdIndex;
	private:
		bool HasSubscribers() const;
		RowChangeEventPtr MakeInsertEvent(const PrimaryAccount* record);
		void PublishRowChange(const RowChangeEventPtr& event);
		void PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events);
		// 接管 record：有订阅者则发布事件，否则记录随句柄回池。调用方必须持有 SharedMutex。
		void EmitRowChange(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record);
		void EmitRowInsert(const PrimaryAccount* record);
		void EmitRowBatchInsert(const std::vector<PrimaryAccount*>& records);
		void EraseUniqueKey(PrimaryAccount* record);
		void EraseIndex(PrimaryAccount* record);

		// 只在启动期注册，之后列表不可变，热路径无锁读
		std::vector<MdbSubscriber*> subscribers_;
		MdbReplicaSubscriber* replicaSubscriber_ = nullptr;
		// 受 SharedMutex 保护
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
		void LockShared();
		void UnlockShared();
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
		void Erase(Account* record);
		bool Update(Account* const oldRecord, Account* const newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

		std::shared_mutex SharedMutex;
		AccountTablePrimaryKey* PrimaryKey;
	private:
		bool HasSubscribers() const;
		RowChangeEventPtr MakeInsertEvent(const Account* record);
		void PublishRowChange(const RowChangeEventPtr& event);
		void PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events);
		// 接管 record：有订阅者则发布事件，否则记录随句柄回池。调用方必须持有 SharedMutex。
		void EmitRowChange(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record);
		void EmitRowInsert(const Account* record);
		void EmitRowBatchInsert(const std::vector<Account*>& records);
		void EraseUniqueKey(Account* record);
		void EraseIndex(Account* record);

		// 只在启动期注册，之后列表不可变，热路径无锁读
		std::vector<MdbSubscriber*> subscribers_;
		MdbReplicaSubscriber* replicaSubscriber_ = nullptr;
		// 受 SharedMutex 保护
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
		void LockShared();
		void UnlockShared();
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
		void Erase(Capital* record);
		int EraseByTradingDayIndex(const DateType& TradingDay);
		bool Update(Capital* const oldRecord, Capital* const newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

		std::shared_mutex SharedMutex;
		CapitalTablePrimaryKey* PrimaryKey;
		CapitalTableIndexTradingDay* TradingDayIndex;
	private:
		bool HasSubscribers() const;
		RowChangeEventPtr MakeInsertEvent(const Capital* record);
		void PublishRowChange(const RowChangeEventPtr& event);
		void PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events);
		// 接管 record：有订阅者则发布事件，否则记录随句柄回池。调用方必须持有 SharedMutex。
		void EmitRowChange(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record);
		void EmitRowInsert(const Capital* record);
		void EmitRowBatchInsert(const std::vector<Capital*>& records);
		void EraseUniqueKey(Capital* record);
		void EraseIndex(Capital* record);

		// 只在启动期注册，之后列表不可变，热路径无锁读
		std::vector<MdbSubscriber*> subscribers_;
		MdbReplicaSubscriber* replicaSubscriber_ = nullptr;
		// 受 SharedMutex 保护
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
		void LockShared();
		void UnlockShared();
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
		void Erase(Position* record);
		int EraseByAccountIndex(const DateType& TradingDay, const AccountIdType& AccountId);
		int EraseByTradingDayIndex(const DateType& TradingDay);
		bool Update(Position* const oldRecord, Position* const newRecord, bool updateDB = true);
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
		void PublishRowChange(const RowChangeEventPtr& event);
		void PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events);
		// 接管 record：有订阅者则发布事件，否则记录随句柄回池。调用方必须持有 SharedMutex。
		void EmitRowChange(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record);
		void EmitRowInsert(const Position* record);
		void EmitRowBatchInsert(const std::vector<Position*>& records);
		void EraseUniqueKey(Position* record);
		void EraseIndex(Position* record);

		// 只在启动期注册，之后列表不可变，热路径无锁读
		std::vector<MdbSubscriber*> subscribers_;
		MdbReplicaSubscriber* replicaSubscriber_ = nullptr;
		// 受 SharedMutex 保护
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
		void LockShared();
		void UnlockShared();
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
		void Erase(PositionDetail* record);
		int EraseByTradeMatchIndex(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const PosiDirectionType& PosiDirection);
		int EraseByTradingDayIndex(const DateType& TradingDay);
		bool Update(PositionDetail* const oldRecord, PositionDetail* const newRecord, bool updateDB = true);
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
		void PublishRowChange(const RowChangeEventPtr& event);
		void PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events);
		// 接管 record：有订阅者则发布事件，否则记录随句柄回池。调用方必须持有 SharedMutex。
		void EmitRowChange(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record);
		void EmitRowInsert(const PositionDetail* record);
		void EmitRowBatchInsert(const std::vector<PositionDetail*>& records);
		void EraseUniqueKey(PositionDetail* record);
		void EraseIndex(PositionDetail* record);

		// 只在启动期注册，之后列表不可变，热路径无锁读
		std::vector<MdbSubscriber*> subscribers_;
		MdbReplicaSubscriber* replicaSubscriber_ = nullptr;
		// 受 SharedMutex 保护
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
		void LockShared();
		void UnlockShared();
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
		void Erase(Order* record);
		bool Update(Order* const oldRecord, Order* const newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

		std::shared_mutex SharedMutex;
		OrderTablePrimaryKey* PrimaryKey;
		OrderTableUniqueKeyClientOrderId* ClientOrderIdUniqueKey;
	private:
		bool HasSubscribers() const;
		RowChangeEventPtr MakeInsertEvent(const Order* record);
		void PublishRowChange(const RowChangeEventPtr& event);
		void PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events);
		// 接管 record：有订阅者则发布事件，否则记录随句柄回池。调用方必须持有 SharedMutex。
		void EmitRowChange(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record);
		void EmitRowInsert(const Order* record);
		void EmitRowBatchInsert(const std::vector<Order*>& records);
		void EraseUniqueKey(Order* record);
		void EraseIndex(Order* record);

		// 只在启动期注册，之后列表不可变，热路径无锁读
		std::vector<MdbSubscriber*> subscribers_;
		MdbReplicaSubscriber* replicaSubscriber_ = nullptr;
		// 受 SharedMutex 保护
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
		void LockShared();
		void UnlockShared();
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
		void Erase(Trade* record);
		bool Update(Trade* const oldRecord, Trade* const newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

		std::shared_mutex SharedMutex;
		TradeTablePrimaryKey* PrimaryKey;
	private:
		bool HasSubscribers() const;
		RowChangeEventPtr MakeInsertEvent(const Trade* record);
		void PublishRowChange(const RowChangeEventPtr& event);
		void PublishRowBatchChange(const std::vector<RowChangeEventPtr>& events);
		// 接管 record：有订阅者则发布事件，否则记录随句柄回池。调用方必须持有 SharedMutex。
		void EmitRowChange(RowChangeType type, unsigned int indexId, bool persistDatabase, RecordHandle record);
		void EmitRowInsert(const Trade* record);
		void EmitRowBatchInsert(const std::vector<Trade*>& records);
		void EraseUniqueKey(Trade* record);
		void EraseIndex(Trade* record);

		// 只在启动期注册，之后列表不可变，热路径无锁读
		std::vector<MdbSubscriber*> subscribers_;
		MdbReplicaSubscriber* replicaSubscriber_ = nullptr;
		// 受 SharedMutex 保护
		std::uint64_t sequence_ = 0;
	};

}
