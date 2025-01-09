#include "MdbPrimaryKeys.h"
#include "MdbTables.h"
#include "Utility.h"
#include <string.h>

using std::unordered_set;


namespace mdb
{
	TradingDayPrimaryKey::TradingDayPrimaryKey(TradingDayTable* table, size_t buckets)
		:m_Table(table), m_Index(buckets)
	{
	}
	TradingDay* TradingDayPrimaryKey::Select(const IntType& PK)
	{
		t_CompareTradingDay.PK = PK;
		
		std::shared_lock guard(m_Table->m_SharedMutex);
		auto it = m_Index.find(&t_CompareTradingDay);
		if (it == m_Index.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<TradingDayPrimaryKey::iterator, TradingDayPrimaryKey::iterator> TradingDayPrimaryKey::SelectAll()
	{
		std::shared_lock guard(m_Table->m_SharedMutex);
		return std::pair<iterator, iterator>(m_Index.begin(), m_Index.end());
	}
	bool TradingDayPrimaryKey::Insert(TradingDay* const record)
	{
		return m_Index.insert(record).second;
	}
	void TradingDayPrimaryKey::Erase(TradingDay* const  record)
	{
		m_Index.erase(record);
	}
	bool TradingDayPrimaryKey::CheckInsert(TradingDay* const record)
	{
		return m_Index.find(record) == m_Index.end();
	}
	bool TradingDayPrimaryKey::CheckUpdate(const TradingDay* const oldRecord, const TradingDay* const newRecord)
	{
		return TradingDayEqualForTradingDayPrimaryKey()(oldRecord, newRecord);
	}

	ExchangePrimaryKey::ExchangePrimaryKey(ExchangeTable* table, size_t buckets)
		:m_Table(table), m_Index(buckets)
	{
	}
	Exchange* ExchangePrimaryKey::Select(const ExchangeIDType& ExchangeID)
	{
		Strcpy(t_CompareExchange.ExchangeID, ExchangeID);
		
		std::shared_lock guard(m_Table->m_SharedMutex);
		auto it = m_Index.find(&t_CompareExchange);
		if (it == m_Index.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<ExchangePrimaryKey::iterator, ExchangePrimaryKey::iterator> ExchangePrimaryKey::SelectAll()
	{
		std::shared_lock guard(m_Table->m_SharedMutex);
		return std::pair<iterator, iterator>(m_Index.begin(), m_Index.end());
	}
	bool ExchangePrimaryKey::Insert(Exchange* const record)
	{
		return m_Index.insert(record).second;
	}
	void ExchangePrimaryKey::Erase(Exchange* const  record)
	{
		m_Index.erase(record);
	}
	bool ExchangePrimaryKey::CheckInsert(Exchange* const record)
	{
		return m_Index.find(record) == m_Index.end();
	}
	bool ExchangePrimaryKey::CheckUpdate(const Exchange* const oldRecord, const Exchange* const newRecord)
	{
		return ExchangeEqualForExchangePrimaryKey()(oldRecord, newRecord);
	}

	ProductPrimaryKey::ProductPrimaryKey(ProductTable* table, size_t buckets)
		:m_Table(table), m_Index(buckets)
	{
	}
	Product* ProductPrimaryKey::Select(const ExchangeIDType& ExchangeID, const ProductIDType& ProductID)
	{
		Strcpy(t_CompareProduct.ExchangeID, ExchangeID);
		Strcpy(t_CompareProduct.ProductID, ProductID);
		
		std::shared_lock guard(m_Table->m_SharedMutex);
		auto it = m_Index.find(&t_CompareProduct);
		if (it == m_Index.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<ProductPrimaryKey::iterator, ProductPrimaryKey::iterator> ProductPrimaryKey::SelectAll()
	{
		std::shared_lock guard(m_Table->m_SharedMutex);
		return std::pair<iterator, iterator>(m_Index.begin(), m_Index.end());
	}
	bool ProductPrimaryKey::Insert(Product* const record)
	{
		return m_Index.insert(record).second;
	}
	void ProductPrimaryKey::Erase(Product* const  record)
	{
		m_Index.erase(record);
	}
	bool ProductPrimaryKey::CheckInsert(Product* const record)
	{
		return m_Index.find(record) == m_Index.end();
	}
	bool ProductPrimaryKey::CheckUpdate(const Product* const oldRecord, const Product* const newRecord)
	{
		return ProductEqualForProductPrimaryKey()(oldRecord, newRecord);
	}

	InstrumentPrimaryKey::InstrumentPrimaryKey(InstrumentTable* table, size_t buckets)
		:m_Table(table), m_Index(buckets)
	{
	}
	Instrument* InstrumentPrimaryKey::Select(const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID)
	{
		Strcpy(t_CompareInstrument.ExchangeID, ExchangeID);
		Strcpy(t_CompareInstrument.InstrumentID, InstrumentID);
		
		std::shared_lock guard(m_Table->m_SharedMutex);
		auto it = m_Index.find(&t_CompareInstrument);
		if (it == m_Index.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<InstrumentPrimaryKey::iterator, InstrumentPrimaryKey::iterator> InstrumentPrimaryKey::SelectAll()
	{
		std::shared_lock guard(m_Table->m_SharedMutex);
		return std::pair<iterator, iterator>(m_Index.begin(), m_Index.end());
	}
	bool InstrumentPrimaryKey::Insert(Instrument* const record)
	{
		return m_Index.insert(record).second;
	}
	void InstrumentPrimaryKey::Erase(Instrument* const  record)
	{
		m_Index.erase(record);
	}
	bool InstrumentPrimaryKey::CheckInsert(Instrument* const record)
	{
		return m_Index.find(record) == m_Index.end();
	}
	bool InstrumentPrimaryKey::CheckUpdate(const Instrument* const oldRecord, const Instrument* const newRecord)
	{
		return InstrumentEqualForInstrumentPrimaryKey()(oldRecord, newRecord);
	}

	AccountPrimaryKey::AccountPrimaryKey(AccountTable* table, size_t buckets)
		:m_Table(table), m_Index(buckets)
	{
	}
	Account* AccountPrimaryKey::Select(const AccountIDType& AccountID)
	{
		Strcpy(t_CompareAccount.AccountID, AccountID);
		
		std::shared_lock guard(m_Table->m_SharedMutex);
		auto it = m_Index.find(&t_CompareAccount);
		if (it == m_Index.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<AccountPrimaryKey::iterator, AccountPrimaryKey::iterator> AccountPrimaryKey::SelectAll()
	{
		std::shared_lock guard(m_Table->m_SharedMutex);
		return std::pair<iterator, iterator>(m_Index.begin(), m_Index.end());
	}
	bool AccountPrimaryKey::Insert(Account* const record)
	{
		return m_Index.insert(record).second;
	}
	void AccountPrimaryKey::Erase(Account* const  record)
	{
		m_Index.erase(record);
	}
	bool AccountPrimaryKey::CheckInsert(Account* const record)
	{
		return m_Index.find(record) == m_Index.end();
	}
	bool AccountPrimaryKey::CheckUpdate(const Account* const oldRecord, const Account* const newRecord)
	{
		return AccountEqualForAccountPrimaryKey()(oldRecord, newRecord);
	}

	PositionPrimaryKey::PositionPrimaryKey(PositionTable* table, size_t buckets)
		:m_Table(table), m_Index(buckets)
	{
	}
	Position* PositionPrimaryKey::Select(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection)
	{
		Strcpy(t_ComparePosition.TradingDay, TradingDay);
		Strcpy(t_ComparePosition.AccountID, AccountID);
		Strcpy(t_ComparePosition.ExchangeID, ExchangeID);
		Strcpy(t_ComparePosition.InstrumentID, InstrumentID);
		t_ComparePosition.PosiDirection = PosiDirection;
		
		std::shared_lock guard(m_Table->m_SharedMutex);
		auto it = m_Index.find(&t_ComparePosition);
		if (it == m_Index.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<PositionPrimaryKey::iterator, PositionPrimaryKey::iterator> PositionPrimaryKey::SelectAll()
	{
		std::shared_lock guard(m_Table->m_SharedMutex);
		return std::pair<iterator, iterator>(m_Index.begin(), m_Index.end());
	}
	bool PositionPrimaryKey::Insert(Position* const record)
	{
		return m_Index.insert(record).second;
	}
	void PositionPrimaryKey::Erase(Position* const  record)
	{
		m_Index.erase(record);
	}
	bool PositionPrimaryKey::CheckInsert(Position* const record)
	{
		return m_Index.find(record) == m_Index.end();
	}
	bool PositionPrimaryKey::CheckUpdate(const Position* const oldRecord, const Position* const newRecord)
	{
		return PositionEqualForPositionPrimaryKey()(oldRecord, newRecord);
	}

	OrderPrimaryKey::OrderPrimaryKey(OrderTable* table, size_t buckets)
		:m_Table(table), m_Index(buckets)
	{
	}
	Order* OrderPrimaryKey::Select(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const OrderIDType& OrderID)
	{
		Strcpy(t_CompareOrder.TradingDay, TradingDay);
		Strcpy(t_CompareOrder.AccountID, AccountID);
		Strcpy(t_CompareOrder.ExchangeID, ExchangeID);
		Strcpy(t_CompareOrder.InstrumentID, InstrumentID);
		t_CompareOrder.OrderID = OrderID;
		
		std::shared_lock guard(m_Table->m_SharedMutex);
		auto it = m_Index.find(&t_CompareOrder);
		if (it == m_Index.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<OrderPrimaryKey::iterator, OrderPrimaryKey::iterator> OrderPrimaryKey::SelectAll()
	{
		std::shared_lock guard(m_Table->m_SharedMutex);
		return std::pair<iterator, iterator>(m_Index.begin(), m_Index.end());
	}
	bool OrderPrimaryKey::Insert(Order* const record)
	{
		return m_Index.insert(record).second;
	}
	void OrderPrimaryKey::Erase(Order* const  record)
	{
		m_Index.erase(record);
	}
	bool OrderPrimaryKey::CheckInsert(Order* const record)
	{
		return m_Index.find(record) == m_Index.end();
	}
	bool OrderPrimaryKey::CheckUpdate(const Order* const oldRecord, const Order* const newRecord)
	{
		return OrderEqualForOrderPrimaryKey()(oldRecord, newRecord);
	}

	OrderUniqueKeyClientOrderID::OrderUniqueKeyClientOrderID(OrderTable* table, size_t buckets)
		:m_Table(table), m_Index(buckets)
	{
	}
	Order* OrderUniqueKeyClientOrderID::Select(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const OrderIDType& ClientOrderID)
	{
		Strcpy(t_CompareOrder.TradingDay, TradingDay);
		Strcpy(t_CompareOrder.AccountID, AccountID);
		Strcpy(t_CompareOrder.ExchangeID, ExchangeID);
		Strcpy(t_CompareOrder.InstrumentID, InstrumentID);
		t_CompareOrder.ClientOrderID = ClientOrderID;
		std::shared_lock guard(m_Table->m_SharedMutex);
		auto it = m_Index.find(&t_CompareOrder);
		if (it == m_Index.end())
		{
			return nullptr;
		}
		return *it;
	}
	bool OrderUniqueKeyClientOrderID::Insert(Order* const record)
	{
		return m_Index.insert(record).second;
	}
	void OrderUniqueKeyClientOrderID::Erase(Order* const  record)
	{
		m_Index.erase(record);
	}
	bool OrderUniqueKeyClientOrderID::CheckInsert(Order* const record)
	{
		return m_Index.find(record) == m_Index.end();
	}
	bool OrderUniqueKeyClientOrderID::CheckUpdate(const Order* const oldRecord, const Order* const newRecord)
	{
		return OrderEqualForClientOrderIDUniqueKey()(oldRecord, newRecord);
	}

	TradePrimaryKey::TradePrimaryKey(TradeTable* table, size_t buckets)
		:m_Table(table), m_Index(buckets)
	{
	}
	Trade* TradePrimaryKey::Select(const DateType& TradingDay, const ExchangeIDType& ExchangeID, const TradeIDType& TradeID, const DirectionType& Direction)
	{
		Strcpy(t_CompareTrade.TradingDay, TradingDay);
		Strcpy(t_CompareTrade.ExchangeID, ExchangeID);
		Strcpy(t_CompareTrade.TradeID, TradeID);
		t_CompareTrade.Direction = Direction;
		
		std::shared_lock guard(m_Table->m_SharedMutex);
		auto it = m_Index.find(&t_CompareTrade);
		if (it == m_Index.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<TradePrimaryKey::iterator, TradePrimaryKey::iterator> TradePrimaryKey::SelectAll()
	{
		std::shared_lock guard(m_Table->m_SharedMutex);
		return std::pair<iterator, iterator>(m_Index.begin(), m_Index.end());
	}
	bool TradePrimaryKey::Insert(Trade* const record)
	{
		return m_Index.insert(record).second;
	}
	void TradePrimaryKey::Erase(Trade* const  record)
	{
		m_Index.erase(record);
	}
	bool TradePrimaryKey::CheckInsert(Trade* const record)
	{
		return m_Index.find(record) == m_Index.end();
	}
	bool TradePrimaryKey::CheckUpdate(const Trade* const oldRecord, const Trade* const newRecord)
	{
		return TradeEqualForTradePrimaryKey()(oldRecord, newRecord);
	}

	MdTickPrimaryKey::MdTickPrimaryKey(MdTickTable* table, size_t buckets)
		:m_Table(table), m_Index(buckets)
	{
	}
	MdTick* MdTickPrimaryKey::Select(const DateType& TradingDay, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID)
	{
		Strcpy(t_CompareMdTick.TradingDay, TradingDay);
		Strcpy(t_CompareMdTick.ExchangeID, ExchangeID);
		Strcpy(t_CompareMdTick.InstrumentID, InstrumentID);
		
		std::shared_lock guard(m_Table->m_SharedMutex);
		auto it = m_Index.find(&t_CompareMdTick);
		if (it == m_Index.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<MdTickPrimaryKey::iterator, MdTickPrimaryKey::iterator> MdTickPrimaryKey::SelectAll()
	{
		std::shared_lock guard(m_Table->m_SharedMutex);
		return std::pair<iterator, iterator>(m_Index.begin(), m_Index.end());
	}
	bool MdTickPrimaryKey::Insert(MdTick* const record)
	{
		return m_Index.insert(record).second;
	}
	void MdTickPrimaryKey::Erase(MdTick* const  record)
	{
		m_Index.erase(record);
	}
	bool MdTickPrimaryKey::CheckInsert(MdTick* const record)
	{
		return m_Index.find(record) == m_Index.end();
	}
	bool MdTickPrimaryKey::CheckUpdate(const MdTick* const oldRecord, const MdTick* const newRecord)
	{
		return MdTickEqualForMdTickPrimaryKey()(oldRecord, newRecord);
	}

}
