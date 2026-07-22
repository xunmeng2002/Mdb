#include "MdbIndexComp.h"
#include <string.h>


namespace mdb
{
	bool PrimaryAccountEqualForOfferIDIndex::operator()(const PrimaryAccount* const left, const PrimaryAccount* const right) const
	{
		return left->OfferID == right->OfferID ;
	}
	bool PrimaryAccountLessForOfferIDIndex::operator()(const PrimaryAccount* const left, const PrimaryAccount* const right) const
	{
		if (left->OfferID < right->OfferID)
			return true;
		else if (left->OfferID > right->OfferID)
			return false;
		return false;
	}
	
	bool CapitalEqualForTradingDayIndex::operator()(const Capital* const left, const Capital* const right) const
	{
		return strcmp(left->TradingDay, right->TradingDay) == 0;
	}
	bool CapitalLessForTradingDayIndex::operator()(const Capital* const left, const Capital* const right) const
	{
		if (strcmp(left->TradingDay, right->TradingDay) < 0)
			return true;
		else if (strcmp(left->TradingDay, right->TradingDay) > 0)
			return false;
		return false;
	}
	
	bool PositionEqualForAccountIndex::operator()(const Position* const left, const Position* const right) const
	{
		return strcmp(left->TradingDay, right->TradingDay) == 0&& strcmp(left->AccountID, right->AccountID) == 0;
	}
	bool PositionLessForAccountIndex::operator()(const Position* const left, const Position* const right) const
	{
		if (strcmp(left->TradingDay, right->TradingDay) < 0)
			return true;
		else if (strcmp(left->TradingDay, right->TradingDay) > 0)
			return false;
		if (strcmp(left->AccountID, right->AccountID) < 0)
			return true;
		else if (strcmp(left->AccountID, right->AccountID) > 0)
			return false;
		return false;
	}
	
	bool PositionEqualForTradingDayIndex::operator()(const Position* const left, const Position* const right) const
	{
		return strcmp(left->TradingDay, right->TradingDay) == 0;
	}
	bool PositionLessForTradingDayIndex::operator()(const Position* const left, const Position* const right) const
	{
		if (strcmp(left->TradingDay, right->TradingDay) < 0)
			return true;
		else if (strcmp(left->TradingDay, right->TradingDay) > 0)
			return false;
		return false;
	}
	
	bool PositionDetailEqualForTradeMatchIndex::operator()(const PositionDetail* const left, const PositionDetail* const right) const
	{
		return strcmp(left->TradingDay, right->TradingDay) == 0&& strcmp(left->AccountID, right->AccountID) == 0&& strcmp(left->ExchangeID, right->ExchangeID) == 0&& strcmp(left->InstrumentID, right->InstrumentID) == 0&& left->PosiDirection == right->PosiDirection ;
	}
	bool PositionDetailLessForTradeMatchIndex::operator()(const PositionDetail* const left, const PositionDetail* const right) const
	{
		if (strcmp(left->TradingDay, right->TradingDay) < 0)
			return true;
		else if (strcmp(left->TradingDay, right->TradingDay) > 0)
			return false;
		if (strcmp(left->AccountID, right->AccountID) < 0)
			return true;
		else if (strcmp(left->AccountID, right->AccountID) > 0)
			return false;
		if (strcmp(left->ExchangeID, right->ExchangeID) < 0)
			return true;
		else if (strcmp(left->ExchangeID, right->ExchangeID) > 0)
			return false;
		if (strcmp(left->InstrumentID, right->InstrumentID) < 0)
			return true;
		else if (strcmp(left->InstrumentID, right->InstrumentID) > 0)
			return false;
		if (left->PosiDirection < right->PosiDirection)
			return true;
		else if (left->PosiDirection > right->PosiDirection)
			return false;
		return false;
	}
	
	bool PositionDetailEqualForTradingDayIndex::operator()(const PositionDetail* const left, const PositionDetail* const right) const
	{
		return strcmp(left->TradingDay, right->TradingDay) == 0;
	}
	bool PositionDetailLessForTradingDayIndex::operator()(const PositionDetail* const left, const PositionDetail* const right) const
	{
		if (strcmp(left->TradingDay, right->TradingDay) < 0)
			return true;
		else if (strcmp(left->TradingDay, right->TradingDay) > 0)
			return false;
		return false;
	}
	
}
