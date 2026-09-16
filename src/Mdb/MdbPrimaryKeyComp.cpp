// 本文件由 ../Templates/Cpp/Mdb/MdbPrimaryKeyComp.cpp.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#include "MdbPrimaryKeyComp.h"
#include <string.h>
#include <string>
#include <functional>

using std::string;


namespace Mdb
{
	bool TradingDayEqualForTradingDayPrimaryKey::operator()(const TradingDay* const left, const TradingDay* const right) const
	{
		return left->PK == right->PK;
	}
	bool TradingDayLessForTradingDayPrimaryKey::operator()(const TradingDay* const left, const TradingDay* const right) const
	{
		if (left->PK < right->PK)
			return true;
		else if (left->PK > right->PK)
			return false;
		return false;
	}
	size_t TradingDayHashForTradingDayPrimaryKey::operator()(const TradingDay* const record) const
	{
		return std::hash<Int32Type>()(record->PK);
	}
	bool ExchangeEqualForExchangePrimaryKey::operator()(const Exchange* const left, const Exchange* const right) const
	{
		return strcmp(left->ExchangeId, right->ExchangeId) == 0;
	}
	bool ExchangeLessForExchangePrimaryKey::operator()(const Exchange* const left, const Exchange* const right) const
	{
		if (strcmp(left->ExchangeId, right->ExchangeId) < 0)
			return true;
		else if (strcmp(left->ExchangeId, right->ExchangeId) > 0)
			return false;
		return false;
	}
	size_t ExchangeHashForExchangePrimaryKey::operator()(const Exchange* const record) const
	{
		return std::hash<string>()(record->ExchangeId);
	}
	bool ProductEqualForProductPrimaryKey::operator()(const Product* const left, const Product* const right) const
	{
		return strcmp(left->ExchangeId, right->ExchangeId) == 0 && strcmp(left->ProductId, right->ProductId) == 0;
	}
	bool ProductLessForProductPrimaryKey::operator()(const Product* const left, const Product* const right) const
	{
		if (strcmp(left->ExchangeId, right->ExchangeId) < 0)
			return true;
		else if (strcmp(left->ExchangeId, right->ExchangeId) > 0)
			return false;
		if (strcmp(left->ProductId, right->ProductId) < 0)
			return true;
		else if (strcmp(left->ProductId, right->ProductId) > 0)
			return false;
		return false;
	}
	size_t ProductHashForProductPrimaryKey::operator()(const Product* const record) const
	{
		return std::hash<string>()(record->ExchangeId) + std::hash<string>()(record->ProductId);
	}
	bool InstrumentEqualForInstrumentPrimaryKey::operator()(const Instrument* const left, const Instrument* const right) const
	{
		return strcmp(left->ExchangeId, right->ExchangeId) == 0 && strcmp(left->InstrumentId, right->InstrumentId) == 0;
	}
	bool InstrumentLessForInstrumentPrimaryKey::operator()(const Instrument* const left, const Instrument* const right) const
	{
		if (strcmp(left->ExchangeId, right->ExchangeId) < 0)
			return true;
		else if (strcmp(left->ExchangeId, right->ExchangeId) > 0)
			return false;
		if (strcmp(left->InstrumentId, right->InstrumentId) < 0)
			return true;
		else if (strcmp(left->InstrumentId, right->InstrumentId) > 0)
			return false;
		return false;
	}
	size_t InstrumentHashForInstrumentPrimaryKey::operator()(const Instrument* const record) const
	{
		return std::hash<string>()(record->ExchangeId) + std::hash<string>()(record->InstrumentId);
	}
	bool PrimaryAccountEqualForPrimaryAccountPrimaryKey::operator()(const PrimaryAccount* const left, const PrimaryAccount* const right) const
	{
		return strcmp(left->PrimaryAccountId, right->PrimaryAccountId) == 0;
	}
	bool PrimaryAccountLessForPrimaryAccountPrimaryKey::operator()(const PrimaryAccount* const left, const PrimaryAccount* const right) const
	{
		if (strcmp(left->PrimaryAccountId, right->PrimaryAccountId) < 0)
			return true;
		else if (strcmp(left->PrimaryAccountId, right->PrimaryAccountId) > 0)
			return false;
		return false;
	}
	size_t PrimaryAccountHashForPrimaryAccountPrimaryKey::operator()(const PrimaryAccount* const record) const
	{
		return std::hash<string>()(record->PrimaryAccountId);
	}
	bool AccountEqualForAccountPrimaryKey::operator()(const Account* const left, const Account* const right) const
	{
		return strcmp(left->AccountId, right->AccountId) == 0;
	}
	bool AccountLessForAccountPrimaryKey::operator()(const Account* const left, const Account* const right) const
	{
		if (strcmp(left->AccountId, right->AccountId) < 0)
			return true;
		else if (strcmp(left->AccountId, right->AccountId) > 0)
			return false;
		return false;
	}
	size_t AccountHashForAccountPrimaryKey::operator()(const Account* const record) const
	{
		return std::hash<string>()(record->AccountId);
	}
	bool CapitalEqualForCapitalPrimaryKey::operator()(const Capital* const left, const Capital* const right) const
	{
		return strcmp(left->TradingDay, right->TradingDay) == 0 && strcmp(left->AccountId, right->AccountId) == 0;
	}
	bool CapitalLessForCapitalPrimaryKey::operator()(const Capital* const left, const Capital* const right) const
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
	size_t CapitalHashForCapitalPrimaryKey::operator()(const Capital* const record) const
	{
		return std::hash<string>()(record->TradingDay) + std::hash<string>()(record->AccountId);
	}
	bool PositionEqualForPositionPrimaryKey::operator()(const Position* const left, const Position* const right) const
	{
		return strcmp(left->TradingDay, right->TradingDay) == 0 && strcmp(left->AccountId, right->AccountId) == 0 && strcmp(left->ExchangeId, right->ExchangeId) == 0 && strcmp(left->InstrumentId, right->InstrumentId) == 0 && left->PosiDirection == right->PosiDirection;
	}
	bool PositionLessForPositionPrimaryKey::operator()(const Position* const left, const Position* const right) const
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
	size_t PositionHashForPositionPrimaryKey::operator()(const Position* const record) const
	{
		return std::hash<string>()(record->TradingDay) + std::hash<string>()(record->AccountId) + std::hash<string>()(record->ExchangeId) + std::hash<string>()(record->InstrumentId) + std::hash<PosiDirectionType>()(record->PosiDirection);
	}
	bool PositionDetailEqualForPositionDetailPrimaryKey::operator()(const PositionDetail* const left, const PositionDetail* const right) const
	{
		return strcmp(left->TradingDay, right->TradingDay) == 0 && strcmp(left->AccountId, right->AccountId) == 0 && strcmp(left->ExchangeId, right->ExchangeId) == 0 && strcmp(left->InstrumentId, right->InstrumentId) == 0 && left->PosiDirection == right->PosiDirection && strcmp(left->OpenDate, right->OpenDate) == 0 && strcmp(left->TradeId, right->TradeId) == 0;
	}
	bool PositionDetailLessForPositionDetailPrimaryKey::operator()(const PositionDetail* const left, const PositionDetail* const right) const
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
		if (strcmp(left->OpenDate, right->OpenDate) < 0)
			return true;
		else if (strcmp(left->OpenDate, right->OpenDate) > 0)
			return false;
		if (strcmp(left->TradeId, right->TradeId) < 0)
			return true;
		else if (strcmp(left->TradeId, right->TradeId) > 0)
			return false;
		return false;
	}
	size_t PositionDetailHashForPositionDetailPrimaryKey::operator()(const PositionDetail* const record) const
	{
		return std::hash<string>()(record->TradingDay) + std::hash<string>()(record->AccountId) + std::hash<string>()(record->ExchangeId) + std::hash<string>()(record->InstrumentId) + std::hash<PosiDirectionType>()(record->PosiDirection) + std::hash<string>()(record->OpenDate) + std::hash<string>()(record->TradeId);
	}
	bool OrderEqualForOrderPrimaryKey::operator()(const Order* const left, const Order* const right) const
	{
		return strcmp(left->TradingDay, right->TradingDay) == 0 && strcmp(left->AccountId, right->AccountId) == 0 && strcmp(left->ExchangeId, right->ExchangeId) == 0 && strcmp(left->InstrumentId, right->InstrumentId) == 0 && left->OrderId == right->OrderId;
	}
	bool OrderLessForOrderPrimaryKey::operator()(const Order* const left, const Order* const right) const
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
		if (left->OrderId < right->OrderId)
			return true;
		else if (left->OrderId > right->OrderId)
			return false;
		return false;
	}
	size_t OrderHashForOrderPrimaryKey::operator()(const Order* const record) const
	{
		return std::hash<string>()(record->TradingDay) + std::hash<string>()(record->AccountId) + std::hash<string>()(record->ExchangeId) + std::hash<string>()(record->InstrumentId) + std::hash<OrderIdType>()(record->OrderId);
	}
	bool OrderEqualForClientOrderIdUniqueKey::operator()(const Order* const left, const Order* const right) const
	{
		return strcmp(left->TradingDay, right->TradingDay) == 0 && strcmp(left->AccountId, right->AccountId) == 0 && strcmp(left->ExchangeId, right->ExchangeId) == 0 && strcmp(left->InstrumentId, right->InstrumentId) == 0 && left->SessionId == right->SessionId && left->ClientOrderId == right->ClientOrderId;
	}
	bool OrderLessForClientOrderIdUniqueKey::operator()(const Order* const left, const Order* const right) const
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
		if (left->SessionId < right->SessionId)
			return true;
		else if (left->SessionId > right->SessionId)
			return false;
		if (left->ClientOrderId < right->ClientOrderId)
			return true;
		else if (left->ClientOrderId > right->ClientOrderId)
			return false;
		return false;
	}
	size_t OrderHashForClientOrderIdUniqueKey::operator()(const Order* const record) const
	{
		return std::hash<string>()(record->TradingDay) + std::hash<string>()(record->AccountId) + std::hash<string>()(record->ExchangeId) + std::hash<string>()(record->InstrumentId) + std::hash<SessionIdType>()(record->SessionId) + std::hash<ClientOrderIdType>()(record->ClientOrderId);
	}
	
	bool TradeEqualForTradePrimaryKey::operator()(const Trade* const left, const Trade* const right) const
	{
		return strcmp(left->TradingDay, right->TradingDay) == 0 && strcmp(left->ExchangeId, right->ExchangeId) == 0 && strcmp(left->TradeId, right->TradeId) == 0 && left->Direction == right->Direction;
	}
	bool TradeLessForTradePrimaryKey::operator()(const Trade* const left, const Trade* const right) const
	{
		if (strcmp(left->TradingDay, right->TradingDay) < 0)
			return true;
		else if (strcmp(left->TradingDay, right->TradingDay) > 0)
			return false;
		if (strcmp(left->ExchangeId, right->ExchangeId) < 0)
			return true;
		else if (strcmp(left->ExchangeId, right->ExchangeId) > 0)
			return false;
		if (strcmp(left->TradeId, right->TradeId) < 0)
			return true;
		else if (strcmp(left->TradeId, right->TradeId) > 0)
			return false;
		if (left->Direction < right->Direction)
			return true;
		else if (left->Direction > right->Direction)
			return false;
		return false;
	}
	size_t TradeHashForTradePrimaryKey::operator()(const Trade* const record) const
	{
		return std::hash<string>()(record->TradingDay) + std::hash<string>()(record->ExchangeId) + std::hash<string>()(record->TradeId) + std::hash<DirectionType>()(record->Direction);
	}
}
