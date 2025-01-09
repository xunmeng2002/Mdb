#include "MdbPrimaryKeyComp.h"
#include <string.h>
#include <string>
#include <functional>

using std::string;


namespace mdb
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
		return std::hash<int>()(record->PK);
	}

	bool ExchangeEqualForExchangePrimaryKey::operator()(const Exchange* const left, const Exchange* const right) const
	{
		return strcmp(left->ExchangeID, right->ExchangeID) == 0;
	}
	bool ExchangeLessForExchangePrimaryKey::operator()(const Exchange* const left, const Exchange* const right) const
	{
		if (strcmp(left->ExchangeID, right->ExchangeID) < 0)
			return true;
		else if (strcmp(left->ExchangeID, right->ExchangeID) > 0)
			return false;
		return false;
	}
	size_t ExchangeHashForExchangePrimaryKey::operator()(const Exchange* const record) const
	{
		return std::hash<string>()(record->ExchangeID);
	}

	bool ProductEqualForProductPrimaryKey::operator()(const Product* const left, const Product* const right) const
	{
		return strcmp(left->ExchangeID, right->ExchangeID) == 0 && strcmp(left->ProductID, right->ProductID) == 0;
	}
	bool ProductLessForProductPrimaryKey::operator()(const Product* const left, const Product* const right) const
	{
		if (strcmp(left->ExchangeID, right->ExchangeID) < 0)
			return true;
		else if (strcmp(left->ExchangeID, right->ExchangeID) > 0)
			return false;
		if (strcmp(left->ProductID, right->ProductID) < 0)
			return true;
		else if (strcmp(left->ProductID, right->ProductID) > 0)
			return false;
		return false;
	}
	size_t ProductHashForProductPrimaryKey::operator()(const Product* const record) const
	{
		return std::hash<string>()(record->ExchangeID) + std::hash<string>()(record->ProductID);
	}

	bool InstrumentEqualForInstrumentPrimaryKey::operator()(const Instrument* const left, const Instrument* const right) const
	{
		return strcmp(left->ExchangeID, right->ExchangeID) == 0 && strcmp(left->InstrumentID, right->InstrumentID) == 0;
	}
	bool InstrumentLessForInstrumentPrimaryKey::operator()(const Instrument* const left, const Instrument* const right) const
	{
		if (strcmp(left->ExchangeID, right->ExchangeID) < 0)
			return true;
		else if (strcmp(left->ExchangeID, right->ExchangeID) > 0)
			return false;
		if (strcmp(left->InstrumentID, right->InstrumentID) < 0)
			return true;
		else if (strcmp(left->InstrumentID, right->InstrumentID) > 0)
			return false;
		return false;
	}
	size_t InstrumentHashForInstrumentPrimaryKey::operator()(const Instrument* const record) const
	{
		return std::hash<string>()(record->ExchangeID) + std::hash<string>()(record->InstrumentID);
	}

	bool AccountEqualForAccountPrimaryKey::operator()(const Account* const left, const Account* const right) const
	{
		return strcmp(left->AccountID, right->AccountID) == 0;
	}
	bool AccountLessForAccountPrimaryKey::operator()(const Account* const left, const Account* const right) const
	{
		if (strcmp(left->AccountID, right->AccountID) < 0)
			return true;
		else if (strcmp(left->AccountID, right->AccountID) > 0)
			return false;
		return false;
	}
	size_t AccountHashForAccountPrimaryKey::operator()(const Account* const record) const
	{
		return std::hash<string>()(record->AccountID);
	}

	bool PositionEqualForPositionPrimaryKey::operator()(const Position* const left, const Position* const right) const
	{
		return strcmp(left->TradingDay, right->TradingDay) == 0 && strcmp(left->AccountID, right->AccountID) == 0 && strcmp(left->ExchangeID, right->ExchangeID) == 0 && strcmp(left->InstrumentID, right->InstrumentID) == 0 && left->PosiDirection == right->PosiDirection;
	}
	bool PositionLessForPositionPrimaryKey::operator()(const Position* const left, const Position* const right) const
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
	size_t PositionHashForPositionPrimaryKey::operator()(const Position* const record) const
	{
		return std::hash<string>()(record->TradingDay) + std::hash<string>()(record->AccountID) + std::hash<string>()(record->ExchangeID) + std::hash<string>()(record->InstrumentID) + std::hash<char>()((char)record->PosiDirection);
	}

	bool OrderEqualForOrderPrimaryKey::operator()(const Order* const left, const Order* const right) const
	{
		return strcmp(left->TradingDay, right->TradingDay) == 0 && strcmp(left->AccountID, right->AccountID) == 0 && strcmp(left->ExchangeID, right->ExchangeID) == 0 && strcmp(left->InstrumentID, right->InstrumentID) == 0 && left->OrderID == right->OrderID;
	}
	bool OrderLessForOrderPrimaryKey::operator()(const Order* const left, const Order* const right) const
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
		if (left->OrderID < right->OrderID)
			return true;
		else if (left->OrderID > right->OrderID)
			return false;
		return false;
	}
	size_t OrderHashForOrderPrimaryKey::operator()(const Order* const record) const
	{
		return std::hash<string>()(record->TradingDay) + std::hash<string>()(record->AccountID) + std::hash<string>()(record->ExchangeID) + std::hash<string>()(record->InstrumentID) + std::hash<int>()(record->OrderID);
	}

	bool OrderEqualForClientOrderIDUniqueKey::operator()(const Order* const left, const Order* const right) const
	{
		return strcmp(left->TradingDay, right->TradingDay) == 0 && strcmp(left->AccountID, right->AccountID) == 0 && strcmp(left->ExchangeID, right->ExchangeID) == 0 && strcmp(left->InstrumentID, right->InstrumentID) == 0 && left->ClientOrderID == right->ClientOrderID;
	}
	bool OrderLessForClientOrderIDUniqueKey::operator()(const Order* const left, const Order* const right) const
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
		if (left->ClientOrderID < right->ClientOrderID)
			return true;
		else if (left->ClientOrderID > right->ClientOrderID)
			return false;
		return false;
	}
	size_t OrderHashForClientOrderIDUniqueKey::operator()(const Order* const record) const
	{
		return std::hash<string>()(record->TradingDay) + std::hash<string>()(record->AccountID) + std::hash<string>()(record->ExchangeID) + std::hash<string>()(record->InstrumentID) + std::hash<int>()(record->ClientOrderID);
	}

	bool TradeEqualForTradePrimaryKey::operator()(const Trade* const left, const Trade* const right) const
	{
		return strcmp(left->TradingDay, right->TradingDay) == 0 && strcmp(left->ExchangeID, right->ExchangeID) == 0 && strcmp(left->TradeID, right->TradeID) == 0 && left->Direction == right->Direction;
	}
	bool TradeLessForTradePrimaryKey::operator()(const Trade* const left, const Trade* const right) const
	{
		if (strcmp(left->TradingDay, right->TradingDay) < 0)
			return true;
		else if (strcmp(left->TradingDay, right->TradingDay) > 0)
			return false;
		if (strcmp(left->ExchangeID, right->ExchangeID) < 0)
			return true;
		else if (strcmp(left->ExchangeID, right->ExchangeID) > 0)
			return false;
		if (strcmp(left->TradeID, right->TradeID) < 0)
			return true;
		else if (strcmp(left->TradeID, right->TradeID) > 0)
			return false;
		if (left->Direction < right->Direction)
			return true;
		else if (left->Direction > right->Direction)
			return false;
		return false;
	}
	size_t TradeHashForTradePrimaryKey::operator()(const Trade* const record) const
	{
		return std::hash<string>()(record->TradingDay) + std::hash<string>()(record->ExchangeID) + std::hash<string>()(record->TradeID) + std::hash<char>()((char)record->Direction);
	}

	bool MdTickEqualForMdTickPrimaryKey::operator()(const MdTick* const left, const MdTick* const right) const
	{
		return strcmp(left->TradingDay, right->TradingDay) == 0 && strcmp(left->ExchangeID, right->ExchangeID) == 0 && strcmp(left->InstrumentID, right->InstrumentID) == 0;
	}
	bool MdTickLessForMdTickPrimaryKey::operator()(const MdTick* const left, const MdTick* const right) const
	{
		if (strcmp(left->TradingDay, right->TradingDay) < 0)
			return true;
		else if (strcmp(left->TradingDay, right->TradingDay) > 0)
			return false;
		if (strcmp(left->ExchangeID, right->ExchangeID) < 0)
			return true;
		else if (strcmp(left->ExchangeID, right->ExchangeID) > 0)
			return false;
		if (strcmp(left->InstrumentID, right->InstrumentID) < 0)
			return true;
		else if (strcmp(left->InstrumentID, right->InstrumentID) > 0)
			return false;
		return false;
	}
	size_t MdTickHashForMdTickPrimaryKey::operator()(const MdTick* const record) const
	{
		return std::hash<string>()(record->TradingDay) + std::hash<string>()(record->ExchangeID) + std::hash<string>()(record->InstrumentID);
	}

}
