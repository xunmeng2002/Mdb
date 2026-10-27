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
	std::optional<TradingDay> TradingDayTablePrimaryKey::Select(const Int32Type& Pk)
	{
		CompareTradingDay.Pk = Pk;

		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&CompareTradingDay);
		if (it == index_.end())
		{
			return std::nullopt;
		}
		return **it;
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
	TradingDay* TradingDayTablePrimaryKey::Find(const TradingDay* record)
	{
		auto it = index_.find(const_cast<TradingDay*>(record));
		return it == index_.end() ? nullptr : *it;
	}
	ExchangeTablePrimaryKey::ExchangeTablePrimaryKey(ExchangeTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	std::optional<Exchange> ExchangeTablePrimaryKey::Select(const ExchangeIdType& ExchangeId)
	{
		Utility::Strcpy(CompareExchange.ExchangeId, ExchangeId);

		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&CompareExchange);
		if (it == index_.end())
		{
			return std::nullopt;
		}
		return **it;
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
	Exchange* ExchangeTablePrimaryKey::Find(const Exchange* record)
	{
		auto it = index_.find(const_cast<Exchange*>(record));
		return it == index_.end() ? nullptr : *it;
	}
	ProductTablePrimaryKey::ProductTablePrimaryKey(ProductTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	std::optional<Product> ProductTablePrimaryKey::Select(const ExchangeIdType& ExchangeId, const ProductIdType& ProductId)
	{
		Utility::Strcpy(CompareProduct.ExchangeId, ExchangeId);
		Utility::Strcpy(CompareProduct.ProductId, ProductId);

		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&CompareProduct);
		if (it == index_.end())
		{
			return std::nullopt;
		}
		return **it;
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
	Product* ProductTablePrimaryKey::Find(const Product* record)
	{
		auto it = index_.find(const_cast<Product*>(record));
		return it == index_.end() ? nullptr : *it;
	}
	InstrumentTablePrimaryKey::InstrumentTablePrimaryKey(InstrumentTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	std::optional<Instrument> InstrumentTablePrimaryKey::Select(const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId)
	{
		Utility::Strcpy(CompareInstrument.ExchangeId, ExchangeId);
		Utility::Strcpy(CompareInstrument.InstrumentId, InstrumentId);

		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&CompareInstrument);
		if (it == index_.end())
		{
			return std::nullopt;
		}
		return **it;
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
	Instrument* InstrumentTablePrimaryKey::Find(const Instrument* record)
	{
		auto it = index_.find(const_cast<Instrument*>(record));
		return it == index_.end() ? nullptr : *it;
	}
	PrimaryAccountTablePrimaryKey::PrimaryAccountTablePrimaryKey(PrimaryAccountTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	std::optional<PrimaryAccount> PrimaryAccountTablePrimaryKey::Select(const AccountIdType& PrimaryAccountId)
	{
		Utility::Strcpy(ComparePrimaryAccount.PrimaryAccountId, PrimaryAccountId);

		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&ComparePrimaryAccount);
		if (it == index_.end())
		{
			return std::nullopt;
		}
		return **it;
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
	PrimaryAccount* PrimaryAccountTablePrimaryKey::Find(const PrimaryAccount* record)
	{
		auto it = index_.find(const_cast<PrimaryAccount*>(record));
		return it == index_.end() ? nullptr : *it;
	}
	AccountTablePrimaryKey::AccountTablePrimaryKey(AccountTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	std::optional<Account> AccountTablePrimaryKey::Select(const AccountIdType& AccountId)
	{
		Utility::Strcpy(CompareAccount.AccountId, AccountId);

		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&CompareAccount);
		if (it == index_.end())
		{
			return std::nullopt;
		}
		return **it;
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
	Account* AccountTablePrimaryKey::Find(const Account* record)
	{
		auto it = index_.find(const_cast<Account*>(record));
		return it == index_.end() ? nullptr : *it;
	}
	CapitalTablePrimaryKey::CapitalTablePrimaryKey(CapitalTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	std::optional<Capital> CapitalTablePrimaryKey::Select(const DateType& TradingDay, const AccountIdType& AccountId)
	{
		Utility::Strcpy(CompareCapital.TradingDay, TradingDay);
		Utility::Strcpy(CompareCapital.AccountId, AccountId);

		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&CompareCapital);
		if (it == index_.end())
		{
			return std::nullopt;
		}
		return **it;
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
	Capital* CapitalTablePrimaryKey::Find(const Capital* record)
	{
		auto it = index_.find(const_cast<Capital*>(record));
		return it == index_.end() ? nullptr : *it;
	}
	PositionTablePrimaryKey::PositionTablePrimaryKey(PositionTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	std::optional<Position> PositionTablePrimaryKey::Select(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const PosiDirectionType& PosiDirection)
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
			return std::nullopt;
		}
		return **it;
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
	Position* PositionTablePrimaryKey::Find(const Position* record)
	{
		auto it = index_.find(const_cast<Position*>(record));
		return it == index_.end() ? nullptr : *it;
	}
	PositionDetailTablePrimaryKey::PositionDetailTablePrimaryKey(PositionDetailTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	std::optional<PositionDetail> PositionDetailTablePrimaryKey::Select(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const PosiDirectionType& PosiDirection, const DateType& OpenDate, const TradeIdType& TradeId)
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
			return std::nullopt;
		}
		return **it;
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
	PositionDetail* PositionDetailTablePrimaryKey::Find(const PositionDetail* record)
	{
		auto it = index_.find(const_cast<PositionDetail*>(record));
		return it == index_.end() ? nullptr : *it;
	}
	OrderTablePrimaryKey::OrderTablePrimaryKey(OrderTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	std::optional<Order> OrderTablePrimaryKey::Select(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const OrderIdType& OrderId)
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
			return std::nullopt;
		}
		return **it;
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
	Order* OrderTablePrimaryKey::Find(const Order* record)
	{
		auto it = index_.find(const_cast<Order*>(record));
		return it == index_.end() ? nullptr : *it;
	}
	OrderTableUniqueKeyClientOrderId::OrderTableUniqueKeyClientOrderId(OrderTable* tableOwner, size_t buckets)
		:table_(tableOwner), index_(buckets)
	{
	}
	std::optional<Order> OrderTableUniqueKeyClientOrderId::Select(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const SessionIdType& SessionId, const ClientOrderIdType& ClientOrderId)
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
			return std::nullopt;
		}
		return **it;
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
	std::optional<Trade> TradeTablePrimaryKey::Select(const DateType& TradingDay, const ExchangeIdType& ExchangeId, const TradeIdType& TradeId, const DirectionType& Direction)
	{
		Utility::Strcpy(CompareTrade.TradingDay, TradingDay);
		Utility::Strcpy(CompareTrade.ExchangeId, ExchangeId);
		Utility::Strcpy(CompareTrade.TradeId, TradeId);
		CompareTrade.Direction = Direction;

		std::shared_lock guard(table_->SharedMutex);
		auto it = index_.find(&CompareTrade);
		if (it == index_.end())
		{
			return std::nullopt;
		}
		return **it;
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
	Trade* TradeTablePrimaryKey::Find(const Trade* record)
	{
		auto it = index_.find(const_cast<Trade*>(record));
		return it == index_.end() ? nullptr : *it;
	}
}
