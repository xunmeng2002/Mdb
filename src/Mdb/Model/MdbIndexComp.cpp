// 本文件由 ../Templates/Cpp/Mdb/MdbIndexComp.cpp.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#include "MdbIndexComp.h"
#include <string.h>


namespace Mdb
{
	bool PrimaryAccountEqualForOfferIdIndex::operator()(const PrimaryAccount* const left, const PrimaryAccount* const right) const
	{
		return left->OfferId == right->OfferId ;
	}
	bool PrimaryAccountLessForOfferIdIndex::operator()(const PrimaryAccount* const left, const PrimaryAccount* const right) const
	{
		if (left->OfferId < right->OfferId)
			return true;
		else if (left->OfferId > right->OfferId)
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
		return strcmp(left->TradingDay, right->TradingDay) == 0&& strcmp(left->AccountId, right->AccountId) == 0;
	}
	bool PositionLessForAccountIndex::operator()(const Position* const left, const Position* const right) const
	{
		if (strcmp(left->TradingDay, right->TradingDay) < 0)
			return true;
		else if (strcmp(left->TradingDay, right->TradingDay) > 0)
			return false;
		if (strcmp(left->AccountId, right->AccountId) < 0)
			return true;
		else if (strcmp(left->AccountId, right->AccountId) > 0)
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
		return strcmp(left->TradingDay, right->TradingDay) == 0&& strcmp(left->AccountId, right->AccountId) == 0&& strcmp(left->ExchangeId, right->ExchangeId) == 0&& strcmp(left->InstrumentId, right->InstrumentId) == 0&& left->PosiDirection == right->PosiDirection ;
	}
	bool PositionDetailLessForTradeMatchIndex::operator()(const PositionDetail* const left, const PositionDetail* const right) const
	{
		if (strcmp(left->TradingDay, right->TradingDay) < 0)
			return true;
		else if (strcmp(left->TradingDay, right->TradingDay) > 0)
			return false;
		if (strcmp(left->AccountId, right->AccountId) < 0)
			return true;
		else if (strcmp(left->AccountId, right->AccountId) > 0)
			return false;
		if (strcmp(left->ExchangeId, right->ExchangeId) < 0)
			return true;
		else if (strcmp(left->ExchangeId, right->ExchangeId) > 0)
			return false;
		if (strcmp(left->InstrumentId, right->InstrumentId) < 0)
			return true;
		else if (strcmp(left->InstrumentId, right->InstrumentId) > 0)
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
