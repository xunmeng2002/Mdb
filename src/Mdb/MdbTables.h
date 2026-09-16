// 本文件由 ../Templates/Cpp/Mdb/MdbTables.h.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#pragma once
#include "MdbStructs.h"
#include "MdbTableBase.h"
#include <DbAdapters/DbInterface/MdbSubscriber.h>
#include "MdbPrimaryKeys.h"
#include "MdbIndexes.h"
#include <shared_mutex>
#include <atomic>
#include <vector>

using DbAdapters::MdbSubscriber;

namespace Mdb
{
	class TradingDayTable : public MdbTableBase
	{
	public:
		TradingDayTable();
		~TradingDayTable() override;
		virtual void Subscribe(MdbSubscriber* subscriber) override;
		virtual void UnSubscribe() override;
		void LockShared();
		void UnlockShared();
		virtual void InitDB() override;
		bool Insert(TradingDay* record);
		void BatchInsert(std::vector<mdb::TradingDay*>* records);
		void Erase(TradingDay* record);
		bool Update(TradingDay* const oldRecord, TradingDay* const newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

	private:
		void EraseUniqueKey(TradingDay* record);
		void EraseIndex(TradingDay* record);

	public:
		std::shared_mutex SharedMutex;
		TradingDayPrimaryKey* PrimaryKey;
	protected:
		MdbSubscriber* mdbSubscriber_;
	};

	class ExchangeTable : public MdbTableBase
	{
	public:
		ExchangeTable();
		~ExchangeTable() override;
		virtual void Subscribe(MdbSubscriber* subscriber) override;
		virtual void UnSubscribe() override;
		void LockShared();
		void UnlockShared();
		virtual void InitDB() override;
		bool Insert(Exchange* record);
		void BatchInsert(std::vector<mdb::Exchange*>* records);
		void Erase(Exchange* record);
		bool Update(Exchange* const oldRecord, Exchange* const newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

	private:
		void EraseUniqueKey(Exchange* record);
		void EraseIndex(Exchange* record);

	public:
		std::shared_mutex SharedMutex;
		ExchangePrimaryKey* PrimaryKey;
	protected:
		MdbSubscriber* mdbSubscriber_;
	};

	class ProductTable : public MdbTableBase
	{
	public:
		ProductTable();
		~ProductTable() override;
		virtual void Subscribe(MdbSubscriber* subscriber) override;
		virtual void UnSubscribe() override;
		void LockShared();
		void UnlockShared();
		virtual void InitDB() override;
		bool Insert(Product* record);
		void BatchInsert(std::vector<mdb::Product*>* records);
		void Erase(Product* record);
		bool Update(Product* const oldRecord, Product* const newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

	private:
		void EraseUniqueKey(Product* record);
		void EraseIndex(Product* record);

	public:
		std::shared_mutex SharedMutex;
		ProductPrimaryKey* PrimaryKey;
	protected:
		MdbSubscriber* mdbSubscriber_;
	};

	class InstrumentTable : public MdbTableBase
	{
	public:
		InstrumentTable();
		~InstrumentTable() override;
		virtual void Subscribe(MdbSubscriber* subscriber) override;
		virtual void UnSubscribe() override;
		void LockShared();
		void UnlockShared();
		virtual void InitDB() override;
		bool Insert(Instrument* record);
		void BatchInsert(std::vector<mdb::Instrument*>* records);
		void Erase(Instrument* record);
		bool Update(Instrument* const oldRecord, Instrument* const newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

	private:
		void EraseUniqueKey(Instrument* record);
		void EraseIndex(Instrument* record);

	public:
		std::shared_mutex SharedMutex;
		InstrumentPrimaryKey* PrimaryKey;
	protected:
		MdbSubscriber* mdbSubscriber_;
	};

	class PrimaryAccountTable : public MdbTableBase
	{
	public:
		PrimaryAccountTable();
		~PrimaryAccountTable() override;
		virtual void Subscribe(MdbSubscriber* subscriber) override;
		virtual void UnSubscribe() override;
		void LockShared();
		void UnlockShared();
		virtual void InitDB() override;
		bool Insert(PrimaryAccount* record);
		void BatchInsert(std::vector<mdb::PrimaryAccount*>* records);
		void Erase(PrimaryAccount* record);
		int EraseByOfferIdIndex(const OfferIdType& OfferId);
		bool Update(PrimaryAccount* const oldRecord, PrimaryAccount* const newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

	private:
		void EraseUniqueKey(PrimaryAccount* record);
		void EraseIndex(PrimaryAccount* record);

	public:
		std::shared_mutex SharedMutex;
		PrimaryAccountPrimaryKey* PrimaryKey;
		PrimaryAccountIndexOfferId* OfferIdIndex;
	protected:
		MdbSubscriber* mdbSubscriber_;
	};

	class AccountTable : public MdbTableBase
	{
	public:
		AccountTable();
		~AccountTable() override;
		virtual void Subscribe(MdbSubscriber* subscriber) override;
		virtual void UnSubscribe() override;
		void LockShared();
		void UnlockShared();
		virtual void InitDB() override;
		bool Insert(Account* record);
		void BatchInsert(std::vector<mdb::Account*>* records);
		void Erase(Account* record);
		bool Update(Account* const oldRecord, Account* const newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

	private:
		void EraseUniqueKey(Account* record);
		void EraseIndex(Account* record);

	public:
		std::shared_mutex SharedMutex;
		AccountPrimaryKey* PrimaryKey;
	protected:
		MdbSubscriber* mdbSubscriber_;
	};

	class CapitalTable : public MdbTableBase
	{
	public:
		CapitalTable();
		~CapitalTable() override;
		virtual void Subscribe(MdbSubscriber* subscriber) override;
		virtual void UnSubscribe() override;
		void LockShared();
		void UnlockShared();
		virtual void InitDB() override;
		bool Insert(Capital* record);
		void BatchInsert(std::vector<mdb::Capital*>* records);
		void Erase(Capital* record);
		int EraseByTradingDayIndex(const DateType& TradingDay);
		bool Update(Capital* const oldRecord, Capital* const newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

	private:
		void EraseUniqueKey(Capital* record);
		void EraseIndex(Capital* record);

	public:
		std::shared_mutex SharedMutex;
		CapitalPrimaryKey* PrimaryKey;
		CapitalIndexTradingDay* TradingDayIndex;
	protected:
		MdbSubscriber* mdbSubscriber_;
	};

	class PositionTable : public MdbTableBase
	{
	public:
		PositionTable();
		~PositionTable() override;
		virtual void Subscribe(MdbSubscriber* subscriber) override;
		virtual void UnSubscribe() override;
		void LockShared();
		void UnlockShared();
		virtual void InitDB() override;
		bool Insert(Position* record);
		void BatchInsert(std::vector<mdb::Position*>* records);
		void Erase(Position* record);
		int EraseByAccountIndex(const DateType& TradingDay, const AccountIdType& AccountId);
		int EraseByTradingDayIndex(const DateType& TradingDay);
		bool Update(Position* const oldRecord, Position* const newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

	private:
		void EraseUniqueKey(Position* record);
		void EraseIndex(Position* record);

	public:
		std::shared_mutex SharedMutex;
		PositionPrimaryKey* PrimaryKey;
		PositionIndexAccount* AccountIndex;
		PositionIndexTradingDay* TradingDayIndex;
	protected:
		MdbSubscriber* mdbSubscriber_;
	};

	class PositionDetailTable : public MdbTableBase
	{
	public:
		PositionDetailTable();
		~PositionDetailTable() override;
		virtual void Subscribe(MdbSubscriber* subscriber) override;
		virtual void UnSubscribe() override;
		void LockShared();
		void UnlockShared();
		virtual void InitDB() override;
		bool Insert(PositionDetail* record);
		void BatchInsert(std::vector<mdb::PositionDetail*>* records);
		void Erase(PositionDetail* record);
		int EraseByTradeMatchIndex(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const PosiDirectionType& PosiDirection);
		int EraseByTradingDayIndex(const DateType& TradingDay);
		bool Update(PositionDetail* const oldRecord, PositionDetail* const newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

	private:
		void EraseUniqueKey(PositionDetail* record);
		void EraseIndex(PositionDetail* record);

	public:
		std::shared_mutex SharedMutex;
		PositionDetailPrimaryKey* PrimaryKey;
		PositionDetailIndexTradeMatch* TradeMatchIndex;
		PositionDetailIndexTradingDay* TradingDayIndex;
	protected:
		MdbSubscriber* mdbSubscriber_;
	};

	class OrderTable : public MdbTableBase
	{
	public:
		OrderTable();
		~OrderTable() override;
		virtual void Subscribe(MdbSubscriber* subscriber) override;
		virtual void UnSubscribe() override;
		void LockShared();
		void UnlockShared();
		virtual void InitDB() override;
		bool Insert(Order* record);
		void BatchInsert(std::vector<mdb::Order*>* records);
		void Erase(Order* record);
		bool Update(Order* const oldRecord, Order* const newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

	private:
		void EraseUniqueKey(Order* record);
		void EraseIndex(Order* record);

	public:
		std::shared_mutex SharedMutex;
		OrderPrimaryKey* PrimaryKey;
		OrderUniqueKeyClientOrderId* ClientOrderIdUniqueKey;
	protected:
		MdbSubscriber* mdbSubscriber_;
	};

	class TradeTable : public MdbTableBase
	{
	public:
		TradeTable();
		~TradeTable() override;
		virtual void Subscribe(MdbSubscriber* subscriber) override;
		virtual void UnSubscribe() override;
		void LockShared();
		void UnlockShared();
		virtual void InitDB() override;
		bool Insert(Trade* record);
		void BatchInsert(std::vector<mdb::Trade*>* records);
		void Erase(Trade* record);
		bool Update(Trade* const oldRecord, Trade* const newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

	private:
		void EraseUniqueKey(Trade* record);
		void EraseIndex(Trade* record);

	public:
		std::shared_mutex SharedMutex;
		TradePrimaryKey* PrimaryKey;
	protected:
		MdbSubscriber* mdbSubscriber_;
	};

}
