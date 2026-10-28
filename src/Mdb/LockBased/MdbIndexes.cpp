// 本文件由 ../Templates/Cpp/Mdb/LockBased/MdbIndexes.cpp.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#include "MdbIndexes.h"
#include "MdbTables.h"
#include <Spark/Core/Utility/Utility.h>
#include <string.h>
#include <utility>


using namespace Spark::Core;
namespace Mdb
{
	PrimaryAccountTableIndexOfferId::PrimaryAccountTableIndexOfferId(PrimaryAccountTable* tableOwner)
		:table_(tableOwner)
	{
	}
	TableRangeLease<PrimaryAccountTableIndexOfferId::iterator> PrimaryAccountTableIndexOfferId::Range(const OfferIdType& lowerOfferId, const OfferIdType& upperOfferId)
	{
		FillCompareRecord(lowerOfferId);
		std::shared_lock guard(table_->SharedMutex);
		auto first = index_.lower_bound(&ComparePrimaryAccount);
		FillCompareRecord(upperOfferId);
		auto last = index_.upper_bound(&ComparePrimaryAccount);
		return TableRangeLease<iterator>(std::move(guard), first, last);
	}
	TableRangeLease<PrimaryAccountTableIndexOfferId::iterator> PrimaryAccountTableIndexOfferId::EqualRange(const OfferIdType& OfferId)
	{
		FillCompareRecord(OfferId);
		std::shared_lock guard(table_->SharedMutex);
		auto range = index_.equal_range(&ComparePrimaryAccount);
		return TableRangeLease<iterator>(std::move(guard), range.first, range.second);
	}
	void PrimaryAccountTableIndexOfferId::Insert(PrimaryAccount* const record)
	{
		index_.insert(record);
	}
	void PrimaryAccountTableIndexOfferId::Erase(PrimaryAccount* const record)
	{
		auto it = FindNode(record);
		index_.erase(it);
	}
	void PrimaryAccountTableIndexOfferId::Update(iterator it)
	{
		auto record = *it;
		index_.erase(it);
		index_.insert(record);
	}
	bool PrimaryAccountTableIndexOfferId::NeedUpdate(const PrimaryAccount* const oldRecord, const PrimaryAccount* const newRecord)
	{
		return !(PrimaryAccountEqualForOfferIdIndex()(oldRecord, newRecord));
	}
	PrimaryAccountTableIndexOfferId::iterator PrimaryAccountTableIndexOfferId::FindNode(PrimaryAccount* const record)
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
	void PrimaryAccountTableIndexOfferId::FillCompareRecord(const OfferIdType& OfferId)
	{
		ComparePrimaryAccount.OfferId = OfferId;
	}
	
	CapitalTableIndexTradingDay::CapitalTableIndexTradingDay(CapitalTable* tableOwner)
		:table_(tableOwner)
	{
	}
	TableRangeLease<CapitalTableIndexTradingDay::iterator> CapitalTableIndexTradingDay::Range(const DateType& lowerTradingDay, const DateType& upperTradingDay)
	{
		FillCompareRecord(lowerTradingDay);
		std::shared_lock guard(table_->SharedMutex);
		auto first = index_.lower_bound(&CompareCapital);
		FillCompareRecord(upperTradingDay);
		auto last = index_.upper_bound(&CompareCapital);
		return TableRangeLease<iterator>(std::move(guard), first, last);
	}
	TableRangeLease<CapitalTableIndexTradingDay::iterator> CapitalTableIndexTradingDay::EqualRange(const DateType& TradingDay)
	{
		FillCompareRecord(TradingDay);
		std::shared_lock guard(table_->SharedMutex);
		auto range = index_.equal_range(&CompareCapital);
		return TableRangeLease<iterator>(std::move(guard), range.first, range.second);
	}
	void CapitalTableIndexTradingDay::Insert(Capital* const record)
	{
		index_.insert(record);
	}
	void CapitalTableIndexTradingDay::Erase(Capital* const record)
	{
		auto it = FindNode(record);
		index_.erase(it);
	}
	void CapitalTableIndexTradingDay::Update(iterator it)
	{
		auto record = *it;
		index_.erase(it);
		index_.insert(record);
	}
	bool CapitalTableIndexTradingDay::NeedUpdate(const Capital* const oldRecord, const Capital* const newRecord)
	{
		return !(CapitalEqualForTradingDayIndex()(oldRecord, newRecord));
	}
	CapitalTableIndexTradingDay::iterator CapitalTableIndexTradingDay::FindNode(Capital* const record)
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
	void CapitalTableIndexTradingDay::FillCompareRecord(const DateType& TradingDay)
	{
		Utility::Strcpy(CompareCapital.TradingDay, TradingDay);
	}
	
	PositionTableIndexAccount::PositionTableIndexAccount(PositionTable* tableOwner)
		:table_(tableOwner)
	{
	}
	TableRangeLease<PositionTableIndexAccount::iterator> PositionTableIndexAccount::Range(const DateType& lowerTradingDay, const AccountIdType& lowerAccountId, const DateType& upperTradingDay, const AccountIdType& upperAccountId)
	{
		FillCompareRecord(lowerTradingDay, lowerAccountId);
		std::shared_lock guard(table_->SharedMutex);
		auto first = index_.lower_bound(&ComparePosition);
		FillCompareRecord(upperTradingDay, upperAccountId);
		auto last = index_.upper_bound(&ComparePosition);
		return TableRangeLease<iterator>(std::move(guard), first, last);
	}
	TableRangeLease<PositionTableIndexAccount::iterator> PositionTableIndexAccount::EqualRange(const DateType& TradingDay, const AccountIdType& AccountId)
	{
		FillCompareRecord(TradingDay, AccountId);
		std::shared_lock guard(table_->SharedMutex);
		auto range = index_.equal_range(&ComparePosition);
		return TableRangeLease<iterator>(std::move(guard), range.first, range.second);
	}
	void PositionTableIndexAccount::Insert(Position* const record)
	{
		index_.insert(record);
	}
	void PositionTableIndexAccount::Erase(Position* const record)
	{
		auto it = FindNode(record);
		index_.erase(it);
	}
	void PositionTableIndexAccount::Update(iterator it)
	{
		auto record = *it;
		index_.erase(it);
		index_.insert(record);
	}
	bool PositionTableIndexAccount::NeedUpdate(const Position* const oldRecord, const Position* const newRecord)
	{
		return !(PositionEqualForAccountIndex()(oldRecord, newRecord));
	}
	PositionTableIndexAccount::iterator PositionTableIndexAccount::FindNode(Position* const record)
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
	void PositionTableIndexAccount::FillCompareRecord(const DateType& TradingDay, const AccountIdType& AccountId)
	{
		Utility::Strcpy(ComparePosition.TradingDay, TradingDay);
		Utility::Strcpy(ComparePosition.AccountId, AccountId);
	}
	
	PositionTableIndexTradingDay::PositionTableIndexTradingDay(PositionTable* tableOwner)
		:table_(tableOwner)
	{
	}
	TableRangeLease<PositionTableIndexTradingDay::iterator> PositionTableIndexTradingDay::Range(const DateType& lowerTradingDay, const DateType& upperTradingDay)
	{
		FillCompareRecord(lowerTradingDay);
		std::shared_lock guard(table_->SharedMutex);
		auto first = index_.lower_bound(&ComparePosition);
		FillCompareRecord(upperTradingDay);
		auto last = index_.upper_bound(&ComparePosition);
		return TableRangeLease<iterator>(std::move(guard), first, last);
	}
	TableRangeLease<PositionTableIndexTradingDay::iterator> PositionTableIndexTradingDay::EqualRange(const DateType& TradingDay)
	{
		FillCompareRecord(TradingDay);
		std::shared_lock guard(table_->SharedMutex);
		auto range = index_.equal_range(&ComparePosition);
		return TableRangeLease<iterator>(std::move(guard), range.first, range.second);
	}
	void PositionTableIndexTradingDay::Insert(Position* const record)
	{
		index_.insert(record);
	}
	void PositionTableIndexTradingDay::Erase(Position* const record)
	{
		auto it = FindNode(record);
		index_.erase(it);
	}
	void PositionTableIndexTradingDay::Update(iterator it)
	{
		auto record = *it;
		index_.erase(it);
		index_.insert(record);
	}
	bool PositionTableIndexTradingDay::NeedUpdate(const Position* const oldRecord, const Position* const newRecord)
	{
		return !(PositionEqualForTradingDayIndex()(oldRecord, newRecord));
	}
	PositionTableIndexTradingDay::iterator PositionTableIndexTradingDay::FindNode(Position* const record)
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
	void PositionTableIndexTradingDay::FillCompareRecord(const DateType& TradingDay)
	{
		Utility::Strcpy(ComparePosition.TradingDay, TradingDay);
	}
	
	PositionDetailTableIndexTradeMatch::PositionDetailTableIndexTradeMatch(PositionDetailTable* tableOwner)
		:table_(tableOwner)
	{
	}
	TableRangeLease<PositionDetailTableIndexTradeMatch::iterator> PositionDetailTableIndexTradeMatch::Range(const DateType& lowerTradingDay, const AccountIdType& lowerAccountId, const ExchangeIdType& lowerExchangeId, const InstrumentIdType& lowerInstrumentId, const PosiDirectionType& lowerPosiDirection, const DateType& upperTradingDay, const AccountIdType& upperAccountId, const ExchangeIdType& upperExchangeId, const InstrumentIdType& upperInstrumentId, const PosiDirectionType& upperPosiDirection)
	{
		FillCompareRecord(lowerTradingDay, lowerAccountId, lowerExchangeId, lowerInstrumentId, lowerPosiDirection);
		std::shared_lock guard(table_->SharedMutex);
		auto first = index_.lower_bound(&ComparePositionDetail);
		FillCompareRecord(upperTradingDay, upperAccountId, upperExchangeId, upperInstrumentId, upperPosiDirection);
		auto last = index_.upper_bound(&ComparePositionDetail);
		return TableRangeLease<iterator>(std::move(guard), first, last);
	}
	TableRangeLease<PositionDetailTableIndexTradeMatch::iterator> PositionDetailTableIndexTradeMatch::EqualRange(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const PosiDirectionType& PosiDirection)
	{
		FillCompareRecord(TradingDay, AccountId, ExchangeId, InstrumentId, PosiDirection);
		std::shared_lock guard(table_->SharedMutex);
		auto range = index_.equal_range(&ComparePositionDetail);
		return TableRangeLease<iterator>(std::move(guard), range.first, range.second);
	}
	void PositionDetailTableIndexTradeMatch::Insert(PositionDetail* const record)
	{
		index_.insert(record);
	}
	void PositionDetailTableIndexTradeMatch::Erase(PositionDetail* const record)
	{
		auto it = FindNode(record);
		index_.erase(it);
	}
	void PositionDetailTableIndexTradeMatch::Update(iterator it)
	{
		auto record = *it;
		index_.erase(it);
		index_.insert(record);
	}
	bool PositionDetailTableIndexTradeMatch::NeedUpdate(const PositionDetail* const oldRecord, const PositionDetail* const newRecord)
	{
		return !(PositionDetailEqualForTradeMatchIndex()(oldRecord, newRecord));
	}
	PositionDetailTableIndexTradeMatch::iterator PositionDetailTableIndexTradeMatch::FindNode(PositionDetail* const record)
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
	void PositionDetailTableIndexTradeMatch::FillCompareRecord(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const PosiDirectionType& PosiDirection)
	{
		Utility::Strcpy(ComparePositionDetail.TradingDay, TradingDay);
		Utility::Strcpy(ComparePositionDetail.AccountId, AccountId);
		Utility::Strcpy(ComparePositionDetail.ExchangeId, ExchangeId);
		Utility::Strcpy(ComparePositionDetail.InstrumentId, InstrumentId);
		ComparePositionDetail.PosiDirection = PosiDirection;
	}
	
	PositionDetailTableIndexTradingDay::PositionDetailTableIndexTradingDay(PositionDetailTable* tableOwner)
		:table_(tableOwner)
	{
	}
	TableRangeLease<PositionDetailTableIndexTradingDay::iterator> PositionDetailTableIndexTradingDay::Range(const DateType& lowerTradingDay, const DateType& upperTradingDay)
	{
		FillCompareRecord(lowerTradingDay);
		std::shared_lock guard(table_->SharedMutex);
		auto first = index_.lower_bound(&ComparePositionDetail);
		FillCompareRecord(upperTradingDay);
		auto last = index_.upper_bound(&ComparePositionDetail);
		return TableRangeLease<iterator>(std::move(guard), first, last);
	}
	TableRangeLease<PositionDetailTableIndexTradingDay::iterator> PositionDetailTableIndexTradingDay::EqualRange(const DateType& TradingDay)
	{
		FillCompareRecord(TradingDay);
		std::shared_lock guard(table_->SharedMutex);
		auto range = index_.equal_range(&ComparePositionDetail);
		return TableRangeLease<iterator>(std::move(guard), range.first, range.second);
	}
	void PositionDetailTableIndexTradingDay::Insert(PositionDetail* const record)
	{
		index_.insert(record);
	}
	void PositionDetailTableIndexTradingDay::Erase(PositionDetail* const record)
	{
		auto it = FindNode(record);
		index_.erase(it);
	}
	void PositionDetailTableIndexTradingDay::Update(iterator it)
	{
		auto record = *it;
		index_.erase(it);
		index_.insert(record);
	}
	bool PositionDetailTableIndexTradingDay::NeedUpdate(const PositionDetail* const oldRecord, const PositionDetail* const newRecord)
	{
		return !(PositionDetailEqualForTradingDayIndex()(oldRecord, newRecord));
	}
	PositionDetailTableIndexTradingDay::iterator PositionDetailTableIndexTradingDay::FindNode(PositionDetail* const record)
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
	void PositionDetailTableIndexTradingDay::FillCompareRecord(const DateType& TradingDay)
	{
		Utility::Strcpy(ComparePositionDetail.TradingDay, TradingDay);
	}
	
}
