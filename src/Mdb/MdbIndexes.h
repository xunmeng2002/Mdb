// 本文件由 ../Templates/Cpp/Mdb/MdbIndexes.h.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#pragma once
#include "MdbStructs.h"
#include "MdbIndexComp.h"
#include <set>

using std::multiset;

namespace Mdb
{
	class PrimaryAccountTable;
	class PrimaryAccountIndexOfferId
	{
	public:
		using iterator = std::multiset<PrimaryAccount*, PrimaryAccountLessForOfferIdIndex>::iterator;
		static constexpr unsigned int IndexID = 0x0000;
	
		PrimaryAccountIndexOfferId(PrimaryAccountTable* tableOwner);
		iterator LowerBound(const OfferIdType& OfferId);
		iterator UpperBound(const OfferIdType& OfferId);
		std::pair<iterator, iterator> EqualRange(const OfferIdType& OfferId);
	
	protected:
		void Insert(PrimaryAccount* const record);
		void Erase(PrimaryAccount* const record);
		void Update(iterator it);
		bool NeedUpdate(const PrimaryAccount* const oldRecord, const PrimaryAccount* const newRecord);
		iterator FindNode(PrimaryAccount* const record);
		void FillCompareRecord(const OfferIdType& OfferId);

	private:
		friend class PrimaryAccountTable;
	
		PrimaryAccountTable* table_;
		multiset<PrimaryAccount*, PrimaryAccountLessForOfferIdIndex> index_;
	};
	
	class CapitalTable;
	class CapitalIndexTradingDay
	{
	public:
		using iterator = std::multiset<Capital*, CapitalLessForTradingDayIndex>::iterator;
		static constexpr unsigned int IndexID = 0x0000;
	
		CapitalIndexTradingDay(CapitalTable* tableOwner);
		iterator LowerBound(const DateType& TradingDay);
		iterator UpperBound(const DateType& TradingDay);
		std::pair<iterator, iterator> EqualRange(const DateType& TradingDay);
	
	protected:
		void Insert(Capital* const record);
		void Erase(Capital* const record);
		void Update(iterator it);
		bool NeedUpdate(const Capital* const oldRecord, const Capital* const newRecord);
		iterator FindNode(Capital* const record);
		void FillCompareRecord(const DateType& TradingDay);

	private:
		friend class CapitalTable;
	
		CapitalTable* table_;
		multiset<Capital*, CapitalLessForTradingDayIndex> index_;
	};
	
	class PositionTable;
	class PositionIndexAccount
	{
	public:
		using iterator = std::multiset<Position*, PositionLessForAccountIndex>::iterator;
		static constexpr unsigned int IndexID = 0x0000;
	
		PositionIndexAccount(PositionTable* tableOwner);
		iterator LowerBound(const DateType& TradingDay, const AccountIdType& AccountId);
		iterator UpperBound(const DateType& TradingDay, const AccountIdType& AccountId);
		std::pair<iterator, iterator> EqualRange(const DateType& TradingDay, const AccountIdType& AccountId);
	
	protected:
		void Insert(Position* const record);
		void Erase(Position* const record);
		void Update(iterator it);
		bool NeedUpdate(const Position* const oldRecord, const Position* const newRecord);
		iterator FindNode(Position* const record);
		void FillCompareRecord(const DateType& TradingDay, const AccountIdType& AccountId);

	private:
		friend class PositionTable;
	
		PositionTable* table_;
		multiset<Position*, PositionLessForAccountIndex> index_;
	};
	
	class PositionIndexTradingDay
	{
	public:
		using iterator = std::multiset<Position*, PositionLessForTradingDayIndex>::iterator;
		static constexpr unsigned int IndexID = 0x0001;
	
		PositionIndexTradingDay(PositionTable* tableOwner);
		iterator LowerBound(const DateType& TradingDay);
		iterator UpperBound(const DateType& TradingDay);
		std::pair<iterator, iterator> EqualRange(const DateType& TradingDay);
	
	protected:
		void Insert(Position* const record);
		void Erase(Position* const record);
		void Update(iterator it);
		bool NeedUpdate(const Position* const oldRecord, const Position* const newRecord);
		iterator FindNode(Position* const record);
		void FillCompareRecord(const DateType& TradingDay);

	private:
		friend class PositionTable;
	
		PositionTable* table_;
		multiset<Position*, PositionLessForTradingDayIndex> index_;
	};
	
	class PositionDetailTable;
	class PositionDetailIndexTradeMatch
	{
	public:
		using iterator = std::multiset<PositionDetail*, PositionDetailLessForTradeMatchIndex>::iterator;
		static constexpr unsigned int IndexID = 0x0000;
	
		PositionDetailIndexTradeMatch(PositionDetailTable* tableOwner);
		iterator LowerBound(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const PosiDirectionType& PosiDirection);
		iterator UpperBound(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const PosiDirectionType& PosiDirection);
		std::pair<iterator, iterator> EqualRange(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const PosiDirectionType& PosiDirection);
	
	protected:
		void Insert(PositionDetail* const record);
		void Erase(PositionDetail* const record);
		void Update(iterator it);
		bool NeedUpdate(const PositionDetail* const oldRecord, const PositionDetail* const newRecord);
		iterator FindNode(PositionDetail* const record);
		void FillCompareRecord(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const PosiDirectionType& PosiDirection);

	private:
		friend class PositionDetailTable;
	
		PositionDetailTable* table_;
		multiset<PositionDetail*, PositionDetailLessForTradeMatchIndex> index_;
	};
	
	class PositionDetailIndexTradingDay
	{
	public:
		using iterator = std::multiset<PositionDetail*, PositionDetailLessForTradingDayIndex>::iterator;
		static constexpr unsigned int IndexID = 0x0001;
	
		PositionDetailIndexTradingDay(PositionDetailTable* tableOwner);
		iterator LowerBound(const DateType& TradingDay);
		iterator UpperBound(const DateType& TradingDay);
		std::pair<iterator, iterator> EqualRange(const DateType& TradingDay);
	
	protected:
		void Insert(PositionDetail* const record);
		void Erase(PositionDetail* const record);
		void Update(iterator it);
		bool NeedUpdate(const PositionDetail* const oldRecord, const PositionDetail* const newRecord);
		iterator FindNode(PositionDetail* const record);
		void FillCompareRecord(const DateType& TradingDay);

	private:
		friend class PositionDetailTable;
	
		PositionDetailTable* table_;
		multiset<PositionDetail*, PositionDetailLessForTradingDayIndex> index_;
	};
	
}
