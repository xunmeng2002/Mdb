// 本文件由 ../Templates/Cpp/Mdb/MdbIndexes.cpp.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#include "MdbIndexes.h"
#include "MdbTables.h"
#include <Spark/Core/Utility/Utility.h>
#include <string.h>
#include <utility>


using namespace Spark::Core;
namespace Mdb
{
	PrimaryAccountIndexOfferId::PrimaryAccountIndexOfferId(PrimaryAccountTable* tableOwner)
		:table_(tableOwner)
	{
	}
	TableRangeLease<PrimaryAccountIndexOfferId::iterator> PrimaryAccountIndexOfferId::Range(const OfferIdType& lowerOfferId, const OfferIdType& upperOfferId)
	{
		FillCompareRecord(lowerOfferId);
		std::shared_lock guard(table_->SharedMutex);
		auto first = index_.lower_bound(&ComparePrimaryAccount);
		FillCompareRecord(upperOfferId);
		auto last = index_.upper_bound(&ComparePrimaryAccount);
		return TableRangeLease<iterator>(std::move(guard), first, last);
	}
	TableRangeLease<PrimaryAccountIndexOfferId::iterator> PrimaryAccountIndexOfferId::EqualRange(const OfferIdType& OfferId)
	{
		FillCompareRecord(OfferId);
		std::shared_lock guard(table_->SharedMutex);
		auto range = index_.equal_range(&ComparePrimaryAccount);
		return TableRangeLease<iterator>(std::move(guard), range.first, range.second);
	}
	void PrimaryAccountIndexOfferId::Insert(PrimaryAccount* const record)
	{
		index_.insert(record);
	}
	void PrimaryAccountIndexOfferId::Erase(PrimaryAccount* const record)
	{
		auto it = FindNode(record);
		index_.erase(it);
	}
	void PrimaryAccountIndexOfferId::Update(iterator it)
	{
		auto record = *it;
		index_.erase(it);
		index_.insert(record);
	}
	bool PrimaryAccountIndexOfferId::NeedUpdate(const PrimaryAccount* const oldRecord, const PrimaryAccount* const newRecord)
	{
		return !(PrimaryAccountEqualForOfferIdIndex()(oldRecord, newRecord));
	}
	PrimaryAccountIndexOfferId::iterator PrimaryAccountIndexOfferId::FindNode(PrimaryAccount* const record)
	{
		auto p = index_.equal_range(record);
		for (auto it = p.first; it != p.second; ++it)
		{
			if (*it == record)
			{
				return it;
			}
		}
		return index_.end();
	}
	void PrimaryAccountIndexOfferId::FillCompareRecord(const OfferIdType& OfferId)
	{
		ComparePrimaryAccount.OfferId = OfferId;
	}
	
	CapitalIndexTradingDay::CapitalIndexTradingDay(CapitalTable* tableOwner)
		:table_(tableOwner)
	{
	}
	TableRangeLease<CapitalIndexTradingDay::iterator> CapitalIndexTradingDay::Range(const DateType& lowerTradingDay, const DateType& upperTradingDay)
	{
		FillCompareRecord(lowerTradingDay);
		std::shared_lock guard(table_->SharedMutex);
		auto first = index_.lower_bound(&CompareCapital);
		FillCompareRecord(upperTradingDay);
		auto last = index_.upper_bound(&CompareCapital);
		return TableRangeLease<iterator>(std::move(guard), first, last);
	}
	TableRangeLease<CapitalIndexTradingDay::iterator> CapitalIndexTradingDay::EqualRange(const DateType& TradingDay)
	{
		FillCompareRecord(TradingDay);
		std::shared_lock guard(table_->SharedMutex);
		auto range = index_.equal_range(&CompareCapital);
		return TableRangeLease<iterator>(std::move(guard), range.first, range.second);
	}
	void CapitalIndexTradingDay::Insert(Capital* const record)
	{
		index_.insert(record);
	}
	void CapitalIndexTradingDay::Erase(Capital* const record)
	{
		auto it = FindNode(record);
		index_.erase(it);
	}
	void CapitalIndexTradingDay::Update(iterator it)
	{
		auto record = *it;
		index_.erase(it);
		index_.insert(record);
	}
	bool CapitalIndexTradingDay::NeedUpdate(const Capital* const oldRecord, const Capital* const newRecord)
	{
		return !(CapitalEqualForTradingDayIndex()(oldRecord, newRecord));
	}
	CapitalIndexTradingDay::iterator CapitalIndexTradingDay::FindNode(Capital* const record)
	{
		auto p = index_.equal_range(record);
		for (auto it = p.first; it != p.second; ++it)
		{
			if (*it == record)
			{
				return it;
			}
		}
		return index_.end();
	}
	void CapitalIndexTradingDay::FillCompareRecord(const DateType& TradingDay)
	{
		Utility::Strcpy(CompareCapital.TradingDay, TradingDay);
	}
	
	PositionIndexAccount::PositionIndexAccount(PositionTable* tableOwner)
		:table_(tableOwner)
	{
	}
	TableRangeLease<PositionIndexAccount::iterator> PositionIndexAccount::Range(const DateType& lowerTradingDay, const AccountIdType& lowerAccountId, const DateType& upperTradingDay, const AccountIdType& upperAccountId)
	{
		FillCompareRecord(lowerTradingDay, lowerAccountId);
		std::shared_lock guard(table_->SharedMutex);
		auto first = index_.lower_bound(&ComparePosition);
		FillCompareRecord(upperTradingDay, upperAccountId);
		auto last = index_.upper_bound(&ComparePosition);
		return TableRangeLease<iterator>(std::move(guard), first, last);
	}
	TableRangeLease<PositionIndexAccount::iterator> PositionIndexAccount::EqualRange(const DateType& TradingDay, const AccountIdType& AccountId)
	{
		FillCompareRecord(TradingDay, AccountId);
		std::shared_lock guard(table_->SharedMutex);
		auto range = index_.equal_range(&ComparePosition);
		return TableRangeLease<iterator>(std::move(guard), range.first, range.second);
	}
	void PositionIndexAccount::Insert(Position* const record)
	{
		index_.insert(record);
	}
	void PositionIndexAccount::Erase(Position* const record)
	{
		auto it = FindNode(record);
		index_.erase(it);
	}
	void PositionIndexAccount::Update(iterator it)
	{
		auto record = *it;
		index_.erase(it);
		index_.insert(record);
	}
	bool PositionIndexAccount::NeedUpdate(const Position* const oldRecord, const Position* const newRecord)
	{
		return !(PositionEqualForAccountIndex()(oldRecord, newRecord));
	}
	PositionIndexAccount::iterator PositionIndexAccount::FindNode(Position* const record)
	{
		auto p = index_.equal_range(record);
		for (auto it = p.first; it != p.second; ++it)
		{
			if (*it == record)
			{
				return it;
			}
		}
		return index_.end();
	}
	void PositionIndexAccount::FillCompareRecord(const DateType& TradingDay, const AccountIdType& AccountId)
	{
		Utility::Strcpy(ComparePosition.TradingDay, TradingDay);
		Utility::Strcpy(ComparePosition.AccountId, AccountId);
	}
	
	PositionIndexTradingDay::PositionIndexTradingDay(PositionTable* tableOwner)
		:table_(tableOwner)
	{
	}
	TableRangeLease<PositionIndexTradingDay::iterator> PositionIndexTradingDay::Range(const DateType& lowerTradingDay, const DateType& upperTradingDay)
	{
		FillCompareRecord(lowerTradingDay);
		std::shared_lock guard(table_->SharedMutex);
		auto first = index_.lower_bound(&ComparePosition);
		FillCompareRecord(upperTradingDay);
		auto last = index_.upper_bound(&ComparePosition);
		return TableRangeLease<iterator>(std::move(guard), first, last);
	}
	TableRangeLease<PositionIndexTradingDay::iterator> PositionIndexTradingDay::EqualRange(const DateType& TradingDay)
	{
		FillCompareRecord(TradingDay);
		std::shared_lock guard(table_->SharedMutex);
		auto range = index_.equal_range(&ComparePosition);
		return TableRangeLease<iterator>(std::move(guard), range.first, range.second);
	}
	void PositionIndexTradingDay::Insert(Position* const record)
	{
		index_.insert(record);
	}
	void PositionIndexTradingDay::Erase(Position* const record)
	{
		auto it = FindNode(record);
		index_.erase(it);
	}
	void PositionIndexTradingDay::Update(iterator it)
	{
		auto record = *it;
		index_.erase(it);
		index_.insert(record);
	}
	bool PositionIndexTradingDay::NeedUpdate(const Position* const oldRecord, const Position* const newRecord)
	{
		return !(PositionEqualForTradingDayIndex()(oldRecord, newRecord));
	}
	PositionIndexTradingDay::iterator PositionIndexTradingDay::FindNode(Position* const record)
	{
		auto p = index_.equal_range(record);
		for (auto it = p.first; it != p.second; ++it)
		{
			if (*it == record)
			{
				return it;
			}
		}
		return index_.end();
	}
	void PositionIndexTradingDay::FillCompareRecord(const DateType& TradingDay)
	{
		Utility::Strcpy(ComparePosition.TradingDay, TradingDay);
	}
	
	PositionDetailIndexTradeMatch::PositionDetailIndexTradeMatch(PositionDetailTable* tableOwner)
		:table_(tableOwner)
	{
	}
	TableRangeLease<PositionDetailIndexTradeMatch::iterator> PositionDetailIndexTradeMatch::Range(const DateType& lowerTradingDay, const AccountIdType& lowerAccountId, const ExchangeIdType& lowerExchangeId, const InstrumentIdType& lowerInstrumentId, const PosiDirectionType& lowerPosiDirection, const DateType& upperTradingDay, const AccountIdType& upperAccountId, const ExchangeIdType& upperExchangeId, const InstrumentIdType& upperInstrumentId, const PosiDirectionType& upperPosiDirection)
	{
		FillCompareRecord(lowerTradingDay, lowerAccountId, lowerExchangeId, lowerInstrumentId, lowerPosiDirection);
		std::shared_lock guard(table_->SharedMutex);
		auto first = index_.lower_bound(&ComparePositionDetail);
		FillCompareRecord(upperTradingDay, upperAccountId, upperExchangeId, upperInstrumentId, upperPosiDirection);
		auto last = index_.upper_bound(&ComparePositionDetail);
		return TableRangeLease<iterator>(std::move(guard), first, last);
	}
	TableRangeLease<PositionDetailIndexTradeMatch::iterator> PositionDetailIndexTradeMatch::EqualRange(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const PosiDirectionType& PosiDirection)
	{
		FillCompareRecord(TradingDay, AccountId, ExchangeId, InstrumentId, PosiDirection);
		std::shared_lock guard(table_->SharedMutex);
		auto range = index_.equal_range(&ComparePositionDetail);
		return TableRangeLease<iterator>(std::move(guard), range.first, range.second);
	}
	void PositionDetailIndexTradeMatch::Insert(PositionDetail* const record)
	{
		index_.insert(record);
	}
	void PositionDetailIndexTradeMatch::Erase(PositionDetail* const record)
	{
		auto it = FindNode(record);
		index_.erase(it);
	}
	void PositionDetailIndexTradeMatch::Update(iterator it)
	{
		auto record = *it;
		index_.erase(it);
		index_.insert(record);
	}
	bool PositionDetailIndexTradeMatch::NeedUpdate(const PositionDetail* const oldRecord, const PositionDetail* const newRecord)
	{
		return !(PositionDetailEqualForTradeMatchIndex()(oldRecord, newRecord));
	}
	PositionDetailIndexTradeMatch::iterator PositionDetailIndexTradeMatch::FindNode(PositionDetail* const record)
	{
		auto p = index_.equal_range(record);
		for (auto it = p.first; it != p.second; ++it)
		{
			if (*it == record)
			{
				return it;
			}
		}
		return index_.end();
	}
	void PositionDetailIndexTradeMatch::FillCompareRecord(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const PosiDirectionType& PosiDirection)
	{
		Utility::Strcpy(ComparePositionDetail.TradingDay, TradingDay);
		Utility::Strcpy(ComparePositionDetail.AccountId, AccountId);
		Utility::Strcpy(ComparePositionDetail.ExchangeId, ExchangeId);
		Utility::Strcpy(ComparePositionDetail.InstrumentId, InstrumentId);
		ComparePositionDetail.PosiDirection = PosiDirection;
	}
	
	PositionDetailIndexTradingDay::PositionDetailIndexTradingDay(PositionDetailTable* tableOwner)
		:table_(tableOwner)
	{
	}
	TableRangeLease<PositionDetailIndexTradingDay::iterator> PositionDetailIndexTradingDay::Range(const DateType& lowerTradingDay, const DateType& upperTradingDay)
	{
		FillCompareRecord(lowerTradingDay);
		std::shared_lock guard(table_->SharedMutex);
		auto first = index_.lower_bound(&ComparePositionDetail);
		FillCompareRecord(upperTradingDay);
		auto last = index_.upper_bound(&ComparePositionDetail);
		return TableRangeLease<iterator>(std::move(guard), first, last);
	}
	TableRangeLease<PositionDetailIndexTradingDay::iterator> PositionDetailIndexTradingDay::EqualRange(const DateType& TradingDay)
	{
		FillCompareRecord(TradingDay);
		std::shared_lock guard(table_->SharedMutex);
		auto range = index_.equal_range(&ComparePositionDetail);
		return TableRangeLease<iterator>(std::move(guard), range.first, range.second);
	}
	void PositionDetailIndexTradingDay::Insert(PositionDetail* const record)
	{
		index_.insert(record);
	}
	void PositionDetailIndexTradingDay::Erase(PositionDetail* const record)
	{
		auto it = FindNode(record);
		index_.erase(it);
	}
	void PositionDetailIndexTradingDay::Update(iterator it)
	{
		auto record = *it;
		index_.erase(it);
		index_.insert(record);
	}
	bool PositionDetailIndexTradingDay::NeedUpdate(const PositionDetail* const oldRecord, const PositionDetail* const newRecord)
	{
		return !(PositionDetailEqualForTradingDayIndex()(oldRecord, newRecord));
	}
	PositionDetailIndexTradingDay::iterator PositionDetailIndexTradingDay::FindNode(PositionDetail* const record)
	{
		auto p = index_.equal_range(record);
		for (auto it = p.first; it != p.second; ++it)
		{
			if (*it == record)
			{
				return it;
			}
		}
		return index_.end();
	}
	void PositionDetailIndexTradingDay::FillCompareRecord(const DateType& TradingDay)
	{
		Utility::Strcpy(ComparePositionDetail.TradingDay, TradingDay);
	}
	
}
