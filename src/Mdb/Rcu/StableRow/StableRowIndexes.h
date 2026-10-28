// 本文件由 ../Templates/Cpp/Mdb/Rcu/StableRow/StableRowIndexes.h.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#pragma once
#include "Common/MdbStructs.h"
#include "Common/MdbIndexComp.h"
#include "Common/MdbReadLease.h"
#include <set>

using std::multiset;

namespace Mdb::Rcu::StableRow
{
	class PrimaryAccountTable;
	class PrimaryAccountTableIndexOfferId
	{
	public:
		using iterator = std::multiset<PrimaryAccount*, PrimaryAccountLessForOfferIdIndex>::iterator;
		static constexpr unsigned int IndexID = PrimaryAccount::IndexOfferIdId;

		explicit PrimaryAccountTableIndexOfferId(PrimaryAccountTable* tableOwner);
		TableRangeLease<iterator> Range(const OfferIdType& lowerOfferId, const OfferIdType& upperOfferId);
		TableRangeLease<iterator> EqualRange(const OfferIdType& OfferId);

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
	class CapitalTableIndexTradingDay
	{
	public:
		using iterator = std::multiset<Capital*, CapitalLessForTradingDayIndex>::iterator;
		static constexpr unsigned int IndexID = Capital::IndexTradingDayId;

		explicit CapitalTableIndexTradingDay(CapitalTable* tableOwner);
		TableRangeLease<iterator> Range(const DateType& lowerTradingDay, const DateType& upperTradingDay);
		TableRangeLease<iterator> EqualRange(const DateType& TradingDay);

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
	class PositionTableIndexAccount
	{
	public:
		using iterator = std::multiset<Position*, PositionLessForAccountIndex>::iterator;
		static constexpr unsigned int IndexID = Position::IndexAccountId;

		explicit PositionTableIndexAccount(PositionTable* tableOwner);
		TableRangeLease<iterator> Range(const DateType& lowerTradingDay, const AccountIdType& lowerAccountId, const DateType& upperTradingDay, const AccountIdType& upperAccountId);
		TableRangeLease<iterator> EqualRange(const DateType& TradingDay, const AccountIdType& AccountId);

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

	class PositionTableIndexTradingDay
	{
	public:
		using iterator = std::multiset<Position*, PositionLessForTradingDayIndex>::iterator;
		static constexpr unsigned int IndexID = Position::IndexTradingDayId;

		explicit PositionTableIndexTradingDay(PositionTable* tableOwner);
		TableRangeLease<iterator> Range(const DateType& lowerTradingDay, const DateType& upperTradingDay);
		TableRangeLease<iterator> EqualRange(const DateType& TradingDay);

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
	class PositionDetailTableIndexTradeMatch
	{
	public:
		using iterator = std::multiset<PositionDetail*, PositionDetailLessForTradeMatchIndex>::iterator;
		static constexpr unsigned int IndexID = PositionDetail::IndexTradeMatchId;

		explicit PositionDetailTableIndexTradeMatch(PositionDetailTable* tableOwner);
		TableRangeLease<iterator> Range(const DateType& lowerTradingDay, const AccountIdType& lowerAccountId, const ExchangeIdType& lowerExchangeId, const InstrumentIdType& lowerInstrumentId, const PosiDirectionType& lowerPosiDirection, const DateType& upperTradingDay, const AccountIdType& upperAccountId, const ExchangeIdType& upperExchangeId, const InstrumentIdType& upperInstrumentId, const PosiDirectionType& upperPosiDirection);
		TableRangeLease<iterator> EqualRange(const DateType& TradingDay, const AccountIdType& AccountId, const ExchangeIdType& ExchangeId, const InstrumentIdType& InstrumentId, const PosiDirectionType& PosiDirection);

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

	class PositionDetailTableIndexTradingDay
	{
	public:
		using iterator = std::multiset<PositionDetail*, PositionDetailLessForTradingDayIndex>::iterator;
		static constexpr unsigned int IndexID = PositionDetail::IndexTradingDayId;

		explicit PositionDetailTableIndexTradingDay(PositionDetailTable* tableOwner);
		TableRangeLease<iterator> Range(const DateType& lowerTradingDay, const DateType& upperTradingDay);
		TableRangeLease<iterator> EqualRange(const DateType& TradingDay);

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
