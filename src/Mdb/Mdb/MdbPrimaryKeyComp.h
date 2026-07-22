#pragma once
#include "MdbStructs.h"
#include <stddef.h>

namespace mdb
{
	struct TradingDayEqualForTradingDayPrimaryKey
	{
		bool operator()(const TradingDay* const left, const TradingDay* const right) const;
	};
	struct TradingDayLessForTradingDayPrimaryKey
	{
		bool operator()(const TradingDay* const left, const TradingDay* const right) const;
	};
	struct TradingDayHashForTradingDayPrimaryKey
	{
		size_t operator()(const TradingDay* const record) const;
	};
	struct ExchangeEqualForExchangePrimaryKey
	{
		bool operator()(const Exchange* const left, const Exchange* const right) const;
	};
	struct ExchangeLessForExchangePrimaryKey
	{
		bool operator()(const Exchange* const left, const Exchange* const right) const;
	};
	struct ExchangeHashForExchangePrimaryKey
	{
		size_t operator()(const Exchange* const record) const;
	};
	struct ProductEqualForProductPrimaryKey
	{
		bool operator()(const Product* const left, const Product* const right) const;
	};
	struct ProductLessForProductPrimaryKey
	{
		bool operator()(const Product* const left, const Product* const right) const;
	};
	struct ProductHashForProductPrimaryKey
	{
		size_t operator()(const Product* const record) const;
	};
	struct InstrumentEqualForInstrumentPrimaryKey
	{
		bool operator()(const Instrument* const left, const Instrument* const right) const;
	};
	struct InstrumentLessForInstrumentPrimaryKey
	{
		bool operator()(const Instrument* const left, const Instrument* const right) const;
	};
	struct InstrumentHashForInstrumentPrimaryKey
	{
		size_t operator()(const Instrument* const record) const;
	};
	struct PrimaryAccountEqualForPrimaryAccountPrimaryKey
	{
		bool operator()(const PrimaryAccount* const left, const PrimaryAccount* const right) const;
	};
	struct PrimaryAccountLessForPrimaryAccountPrimaryKey
	{
		bool operator()(const PrimaryAccount* const left, const PrimaryAccount* const right) const;
	};
	struct PrimaryAccountHashForPrimaryAccountPrimaryKey
	{
		size_t operator()(const PrimaryAccount* const record) const;
	};
	struct AccountEqualForAccountPrimaryKey
	{
		bool operator()(const Account* const left, const Account* const right) const;
	};
	struct AccountLessForAccountPrimaryKey
	{
		bool operator()(const Account* const left, const Account* const right) const;
	};
	struct AccountHashForAccountPrimaryKey
	{
		size_t operator()(const Account* const record) const;
	};
	struct CapitalEqualForCapitalPrimaryKey
	{
		bool operator()(const Capital* const left, const Capital* const right) const;
	};
	struct CapitalLessForCapitalPrimaryKey
	{
		bool operator()(const Capital* const left, const Capital* const right) const;
	};
	struct CapitalHashForCapitalPrimaryKey
	{
		size_t operator()(const Capital* const record) const;
	};
	struct PositionEqualForPositionPrimaryKey
	{
		bool operator()(const Position* const left, const Position* const right) const;
	};
	struct PositionLessForPositionPrimaryKey
	{
		bool operator()(const Position* const left, const Position* const right) const;
	};
	struct PositionHashForPositionPrimaryKey
	{
		size_t operator()(const Position* const record) const;
	};
	struct PositionDetailEqualForPositionDetailPrimaryKey
	{
		bool operator()(const PositionDetail* const left, const PositionDetail* const right) const;
	};
	struct PositionDetailLessForPositionDetailPrimaryKey
	{
		bool operator()(const PositionDetail* const left, const PositionDetail* const right) const;
	};
	struct PositionDetailHashForPositionDetailPrimaryKey
	{
		size_t operator()(const PositionDetail* const record) const;
	};
	struct OrderEqualForOrderPrimaryKey
	{
		bool operator()(const Order* const left, const Order* const right) const;
	};
	struct OrderLessForOrderPrimaryKey
	{
		bool operator()(const Order* const left, const Order* const right) const;
	};
	struct OrderHashForOrderPrimaryKey
	{
		size_t operator()(const Order* const record) const;
	};
	struct OrderEqualForClientOrderIDUniqueKey
	{
		bool operator()(const Order* const left, const Order* const right) const;
	};
	struct OrderLessForClientOrderIDUniqueKey
	{
		bool operator()(const Order* const left, const Order* const right) const;
	};
	struct OrderHashForClientOrderIDUniqueKey
	{
		size_t operator()(const Order* const record) const;
	};
	
	struct TradeEqualForTradePrimaryKey
	{
		bool operator()(const Trade* const left, const Trade* const right) const;
	};
	struct TradeLessForTradePrimaryKey
	{
		bool operator()(const Trade* const left, const Trade* const right) const;
	};
	struct TradeHashForTradePrimaryKey
	{
		size_t operator()(const Trade* const record) const;
	};
}

