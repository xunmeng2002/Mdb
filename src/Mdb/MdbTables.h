#pragma once
#include "MdbStructs.h"
#include "MdbTableBase.h"
#include <DBAdapters/DBInterface/MdbSubscriber.h>
#include "MdbPrimaryKeys.h"
#include "MdbIndexes.h"
#include <shared_mutex>
#include <atomic>
#include <vector>

using dbadapters::MdbSubscriber;

namespace mdb
{
	class TradingDayTable : public MdbTableBase
	{
	public:
		TradingDayTable();
		~TradingDayTable() override;
		virtual void Subscribe(MdbSubscriber* mdbSubscriber) override;
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
		MdbSubscriber* m_MdbSubscriber;
		std::shared_mutex m_SharedMutex;
		TradingDayPrimaryKey* m_PrimaryKey;
	};

	class ExchangeTable : public MdbTableBase
	{
	public:
		ExchangeTable();
		~ExchangeTable() override;
		virtual void Subscribe(MdbSubscriber* mdbSubscriber) override;
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
		MdbSubscriber* m_MdbSubscriber;
		std::shared_mutex m_SharedMutex;
		ExchangePrimaryKey* m_PrimaryKey;
	};

	class ProductTable : public MdbTableBase
	{
	public:
		ProductTable();
		~ProductTable() override;
		virtual void Subscribe(MdbSubscriber* mdbSubscriber) override;
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
		MdbSubscriber* m_MdbSubscriber;
		std::shared_mutex m_SharedMutex;
		ProductPrimaryKey* m_PrimaryKey;
	};

	class InstrumentTable : public MdbTableBase
	{
	public:
		InstrumentTable();
		~InstrumentTable() override;
		virtual void Subscribe(MdbSubscriber* mdbSubscriber) override;
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
		MdbSubscriber* m_MdbSubscriber;
		std::shared_mutex m_SharedMutex;
		InstrumentPrimaryKey* m_PrimaryKey;
	};

	class PrimaryAccountTable : public MdbTableBase
	{
	public:
		PrimaryAccountTable();
		~PrimaryAccountTable() override;
		virtual void Subscribe(MdbSubscriber* mdbSubscriber) override;
		virtual void UnSubscribe() override;
		void LockShared();
		void UnlockShared();
		virtual void InitDB() override;
		bool Insert(PrimaryAccount* record);
		void BatchInsert(std::vector<mdb::PrimaryAccount*>* records);
		void Erase(PrimaryAccount* record);
		int EraseByOfferIDIndex(const OfferIDType& OfferID);
		bool Update(PrimaryAccount* const oldRecord, PrimaryAccount* const newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

	private:
		void EraseUniqueKey(PrimaryAccount* record);
		void EraseIndex(PrimaryAccount* record);

	public:
		MdbSubscriber* m_MdbSubscriber;
		std::shared_mutex m_SharedMutex;
		PrimaryAccountPrimaryKey* m_PrimaryKey;
		PrimaryAccountIndexOfferID* m_OfferIDIndex;
	};

	class AccountTable : public MdbTableBase
	{
	public:
		AccountTable();
		~AccountTable() override;
		virtual void Subscribe(MdbSubscriber* mdbSubscriber) override;
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
		MdbSubscriber* m_MdbSubscriber;
		std::shared_mutex m_SharedMutex;
		AccountPrimaryKey* m_PrimaryKey;
	};

	class CapitalTable : public MdbTableBase
	{
	public:
		CapitalTable();
		~CapitalTable() override;
		virtual void Subscribe(MdbSubscriber* mdbSubscriber) override;
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
		MdbSubscriber* m_MdbSubscriber;
		std::shared_mutex m_SharedMutex;
		CapitalPrimaryKey* m_PrimaryKey;
		CapitalIndexTradingDay* m_TradingDayIndex;
	};

	class PositionTable : public MdbTableBase
	{
	public:
		PositionTable();
		~PositionTable() override;
		virtual void Subscribe(MdbSubscriber* mdbSubscriber) override;
		virtual void UnSubscribe() override;
		void LockShared();
		void UnlockShared();
		virtual void InitDB() override;
		bool Insert(Position* record);
		void BatchInsert(std::vector<mdb::Position*>* records);
		void Erase(Position* record);
		int EraseByAccountIndex(const DateType& TradingDay, const AccountIDType& AccountID);
		int EraseByTradingDayIndex(const DateType& TradingDay);
		bool Update(Position* const oldRecord, Position* const newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

	private:
		void EraseUniqueKey(Position* record);
		void EraseIndex(Position* record);

	public:
		MdbSubscriber* m_MdbSubscriber;
		std::shared_mutex m_SharedMutex;
		PositionPrimaryKey* m_PrimaryKey;
		PositionIndexAccount* m_AccountIndex;
		PositionIndexTradingDay* m_TradingDayIndex;
	};

	class PositionDetailTable : public MdbTableBase
	{
	public:
		PositionDetailTable();
		~PositionDetailTable() override;
		virtual void Subscribe(MdbSubscriber* mdbSubscriber) override;
		virtual void UnSubscribe() override;
		void LockShared();
		void UnlockShared();
		virtual void InitDB() override;
		bool Insert(PositionDetail* record);
		void BatchInsert(std::vector<mdb::PositionDetail*>* records);
		void Erase(PositionDetail* record);
		int EraseByTradeMatchIndex(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection);
		int EraseByTradingDayIndex(const DateType& TradingDay);
		bool Update(PositionDetail* const oldRecord, PositionDetail* const newRecord, bool updateDB = true);
		virtual void TruncateTables() override;
		void TruncateTable();
		virtual void Dump(const char* dir) override;

	private:
		void EraseUniqueKey(PositionDetail* record);
		void EraseIndex(PositionDetail* record);

	public:
		MdbSubscriber* m_MdbSubscriber;
		std::shared_mutex m_SharedMutex;
		PositionDetailPrimaryKey* m_PrimaryKey;
		PositionDetailIndexTradeMatch* m_TradeMatchIndex;
		PositionDetailIndexTradingDay* m_TradingDayIndex;
	};

	class OrderTable : public MdbTableBase
	{
	public:
		OrderTable();
		~OrderTable() override;
		virtual void Subscribe(MdbSubscriber* mdbSubscriber) override;
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
		MdbSubscriber* m_MdbSubscriber;
		std::shared_mutex m_SharedMutex;
		OrderPrimaryKey* m_PrimaryKey;
		OrderUniqueKeyClientOrderID* m_ClientOrderIDUniqueKey;
	};

	class TradeTable : public MdbTableBase
	{
	public:
		TradeTable();
		~TradeTable() override;
		virtual void Subscribe(MdbSubscriber* mdbSubscriber) override;
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
		MdbSubscriber* m_MdbSubscriber;
		std::shared_mutex m_SharedMutex;
		TradePrimaryKey* m_PrimaryKey;
	};

}
