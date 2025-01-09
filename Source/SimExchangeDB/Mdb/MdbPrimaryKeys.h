#pragma once
#include <unordered_set>
#include "MdbStructs.h"
#include "MdbPrimaryKeyComp.h"

namespace mdb
{
	class TradingDayTable;
	class TradingDayPrimaryKey
	{
		using iterator = std::unordered_set<TradingDay*, TradingDayHashForTradingDayPrimaryKey, TradingDayEqualForTradingDayPrimaryKey>::iterator;
		friend class TradingDayTable;
	public:
		TradingDayPrimaryKey(TradingDayTable* table, size_t buckets = 1000);
		TradingDay* Select(const IntType& PK);
		std::pair<iterator, iterator> SelectAll();
		
	protected:
		bool Insert(TradingDay* const record);
		void Erase(TradingDay* const record);
		bool CheckInsert(TradingDay* const record);
		bool CheckUpdate(const TradingDay* const oldRecord, const TradingDay* const newRecord);

	private:
		TradingDayTable* m_Table;
		std::unordered_set<TradingDay*, TradingDayHashForTradingDayPrimaryKey, TradingDayEqualForTradingDayPrimaryKey> m_Index;
	};

	class ExchangeTable;
	class ExchangePrimaryKey
	{
		using iterator = std::unordered_set<Exchange*, ExchangeHashForExchangePrimaryKey, ExchangeEqualForExchangePrimaryKey>::iterator;
		friend class ExchangeTable;
	public:
		ExchangePrimaryKey(ExchangeTable* table, size_t buckets = 1000);
		Exchange* Select(const ExchangeIDType& ExchangeID);
		std::pair<iterator, iterator> SelectAll();
		
	protected:
		bool Insert(Exchange* const record);
		void Erase(Exchange* const record);
		bool CheckInsert(Exchange* const record);
		bool CheckUpdate(const Exchange* const oldRecord, const Exchange* const newRecord);

	private:
		ExchangeTable* m_Table;
		std::unordered_set<Exchange*, ExchangeHashForExchangePrimaryKey, ExchangeEqualForExchangePrimaryKey> m_Index;
	};

	class ProductTable;
	class ProductPrimaryKey
	{
		using iterator = std::unordered_set<Product*, ProductHashForProductPrimaryKey, ProductEqualForProductPrimaryKey>::iterator;
		friend class ProductTable;
	public:
		ProductPrimaryKey(ProductTable* table, size_t buckets = 1000);
		Product* Select(const ExchangeIDType& ExchangeID, const ProductIDType& ProductID);
		std::pair<iterator, iterator> SelectAll();
		
	protected:
		bool Insert(Product* const record);
		void Erase(Product* const record);
		bool CheckInsert(Product* const record);
		bool CheckUpdate(const Product* const oldRecord, const Product* const newRecord);

	private:
		ProductTable* m_Table;
		std::unordered_set<Product*, ProductHashForProductPrimaryKey, ProductEqualForProductPrimaryKey> m_Index;
	};

	class InstrumentTable;
	class InstrumentPrimaryKey
	{
		using iterator = std::unordered_set<Instrument*, InstrumentHashForInstrumentPrimaryKey, InstrumentEqualForInstrumentPrimaryKey>::iterator;
		friend class InstrumentTable;
	public:
		InstrumentPrimaryKey(InstrumentTable* table, size_t buckets = 1000);
		Instrument* Select(const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID);
		std::pair<iterator, iterator> SelectAll();
		
	protected:
		bool Insert(Instrument* const record);
		void Erase(Instrument* const record);
		bool CheckInsert(Instrument* const record);
		bool CheckUpdate(const Instrument* const oldRecord, const Instrument* const newRecord);

	private:
		InstrumentTable* m_Table;
		std::unordered_set<Instrument*, InstrumentHashForInstrumentPrimaryKey, InstrumentEqualForInstrumentPrimaryKey> m_Index;
	};

	class AccountTable;
	class AccountPrimaryKey
	{
		using iterator = std::unordered_set<Account*, AccountHashForAccountPrimaryKey, AccountEqualForAccountPrimaryKey>::iterator;
		friend class AccountTable;
	public:
		AccountPrimaryKey(AccountTable* table, size_t buckets = 1000);
		Account* Select(const AccountIDType& AccountID);
		std::pair<iterator, iterator> SelectAll();
		
	protected:
		bool Insert(Account* const record);
		void Erase(Account* const record);
		bool CheckInsert(Account* const record);
		bool CheckUpdate(const Account* const oldRecord, const Account* const newRecord);

	private:
		AccountTable* m_Table;
		std::unordered_set<Account*, AccountHashForAccountPrimaryKey, AccountEqualForAccountPrimaryKey> m_Index;
	};

	class PositionTable;
	class PositionPrimaryKey
	{
		using iterator = std::unordered_set<Position*, PositionHashForPositionPrimaryKey, PositionEqualForPositionPrimaryKey>::iterator;
		friend class PositionTable;
	public:
		PositionPrimaryKey(PositionTable* table, size_t buckets = 1000);
		Position* Select(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection);
		std::pair<iterator, iterator> SelectAll();
		
	protected:
		bool Insert(Position* const record);
		void Erase(Position* const record);
		bool CheckInsert(Position* const record);
		bool CheckUpdate(const Position* const oldRecord, const Position* const newRecord);

	private:
		PositionTable* m_Table;
		std::unordered_set<Position*, PositionHashForPositionPrimaryKey, PositionEqualForPositionPrimaryKey> m_Index;
	};

	class OrderTable;
	class OrderPrimaryKey
	{
		using iterator = std::unordered_set<Order*, OrderHashForOrderPrimaryKey, OrderEqualForOrderPrimaryKey>::iterator;
		friend class OrderTable;
	public:
		OrderPrimaryKey(OrderTable* table, size_t buckets = 1000);
		Order* Select(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const OrderIDType& OrderID);
		std::pair<iterator, iterator> SelectAll();
		
	protected:
		bool Insert(Order* const record);
		void Erase(Order* const record);
		bool CheckInsert(Order* const record);
		bool CheckUpdate(const Order* const oldRecord, const Order* const newRecord);

	private:
		OrderTable* m_Table;
		std::unordered_set<Order*, OrderHashForOrderPrimaryKey, OrderEqualForOrderPrimaryKey> m_Index;
	};

	class OrderUniqueKeyClientOrderID
	{
		friend class OrderTable;
	public:
		OrderUniqueKeyClientOrderID(OrderTable* table, size_t buckets = 1000);
		Order* Select(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const OrderIDType& ClientOrderID);
		
	protected:
		bool Insert(Order* const record);
		void Erase(Order* const record);
		bool CheckInsert(Order* const record);
		bool CheckUpdate(const Order* const oldRecord, const Order* const newRecord);

	private:
		OrderTable* m_Table;
		std::unordered_set<Order*, OrderHashForClientOrderIDUniqueKey, OrderEqualForClientOrderIDUniqueKey> m_Index;
	};

	class TradeTable;
	class TradePrimaryKey
	{
		using iterator = std::unordered_set<Trade*, TradeHashForTradePrimaryKey, TradeEqualForTradePrimaryKey>::iterator;
		friend class TradeTable;
	public:
		TradePrimaryKey(TradeTable* table, size_t buckets = 1000);
		Trade* Select(const DateType& TradingDay, const ExchangeIDType& ExchangeID, const TradeIDType& TradeID, const DirectionType& Direction);
		std::pair<iterator, iterator> SelectAll();
		
	protected:
		bool Insert(Trade* const record);
		void Erase(Trade* const record);
		bool CheckInsert(Trade* const record);
		bool CheckUpdate(const Trade* const oldRecord, const Trade* const newRecord);

	private:
		TradeTable* m_Table;
		std::unordered_set<Trade*, TradeHashForTradePrimaryKey, TradeEqualForTradePrimaryKey> m_Index;
	};

	class MdTickTable;
	class MdTickPrimaryKey
	{
		using iterator = std::unordered_set<MdTick*, MdTickHashForMdTickPrimaryKey, MdTickEqualForMdTickPrimaryKey>::iterator;
		friend class MdTickTable;
	public:
		MdTickPrimaryKey(MdTickTable* table, size_t buckets = 1000);
		MdTick* Select(const DateType& TradingDay, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID);
		std::pair<iterator, iterator> SelectAll();
		
	protected:
		bool Insert(MdTick* const record);
		void Erase(MdTick* const record);
		bool CheckInsert(MdTick* const record);
		bool CheckUpdate(const MdTick* const oldRecord, const MdTick* const newRecord);

	private:
		MdTickTable* m_Table;
		std::unordered_set<MdTick*, MdTickHashForMdTickPrimaryKey, MdTickEqualForMdTickPrimaryKey> m_Index;
	};

}
