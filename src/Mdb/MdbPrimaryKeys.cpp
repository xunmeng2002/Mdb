// 本文件由 ../Templates/Cpp/Mdb/MdbPrimaryKeys.cpp.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#include "MdbPrimaryKeys.h"
#include "MdbTables.h"
#include <Spark/Core/Utility/Utility.h>
#include <string.h>
#include <utility>

using std::unordered_set;
using namespace Spark::Core;


namespace Mdb
{
	TradingDayTablePrimaryKey::TradingDayTablePrimaryKey(TradingDayTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	TableReadLease<TradingDay> TradingDayTablePrimaryKey::Select(const Int32Type& Pk)
	{
		CompareTradingDay.Pk = Pk;
		
		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&CompareTradingDay);
		if (it == index_.end())
		{
			return TableReadLease<TradingDay>();
		}
		return TableReadLease<TradingDay>(std::move(guard), *it);
	}
	TableRangeLease<TradingDayTablePrimaryKey::iterator> TradingDayTablePrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		return TableRangeLease<iterator>(std::move(guard), index_.begin(), index_.end());
	}
	bool TradingDayTablePrimaryKey::Insert(TradingDay* const record)
	{
		return index_.insert(record).second;
	}
	void TradingDayTablePrimaryKey::Erase(TradingDay* const  record)
	{
		index_.erase(record);
	}
	bool TradingDayTablePrimaryKey::CheckInsert(TradingDay* const record)
	{
		return index_.find(record) == index_.end();
	}
	bool TradingDayTablePrimaryKey::CheckUpdate(const TradingDay* const oldRecord, const TradingDay* const newRecord)
	{
		return TradingDayEqualForTradingDayPrimaryKey()(oldRecord, newRecord);
	}
	ExchangeTablePrimaryKey::ExchangeTablePrimaryKey(ExchangeTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	TableReadLease<Exchange> ExchangeTablePrimaryKey::Select(const ExchangeIdType& ExchangeId)
	{
		Utility::Strcpy(CompareExchange.ExchangeId, ExchangeId);
		
		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&CompareExchange);
		if (it == index_.end())
		{
			return TableReadLease<Exchange>();
		}
		return TableReadLease<Exchange>(std::move(guard), *it);
	}
	TableRangeLease<ExchangeTablePrimaryKey::iterator> ExchangeTablePrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		return TableRangeLease<iterator>(std::move(guard), index_.begin(), index_.end());
	}
	bool ExchangeTablePrimaryKey::Insert(Exchange* const record)
	{
		return index_.insert(record).second;
	}
	void ExchangeTablePrimaryKey::Erase(Exchange* const  record)
	{
		index_.erase(record);
	}
	bool ExchangeTablePrimaryKey::CheckInsert(Exchange* const record)
	{
		return index_.find(record) == index_.end();
	}
	bool ExchangeTablePrimaryKey::CheckUpdate(const Exchange* const oldRecord, const Exchange* const newRecord)
	{
		return ExchangeEqualForExchangePrimaryKey()(oldRecord, newRecord);
	}
	ProductTablePrimaryKey::ProductTablePrimaryKey(ProductTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	TableReadLease<Product> ProductTablePrimaryKey::Select(const ExchangeIdType& ExchangeId, const ProductIdType& ProductId)
	{
		Utility::Strcpy(CompareProduct.ExchangeId, ExchangeId);
		Utility::Strcpy(CompareProduct.ProductId, ProductId);
		
		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&CompareProduct);
		if (it == index_.end())
		{
			return TableReadLease<Product>();
		}
		return TableReadLease<Product>(std::move(guard), *it);
	}
	TableRangeLease<ProductTablePrimaryKey::iterator> ProductTablePrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		return TableRangeLease<iterator>(std::move(guard), index_.begin(), index_.end());
	}
	bool ProductTablePrimaryKey::Insert(Product* const record)
	{
		return index_.insert(record).second;
	}
	void ProductTablePrimaryKey::Erase(Product* const  record)
	{
		index_.erase(record);
	}
	bool ProductTablePrimaryKey::CheckInsert(Product* const record)
	{
		return index_.find(record) == index_.end();
	}
	bool ProductTablePrimaryKey::CheckUpdate(const Product* const oldRecord, const Product* const newRecord)
	{
		return ProductEqualForProductPrimaryKey()(oldRecord, newRecord);
	}
	InstrumentTablePrimaryKey::InstrumentTablePrimaryKey(InstrumentTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	TableReadLease<Instrument> InstrumentTablePrimaryKey::Select(const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId)
	{
		Utility::Strcpy(CompareInstrument.ExchangeId, ExchangeId);
		Utility::Strcpy(CompareInstrument.InstrumentId, InstrumentId);
		
		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&CompareInstrument);
		if (it == index_.end())
		{
			return TableReadLease<Instrument>();
		}
		return TableReadLease<Instrument>(std::move(guard), *it);
	}
	TableRangeLease<InstrumentTablePrimaryKey::iterator> InstrumentTablePrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		return TableRangeLease<iterator>(std::move(guard), index_.begin(), index_.end());
	}
	bool InstrumentTablePrimaryKey::Insert(Instrument* const record)
	{
		return index_.insert(record).second;
	}
	void InstrumentTablePrimaryKey::Erase(Instrument* const  record)
	{
		index_.erase(record);
	}
	bool InstrumentTablePrimaryKey::CheckInsert(Instrument* const record)
	{
		return index_.find(record) == index_.end();
	}
	bool InstrumentTablePrimaryKey::CheckUpdate(const Instrument* const oldRecord, const Instrument* const newRecord)
	{
		return InstrumentEqualForInstrumentPrimaryKey()(oldRecord, newRecord);
	}
	PrimaryAccountTablePrimaryKey::PrimaryAccountTablePrimaryKey(PrimaryAccountTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	TableReadLease<PrimaryAccount> PrimaryAccountTablePrimaryKey::Select(const AccountIdType& PrimaryAccountId)
	{
		Utility::Strcpy(ComparePrimaryAccount.PrimaryAccountId, PrimaryAccountId);
		
		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&ComparePrimaryAccount);
		if (it == index_.end())
		{
			return TableReadLease<PrimaryAccount>();
		}
		return TableReadLease<PrimaryAccount>(std::move(guard), *it);
	}
	TableRangeLease<PrimaryAccountTablePrimaryKey::iterator> PrimaryAccountTablePrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		return TableRangeLease<iterator>(std::move(guard), index_.begin(), index_.end());
	}
	bool PrimaryAccountTablePrimaryKey::Insert(PrimaryAccount* const record)
	{
		return index_.insert(record).second;
	}
	void PrimaryAccountTablePrimaryKey::Erase(PrimaryAccount* const  record)
	{
		index_.erase(record);
	}
	bool PrimaryAccountTablePrimaryKey::CheckInsert(PrimaryAccount* const record)
	{
		return index_.find(record) == index_.end();
	}
	bool PrimaryAccountTablePrimaryKey::CheckUpdate(const PrimaryAccount* const oldRecord, const PrimaryAccount* const newRecord)
	{
		return PrimaryAccountEqualForPrimaryAccountPrimaryKey()(oldRecord, newRecord);
	}
	AccountTablePrimaryKey::AccountTablePrimaryKey(AccountTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	TableReadLease<Account> AccountTablePrimaryKey::Select(const AccountIdType& AccountId)
	{
		Utility::Strcpy(CompareAccount.AccountId, AccountId);
		
		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&CompareAccount);
		if (it == index_.end())
		{
			return TableReadLease<Account>();
		}
		return TableReadLease<Account>(std::move(guard), *it);
	}
	TableRangeLease<AccountTablePrimaryKey::iterator> AccountTablePrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		return TableRangeLease<iterator>(std::move(guard), index_.begin(), index_.end());
	}
	bool AccountTablePrimaryKey::Insert(Account* const record)
	{
		return index_.insert(record).second;
	}
	void AccountTablePrimaryKey::Erase(Account* const  record)
	{
		index_.erase(record);
	}
	bool AccountTablePrimaryKey::CheckInsert(Account* const record)
	{
		return index_.find(record) == index_.end();
	}
	bool AccountTablePrimaryKey::CheckUpdate(const Account* const oldRecord, const Account* const newRecord)
	{
		return AccountEqualForAccountPrimaryKey()(oldRecord, newRecord);
	}
	CapitalTablePrimaryKey::CapitalTablePrimaryKey(CapitalTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	TableReadLease<Capital> CapitalTablePrimaryKey::Select(const DateType& TradingDay, const AccountIdType& AccountId)
	{
		Utility::Strcpy(CompareCapital.TradingDay, TradingDay);
		Utility::Strcpy(CompareCapital.AccountId, AccountId);
		
		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&CompareCapital);
		if (it == index_.end())
		{
			return TableReadLease<Capital>();
		}
		return TableReadLease<Capital>(std::move(guard), *it);
	}
	TableRangeLease<CapitalTablePrimaryKey::iterator> CapitalTablePrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		return TableRangeLease<iterator>(std::move(guard), index_.begin(), index_.end());
	}
	bool CapitalTablePrimaryKey::Insert(Capital* const record)
	{
		return index_.insert(record).second;
	}
	void CapitalTablePrimaryKey::Erase(Capital* const  record)
	{
		index_.erase(record);
	}
	bool CapitalTablePrimaryKey::CheckInsert(Capital* const record)
	{
		return index_.find(record) == index_.end();
	}
	bool CapitalTablePrimaryKey::CheckUpdate(const Capital* const oldRecord, const Capital* const newRecord)
	{
		return CapitalEqualForCapitalPrimaryKey()(oldRecord, newRecord);
	}
	PositionTablePrimaryKey::PositionTablePrimaryKey(PositionTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	TableReadLease<Position> PositionTablePrimaryKey::Select(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const PosiDirectionType& PosiDirection)
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
			return TableReadLease<Position>();
		}
		return TableReadLease<Position>(std::move(guard), *it);
	}
	TableRangeLease<PositionTablePrimaryKey::iterator> PositionTablePrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		return TableRangeLease<iterator>(std::move(guard), index_.begin(), index_.end());
	}
	bool PositionTablePrimaryKey::Insert(Position* const record)
	{
		return index_.insert(record).second;
	}
	void PositionTablePrimaryKey::Erase(Position* const  record)
	{
		index_.erase(record);
	}
	bool PositionTablePrimaryKey::CheckInsert(Position* const record)
	{
		return index_.find(record) == index_.end();
	}
	bool PositionTablePrimaryKey::CheckUpdate(const Position* const oldRecord, const Position* const newRecord)
	{
		return PositionEqualForPositionPrimaryKey()(oldRecord, newRecord);
	}
	PositionDetailTablePrimaryKey::PositionDetailTablePrimaryKey(PositionDetailTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	TableReadLease<PositionDetail> PositionDetailTablePrimaryKey::Select(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const PosiDirectionType& PosiDirection, const DateType& OpenDate, const TradeIdType& TradeId)
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
			return TableReadLease<PositionDetail>();
		}
		return TableReadLease<PositionDetail>(std::move(guard), *it);
	}
	TableRangeLease<PositionDetailTablePrimaryKey::iterator> PositionDetailTablePrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		return TableRangeLease<iterator>(std::move(guard), index_.begin(), index_.end());
	}
	bool PositionDetailTablePrimaryKey::Insert(PositionDetail* const record)
	{
		return index_.insert(record).second;
	}
	void PositionDetailTablePrimaryKey::Erase(PositionDetail* const  record)
	{
		index_.erase(record);
	}
	bool PositionDetailTablePrimaryKey::CheckInsert(PositionDetail* const record)
	{
		return index_.find(record) == index_.end();
	}
	bool PositionDetailTablePrimaryKey::CheckUpdate(const PositionDetail* const oldRecord, const PositionDetail* const newRecord)
	{
		return PositionDetailEqualForPositionDetailPrimaryKey()(oldRecord, newRecord);
	}
	OrderTablePrimaryKey::OrderTablePrimaryKey(OrderTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	TableReadLease<Order> OrderTablePrimaryKey::Select(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const OrderIdType& OrderId)
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
			return TableReadLease<Order>();
		}
		return TableReadLease<Order>(std::move(guard), *it);
	}
	TableRangeLease<OrderTablePrimaryKey::iterator> OrderTablePrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		return TableRangeLease<iterator>(std::move(guard), index_.begin(), index_.end());
	}
	bool OrderTablePrimaryKey::Insert(Order* const record)
	{
		return index_.insert(record).second;
	}
	void OrderTablePrimaryKey::Erase(Order* const  record)
	{
		index_.erase(record);
	}
	bool OrderTablePrimaryKey::CheckInsert(Order* const record)
	{
		return index_.find(record) == index_.end();
	}
	bool OrderTablePrimaryKey::CheckUpdate(const Order* const oldRecord, const Order* const newRecord)
	{
		return OrderEqualForOrderPrimaryKey()(oldRecord, newRecord);
	}
	OrderTableUniqueKeyClientOrderId::OrderTableUniqueKeyClientOrderId(OrderTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	TableReadLease<Order> OrderTableUniqueKeyClientOrderId::Select(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const SessionIdType& SessionId, const ClientOrderIdType& ClientOrderId)
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
			return TableReadLease<Order>();
		}
		return TableReadLease<Order>(std::move(guard), *it);
	}
	bool OrderTableUniqueKeyClientOrderId::Insert(Order* const record)
	{
		return index_.insert(record).second;
	}
	void OrderTableUniqueKeyClientOrderId::Erase(Order* const  record)
	{
		index_.erase(record);
	}
	bool OrderTableUniqueKeyClientOrderId::CheckInsert(Order* const record)
	{
		return index_.find(record) == index_.end();
	}
	bool OrderTableUniqueKeyClientOrderId::CheckUpdate(const Order* const oldRecord, const Order* const newRecord)
	{
		return OrderEqualForClientOrderIdUniqueKey()(oldRecord, newRecord);
	}
	
	TradeTablePrimaryKey::TradeTablePrimaryKey(TradeTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	TableReadLease<Trade> TradeTablePrimaryKey::Select(const DateType& TradingDay, const ExchangeIdType& ExchangeId, const TradeIdType& TradeId, const DirectionType& Direction)
	{
		Utility::Strcpy(CompareTrade.TradingDay, TradingDay);
		Utility::Strcpy(CompareTrade.ExchangeId, ExchangeId);
		Utility::Strcpy(CompareTrade.TradeId, TradeId);
		CompareTrade.Direction = Direction;
		
		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&CompareTrade);
		if (it == index_.end())
		{
			return TableReadLease<Trade>();
		}
		return TableReadLease<Trade>(std::move(guard), *it);
	}
	TableRangeLease<TradeTablePrimaryKey::iterator> TradeTablePrimaryKey::SelectAll()
	{
		std::shared_lock guard(table_->SharedMutex);
		return TableRangeLease<iterator>(std::move(guard), index_.begin(), index_.end());
	}
	bool TradeTablePrimaryKey::Insert(Trade* const record)
	{
		return index_.insert(record).second;
	}
	void TradeTablePrimaryKey::Erase(Trade* const  record)
	{
		index_.erase(record);
	}
	bool TradeTablePrimaryKey::CheckInsert(Trade* const record)
	{
		return index_.find(record) == index_.end();
	}
	bool TradeTablePrimaryKey::CheckUpdate(const Trade* const oldRecord, const Trade* const newRecord)
	{
		return TradeEqualForTradePrimaryKey()(oldRecord, newRecord);
	}
}
