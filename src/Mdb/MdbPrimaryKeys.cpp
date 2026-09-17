// 本文件由 ../Templates/Cpp/Mdb/MdbPrimaryKeys.cpp.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#include "MdbPrimaryKeys.h"
#include "MdbTables.h"
#include <Spark/Core/Utility/Utility.h>
#include <string.h>

using std::unordered_set;
using namespace Spark::Core;


namespace Mdb
{
	TradingDayPrimaryKey::TradingDayPrimaryKey(TradingDayTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	TradingDay* TradingDayPrimaryKey::Select(const Int32Type& Pk)
	{
		CompareTradingDay.Pk = Pk;
		
		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&CompareTradingDay);
		if (it == index_.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<TradingDayPrimaryKey::iterator, TradingDayPrimaryKey::iterator> TradingDayPrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		return std::pair<iterator, iterator>(index_.begin(), index_.end());
	}
	bool TradingDayPrimaryKey::Insert(TradingDay* const record)
	{
		return index_.insert(record).second;
	}
	void TradingDayPrimaryKey::Erase(TradingDay* const  record)
	{
		index_.erase(record);
	}
	bool TradingDayPrimaryKey::CheckInsert(TradingDay* const record)
	{
		return index_.find(record) == index_.end();
	}
	bool TradingDayPrimaryKey::CheckUpdate(const TradingDay* const oldRecord, const TradingDay* const newRecord)
	{
		return TradingDayEqualForTradingDayPrimaryKey()(oldRecord, newRecord);
	}
	ExchangePrimaryKey::ExchangePrimaryKey(ExchangeTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	Exchange* ExchangePrimaryKey::Select(const ExchangeIdType& ExchangeId)
	{
		Utility::Strcpy(CompareExchange.ExchangeId, ExchangeId);
		
		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&CompareExchange);
		if (it == index_.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<ExchangePrimaryKey::iterator, ExchangePrimaryKey::iterator> ExchangePrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		return std::pair<iterator, iterator>(index_.begin(), index_.end());
	}
	bool ExchangePrimaryKey::Insert(Exchange* const record)
	{
		return index_.insert(record).second;
	}
	void ExchangePrimaryKey::Erase(Exchange* const  record)
	{
		index_.erase(record);
	}
	bool ExchangePrimaryKey::CheckInsert(Exchange* const record)
	{
		return index_.find(record) == index_.end();
	}
	bool ExchangePrimaryKey::CheckUpdate(const Exchange* const oldRecord, const Exchange* const newRecord)
	{
		return ExchangeEqualForExchangePrimaryKey()(oldRecord, newRecord);
	}
	ProductPrimaryKey::ProductPrimaryKey(ProductTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	Product* ProductPrimaryKey::Select(const ExchangeIdType& ExchangeId, const ProductIdType& ProductId)
	{
		Utility::Strcpy(CompareProduct.ExchangeId, ExchangeId);
		Utility::Strcpy(CompareProduct.ProductId, ProductId);
		
		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&CompareProduct);
		if (it == index_.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<ProductPrimaryKey::iterator, ProductPrimaryKey::iterator> ProductPrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		return std::pair<iterator, iterator>(index_.begin(), index_.end());
	}
	bool ProductPrimaryKey::Insert(Product* const record)
	{
		return index_.insert(record).second;
	}
	void ProductPrimaryKey::Erase(Product* const  record)
	{
		index_.erase(record);
	}
	bool ProductPrimaryKey::CheckInsert(Product* const record)
	{
		return index_.find(record) == index_.end();
	}
	bool ProductPrimaryKey::CheckUpdate(const Product* const oldRecord, const Product* const newRecord)
	{
		return ProductEqualForProductPrimaryKey()(oldRecord, newRecord);
	}
	InstrumentPrimaryKey::InstrumentPrimaryKey(InstrumentTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	Instrument* InstrumentPrimaryKey::Select(const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId)
	{
		Utility::Strcpy(CompareInstrument.ExchangeId, ExchangeId);
		Utility::Strcpy(CompareInstrument.InstrumentId, InstrumentId);
		
		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&CompareInstrument);
		if (it == index_.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<InstrumentPrimaryKey::iterator, InstrumentPrimaryKey::iterator> InstrumentPrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		return std::pair<iterator, iterator>(index_.begin(), index_.end());
	}
	bool InstrumentPrimaryKey::Insert(Instrument* const record)
	{
		return index_.insert(record).second;
	}
	void InstrumentPrimaryKey::Erase(Instrument* const  record)
	{
		index_.erase(record);
	}
	bool InstrumentPrimaryKey::CheckInsert(Instrument* const record)
	{
		return index_.find(record) == index_.end();
	}
	bool InstrumentPrimaryKey::CheckUpdate(const Instrument* const oldRecord, const Instrument* const newRecord)
	{
		return InstrumentEqualForInstrumentPrimaryKey()(oldRecord, newRecord);
	}
	PrimaryAccountPrimaryKey::PrimaryAccountPrimaryKey(PrimaryAccountTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	PrimaryAccount* PrimaryAccountPrimaryKey::Select(const AccountIdType& PrimaryAccountId)
	{
		Utility::Strcpy(ComparePrimaryAccount.PrimaryAccountId, PrimaryAccountId);
		
		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&ComparePrimaryAccount);
		if (it == index_.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<PrimaryAccountPrimaryKey::iterator, PrimaryAccountPrimaryKey::iterator> PrimaryAccountPrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		return std::pair<iterator, iterator>(index_.begin(), index_.end());
	}
	bool PrimaryAccountPrimaryKey::Insert(PrimaryAccount* const record)
	{
		return index_.insert(record).second;
	}
	void PrimaryAccountPrimaryKey::Erase(PrimaryAccount* const  record)
	{
		index_.erase(record);
	}
	bool PrimaryAccountPrimaryKey::CheckInsert(PrimaryAccount* const record)
	{
		return index_.find(record) == index_.end();
	}
	bool PrimaryAccountPrimaryKey::CheckUpdate(const PrimaryAccount* const oldRecord, const PrimaryAccount* const newRecord)
	{
		return PrimaryAccountEqualForPrimaryAccountPrimaryKey()(oldRecord, newRecord);
	}
	AccountPrimaryKey::AccountPrimaryKey(AccountTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	Account* AccountPrimaryKey::Select(const AccountIdType& AccountId)
	{
		Utility::Strcpy(CompareAccount.AccountId, AccountId);
		
		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&CompareAccount);
		if (it == index_.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<AccountPrimaryKey::iterator, AccountPrimaryKey::iterator> AccountPrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		return std::pair<iterator, iterator>(index_.begin(), index_.end());
	}
	bool AccountPrimaryKey::Insert(Account* const record)
	{
		return index_.insert(record).second;
	}
	void AccountPrimaryKey::Erase(Account* const  record)
	{
		index_.erase(record);
	}
	bool AccountPrimaryKey::CheckInsert(Account* const record)
	{
		return index_.find(record) == index_.end();
	}
	bool AccountPrimaryKey::CheckUpdate(const Account* const oldRecord, const Account* const newRecord)
	{
		return AccountEqualForAccountPrimaryKey()(oldRecord, newRecord);
	}
	CapitalPrimaryKey::CapitalPrimaryKey(CapitalTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	Capital* CapitalPrimaryKey::Select(const DateType& TradingDay, const AccountIdType& AccountId)
	{
		Utility::Strcpy(CompareCapital.TradingDay, TradingDay);
		Utility::Strcpy(CompareCapital.AccountId, AccountId);
		
		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&CompareCapital);
		if (it == index_.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<CapitalPrimaryKey::iterator, CapitalPrimaryKey::iterator> CapitalPrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		return std::pair<iterator, iterator>(index_.begin(), index_.end());
	}
	bool CapitalPrimaryKey::Insert(Capital* const record)
	{
		return index_.insert(record).second;
	}
	void CapitalPrimaryKey::Erase(Capital* const  record)
	{
		index_.erase(record);
	}
	bool CapitalPrimaryKey::CheckInsert(Capital* const record)
	{
		return index_.find(record) == index_.end();
	}
	bool CapitalPrimaryKey::CheckUpdate(const Capital* const oldRecord, const Capital* const newRecord)
	{
		return CapitalEqualForCapitalPrimaryKey()(oldRecord, newRecord);
	}
	PositionPrimaryKey::PositionPrimaryKey(PositionTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	Position* PositionPrimaryKey::Select(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const PosiDirectionType& PosiDirection)
	{
		Utility::Strcpy(ComparePosition.TradingDay, TradingDay);
		Utility::Strcpy(ComparePosition.AccountId, AccountId);
		Utility::Strcpy(ComparePosition.ExchangeId, ExchangeId);
		Utility::Strcpy(ComparePosition.InstrumentId, InstrumentId);
		ComparePosition.PosiDirection = PosiDirection;
		
		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&ComparePosition);
		if (it == index_.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<PositionPrimaryKey::iterator, PositionPrimaryKey::iterator> PositionPrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		return std::pair<iterator, iterator>(index_.begin(), index_.end());
	}
	bool PositionPrimaryKey::Insert(Position* const record)
	{
		return index_.insert(record).second;
	}
	void PositionPrimaryKey::Erase(Position* const  record)
	{
		index_.erase(record);
	}
	bool PositionPrimaryKey::CheckInsert(Position* const record)
	{
		return index_.find(record) == index_.end();
	}
	bool PositionPrimaryKey::CheckUpdate(const Position* const oldRecord, const Position* const newRecord)
	{
		return PositionEqualForPositionPrimaryKey()(oldRecord, newRecord);
	}
	PositionDetailPrimaryKey::PositionDetailPrimaryKey(PositionDetailTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	PositionDetail* PositionDetailPrimaryKey::Select(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const PosiDirectionType& PosiDirection, const DateType& OpenDate, const TradeIdType& TradeId)
	{
		Utility::Strcpy(ComparePositionDetail.TradingDay, TradingDay);
		Utility::Strcpy(ComparePositionDetail.AccountId, AccountId);
		Utility::Strcpy(ComparePositionDetail.ExchangeId, ExchangeId);
		Utility::Strcpy(ComparePositionDetail.InstrumentId, InstrumentId);
		ComparePositionDetail.PosiDirection = PosiDirection;
		Utility::Strcpy(ComparePositionDetail.OpenDate, OpenDate);
		Utility::Strcpy(ComparePositionDetail.TradeId, TradeId);
		
		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&ComparePositionDetail);
		if (it == index_.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<PositionDetailPrimaryKey::iterator, PositionDetailPrimaryKey::iterator> PositionDetailPrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		return std::pair<iterator, iterator>(index_.begin(), index_.end());
	}
	bool PositionDetailPrimaryKey::Insert(PositionDetail* const record)
	{
		return index_.insert(record).second;
	}
	void PositionDetailPrimaryKey::Erase(PositionDetail* const  record)
	{
		index_.erase(record);
	}
	bool PositionDetailPrimaryKey::CheckInsert(PositionDetail* const record)
	{
		return index_.find(record) == index_.end();
	}
	bool PositionDetailPrimaryKey::CheckUpdate(const PositionDetail* const oldRecord, const PositionDetail* const newRecord)
	{
		return PositionDetailEqualForPositionDetailPrimaryKey()(oldRecord, newRecord);
	}
	OrderPrimaryKey::OrderPrimaryKey(OrderTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	Order* OrderPrimaryKey::Select(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const OrderIdType& OrderId)
	{
		Utility::Strcpy(CompareOrder.TradingDay, TradingDay);
		Utility::Strcpy(CompareOrder.AccountId, AccountId);
		Utility::Strcpy(CompareOrder.ExchangeId, ExchangeId);
		Utility::Strcpy(CompareOrder.InstrumentId, InstrumentId);
		CompareOrder.OrderId = OrderId;
		
		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&CompareOrder);
		if (it == index_.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<OrderPrimaryKey::iterator, OrderPrimaryKey::iterator> OrderPrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		return std::pair<iterator, iterator>(index_.begin(), index_.end());
	}
	bool OrderPrimaryKey::Insert(Order* const record)
	{
		return index_.insert(record).second;
	}
	void OrderPrimaryKey::Erase(Order* const  record)
	{
		index_.erase(record);
	}
	bool OrderPrimaryKey::CheckInsert(Order* const record)
	{
		return index_.find(record) == index_.end();
	}
	bool OrderPrimaryKey::CheckUpdate(const Order* const oldRecord, const Order* const newRecord)
	{
		return OrderEqualForOrderPrimaryKey()(oldRecord, newRecord);
	}
	OrderUniqueKeyClientOrderId::OrderUniqueKeyClientOrderId(OrderTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	Order* OrderUniqueKeyClientOrderId::Select(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const SessionIdType& SessionId, const ClientOrderIdType& ClientOrderId)
	{
		Utility::Strcpy(CompareOrder.TradingDay, TradingDay);
		Utility::Strcpy(CompareOrder.AccountId, AccountId);
		Utility::Strcpy(CompareOrder.ExchangeId, ExchangeId);
		Utility::Strcpy(CompareOrder.InstrumentId, InstrumentId);
		CompareOrder.SessionId = SessionId;
		CompareOrder.ClientOrderId = ClientOrderId;
		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&CompareOrder);
		if (it == index_.end())
		{
			return nullptr;
		}
		return *it;
	}
	bool OrderUniqueKeyClientOrderId::Insert(Order* const record)
	{
		return index_.insert(record).second;
	}
	void OrderUniqueKeyClientOrderId::Erase(Order* const  record)
	{
		index_.erase(record);
	}
	bool OrderUniqueKeyClientOrderId::CheckInsert(Order* const record)
	{
		return index_.find(record) == index_.end();
	}
	bool OrderUniqueKeyClientOrderId::CheckUpdate(const Order* const oldRecord, const Order* const newRecord)
	{
		return OrderEqualForClientOrderIdUniqueKey()(oldRecord, newRecord);
	}
	
	TradePrimaryKey::TradePrimaryKey(TradeTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	Trade* TradePrimaryKey::Select(const DateType& TradingDay, const ExchangeIdType& ExchangeId, const TradeIdType& TradeId, const DirectionType& Direction)
	{
		Utility::Strcpy(CompareTrade.TradingDay, TradingDay);
		Utility::Strcpy(CompareTrade.ExchangeId, ExchangeId);
		Utility::Strcpy(CompareTrade.TradeId, TradeId);
		CompareTrade.Direction = Direction;
		
		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&CompareTrade);
		if (it == index_.end())
		{
			return nullptr;
		}
		return *it;
	}
	std::pair<TradePrimaryKey::iterator, TradePrimaryKey::iterator> TradePrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		return std::pair<iterator, iterator>(index_.begin(), index_.end());
	}
	bool TradePrimaryKey::Insert(Trade* const record)
	{
		return index_.insert(record).second;
	}
	void TradePrimaryKey::Erase(Trade* const  record)
	{
		index_.erase(record);
	}
	bool TradePrimaryKey::CheckInsert(Trade* const record)
	{
		return index_.find(record) == index_.end();
	}
	bool TradePrimaryKey::CheckUpdate(const Trade* const oldRecord, const Trade* const newRecord)
	{
		return TradeEqualForTradePrimaryKey()(oldRecord, newRecord);
	}
}
