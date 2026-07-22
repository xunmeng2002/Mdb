#pragma once
#include "MdbStructs.h"
#include "MdbIndexComp.h"
#include <set>

using std::multiset;

namespace mdb
{
	class PrimaryAccountTable;
	class PrimaryAccountIndexOfferID
	{
		using iterator = std::multiset<PrimaryAccount*, PrimaryAccountLessForOfferIDIndex>::iterator;
		friend class PrimaryAccountTable;
	public:
		PrimaryAccountIndexOfferID(PrimaryAccountTable* table);
		iterator LowerBound(const OfferIDType& OfferID);
		iterator UpperBound(const OfferIDType& OfferID);
		std::pair<iterator, iterator> EqualRange(const OfferIDType& OfferID);
	public:
		static constexpr unsigned int IndexID = 0x0000;
	protected:
		void Insert(PrimaryAccount* const record);
		void Erase(PrimaryAccount* const record);
		void Update(iterator it);
		bool NeedUpdate(const PrimaryAccount* const oldRecord, const PrimaryAccount* const newRecord);
		iterator FindNode(PrimaryAccount* const record);
		void FillCompareRecord(const OfferIDType& OfferID);

	private:
		PrimaryAccountTable* m_Table;
		multiset<PrimaryAccount*, PrimaryAccountLessForOfferIDIndex> m_Index;
	};
	
	class CapitalTable;
	class CapitalIndexTradingDay
	{
		using iterator = std::multiset<Capital*, CapitalLessForTradingDayIndex>::iterator;
		friend class CapitalTable;
	public:
		CapitalIndexTradingDay(CapitalTable* table);
		iterator LowerBound(const DateType& TradingDay);
		iterator UpperBound(const DateType& TradingDay);
		std::pair<iterator, iterator> EqualRange(const DateType& TradingDay);
	public:
		static constexpr unsigned int IndexID = 0x0000;
	protected:
		void Insert(Capital* const record);
		void Erase(Capital* const record);
		void Update(iterator it);
		bool NeedUpdate(const Capital* const oldRecord, const Capital* const newRecord);
		iterator FindNode(Capital* const record);
		void FillCompareRecord(const DateType& TradingDay);

	private:
		CapitalTable* m_Table;
		multiset<Capital*, CapitalLessForTradingDayIndex> m_Index;
	};
	
	class PositionTable;
	class PositionIndexAccount
	{
		using iterator = std::multiset<Position*, PositionLessForAccountIndex>::iterator;
		friend class PositionTable;
	public:
		PositionIndexAccount(PositionTable* table);
		iterator LowerBound(const DateType& TradingDay, const AccountIDType& AccountID);
		iterator UpperBound(const DateType& TradingDay, const AccountIDType& AccountID);
		std::pair<iterator, iterator> EqualRange(const DateType& TradingDay, const AccountIDType& AccountID);
	public:
		static constexpr unsigned int IndexID = 0x0000;
	protected:
		void Insert(Position* const record);
		void Erase(Position* const record);
		void Update(iterator it);
		bool NeedUpdate(const Position* const oldRecord, const Position* const newRecord);
		iterator FindNode(Position* const record);
		void FillCompareRecord(const DateType& TradingDay, const AccountIDType& AccountID);

	private:
		PositionTable* m_Table;
		multiset<Position*, PositionLessForAccountIndex> m_Index;
	};
	
	class PositionIndexTradingDay
	{
		using iterator = std::multiset<Position*, PositionLessForTradingDayIndex>::iterator;
		friend class PositionTable;
	public:
		PositionIndexTradingDay(PositionTable* table);
		iterator LowerBound(const DateType& TradingDay);
		iterator UpperBound(const DateType& TradingDay);
		std::pair<iterator, iterator> EqualRange(const DateType& TradingDay);
	public:
		static constexpr unsigned int IndexID = 0x0001;
	protected:
		void Insert(Position* const record);
		void Erase(Position* const record);
		void Update(iterator it);
		bool NeedUpdate(const Position* const oldRecord, const Position* const newRecord);
		iterator FindNode(Position* const record);
		void FillCompareRecord(const DateType& TradingDay);

	private:
		PositionTable* m_Table;
		multiset<Position*, PositionLessForTradingDayIndex> m_Index;
	};
	
	class PositionDetailTable;
	class PositionDetailIndexTradeMatch
	{
		using iterator = std::multiset<PositionDetail*, PositionDetailLessForTradeMatchIndex>::iterator;
		friend class PositionDetailTable;
	public:
		PositionDetailIndexTradeMatch(PositionDetailTable* table);
		iterator LowerBound(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection);
		iterator UpperBound(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection);
		std::pair<iterator, iterator> EqualRange(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection);
	public:
		static constexpr unsigned int IndexID = 0x0000;
	protected:
		void Insert(PositionDetail* const record);
		void Erase(PositionDetail* const record);
		void Update(iterator it);
		bool NeedUpdate(const PositionDetail* const oldRecord, const PositionDetail* const newRecord);
		iterator FindNode(PositionDetail* const record);
		void FillCompareRecord(const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection);

	private:
		PositionDetailTable* m_Table;
		multiset<PositionDetail*, PositionDetailLessForTradeMatchIndex> m_Index;
	};
	
	class PositionDetailIndexTradingDay
	{
		using iterator = std::multiset<PositionDetail*, PositionDetailLessForTradingDayIndex>::iterator;
		friend class PositionDetailTable;
	public:
		PositionDetailIndexTradingDay(PositionDetailTable* table);
		iterator LowerBound(const DateType& TradingDay);
		iterator UpperBound(const DateType& TradingDay);
		std::pair<iterator, iterator> EqualRange(const DateType& TradingDay);
	public:
		static constexpr unsigned int IndexID = 0x0001;
	protected:
		void Insert(PositionDetail* const record);
		void Erase(PositionDetail* const record);
		void Update(iterator it);
		bool NeedUpdate(const PositionDetail* const oldRecord, const PositionDetail* const newRecord);
		iterator FindNode(PositionDetail* const record);
		void FillCompareRecord(const DateType& TradingDay);

	private:
		PositionDetailTable* m_Table;
		multiset<PositionDetail*, PositionDetailLessForTradingDayIndex> m_Index;
	};
	
}
