// 本文件由 ../Templates/Cpp/Mdb/MdbPrimaryKeys.h.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#pragma once
#include <optional>
#include <unordered_set>
#include "MdbStructs.h"
#include "MdbPrimaryKeyComp.h"
#include "MdbReadLease.h"

namespace Mdb
{
	class TradingDayTable;
	class TradingDayTablePrimaryKey
	{
	public:
		using iterator = std::unordered_set<TradingDay*, TradingDayHashForTradingDayPrimaryKey, TradingDayEqualForTradingDayPrimaryKey>::iterator;
		TradingDayTablePrimaryKey(TradingDayTable* tableOwner, size_t buckets = 1000);
		std::optional<TradingDay> Select(const Int32Type& Pk);
		TableRangeLease<iterator> SelectAll();

	protected:
		bool Insert(TradingDay* const record);
		void Erase(TradingDay* const record);
		bool CheckInsert(TradingDay* const record);
		// 调用方必须已持有 SharedMutex；返回表内活记录，不存在则为空
		TradingDay* Find(const TradingDay* record);

	private:
		friend class TradingDayTable;
		
		TradingDayTable* table_;
		std::unordered_set<TradingDay*, TradingDayHashForTradingDayPrimaryKey, TradingDayEqualForTradingDayPrimaryKey> index_;
	};
	class ExchangeTable;
	class ExchangeTablePrimaryKey
	{
	public:
		using iterator = std::unordered_set<Exchange*, ExchangeHashForExchangePrimaryKey, ExchangeEqualForExchangePrimaryKey>::iterator;
		ExchangeTablePrimaryKey(ExchangeTable* tableOwner, size_t buckets = 1000);
		std::optional<Exchange> Select(const ExchangeIdType& ExchangeId);
		TableRangeLease<iterator> SelectAll();

	protected:
		bool Insert(Exchange* const record);
		void Erase(Exchange* const record);
		bool CheckInsert(Exchange* const record);
		// 调用方必须已持有 SharedMutex；返回表内活记录，不存在则为空
		Exchange* Find(const Exchange* record);

	private:
		friend class ExchangeTable;
		
		ExchangeTable* table_;
		std::unordered_set<Exchange*, ExchangeHashForExchangePrimaryKey, ExchangeEqualForExchangePrimaryKey> index_;
	};
	class ProductTable;
	class ProductTablePrimaryKey
	{
	public:
		using iterator = std::unordered_set<Product*, ProductHashForProductPrimaryKey, ProductEqualForProductPrimaryKey>::iterator;
		ProductTablePrimaryKey(ProductTable* tableOwner, size_t buckets = 1000);
		std::optional<Product> Select(const ExchangeIdType& ExchangeId, const ProductIdType& ProductId);
		TableRangeLease<iterator> SelectAll();

	protected:
		bool Insert(Product* const record);
		void Erase(Product* const record);
		bool CheckInsert(Product* const record);
		// 调用方必须已持有 SharedMutex；返回表内活记录，不存在则为空
		Product* Find(const Product* record);

	private:
		friend class ProductTable;
		
		ProductTable* table_;
		std::unordered_set<Product*, ProductHashForProductPrimaryKey, ProductEqualForProductPrimaryKey> index_;
	};
	class InstrumentTable;
	class InstrumentTablePrimaryKey
	{
	public:
		using iterator = std::unordered_set<Instrument*, InstrumentHashForInstrumentPrimaryKey, InstrumentEqualForInstrumentPrimaryKey>::iterator;
		InstrumentTablePrimaryKey(InstrumentTable* tableOwner, size_t buckets = 1000);
		std::optional<Instrument> Select(const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId);
		TableRangeLease<iterator> SelectAll();

	protected:
		bool Insert(Instrument* const record);
		void Erase(Instrument* const record);
		bool CheckInsert(Instrument* const record);
		// 调用方必须已持有 SharedMutex；返回表内活记录，不存在则为空
		Instrument* Find(const Instrument* record);

	private:
		friend class InstrumentTable;
		
		InstrumentTable* table_;
		std::unordered_set<Instrument*, InstrumentHashForInstrumentPrimaryKey, InstrumentEqualForInstrumentPrimaryKey> index_;
	};
	class PrimaryAccountTable;
	class PrimaryAccountTablePrimaryKey
	{
	public:
		using iterator = std::unordered_set<PrimaryAccount*, PrimaryAccountHashForPrimaryAccountPrimaryKey, PrimaryAccountEqualForPrimaryAccountPrimaryKey>::iterator;
		PrimaryAccountTablePrimaryKey(PrimaryAccountTable* tableOwner, size_t buckets = 1000);
		std::optional<PrimaryAccount> Select(const AccountIdType& PrimaryAccountId);
		TableRangeLease<iterator> SelectAll();

	protected:
		bool Insert(PrimaryAccount* const record);
		void Erase(PrimaryAccount* const record);
		bool CheckInsert(PrimaryAccount* const record);
		// 调用方必须已持有 SharedMutex；返回表内活记录，不存在则为空
		PrimaryAccount* Find(const PrimaryAccount* record);

	private:
		friend class PrimaryAccountTable;
		
		PrimaryAccountTable* table_;
		std::unordered_set<PrimaryAccount*, PrimaryAccountHashForPrimaryAccountPrimaryKey, PrimaryAccountEqualForPrimaryAccountPrimaryKey> index_;
	};
	class AccountTable;
	class AccountTablePrimaryKey
	{
	public:
		using iterator = std::unordered_set<Account*, AccountHashForAccountPrimaryKey, AccountEqualForAccountPrimaryKey>::iterator;
		AccountTablePrimaryKey(AccountTable* tableOwner, size_t buckets = 1000);
		std::optional<Account> Select(const AccountIdType& AccountId);
		TableRangeLease<iterator> SelectAll();

	protected:
		bool Insert(Account* const record);
		void Erase(Account* const record);
		bool CheckInsert(Account* const record);
		// 调用方必须已持有 SharedMutex；返回表内活记录，不存在则为空
		Account* Find(const Account* record);

	private:
		friend class AccountTable;
		
		AccountTable* table_;
		std::unordered_set<Account*, AccountHashForAccountPrimaryKey, AccountEqualForAccountPrimaryKey> index_;
	};
	class CapitalTable;
	class CapitalTablePrimaryKey
	{
	public:
		using iterator = std::unordered_set<Capital*, CapitalHashForCapitalPrimaryKey, CapitalEqualForCapitalPrimaryKey>::iterator;
		CapitalTablePrimaryKey(CapitalTable* tableOwner, size_t buckets = 1000);
		std::optional<Capital> Select(const DateType& TradingDay, const AccountIdType& AccountId);
		TableRangeLease<iterator> SelectAll();

	protected:
		bool Insert(Capital* const record);
		void Erase(Capital* const record);
		bool CheckInsert(Capital* const record);
		// 调用方必须已持有 SharedMutex；返回表内活记录，不存在则为空
		Capital* Find(const Capital* record);

	private:
		friend class CapitalTable;
		
		CapitalTable* table_;
		std::unordered_set<Capital*, CapitalHashForCapitalPrimaryKey, CapitalEqualForCapitalPrimaryKey> index_;
	};
	class PositionTable;
	class PositionTablePrimaryKey
	{
	public:
		using iterator = std::unordered_set<Position*, PositionHashForPositionPrimaryKey, PositionEqualForPositionPrimaryKey>::iterator;
		PositionTablePrimaryKey(PositionTable* tableOwner, size_t buckets = 1000);
		std::optional<Position> Select(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const PosiDirectionType& PosiDirection);
		TableRangeLease<iterator> SelectAll();

	protected:
		bool Insert(Position* const record);
		void Erase(Position* const record);
		bool CheckInsert(Position* const record);
		// 调用方必须已持有 SharedMutex；返回表内活记录，不存在则为空
		Position* Find(const Position* record);

	private:
		friend class PositionTable;
		
		PositionTable* table_;
		std::unordered_set<Position*, PositionHashForPositionPrimaryKey, PositionEqualForPositionPrimaryKey> index_;
	};
	class PositionDetailTable;
	class PositionDetailTablePrimaryKey
	{
	public:
		using iterator = std::unordered_set<PositionDetail*, PositionDetailHashForPositionDetailPrimaryKey, PositionDetailEqualForPositionDetailPrimaryKey>::iterator;
		PositionDetailTablePrimaryKey(PositionDetailTable* tableOwner, size_t buckets = 1000);
		std::optional<PositionDetail> Select(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const PosiDirectionType& PosiDirection, const DateType& OpenDate, const TradeIdType& TradeId);
		TableRangeLease<iterator> SelectAll();

	protected:
		bool Insert(PositionDetail* const record);
		void Erase(PositionDetail* const record);
		bool CheckInsert(PositionDetail* const record);
		// 调用方必须已持有 SharedMutex；返回表内活记录，不存在则为空
		PositionDetail* Find(const PositionDetail* record);

	private:
		friend class PositionDetailTable;
		
		PositionDetailTable* table_;
		std::unordered_set<PositionDetail*, PositionDetailHashForPositionDetailPrimaryKey, PositionDetailEqualForPositionDetailPrimaryKey> index_;
	};
	class OrderTable;
	class OrderTablePrimaryKey
	{
	public:
		using iterator = std::unordered_set<Order*, OrderHashForOrderPrimaryKey, OrderEqualForOrderPrimaryKey>::iterator;
		OrderTablePrimaryKey(OrderTable* tableOwner, size_t buckets = 1000);
		std::optional<Order> Select(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const OrderIdType& OrderId);
		TableRangeLease<iterator> SelectAll();

	protected:
		bool Insert(Order* const record);
		void Erase(Order* const record);
		bool CheckInsert(Order* const record);
		// 调用方必须已持有 SharedMutex；返回表内活记录，不存在则为空
		Order* Find(const Order* record);

	private:
		friend class OrderTable;
		
		OrderTable* table_;
		std::unordered_set<Order*, OrderHashForOrderPrimaryKey, OrderEqualForOrderPrimaryKey> index_;
	};
	class OrderTableUniqueKeyClientOrderId
	{
	public:
		OrderTableUniqueKeyClientOrderId(OrderTable* tableOwner, size_t buckets = 1000);
		std::optional<Order> Select(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const SessionIdType& SessionId, const ClientOrderIdType& ClientOrderId);

	protected:
		bool Insert(Order* const record);
		void Erase(Order* const record);
		bool CheckInsert(Order* const record);
		bool CheckUpdate(const Order* const oldRecord, const Order* const newRecord);

	private:
		friend class OrderTable;
	
		OrderTable* table_;
		std::unordered_set<Order*, OrderHashForClientOrderIdUniqueKey, OrderEqualForClientOrderIdUniqueKey> index_;
	};
	
	class TradeTable;
	class TradeTablePrimaryKey
	{
	public:
		using iterator = std::unordered_set<Trade*, TradeHashForTradePrimaryKey, TradeEqualForTradePrimaryKey>::iterator;
		TradeTablePrimaryKey(TradeTable* tableOwner, size_t buckets = 1000);
		std::optional<Trade> Select(const DateType& TradingDay, const ExchangeIdType& ExchangeId, const TradeIdType& TradeId, const DirectionType& Direction);
		TableRangeLease<iterator> SelectAll();

	protected:
		bool Insert(Trade* const record);
		void Erase(Trade* const record);
		bool CheckInsert(Trade* const record);
		// 调用方必须已持有 SharedMutex；返回表内活记录，不存在则为空
		Trade* Find(const Trade* record);

	private:
		friend class TradeTable;
		
		TradeTable* table_;
		std::unordered_set<Trade*, TradeHashForTradePrimaryKey, TradeEqualForTradePrimaryKey> index_;
	};
}
