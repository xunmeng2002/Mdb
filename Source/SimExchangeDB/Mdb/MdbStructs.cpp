#include "MdbStructs.h"
#include "MemCacheTemplateSingleton.h"
#include <string>
#include <cstring>

namespace mdb
{
	thread_local char t_MdbDataStringBuffer[10240];

	TradingDay* TradingDay::Allocate()
	{
		return ::Allocate<TradingDay>();
	}
	void TradingDay::Free()
	{
		::Free<TradingDay>(this);
	}
	const char* TradingDay::GetString() const
	{
		sprintf(t_MdbDataStringBuffer, "%d,%s,%s",
			PK, CurrTradingDay, PreTradingDay);
		return t_MdbDataStringBuffer;
	}
	int TradingDay::GetSqlString(char* buff) const
	{
		return sprintf(buff, "\n('%d','%s','%s'),",
			PK, CurrTradingDay, PreTradingDay);
	}
	const char* TradingDay::GetDebugString() const
	{
		sprintf(t_MdbDataStringBuffer, "TradingDay:PK:[%d], CurrTradingDay:[%s], PreTradingDay:[%s]",
			PK, CurrTradingDay, PreTradingDay);
		return t_MdbDataStringBuffer;
	}

	Exchange* Exchange::Allocate()
	{
		return ::Allocate<Exchange>();
	}
	void Exchange::Free()
	{
		::Free<Exchange>(this);
	}
	const char* Exchange::GetString() const
	{
		sprintf(t_MdbDataStringBuffer, "%s,%s",
			ExchangeID, ExchangeName);
		return t_MdbDataStringBuffer;
	}
	int Exchange::GetSqlString(char* buff) const
	{
		return sprintf(buff, "\n('%s','%s'),",
			ExchangeID, ExchangeName);
	}
	const char* Exchange::GetDebugString() const
	{
		sprintf(t_MdbDataStringBuffer, "Exchange:ExchangeID:[%s], ExchangeName:[%s]",
			ExchangeID, ExchangeName);
		return t_MdbDataStringBuffer;
	}

	Product* Product::Allocate()
	{
		return ::Allocate<Product>();
	}
	void Product::Free()
	{
		::Free<Product>(this);
	}
	const char* Product::GetString() const
	{
		sprintf(t_MdbDataStringBuffer, "%s,%s,%s,%d,%d,%f,%d,%d,%d,%d,%s",
			ExchangeID, ProductID, ProductName, (int)ProductClass, VolumeMultiple, PriceTick, MaxMarketOrderVolume, MinMarketOrderVolume, MaxLimitOrderVolume, MinLimitOrderVolume, SessionName);
		return t_MdbDataStringBuffer;
	}
	int Product::GetSqlString(char* buff) const
	{
		return sprintf(buff, "\n('%s','%s','%s','%d','%d','%f','%d','%d','%d','%d','%s'),",
			ExchangeID, ProductID, ProductName, (int)ProductClass, VolumeMultiple, PriceTick, MaxMarketOrderVolume, MinMarketOrderVolume, MaxLimitOrderVolume, MinLimitOrderVolume, SessionName);
	}
	const char* Product::GetDebugString() const
	{
		sprintf(t_MdbDataStringBuffer, "Product:ExchangeID:[%s], ProductID:[%s], ProductName:[%s], ProductClass:[%d], VolumeMultiple:[%d], PriceTick:[%f], MaxMarketOrderVolume:[%d], MinMarketOrderVolume:[%d], MaxLimitOrderVolume:[%d], MinLimitOrderVolume:[%d], SessionName:[%s]",
			ExchangeID, ProductID, ProductName, (int)ProductClass, VolumeMultiple, PriceTick, MaxMarketOrderVolume, MinMarketOrderVolume, MaxLimitOrderVolume, MinLimitOrderVolume, SessionName);
		return t_MdbDataStringBuffer;
	}

	Instrument* Instrument::Allocate()
	{
		return ::Allocate<Instrument>();
	}
	void Instrument::Free()
	{
		::Free<Instrument>(this);
	}
	const char* Instrument::GetString() const
	{
		sprintf(t_MdbDataStringBuffer, "%s,%s,%s,%s,%d,%d,%f,%d,%d,%d,%d,%s,%d,%d",
			ExchangeID, InstrumentID, InstrumentName, ProductID, (int)ProductClass, VolumeMultiple, PriceTick, MaxMarketOrderVolume, MinMarketOrderVolume, MaxLimitOrderVolume, MinLimitOrderVolume, SessionName, DeliveryYear, DeliveryMonth);
		return t_MdbDataStringBuffer;
	}
	int Instrument::GetSqlString(char* buff) const
	{
		return sprintf(buff, "\n('%s','%s','%s','%s','%d','%d','%f','%d','%d','%d','%d','%s','%d','%d'),",
			ExchangeID, InstrumentID, InstrumentName, ProductID, (int)ProductClass, VolumeMultiple, PriceTick, MaxMarketOrderVolume, MinMarketOrderVolume, MaxLimitOrderVolume, MinLimitOrderVolume, SessionName, DeliveryYear, DeliveryMonth);
	}
	const char* Instrument::GetDebugString() const
	{
		sprintf(t_MdbDataStringBuffer, "Instrument:ExchangeID:[%s], InstrumentID:[%s], InstrumentName:[%s], ProductID:[%s], ProductClass:[%d], VolumeMultiple:[%d], PriceTick:[%f], MaxMarketOrderVolume:[%d], MinMarketOrderVolume:[%d], MaxLimitOrderVolume:[%d], MinLimitOrderVolume:[%d], SessionName:[%s], DeliveryYear:[%d], DeliveryMonth:[%d]",
			ExchangeID, InstrumentID, InstrumentName, ProductID, (int)ProductClass, VolumeMultiple, PriceTick, MaxMarketOrderVolume, MinMarketOrderVolume, MaxLimitOrderVolume, MinLimitOrderVolume, SessionName, DeliveryYear, DeliveryMonth);
		return t_MdbDataStringBuffer;
	}

	Account* Account::Allocate()
	{
		return ::Allocate<Account>();
	}
	void Account::Free()
	{
		::Free<Account>(this);
	}
	const char* Account::GetString() const
	{
		sprintf(t_MdbDataStringBuffer, "%s,%s,%s,%s,%f,%f,%f,%f,%f,%f,%f,%f",
			TradingDay, AccountID, AccountName, Password, PreBalance, Balance, CloseProfitByDate, PositionProfitByDate, PositionProfitByTrade, PremiumIn, PremiumOut, MarketValue);
		return t_MdbDataStringBuffer;
	}
	int Account::GetSqlString(char* buff) const
	{
		return sprintf(buff, "\n('%s','%s','%s','%s','%f','%f','%f','%f','%f','%f','%f','%f'),",
			TradingDay, AccountID, AccountName, Password, PreBalance, Balance, CloseProfitByDate, PositionProfitByDate, PositionProfitByTrade, PremiumIn, PremiumOut, MarketValue);
	}
	const char* Account::GetDebugString() const
	{
		sprintf(t_MdbDataStringBuffer, "Account:TradingDay:[%s], AccountID:[%s], AccountName:[%s], Password:[%s], PreBalance:[%f], Balance:[%f], CloseProfitByDate:[%f], PositionProfitByDate:[%f], PositionProfitByTrade:[%f], PremiumIn:[%f], PremiumOut:[%f], MarketValue:[%f]",
			TradingDay, AccountID, AccountName, Password, PreBalance, Balance, CloseProfitByDate, PositionProfitByDate, PositionProfitByTrade, PremiumIn, PremiumOut, MarketValue);
		return t_MdbDataStringBuffer;
	}

	Position* Position::Allocate()
	{
		return ::Allocate<Position>();
	}
	void Position::Free()
	{
		::Free<Position>(this);
	}
	const char* Position::GetString() const
	{
		sprintf(t_MdbDataStringBuffer, "%s,%s,%s,%s,%d,%d,%d,%d,%d,%f,%f,%f,%f,%f,%f,%f,%d,%f,%f",
			TradingDay, AccountID, ExchangeID, InstrumentID, (int)ProductClass, (int)PosiDirection, TotalPosition, TodayPosition, FrozenPosition, CloseProfitByDate, CloseProfitByTrade, PositionProfitByDate, PositionProfitByTrade, PremiumIn, PremiumOut, MarketValue, VolumeMultiple, SettlementPrice, PreSettlementPrice);
		return t_MdbDataStringBuffer;
	}
	int Position::GetSqlString(char* buff) const
	{
		return sprintf(buff, "\n('%s','%s','%s','%s','%d','%d','%d','%d','%d','%f','%f','%f','%f','%f','%f','%f','%d','%f','%f'),",
			TradingDay, AccountID, ExchangeID, InstrumentID, (int)ProductClass, (int)PosiDirection, TotalPosition, TodayPosition, FrozenPosition, CloseProfitByDate, CloseProfitByTrade, PositionProfitByDate, PositionProfitByTrade, PremiumIn, PremiumOut, MarketValue, VolumeMultiple, SettlementPrice, PreSettlementPrice);
	}
	const char* Position::GetDebugString() const
	{
		sprintf(t_MdbDataStringBuffer, "Position:TradingDay:[%s], AccountID:[%s], ExchangeID:[%s], InstrumentID:[%s], ProductClass:[%d], PosiDirection:[%d], TotalPosition:[%d], TodayPosition:[%d], FrozenPosition:[%d], CloseProfitByDate:[%f], CloseProfitByTrade:[%f], PositionProfitByDate:[%f], PositionProfitByTrade:[%f], PremiumIn:[%f], PremiumOut:[%f], MarketValue:[%f], VolumeMultiple:[%d], SettlementPrice:[%f], PreSettlementPrice:[%f]",
			TradingDay, AccountID, ExchangeID, InstrumentID, (int)ProductClass, (int)PosiDirection, TotalPosition, TodayPosition, FrozenPosition, CloseProfitByDate, CloseProfitByTrade, PositionProfitByDate, PositionProfitByTrade, PremiumIn, PremiumOut, MarketValue, VolumeMultiple, SettlementPrice, PreSettlementPrice);
		return t_MdbDataStringBuffer;
	}

	Order* Order::Allocate()
	{
		return ::Allocate<Order>();
	}
	void Order::Free()
	{
		::Free<Order>(this);
	}
	const char* Order::GetString() const
	{
		sprintf(t_MdbDataStringBuffer, "%s,%s,%s,%s,%d,%d,%d,%d,%d,%f,%d,%d,%d,%d,%d,%s,%s,%s,%s",
			TradingDay, AccountID, ExchangeID, InstrumentID, OrderID, ClientOrderID, (int)Direction, (int)OffsetFlag, (int)OrderPriceType, Price, Volume, VolumeRemain, VolumeTraded, VolumeMultiple, (int)OrderStatus, OrderDate, OrderTime, CancelDate, CancelTime);
		return t_MdbDataStringBuffer;
	}
	int Order::GetSqlString(char* buff) const
	{
		return sprintf(buff, "\n('%s','%s','%s','%s','%d','%d','%d','%d','%d','%f','%d','%d','%d','%d','%d','%s','%s','%s','%s'),",
			TradingDay, AccountID, ExchangeID, InstrumentID, OrderID, ClientOrderID, (int)Direction, (int)OffsetFlag, (int)OrderPriceType, Price, Volume, VolumeRemain, VolumeTraded, VolumeMultiple, (int)OrderStatus, OrderDate, OrderTime, CancelDate, CancelTime);
	}
	const char* Order::GetDebugString() const
	{
		sprintf(t_MdbDataStringBuffer, "Order:TradingDay:[%s], AccountID:[%s], ExchangeID:[%s], InstrumentID:[%s], OrderID:[%d], ClientOrderID:[%d], Direction:[%d], OffsetFlag:[%d], OrderPriceType:[%d], Price:[%f], Volume:[%d], VolumeRemain:[%d], VolumeTraded:[%d], VolumeMultiple:[%d], OrderStatus:[%d], OrderDate:[%s], OrderTime:[%s], CancelDate:[%s], CancelTime:[%s]",
			TradingDay, AccountID, ExchangeID, InstrumentID, OrderID, ClientOrderID, (int)Direction, (int)OffsetFlag, (int)OrderPriceType, Price, Volume, VolumeRemain, VolumeTraded, VolumeMultiple, (int)OrderStatus, OrderDate, OrderTime, CancelDate, CancelTime);
		return t_MdbDataStringBuffer;
	}

	Trade* Trade::Allocate()
	{
		return ::Allocate<Trade>();
	}
	void Trade::Free()
	{
		::Free<Trade>(this);
	}
	const char* Trade::GetString() const
	{
		sprintf(t_MdbDataStringBuffer, "%s,%s,%s,%s,%d,%s,%d,%d,%f,%d,%f,%f,%f,%s,%s",
			TradingDay, AccountID, ExchangeID, InstrumentID, OrderID, TradeID, (int)Direction, (int)OffsetFlag, Price, Volume, TradeAmount, PremiumIn, PremiumOut, TradeDate, TradeTime);
		return t_MdbDataStringBuffer;
	}
	int Trade::GetSqlString(char* buff) const
	{
		return sprintf(buff, "\n('%s','%s','%s','%s','%d','%s','%d','%d','%f','%d','%f','%f','%f','%s','%s'),",
			TradingDay, AccountID, ExchangeID, InstrumentID, OrderID, TradeID, (int)Direction, (int)OffsetFlag, Price, Volume, TradeAmount, PremiumIn, PremiumOut, TradeDate, TradeTime);
	}
	const char* Trade::GetDebugString() const
	{
		sprintf(t_MdbDataStringBuffer, "Trade:TradingDay:[%s], AccountID:[%s], ExchangeID:[%s], InstrumentID:[%s], OrderID:[%d], TradeID:[%s], Direction:[%d], OffsetFlag:[%d], Price:[%f], Volume:[%d], TradeAmount:[%f], PremiumIn:[%f], PremiumOut:[%f], TradeDate:[%s], TradeTime:[%s]",
			TradingDay, AccountID, ExchangeID, InstrumentID, OrderID, TradeID, (int)Direction, (int)OffsetFlag, Price, Volume, TradeAmount, PremiumIn, PremiumOut, TradeDate, TradeTime);
		return t_MdbDataStringBuffer;
	}

	MdTick* MdTick::Allocate()
	{
		return ::Allocate<MdTick>();
	}
	void MdTick::Free()
	{
		::Free<MdTick>(this);
	}
	const char* MdTick::GetString() const
	{
		sprintf(t_MdbDataStringBuffer, "%s,%s,%s,%f,%f,%f,%d,%f,%f,%f,%d,%f,%d,%f,%f,%s,%d,%f,%f,%f,%f,%f,%d,%d,%d,%d,%d,%f,%f,%f,%f,%f,%d,%d,%d,%d,%d,%f",
			TradingDay, ExchangeID, InstrumentID, LastPrice, PreSettlementPrice, PreClosePrice, PreOpenInterest, OpenPrice, HighestPrice, LowestPrice, Volume, Turnover, OpenInterest, UpperLimitPrice, LowerLimitPrice, UpdateTime, UpdateMillisec, AskPrice1, AskPrice2, AskPrice3, AskPrice4, AskPrice5, AskVolume1, AskVolume2, AskVolume3, AskVolume4, AskVolume5, BidPrice1, BidPrice2, BidPrice3, BidPrice4, BidPrice5, BidVolume1, BidVolume2, BidVolume3, BidVolume4, BidVolume5, AveragePrice);
		return t_MdbDataStringBuffer;
	}
	int MdTick::GetSqlString(char* buff) const
	{
		return sprintf(buff, "\n('%s','%s','%s','%f','%f','%f','%d','%f','%f','%f','%d','%f','%d','%f','%f','%s','%d','%f','%f','%f','%f','%f','%d','%d','%d','%d','%d','%f','%f','%f','%f','%f','%d','%d','%d','%d','%d','%f'),",
			TradingDay, ExchangeID, InstrumentID, LastPrice, PreSettlementPrice, PreClosePrice, PreOpenInterest, OpenPrice, HighestPrice, LowestPrice, Volume, Turnover, OpenInterest, UpperLimitPrice, LowerLimitPrice, UpdateTime, UpdateMillisec, AskPrice1, AskPrice2, AskPrice3, AskPrice4, AskPrice5, AskVolume1, AskVolume2, AskVolume3, AskVolume4, AskVolume5, BidPrice1, BidPrice2, BidPrice3, BidPrice4, BidPrice5, BidVolume1, BidVolume2, BidVolume3, BidVolume4, BidVolume5, AveragePrice);
	}
	const char* MdTick::GetDebugString() const
	{
		sprintf(t_MdbDataStringBuffer, "MdTick:TradingDay:[%s], ExchangeID:[%s], InstrumentID:[%s], LastPrice:[%f], PreSettlementPrice:[%f], PreClosePrice:[%f], PreOpenInterest:[%d], OpenPrice:[%f], HighestPrice:[%f], LowestPrice:[%f], Volume:[%d], Turnover:[%f], OpenInterest:[%d], UpperLimitPrice:[%f], LowerLimitPrice:[%f], UpdateTime:[%s], UpdateMillisec:[%d], AskPrice1:[%f], AskPrice2:[%f], AskPrice3:[%f], AskPrice4:[%f], AskPrice5:[%f], AskVolume1:[%d], AskVolume2:[%d], AskVolume3:[%d], AskVolume4:[%d], AskVolume5:[%d], BidPrice1:[%f], BidPrice2:[%f], BidPrice3:[%f], BidPrice4:[%f], BidPrice5:[%f], BidVolume1:[%d], BidVolume2:[%d], BidVolume3:[%d], BidVolume4:[%d], BidVolume5:[%d], AveragePrice:[%f]",
			TradingDay, ExchangeID, InstrumentID, LastPrice, PreSettlementPrice, PreClosePrice, PreOpenInterest, OpenPrice, HighestPrice, LowestPrice, Volume, Turnover, OpenInterest, UpperLimitPrice, LowerLimitPrice, UpdateTime, UpdateMillisec, AskPrice1, AskPrice2, AskPrice3, AskPrice4, AskPrice5, AskVolume1, AskVolume2, AskVolume3, AskVolume4, AskVolume5, BidPrice1, BidPrice2, BidPrice3, BidPrice4, BidPrice5, BidVolume1, BidVolume2, BidVolume3, BidVolume4, BidVolume5, AveragePrice);
		return t_MdbDataStringBuffer;
	}


	thread_local TradingDay t_CompareTradingDay;
	thread_local Exchange t_CompareExchange;
	thread_local Product t_CompareProduct;
	thread_local Instrument t_CompareInstrument;
	thread_local Account t_CompareAccount;
	thread_local Position t_ComparePosition;
	thread_local Order t_CompareOrder;
	thread_local Trade t_CompareTrade;
	thread_local MdTick t_CompareMdTick;

}
