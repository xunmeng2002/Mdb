// 本文件由 ../Templates/Cpp/Mdb/MdbPrimaryKeys.cpp.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#include "MdbPrimaryKeys.h"
#include "MdbTables.h"
#include <Spark/Core/Utility/Utility.h>
#include <string.h>

using std::unordered_set;
using namespace spark::core;


namespace mdb
{
	TradingDayPrimaryKey::TradingDayPrimaryKey(TradingDayTable* tableOwner, size_t buckets)
		:table(tableOwner), index(buckets)
	{
	}
	TradingDay* TradingDayPrimaryKey::Select(const Int32Type& PK)
	{
		CompareTradingDay.PK = PK;
		
		std::shared_lock guard(table->sharedMutex);
		auto it = index.find(&CompareTradingDay);
		if (it == index.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<TradingDayPrimaryKey::iterator, TradingDayPrimaryKey::iterator> TradingDayPrimaryKey::SelectAll()
	{
		std::shared_lock guard(table->sharedMutex);
		return std::pair<iterator, iterator>(index.begin(), index.end());
	}
	bool TradingDayPrimaryKey::Insert(TradingDay* const record)
	{
		return index.insert(record).second;
	}
	void TradingDayPrimaryKey::Erase(TradingDay* const  record)
	{
		index.erase(record);
	}
	bool TradingDayPrimaryKey::CheckInsert(TradingDay* const record)
	{
		return index.find(record) == index.end();
	}
	bool TradingDayPrimaryKey::CheckUpdate(const TradingDay* const oldRecord, const TradingDay* const newRecord)
	{
		return TradingDayEqualForTradingDayPrimaryKey()(oldRecord, newRecord);
	}
	ExchangePrimaryKey::ExchangePrimaryKey(ExchangeTable* tableOwner, size_t buckets)
		:table(tableOwner), index(buckets)
	{
	}
	Exchange* ExchangePrimaryKey::Select(const ExchangeIDType& ExchangeID)
	{
		Utility::Strcpy(CompareExchange.ExchangeID, ExchangeID);
		
		std::shared_lock guard(table->sharedMutex);
		auto it = index.find(&CompareExchange);
		if (it == index.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<ExchangePrimaryKey::iterator, ExchangePrimaryKey::iterator> ExchangePrimaryKey::SelectAll()
	{
		std::shared_lock guard(table->sharedMutex);
		return std::pair<iterator, iterator>(index.begin(), index.end());
	}
	bool ExchangePrimaryKey::Insert(Exchange* const record)
	{
		return index.insert(record).second;
	}
	void ExchangePrimaryKey::Erase(Exchange* const  record)
	{
		index.erase(record);
	}
	bool ExchangePrimaryKey::CheckInsert(Exchange* const record)
	{
		return index.find(record) == index.end();
	}
	bool ExchangePrimaryKey::CheckUpdate(const Exchange* const oldRecord, const Exchange* const newRecord)
	{
		return ExchangeEqualForExchangePrimaryKey()(oldRecord, newRecord);
	}
	ProductPrimaryKey::ProductPrimaryKey(ProductTable* tableOwner, size_t buckets)
		:table(tableOwner), index(buckets)
	{
	}
	Product* ProductPrimaryKey::Select(const ExchangeIDType& ExchangeID, const ProductIDType& ProductID)
	{
		Utility::Strcpy(CompareProduct.ExchangeID, ExchangeID);
		Utility::Strcpy(CompareProduct.ProductID, ProductID);
		
		std::shared_lock guard(table->sharedMutex);
		auto it = index.find(&CompareProduct);
		if (it == index.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<ProductPrimaryKey::iterator, ProductPrimaryKey::iterator> ProductPrimaryKey::SelectAll()
	{
		std::shared_lock guard(table->sharedMutex);
		return std::pair<iterator, iterator>(index.begin(), index.end());
	}
	bool ProductPrimaryKey::Insert(Product* const record)
	{
		return index.insert(record).second;
	}
	void ProductPrimaryKey::Erase(Product* const  record)
	{
		index.erase(record);
	}
	bool ProductPrimaryKey::CheckInsert(Product* const record)
	{
		return index.find(record) == index.end();
	}
	bool ProductPrimaryKey::CheckUpdate(const Product* const oldRecord, const Product* const newRecord)
	{
		return ProductEqualForProductPrimaryKey()(oldRecord, newRecord);
	}
	InstrumentPrimaryKey::InstrumentPrimaryKey(InstrumentTable* tableOwner, size_t buckets)
		:table(tableOwner), index(buckets)
	{
	}
	Instrument* InstrumentPrimaryKey::Select(const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID)
	{
		Utility::Strcpy(CompareInstrument.ExchangeID, ExchangeID);
		Utility::Strcpy(CompareInstrument.InstrumentID, InstrumentID);
		
		std::shared_lock guard(table->sharedMutex);
		auto it = index.find(&CompareInstrument);
		if (it == index.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<InstrumentPrimaryKey::iterator, InstrumentPrimaryKey::iterator> InstrumentPrimaryKey::SelectAll()
	{
		std::shared_lock guard(table->sharedMutex);
		return std::pair<iterator, iterator>(index.begin(), index.end());
	}
	bool InstrumentPrimaryKey::Insert(Instrument* const record)
	{
		return index.insert(record).second;
	}
	void InstrumentPrimaryKey::Erase(Instrument* const  record)
	{
		index.erase(record);
	}
	bool InstrumentPrimaryKey::CheckInsert(Instrument* const record)
	{
		return index.find(record) == index.end();
	}
	bool InstrumentPrimaryKey::CheckUpdate(const Instrument* const oldRecord, const Instrument* const newRecord)
	{
		return InstrumentEqualForInstrumentPrimaryKey()(oldRecord, newRecord);
	}
	PrimaryAccountPrimaryKey::PrimaryAccountPrimaryKey(PrimaryAccountTable* tableOwner, size_t buckets)
		:table(tableOwner), index(buckets)
	{
	}
	PrimaryAccount* PrimaryAccountPrimaryKey::Select(const AccountIDType& PrimaryAccountID)
	{
		Utility::Strcpy(ComparePrimaryAccount.PrimaryAccountID, PrimaryAccountID);
		
		std::shared_lock guard(table->sharedMutex);
		auto it = index.find(&ComparePrimaryAccount);
		if (it == index.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<PrimaryAccountPrimaryKey::iterator, PrimaryAccountPrimaryKey::iterator> PrimaryAccountPrimaryKey::SelectAll()
	{
		std::shared_lock guard(table->sharedMutex);
		return std::pair<iterator, iterator>(index.begin(), index.end());
	}
	bool PrimaryAccountPrimaryKey::Insert(PrimaryAccount* const record)
	{
		return index.insert(record).second;
	}
	void PrimaryAccountPrimaryKey::Erase(PrimaryAccount* const  record)
	{
		index.erase(record);
	}
	bool PrimaryAccountPrimaryKey::CheckInsert(PrimaryAccount* const record)
	{
		return index.find(record) == index.end();
	}
	bool PrimaryAccountPrimaryKey::CheckUpdate(const PrimaryAccount* const oldRecord, const PrimaryAccount* const newRecord)
	{
		return PrimaryAccountEqualForPrimaryAccountPrimaryKey()(oldRecord, newRecord);
	}
	AccountPrimaryKey::AccountPrimaryKey(AccountTable* tableOwner, size_t buckets)
		:table(tableOwner), index(buckets)
	{
	}
	Account* AccountPrimaryKey::Select(const AccountIDType& AccountID)
	{
		Utility::Strcpy(CompareAccount.AccountID, AccountID);
		
		std::shared_lock guard(table->sharedMutex);
		auto it = index.find(&CompareAccount);
		if (it == index.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<AccountPrimaryKey::iterator, AccountPrimaryKey::iterator> AccountPrimaryKey::SelectAll()
	{
		std::shared_lock guard(table->sharedMutex);
		return std::pair<iterator, iterator>(index.begin(), index.end());
	}
	bool AccountPrimaryKey::Insert(Account* const record)
	{
		return index.insert(record).second;
	}
	void AccountPrimaryKey::Erase(Account* const  record)
	{
		index.erase(record);
	}
	bool AccountPrimaryKey::CheckInsert(Account* const record)
	{
		return index.find(record) == index.end();
	}
	bool AccountPrimaryKey::CheckUpdate(const Account* const oldRecord, const Account* const newRecord)
	{
		return AccountEqualForAccountPrimaryKey()(oldRecord, newRecord);
	}
	CapitalPrimaryKey::CapitalPrimaryKey(CapitalTable* tableOwner, size_t buckets)
		:table(tableOwner), index(buckets)
	{
	}
	Capital* CapitalPrimaryKey::Select(const DateType& TradingDay, const AccountIDType& AccountID)
	{
		Utility::Strcpy(CompareCapital.TradingDay, TradingDay);
		Utility::Strcpy(CompareCapital.AccountID, AccountID);
		
		std::shared_lock guard(table->sharedMutex);
		auto it = index.find(&CompareCapital);
		if (it == index.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<CapitalPrimaryKey::iterator, CapitalPrimaryKey::iterator> CapitalPrimaryKey::SelectAll()
	{
		std::shared_lock guard(table->sharedMutex);
		return std::pair<iterator, iterator>(index.begin(), index.end());
	}
	bool CapitalPrimaryKey::Insert(Capital* const record)
	{
		return index.insert(record).second;
	}
	void CapitalPrimaryKey::Erase(Capital* const  record)
	{
		index.erase(record);
	}
	bool CapitalPrimaryKey::CheckInsert(Capital* const record)
	{
		return index.find(record) == index.end();
	}
	bool CapitalPrimaryKey::CheckUpdate(const Capital* const oldRecord, const Capital* const newRecord)
	{
		return CapitalEqualForCapitalPrimaryKey()(oldRecord, newRecord);
	}
	PositionPrimaryKey::PositionPrimaryKey(PositionTable* tableOwner, size_t buckets)
		:table(tableOwner), index(buckets)
	{
	}
	Position* PositionPrimaryKey::Select(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection)
	{
		Utility::Strcpy(ComparePosition.TradingDay, TradingDay);
		Utility::Strcpy(ComparePosition.AccountID, AccountID);
		Utility::Strcpy(ComparePosition.ExchangeID, ExchangeID);
		Utility::Strcpy(ComparePosition.InstrumentID, InstrumentID);
		ComparePosition.PosiDirection = PosiDirection;
		
		std::shared_lock guard(table->sharedMutex);
		auto it = index.find(&ComparePosition);
		if (it == index.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<PositionPrimaryKey::iterator, PositionPrimaryKey::iterator> PositionPrimaryKey::SelectAll()
	{
		std::shared_lock guard(table->sharedMutex);
		return std::pair<iterator, iterator>(index.begin(), index.end());
	}
	bool PositionPrimaryKey::Insert(Position* const record)
	{
		return index.insert(record).second;
	}
	void PositionPrimaryKey::Erase(Position* const  record)
	{
		index.erase(record);
	}
	bool PositionPrimaryKey::CheckInsert(Position* const record)
	{
		return index.find(record) == index.end();
	}
	bool PositionPrimaryKey::CheckUpdate(const Position* const oldRecord, const Position* const newRecord)
	{
		return PositionEqualForPositionPrimaryKey()(oldRecord, newRecord);
	}
	PositionDetailPrimaryKey::PositionDetailPrimaryKey(PositionDetailTable* tableOwner, size_t buckets)
		:table(tableOwner), index(buckets)
	{
	}
	PositionDetail* PositionDetailPrimaryKey::Select(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection, const DateType& OpenDate, const TradeIDType& TradeID)
	{
		Utility::Strcpy(ComparePositionDetail.TradingDay, TradingDay);
		Utility::Strcpy(ComparePositionDetail.AccountID, AccountID);
		Utility::Strcpy(ComparePositionDetail.ExchangeID, ExchangeID);
		Utility::Strcpy(ComparePositionDetail.InstrumentID, InstrumentID);
		ComparePositionDetail.PosiDirection = PosiDirection;
		Utility::Strcpy(ComparePositionDetail.OpenDate, OpenDate);
		Utility::Strcpy(ComparePositionDetail.TradeID, TradeID);
		
		std::shared_lock guard(table->sharedMutex);
		auto it = index.find(&ComparePositionDetail);
		if (it == index.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<PositionDetailPrimaryKey::iterator, PositionDetailPrimaryKey::iterator> PositionDetailPrimaryKey::SelectAll()
	{
		std::shared_lock guard(table->sharedMutex);
		return std::pair<iterator, iterator>(index.begin(), index.end());
	}
	bool PositionDetailPrimaryKey::Insert(PositionDetail* const record)
	{
		return index.insert(record).second;
	}
	void PositionDetailPrimaryKey::Erase(PositionDetail* const  record)
	{
		index.erase(record);
	}
	bool PositionDetailPrimaryKey::CheckInsert(PositionDetail* const record)
	{
		return index.find(record) == index.end();
	}
	bool PositionDetailPrimaryKey::CheckUpdate(const PositionDetail* const oldRecord, const PositionDetail* const newRecord)
	{
		return PositionDetailEqualForPositionDetailPrimaryKey()(oldRecord, newRecord);
	}
	OrderPrimaryKey::OrderPrimaryKey(OrderTable* tableOwner, size_t buckets)
		:table(tableOwner), index(buckets)
	{
	}
	Order* OrderPrimaryKey::Select(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const OrderIDType& OrderID)
	{
		Utility::Strcpy(CompareOrder.TradingDay, TradingDay);
		Utility::Strcpy(CompareOrder.AccountID, AccountID);
		Utility::Strcpy(CompareOrder.ExchangeID, ExchangeID);
		Utility::Strcpy(CompareOrder.InstrumentID, InstrumentID);
		CompareOrder.OrderID = OrderID;
		
		std::shared_lock guard(table->sharedMutex);
		auto it = index.find(&CompareOrder);
		if (it == index.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<OrderPrimaryKey::iterator, OrderPrimaryKey::iterator> OrderPrimaryKey::SelectAll()
	{
		std::shared_lock guard(table->sharedMutex);
		return std::pair<iterator, iterator>(index.begin(), index.end());
	}
	bool OrderPrimaryKey::Insert(Order* const record)
	{
		return index.insert(record).second;
	}
	void OrderPrimaryKey::Erase(Order* const  record)
	{
		index.erase(record);
	}
	bool OrderPrimaryKey::CheckInsert(Order* const record)
	{
		return index.find(record) == index.end();
	}
	bool OrderPrimaryKey::CheckUpdate(const Order* const oldRecord, const Order* const newRecord)
	{
		return OrderEqualForOrderPrimaryKey()(oldRecord, newRecord);
	}
	OrderUniqueKeyClientOrderID::OrderUniqueKeyClientOrderID(OrderTable* tableOwner, size_t buckets)
		:table(tableOwner), index(buckets)
	{
	}
	Order* OrderUniqueKeyClientOrderID::Select(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const SessionIDType& SessionID, const ClientOrderIDType& ClientOrderID)
	{
		Utility::Strcpy(CompareOrder.TradingDay, TradingDay);
		Utility::Strcpy(CompareOrder.AccountID, AccountID);
		Utility::Strcpy(CompareOrder.ExchangeID, ExchangeID);
		Utility::Strcpy(CompareOrder.InstrumentID, InstrumentID);
		CompareOrder.SessionID = SessionID;
		CompareOrder.ClientOrderID = ClientOrderID;
		std::shared_lock guard(table->sharedMutex);
		auto it = index.find(&CompareOrder);
		if (it == index.end())
		{
			return nullptr;
		}
		return *it;
	}
	bool OrderUniqueKeyClientOrderID::Insert(Order* const record)
	{
		return index.insert(record).second;
	}
	void OrderUniqueKeyClientOrderID::Erase(Order* const  record)
	{
		index.erase(record);
	}
	bool OrderUniqueKeyClientOrderID::CheckInsert(Order* const record)
	{
		return index.find(record) == index.end();
	}
	bool OrderUniqueKeyClientOrderID::CheckUpdate(const Order* const oldRecord, const Order* const newRecord)
	{
		return OrderEqualForClientOrderIDUniqueKey()(oldRecord, newRecord);
	}
	
	TradePrimaryKey::TradePrimaryKey(TradeTable* tableOwner, size_t buckets)
		:table(tableOwner), index(buckets)
	{
	}
	Trade* TradePrimaryKey::Select(const DateType& TradingDay, const ExchangeIDType& ExchangeID, const TradeIDType& TradeID, const DirectionType& Direction)
	{
		Utility::Strcpy(CompareTrade.TradingDay, TradingDay);
		Utility::Strcpy(CompareTrade.ExchangeID, ExchangeID);
		Utility::Strcpy(CompareTrade.TradeID, TradeID);
		CompareTrade.Direction = Direction;
		
		std::shared_lock guard(table->sharedMutex);
		auto it = index.find(&CompareTrade);
		if (it == index.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<TradePrimaryKey::iterator, TradePrimaryKey::iterator> TradePrimaryKey::SelectAll()
	{
		std::shared_lock guard(table->sharedMutex);
		return std::pair<iterator, iterator>(index.begin(), index.end());
	}
	bool TradePrimaryKey::Insert(Trade* const record)
	{
		return index.insert(record).second;
	}
	void TradePrimaryKey::Erase(Trade* const  record)
	{
		index.erase(record);
	}
	bool TradePrimaryKey::CheckInsert(Trade* const record)
	{
		return index.find(record) == index.end();
	}
	bool TradePrimaryKey::CheckUpdate(const Trade* const oldRecord, const Trade* const newRecord)
	{
		return TradeEqualForTradePrimaryKey()(oldRecord, newRecord);
	}
}
