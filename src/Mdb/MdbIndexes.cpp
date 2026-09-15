// 本文件由 ../Templates/Cpp/Mdb/MdbIndexes.cpp.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#include "MdbIndexes.h"
#include "MdbTables.h"
#include <Spark/Core/Utility/Utility.h>
#include <string.h>


using namespace spark::core;
namespace mdb
{
	PrimaryAccountIndexOfferID::PrimaryAccountIndexOfferID(PrimaryAccountTable* tableOwner)
		:table(tableOwner)
	{
	}
	PrimaryAccountIndexOfferID::iterator PrimaryAccountIndexOfferID::LowerBound(const OfferIDType& OfferID)
	{
		FillCompareRecord(OfferID);
		std::shared_lock guard(table->sharedMutex);
		return index.lower_bound(&ComparePrimaryAccount);
	}
	PrimaryAccountIndexOfferID::iterator PrimaryAccountIndexOfferID::UpperBound(const OfferIDType& OfferID)
	{
		FillCompareRecord(OfferID);
		std::shared_lock guard(table->sharedMutex);
		return index.upper_bound(&ComparePrimaryAccount);
	}
	std::pair<PrimaryAccountIndexOfferID::iterator, PrimaryAccountIndexOfferID::iterator> PrimaryAccountIndexOfferID::EqualRange(const OfferIDType& OfferID)
	{
		FillCompareRecord(OfferID);
		std::shared_lock guard(table->sharedMutex);
		return index.equal_range(&ComparePrimaryAccount);
	}
	void PrimaryAccountIndexOfferID::Insert(PrimaryAccount* const record)
	{
		index.insert(record);
	}
	void PrimaryAccountIndexOfferID::Erase(PrimaryAccount* const record)
	{
		auto it = FindNode(record);
		index.erase(it);
	}
	void PrimaryAccountIndexOfferID::Update(iterator it)
	{
		auto record = *it;
		index.erase(it);
		index.insert(record);
	}
	bool PrimaryAccountIndexOfferID::NeedUpdate(const PrimaryAccount* const oldRecord, const PrimaryAccount* const newRecord)
	{
		return !(PrimaryAccountEqualForOfferIDIndex()(oldRecord, newRecord));
	}
	PrimaryAccountIndexOfferID::iterator PrimaryAccountIndexOfferID::FindNode(PrimaryAccount* const record)
	{
		auto p = index.equal_range(record);
		for (auto it = p.first; it != p.second; ++it)
		{
			if (*it == record)
			{
				return it;
			}
		}
		return index.end();
	}
	void PrimaryAccountIndexOfferID::FillCompareRecord(const OfferIDType& OfferID)
	{
		ComparePrimaryAccount.OfferID = OfferID;
	}
	
	CapitalIndexTradingDay::CapitalIndexTradingDay(CapitalTable* tableOwner)
		:table(tableOwner)
	{
	}
	CapitalIndexTradingDay::iterator CapitalIndexTradingDay::LowerBound(const DateType& TradingDay)
	{
		FillCompareRecord(TradingDay);
		std::shared_lock guard(table->sharedMutex);
		return index.lower_bound(&CompareCapital);
	}
	CapitalIndexTradingDay::iterator CapitalIndexTradingDay::UpperBound(const DateType& TradingDay)
	{
		FillCompareRecord(TradingDay);
		std::shared_lock guard(table->sharedMutex);
		return index.upper_bound(&CompareCapital);
	}
	std::pair<CapitalIndexTradingDay::iterator, CapitalIndexTradingDay::iterator> CapitalIndexTradingDay::EqualRange(const DateType& TradingDay)
	{
		FillCompareRecord(TradingDay);
		std::shared_lock guard(table->sharedMutex);
		return index.equal_range(&CompareCapital);
	}
	void CapitalIndexTradingDay::Insert(Capital* const record)
	{
		index.insert(record);
	}
	void CapitalIndexTradingDay::Erase(Capital* const record)
	{
		auto it = FindNode(record);
		index.erase(it);
	}
	void CapitalIndexTradingDay::Update(iterator it)
	{
		auto record = *it;
		index.erase(it);
		index.insert(record);
	}
	bool CapitalIndexTradingDay::NeedUpdate(const Capital* const oldRecord, const Capital* const newRecord)
	{
		return !(CapitalEqualForTradingDayIndex()(oldRecord, newRecord));
	}
	CapitalIndexTradingDay::iterator CapitalIndexTradingDay::FindNode(Capital* const record)
	{
		auto p = index.equal_range(record);
		for (auto it = p.first; it != p.second; ++it)
		{
			if (*it == record)
			{
				return it;
			}
		}
		return index.end();
	}
	void CapitalIndexTradingDay::FillCompareRecord(const DateType& TradingDay)
	{
		Utility::Strcpy(CompareCapital.TradingDay, TradingDay);
	}
	
	PositionIndexAccount::PositionIndexAccount(PositionTable* tableOwner)
		:table(tableOwner)
	{
	}
	PositionIndexAccount::iterator PositionIndexAccount::LowerBound(const DateType& TradingDay, const AccountIDType& AccountID)
	{
		FillCompareRecord(TradingDay, AccountID);
		std::shared_lock guard(table->sharedMutex);
		return index.lower_bound(&ComparePosition);
	}
	PositionIndexAccount::iterator PositionIndexAccount::UpperBound(const DateType& TradingDay, const AccountIDType& AccountID)
	{
		FillCompareRecord(TradingDay, AccountID);
		std::shared_lock guard(table->sharedMutex);
		return index.upper_bound(&ComparePosition);
	}
	std::pair<PositionIndexAccount::iterator, PositionIndexAccount::iterator> PositionIndexAccount::EqualRange(const DateType& TradingDay, const AccountIDType& AccountID)
	{
		FillCompareRecord(TradingDay, AccountID);
		std::shared_lock guard(table->sharedMutex);
		return index.equal_range(&ComparePosition);
	}
	void PositionIndexAccount::Insert(Position* const record)
	{
		index.insert(record);
	}
	void PositionIndexAccount::Erase(Position* const record)
	{
		auto it = FindNode(record);
		index.erase(it);
	}
	void PositionIndexAccount::Update(iterator it)
	{
		auto record = *it;
		index.erase(it);
		index.insert(record);
	}
	bool PositionIndexAccount::NeedUpdate(const Position* const oldRecord, const Position* const newRecord)
	{
		return !(PositionEqualForAccountIndex()(oldRecord, newRecord));
	}
	PositionIndexAccount::iterator PositionIndexAccount::FindNode(Position* const record)
	{
		auto p = index.equal_range(record);
		for (auto it = p.first; it != p.second; ++it)
		{
			if (*it == record)
			{
				return it;
			}
		}
		return index.end();
	}
	void PositionIndexAccount::FillCompareRecord(const DateType& TradingDay, const AccountIDType& AccountID)
	{
		Utility::Strcpy(ComparePosition.TradingDay, TradingDay);
		Utility::Strcpy(ComparePosition.AccountID, AccountID);
	}
	
	PositionIndexTradingDay::PositionIndexTradingDay(PositionTable* tableOwner)
		:table(tableOwner)
	{
	}
	PositionIndexTradingDay::iterator PositionIndexTradingDay::LowerBound(const DateType& TradingDay)
	{
		FillCompareRecord(TradingDay);
		std::shared_lock guard(table->sharedMutex);
		return index.lower_bound(&ComparePosition);
	}
	PositionIndexTradingDay::iterator PositionIndexTradingDay::UpperBound(const DateType& TradingDay)
	{
		FillCompareRecord(TradingDay);
		std::shared_lock guard(table->sharedMutex);
		return index.upper_bound(&ComparePosition);
	}
	std::pair<PositionIndexTradingDay::iterator, PositionIndexTradingDay::iterator> PositionIndexTradingDay::EqualRange(const DateType& TradingDay)
	{
		FillCompareRecord(TradingDay);
		std::shared_lock guard(table->sharedMutex);
		return index.equal_range(&ComparePosition);
	}
	void PositionIndexTradingDay::Insert(Position* const record)
	{
		index.insert(record);
	}
	void PositionIndexTradingDay::Erase(Position* const record)
	{
		auto it = FindNode(record);
		index.erase(it);
	}
	void PositionIndexTradingDay::Update(iterator it)
	{
		auto record = *it;
		index.erase(it);
		index.insert(record);
	}
	bool PositionIndexTradingDay::NeedUpdate(const Position* const oldRecord, const Position* const newRecord)
	{
		return !(PositionEqualForTradingDayIndex()(oldRecord, newRecord));
	}
	PositionIndexTradingDay::iterator PositionIndexTradingDay::FindNode(Position* const record)
	{
		auto p = index.equal_range(record);
		for (auto it = p.first; it != p.second; ++it)
		{
			if (*it == record)
			{
				return it;
			}
		}
		return index.end();
	}
	void PositionIndexTradingDay::FillCompareRecord(const DateType& TradingDay)
	{
		Utility::Strcpy(ComparePosition.TradingDay, TradingDay);
	}
	
	PositionDetailIndexTradeMatch::PositionDetailIndexTradeMatch(PositionDetailTable* tableOwner)
		:table(tableOwner)
	{
	}
	PositionDetailIndexTradeMatch::iterator PositionDetailIndexTradeMatch::LowerBound(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection)
	{
		FillCompareRecord(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection);
		std::shared_lock guard(table->sharedMutex);
		return index.lower_bound(&ComparePositionDetail);
	}
	PositionDetailIndexTradeMatch::iterator PositionDetailIndexTradeMatch::UpperBound(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection)
	{
		FillCompareRecord(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection);
		std::shared_lock guard(table->sharedMutex);
		return index.upper_bound(&ComparePositionDetail);
	}
	std::pair<PositionDetailIndexTradeMatch::iterator, PositionDetailIndexTradeMatch::iterator> PositionDetailIndexTradeMatch::EqualRange(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection)
	{
		FillCompareRecord(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection);
		std::shared_lock guard(table->sharedMutex);
		return index.equal_range(&ComparePositionDetail);
	}
	void PositionDetailIndexTradeMatch::Insert(PositionDetail* const record)
	{
		index.insert(record);
	}
	void PositionDetailIndexTradeMatch::Erase(PositionDetail* const record)
	{
		auto it = FindNode(record);
		index.erase(it);
	}
	void PositionDetailIndexTradeMatch::Update(iterator it)
	{
		auto record = *it;
		index.erase(it);
		index.insert(record);
	}
	bool PositionDetailIndexTradeMatch::NeedUpdate(const PositionDetail* const oldRecord, const PositionDetail* const newRecord)
	{
		return !(PositionDetailEqualForTradeMatchIndex()(oldRecord, newRecord));
	}
	PositionDetailIndexTradeMatch::iterator PositionDetailIndexTradeMatch::FindNode(PositionDetail* const record)
	{
		auto p = index.equal_range(record);
		for (auto it = p.first; it != p.second; ++it)
		{
			if (*it == record)
			{
				return it;
			}
		}
		return index.end();
	}
	void PositionDetailIndexTradeMatch::FillCompareRecord(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection)
	{
		Utility::Strcpy(ComparePositionDetail.TradingDay, TradingDay);
		Utility::Strcpy(ComparePositionDetail.AccountID, AccountID);
		Utility::Strcpy(ComparePositionDetail.ExchangeID, ExchangeID);
		Utility::Strcpy(ComparePositionDetail.InstrumentID, InstrumentID);
		ComparePositionDetail.PosiDirection = PosiDirection;
	}
	
	PositionDetailIndexTradingDay::PositionDetailIndexTradingDay(PositionDetailTable* tableOwner)
		:table(tableOwner)
	{
	}
	PositionDetailIndexTradingDay::iterator PositionDetailIndexTradingDay::LowerBound(const DateType& TradingDay)
	{
		FillCompareRecord(TradingDay);
		std::shared_lock guard(table->sharedMutex);
		return index.lower_bound(&ComparePositionDetail);
	}
	PositionDetailIndexTradingDay::iterator PositionDetailIndexTradingDay::UpperBound(const DateType& TradingDay)
	{
		FillCompareRecord(TradingDay);
		std::shared_lock guard(table->sharedMutex);
		return index.upper_bound(&ComparePositionDetail);
	}
	std::pair<PositionDetailIndexTradingDay::iterator, PositionDetailIndexTradingDay::iterator> PositionDetailIndexTradingDay::EqualRange(const DateType& TradingDay)
	{
		FillCompareRecord(TradingDay);
		std::shared_lock guard(table->sharedMutex);
		return index.equal_range(&ComparePositionDetail);
	}
	void PositionDetailIndexTradingDay::Insert(PositionDetail* const record)
	{
		index.insert(record);
	}
	void PositionDetailIndexTradingDay::Erase(PositionDetail* const record)
	{
		auto it = FindNode(record);
		index.erase(it);
	}
	void PositionDetailIndexTradingDay::Update(iterator it)
	{
		auto record = *it;
		index.erase(it);
		index.insert(record);
	}
	bool PositionDetailIndexTradingDay::NeedUpdate(const PositionDetail* const oldRecord, const PositionDetail* const newRecord)
	{
		return !(PositionDetailEqualForTradingDayIndex()(oldRecord, newRecord));
	}
	PositionDetailIndexTradingDay::iterator PositionDetailIndexTradingDay::FindNode(PositionDetail* const record)
	{
		auto p = index.equal_range(record);
		for (auto it = p.first; it != p.second; ++it)
		{
			if (*it == record)
			{
				return it;
			}
		}
		return index.end();
	}
	void PositionDetailIndexTradingDay::FillCompareRecord(const DateType& TradingDay)
	{
		Utility::Strcpy(ComparePositionDetail.TradingDay, TradingDay);
	}
	
}
