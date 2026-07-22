#include "MdbStructs.h"
#include "MdbIndexes.h"
#include <PersonalLib/TemplateLib/TemplateLib.h>
#include <string>
#include <cstring>

namespace mdb
{
	thread_local char t_MdbDataStringBuffer[10240];

	TradingDay* TradingDay::Allocate()
	{
		return ObjectPool<TradingDay>::GetInstance().Allocate();
	}
	void TradingDay::Deallocate()
	{
		ObjectPool<TradingDay>::GetInstance().Deallocate(this);
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
		return ObjectPool<Exchange>::GetInstance().Allocate();
	}
	void Exchange::Deallocate()
	{
		ObjectPool<Exchange>::GetInstance().Deallocate(this);
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
		return ObjectPool<Product>::GetInstance().Allocate();
	}
	void Product::Deallocate()
	{
		ObjectPool<Product>::GetInstance().Deallocate(this);
	}
	const char* Product::GetString() const
	{
		sprintf(t_MdbDataStringBuffer, "%s,%s,%s,%d,%d,%f,%lld,%lld,%lld,%lld,%s",
			ExchangeID, ProductID, ProductName, (int)ProductClass, VolumeMultiple, PriceTick, MaxMarketOrderVolume, MinMarketOrderVolume, MaxLimitOrderVolume, MinLimitOrderVolume, SessionName);
		return t_MdbDataStringBuffer;
	}
	int Product::GetSqlString(char* buff) const
	{
		return sprintf(buff, "\n('%s','%s','%s','%d','%d','%f','%lld','%lld','%lld','%lld','%s'),",
			ExchangeID, ProductID, ProductName, (int)ProductClass, VolumeMultiple, PriceTick, MaxMarketOrderVolume, MinMarketOrderVolume, MaxLimitOrderVolume, MinLimitOrderVolume, SessionName);
	}
	const char* Product::GetDebugString() const
	{
		sprintf(t_MdbDataStringBuffer, "Product:ExchangeID:[%s], ProductID:[%s], ProductName:[%s], ProductClass:[%d], VolumeMultiple:[%d], PriceTick:[%f], MaxMarketOrderVolume:[%lld], MinMarketOrderVolume:[%lld], MaxLimitOrderVolume:[%lld], MinLimitOrderVolume:[%lld], SessionName:[%s]",
			ExchangeID, ProductID, ProductName, (int)ProductClass, VolumeMultiple, PriceTick, MaxMarketOrderVolume, MinMarketOrderVolume, MaxLimitOrderVolume, MinLimitOrderVolume, SessionName);
		return t_MdbDataStringBuffer;
	}

	Instrument* Instrument::Allocate()
	{
		return ObjectPool<Instrument>::GetInstance().Allocate();
	}
	void Instrument::Deallocate()
	{
		ObjectPool<Instrument>::GetInstance().Deallocate(this);
	}
	const char* Instrument::GetString() const
	{
		sprintf(t_MdbDataStringBuffer, "%s,%s,%s,%s,%s,%d,%d,%d,%d,%f,%lld,%lld,%lld,%lld,%s",
			ExchangeID, InstrumentID, ExchangeInstID, InstrumentName, ProductID, (int)ProductClass, (int)InstrumentClass, Rank, VolumeMultiple, PriceTick, MaxMarketOrderVolume, MinMarketOrderVolume, MaxLimitOrderVolume, MinLimitOrderVolume, SessionName);
		return t_MdbDataStringBuffer;
	}
	int Instrument::GetSqlString(char* buff) const
	{
		return sprintf(buff, "\n('%s','%s','%s','%s','%s','%d','%d','%d','%d','%f','%lld','%lld','%lld','%lld','%s'),",
			ExchangeID, InstrumentID, ExchangeInstID, InstrumentName, ProductID, (int)ProductClass, (int)InstrumentClass, Rank, VolumeMultiple, PriceTick, MaxMarketOrderVolume, MinMarketOrderVolume, MaxLimitOrderVolume, MinLimitOrderVolume, SessionName);
	}
	const char* Instrument::GetDebugString() const
	{
		sprintf(t_MdbDataStringBuffer, "Instrument:ExchangeID:[%s], InstrumentID:[%s], ExchangeInstID:[%s], InstrumentName:[%s], ProductID:[%s], ProductClass:[%d], InstrumentClass:[%d], Rank:[%d], VolumeMultiple:[%d], PriceTick:[%f], MaxMarketOrderVolume:[%lld], MinMarketOrderVolume:[%lld], MaxLimitOrderVolume:[%lld], MinLimitOrderVolume:[%lld], SessionName:[%s]",
			ExchangeID, InstrumentID, ExchangeInstID, InstrumentName, ProductID, (int)ProductClass, (int)InstrumentClass, Rank, VolumeMultiple, PriceTick, MaxMarketOrderVolume, MinMarketOrderVolume, MaxLimitOrderVolume, MinLimitOrderVolume, SessionName);
		return t_MdbDataStringBuffer;
	}

	PrimaryAccount* PrimaryAccount::Allocate()
	{
		return ObjectPool<PrimaryAccount>::GetInstance().Allocate();
	}
	void PrimaryAccount::Deallocate()
	{
		ObjectPool<PrimaryAccount>::GetInstance().Deallocate(this);
	}
	const char* PrimaryAccount::GetString() const
	{
		sprintf(t_MdbDataStringBuffer, "%s,%s,%d,%s,%d,%d,%d,%d,%d",
			PrimaryAccountID, PrimaryAccountName, (int)AccountClass, BrokerPassword, OfferID, IsAllowLogin, IsSimulateAccount, (int)LoginStatus, (int)InitStatus);
		return t_MdbDataStringBuffer;
	}
	int PrimaryAccount::GetSqlString(char* buff) const
	{
		return sprintf(buff, "\n('%s','%s','%d','%s','%d','%d','%d','%d','%d'),",
			PrimaryAccountID, PrimaryAccountName, (int)AccountClass, BrokerPassword, OfferID, IsAllowLogin, IsSimulateAccount, (int)LoginStatus, (int)InitStatus);
	}
	const char* PrimaryAccount::GetDebugString() const
	{
		sprintf(t_MdbDataStringBuffer, "PrimaryAccount:PrimaryAccountID:[%s], PrimaryAccountName:[%s], AccountClass:[%d], BrokerPassword:[%s], OfferID:[%d], IsAllowLogin:[%d], IsSimulateAccount:[%d], LoginStatus:[%d], InitStatus:[%d]",
			PrimaryAccountID, PrimaryAccountName, (int)AccountClass, BrokerPassword, OfferID, IsAllowLogin, IsSimulateAccount, (int)LoginStatus, (int)InitStatus);
		return t_MdbDataStringBuffer;
	}

	Account* Account::Allocate()
	{
		return ObjectPool<Account>::GetInstance().Allocate();
	}
	void Account::Deallocate()
	{
		ObjectPool<Account>::GetInstance().Deallocate(this);
	}
	const char* Account::GetString() const
	{
		sprintf(t_MdbDataStringBuffer, "%s,%s,%d,%d,%s,%d,%d,%d",
			AccountID, AccountName, (int)AccountType, (int)AccountStatus, Password, TradeGroupID, RiskGroupID, CommissionGroupID);
		return t_MdbDataStringBuffer;
	}
	int Account::GetSqlString(char* buff) const
	{
		return sprintf(buff, "\n('%s','%s','%d','%d','%s','%d','%d','%d'),",
			AccountID, AccountName, (int)AccountType, (int)AccountStatus, Password, TradeGroupID, RiskGroupID, CommissionGroupID);
	}
	const char* Account::GetDebugString() const
	{
		sprintf(t_MdbDataStringBuffer, "Account:AccountID:[%s], AccountName:[%s], AccountType:[%d], AccountStatus:[%d], Password:[%s], TradeGroupID:[%d], RiskGroupID:[%d], CommissionGroupID:[%d]",
			AccountID, AccountName, (int)AccountType, (int)AccountStatus, Password, TradeGroupID, RiskGroupID, CommissionGroupID);
		return t_MdbDataStringBuffer;
	}

	Capital* Capital::Allocate()
	{
		return ObjectPool<Capital>::GetInstance().Allocate();
	}
	void Capital::Deallocate()
	{
		ObjectPool<Capital>::GetInstance().Deallocate(this);
	}
	const char* Capital::GetString() const
	{
		sprintf(t_MdbDataStringBuffer, "%s,%s,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
			TradingDay, AccountID, (int)AccountType, Balance, PreBalance, Available, MarketValue, CashIn, CashOut, Margin, Commission, FrozenCash, FrozenMargin, FrozenCommission, CloseProfitByDate, CloseProfitByTrade, PositionProfitByDate, PositionProfitByTrade, Deposit, Withdraw);
		return t_MdbDataStringBuffer;
	}
	int Capital::GetSqlString(char* buff) const
	{
		return sprintf(buff, "\n('%s','%s','%d','%f','%f','%f','%f','%f','%f','%f','%f','%f','%f','%f','%f','%f','%f','%f','%f','%f'),",
			TradingDay, AccountID, (int)AccountType, Balance, PreBalance, Available, MarketValue, CashIn, CashOut, Margin, Commission, FrozenCash, FrozenMargin, FrozenCommission, CloseProfitByDate, CloseProfitByTrade, PositionProfitByDate, PositionProfitByTrade, Deposit, Withdraw);
	}
	const char* Capital::GetDebugString() const
	{
		sprintf(t_MdbDataStringBuffer, "Capital:TradingDay:[%s], AccountID:[%s], AccountType:[%d], Balance:[%f], PreBalance:[%f], Available:[%f], MarketValue:[%f], CashIn:[%f], CashOut:[%f], Margin:[%f], Commission:[%f], FrozenCash:[%f], FrozenMargin:[%f], FrozenCommission:[%f], CloseProfitByDate:[%f], CloseProfitByTrade:[%f], PositionProfitByDate:[%f], PositionProfitByTrade:[%f], Deposit:[%f], Withdraw:[%f]",
			TradingDay, AccountID, (int)AccountType, Balance, PreBalance, Available, MarketValue, CashIn, CashOut, Margin, Commission, FrozenCash, FrozenMargin, FrozenCommission, CloseProfitByDate, CloseProfitByTrade, PositionProfitByDate, PositionProfitByTrade, Deposit, Withdraw);
		return t_MdbDataStringBuffer;
	}

	Position* Position::Allocate()
	{
		return ObjectPool<Position>::GetInstance().Allocate();
	}
	void Position::Deallocate()
	{
		ObjectPool<Position>::GetInstance().Deallocate(this);
	}
	const char* Position::GetString() const
	{
		sprintf(t_MdbDataStringBuffer, "%s,%s,%d,%s,%s,%d,%d,%lld,%lld,%lld,%f,%f,%f,%f,%f,%f,%f,%f,%d,%f,%f,%f,%f,%f,%f",
			TradingDay, AccountID, (int)AccountType, ExchangeID, InstrumentID, (int)ProductClass, (int)PosiDirection, TotalPosition, PositionFrozen, TodayPosition, MarketValue, CashIn, CashOut, Margin, Commission, FrozenCash, FrozenMargin, FrozenCommission, VolumeMultiple, CloseProfitByDate, CloseProfitByTrade, PositionProfitByDate, PositionProfitByTrade, SettlementPrice, PreSettlementPrice);
		return t_MdbDataStringBuffer;
	}
	int Position::GetSqlString(char* buff) const
	{
		return sprintf(buff, "\n('%s','%s','%d','%s','%s','%d','%d','%lld','%lld','%lld','%f','%f','%f','%f','%f','%f','%f','%f','%d','%f','%f','%f','%f','%f','%f'),",
			TradingDay, AccountID, (int)AccountType, ExchangeID, InstrumentID, (int)ProductClass, (int)PosiDirection, TotalPosition, PositionFrozen, TodayPosition, MarketValue, CashIn, CashOut, Margin, Commission, FrozenCash, FrozenMargin, FrozenCommission, VolumeMultiple, CloseProfitByDate, CloseProfitByTrade, PositionProfitByDate, PositionProfitByTrade, SettlementPrice, PreSettlementPrice);
	}
	const char* Position::GetDebugString() const
	{
		sprintf(t_MdbDataStringBuffer, "Position:TradingDay:[%s], AccountID:[%s], AccountType:[%d], ExchangeID:[%s], InstrumentID:[%s], ProductClass:[%d], PosiDirection:[%d], TotalPosition:[%lld], PositionFrozen:[%lld], TodayPosition:[%lld], MarketValue:[%f], CashIn:[%f], CashOut:[%f], Margin:[%f], Commission:[%f], FrozenCash:[%f], FrozenMargin:[%f], FrozenCommission:[%f], VolumeMultiple:[%d], CloseProfitByDate:[%f], CloseProfitByTrade:[%f], PositionProfitByDate:[%f], PositionProfitByTrade:[%f], SettlementPrice:[%f], PreSettlementPrice:[%f]",
			TradingDay, AccountID, (int)AccountType, ExchangeID, InstrumentID, (int)ProductClass, (int)PosiDirection, TotalPosition, PositionFrozen, TodayPosition, MarketValue, CashIn, CashOut, Margin, Commission, FrozenCash, FrozenMargin, FrozenCommission, VolumeMultiple, CloseProfitByDate, CloseProfitByTrade, PositionProfitByDate, PositionProfitByTrade, SettlementPrice, PreSettlementPrice);
		return t_MdbDataStringBuffer;
	}

	PositionDetail* PositionDetail::Allocate()
	{
		return ObjectPool<PositionDetail>::GetInstance().Allocate();
	}
	void PositionDetail::Deallocate()
	{
		ObjectPool<PositionDetail>::GetInstance().Deallocate(this);
	}
	const char* PositionDetail::GetString() const
	{
		sprintf(t_MdbDataStringBuffer, "%s,%s,%d,%s,%s,%d,%d,%s,%s,%lld,%f,%f,%f,%f,%f,%f,%d,%f,%f,%f,%f,%f,%f,%lld,%f",
			TradingDay, AccountID, (int)AccountType, ExchangeID, InstrumentID, (int)ProductClass, (int)PosiDirection, OpenDate, TradeID, Volume, OpenPrice, MarketValue, CashIn, CashOut, Margin, Commission, VolumeMultiple, CloseProfitByDate, CloseProfitByTrade, PositionProfitByDate, PositionProfitByTrade, SettlementPrice, PreSettlementPrice, CloseVolume, CloseAmount);
		return t_MdbDataStringBuffer;
	}
	int PositionDetail::GetSqlString(char* buff) const
	{
		return sprintf(buff, "\n('%s','%s','%d','%s','%s','%d','%d','%s','%s','%lld','%f','%f','%f','%f','%f','%f','%d','%f','%f','%f','%f','%f','%f','%lld','%f'),",
			TradingDay, AccountID, (int)AccountType, ExchangeID, InstrumentID, (int)ProductClass, (int)PosiDirection, OpenDate, TradeID, Volume, OpenPrice, MarketValue, CashIn, CashOut, Margin, Commission, VolumeMultiple, CloseProfitByDate, CloseProfitByTrade, PositionProfitByDate, PositionProfitByTrade, SettlementPrice, PreSettlementPrice, CloseVolume, CloseAmount);
	}
	const char* PositionDetail::GetDebugString() const
	{
		sprintf(t_MdbDataStringBuffer, "PositionDetail:TradingDay:[%s], AccountID:[%s], AccountType:[%d], ExchangeID:[%s], InstrumentID:[%s], ProductClass:[%d], PosiDirection:[%d], OpenDate:[%s], TradeID:[%s], Volume:[%lld], OpenPrice:[%f], MarketValue:[%f], CashIn:[%f], CashOut:[%f], Margin:[%f], Commission:[%f], VolumeMultiple:[%d], CloseProfitByDate:[%f], CloseProfitByTrade:[%f], PositionProfitByDate:[%f], PositionProfitByTrade:[%f], SettlementPrice:[%f], PreSettlementPrice:[%f], CloseVolume:[%lld], CloseAmount:[%f]",
			TradingDay, AccountID, (int)AccountType, ExchangeID, InstrumentID, (int)ProductClass, (int)PosiDirection, OpenDate, TradeID, Volume, OpenPrice, MarketValue, CashIn, CashOut, Margin, Commission, VolumeMultiple, CloseProfitByDate, CloseProfitByTrade, PositionProfitByDate, PositionProfitByTrade, SettlementPrice, PreSettlementPrice, CloseVolume, CloseAmount);
		return t_MdbDataStringBuffer;
	}

	Order* Order::Allocate()
	{
		return ObjectPool<Order>::GetInstance().Allocate();
	}
	void Order::Deallocate()
	{
		ObjectPool<Order>::GetInstance().Deallocate(this);
	}
	const char* Order::GetString() const
	{
		sprintf(t_MdbDataStringBuffer, "%s,%s,%d,%s,%s,%d,%d,%s,%d,%d,%d,%f,%lld,%lld,%lld,%d,%d,%s,%s,%s,%s,%lld,%d,%d,%d,%d,%d,%d,%f,%f,%f,%d,%d",
			TradingDay, AccountID, (int)AccountType, ExchangeID, InstrumentID, (int)ProductClass, OrderID, OrderSysID, (int)Direction, (int)OffsetFlag, (int)OrderPriceType, Price, Volume, VolumeTotal, VolumeTraded, VolumeMultiple, (int)OrderStatus, OrderDate, OrderTime, CancelDate, CancelTime, SessionID, ClientOrderID, RequestID, OfferID, TradeGroupID, RiskGroupID, CommissionGroupID, FrozenCash, FrozenMargin, FrozenCommission, RebuildMark, IsForceClose);
		return t_MdbDataStringBuffer;
	}
	int Order::GetSqlString(char* buff) const
	{
		return sprintf(buff, "\n('%s','%s','%d','%s','%s','%d','%d','%s','%d','%d','%d','%f','%lld','%lld','%lld','%d','%d','%s','%s','%s','%s','%lld','%d','%d','%d','%d','%d','%d','%f','%f','%f','%d','%d'),",
			TradingDay, AccountID, (int)AccountType, ExchangeID, InstrumentID, (int)ProductClass, OrderID, OrderSysID, (int)Direction, (int)OffsetFlag, (int)OrderPriceType, Price, Volume, VolumeTotal, VolumeTraded, VolumeMultiple, (int)OrderStatus, OrderDate, OrderTime, CancelDate, CancelTime, SessionID, ClientOrderID, RequestID, OfferID, TradeGroupID, RiskGroupID, CommissionGroupID, FrozenCash, FrozenMargin, FrozenCommission, RebuildMark, IsForceClose);
	}
	const char* Order::GetDebugString() const
	{
		sprintf(t_MdbDataStringBuffer, "Order:TradingDay:[%s], AccountID:[%s], AccountType:[%d], ExchangeID:[%s], InstrumentID:[%s], ProductClass:[%d], OrderID:[%d], OrderSysID:[%s], Direction:[%d], OffsetFlag:[%d], OrderPriceType:[%d], Price:[%f], Volume:[%lld], VolumeTotal:[%lld], VolumeTraded:[%lld], VolumeMultiple:[%d], OrderStatus:[%d], OrderDate:[%s], OrderTime:[%s], CancelDate:[%s], CancelTime:[%s], SessionID:[%lld], ClientOrderID:[%d], RequestID:[%d], OfferID:[%d], TradeGroupID:[%d], RiskGroupID:[%d], CommissionGroupID:[%d], FrozenCash:[%f], FrozenMargin:[%f], FrozenCommission:[%f], RebuildMark:[%d], IsForceClose:[%d]",
			TradingDay, AccountID, (int)AccountType, ExchangeID, InstrumentID, (int)ProductClass, OrderID, OrderSysID, (int)Direction, (int)OffsetFlag, (int)OrderPriceType, Price, Volume, VolumeTotal, VolumeTraded, VolumeMultiple, (int)OrderStatus, OrderDate, OrderTime, CancelDate, CancelTime, SessionID, ClientOrderID, RequestID, OfferID, TradeGroupID, RiskGroupID, CommissionGroupID, FrozenCash, FrozenMargin, FrozenCommission, RebuildMark, IsForceClose);
		return t_MdbDataStringBuffer;
	}

	Trade* Trade::Allocate()
	{
		return ObjectPool<Trade>::GetInstance().Allocate();
	}
	void Trade::Deallocate()
	{
		ObjectPool<Trade>::GetInstance().Deallocate(this);
	}
	const char* Trade::GetString() const
	{
		sprintf(t_MdbDataStringBuffer, "%s,%s,%d,%s,%s,%d,%d,%s,%s,%d,%d,%f,%lld,%d,%f,%f,%s,%s",
			TradingDay, AccountID, (int)AccountType, ExchangeID, InstrumentID, (int)ProductClass, OrderID, OrderSysID, TradeID, (int)Direction, (int)OffsetFlag, Price, Volume, VolumeMultiple, TradeAmount, Commission, TradeDate, TradeTime);
		return t_MdbDataStringBuffer;
	}
	int Trade::GetSqlString(char* buff) const
	{
		return sprintf(buff, "\n('%s','%s','%d','%s','%s','%d','%d','%s','%s','%d','%d','%f','%lld','%d','%f','%f','%s','%s'),",
			TradingDay, AccountID, (int)AccountType, ExchangeID, InstrumentID, (int)ProductClass, OrderID, OrderSysID, TradeID, (int)Direction, (int)OffsetFlag, Price, Volume, VolumeMultiple, TradeAmount, Commission, TradeDate, TradeTime);
	}
	const char* Trade::GetDebugString() const
	{
		sprintf(t_MdbDataStringBuffer, "Trade:TradingDay:[%s], AccountID:[%s], AccountType:[%d], ExchangeID:[%s], InstrumentID:[%s], ProductClass:[%d], OrderID:[%d], OrderSysID:[%s], TradeID:[%s], Direction:[%d], OffsetFlag:[%d], Price:[%f], Volume:[%lld], VolumeMultiple:[%d], TradeAmount:[%f], Commission:[%f], TradeDate:[%s], TradeTime:[%s]",
			TradingDay, AccountID, (int)AccountType, ExchangeID, InstrumentID, (int)ProductClass, OrderID, OrderSysID, TradeID, (int)Direction, (int)OffsetFlag, Price, Volume, VolumeMultiple, TradeAmount, Commission, TradeDate, TradeTime);
		return t_MdbDataStringBuffer;
	}


	thread_local TradingDay t_CompareTradingDay;
	thread_local Exchange t_CompareExchange;
	thread_local Product t_CompareProduct;
	thread_local Instrument t_CompareInstrument;
	thread_local PrimaryAccount t_ComparePrimaryAccount;
	thread_local Account t_CompareAccount;
	thread_local Capital t_CompareCapital;
	thread_local Position t_ComparePosition;
	thread_local PositionDetail t_ComparePositionDetail;
	thread_local Order t_CompareOrder;
	thread_local Trade t_CompareTrade;

// ====== Schema definitions ======
// name, type, struct offset, array size (for Char)
static const FieldDescriptor TradingDayFields[] = {
	{"PK",             FieldType::Int,   offsetof(TradingDay, PK), 0},
	{"CurrTradingDay", FieldType::Char,  offsetof(TradingDay, CurrTradingDay), sizeof(TradingDay::CurrTradingDay)},
	{"PreTradingDay",  FieldType::Char,  offsetof(TradingDay, PreTradingDay), sizeof(TradingDay::PreTradingDay)},
};
static const int TradingDayPKIndices[] = {0};
static void DeallocateTradingDay(void* r) { static_cast<TradingDay*>(r)->Deallocate(); }

const TableSchema& TradingDay::GetSchema()
{
	static const TableSchema schema = {"t_TradingDay", TradingDayFields, 3, TradingDayPKIndices, 1, DeallocateTradingDay, nullptr, 0};
	return schema;
}

static const FieldDescriptor ExchangeFields[] = {
	{"ExchangeID",   FieldType::Char, offsetof(Exchange, ExchangeID),   sizeof(Exchange::ExchangeID)},
	{"ExchangeName", FieldType::Char, offsetof(Exchange, ExchangeName), sizeof(Exchange::ExchangeName)},
};
static const int ExchangePKIndices[] = {0};
static void DeallocateExchange(void* r) { static_cast<Exchange*>(r)->Deallocate(); }
const TableSchema& Exchange::GetSchema()
{
	static const TableSchema schema = {"t_Exchange", ExchangeFields, 2, ExchangePKIndices, 1, DeallocateExchange, nullptr, 0};
	return schema;
}

static const FieldDescriptor ProductFields[] = {
	{"ExchangeID",           FieldType::Char,   offsetof(Product, ExchangeID),           sizeof(Product::ExchangeID)},
	{"ProductID",            FieldType::Char,   offsetof(Product, ProductID),            sizeof(Product::ProductID)},
	{"ProductName",          FieldType::Char,   offsetof(Product, ProductName),          sizeof(Product::ProductName)},
	{"ProductClass",         FieldType::Int,    offsetof(Product, ProductClass),         0},
	{"VolumeMultiple",       FieldType::Int,    offsetof(Product, VolumeMultiple),       0},
	{"PriceTick",            FieldType::Double, offsetof(Product, PriceTick),            0},
	{"MaxMarketOrderVolume", FieldType::Int64,  offsetof(Product, MaxMarketOrderVolume), 0},
	{"MinMarketOrderVolume", FieldType::Int64,  offsetof(Product, MinMarketOrderVolume), 0},
	{"MaxLimitOrderVolume",  FieldType::Int64,  offsetof(Product, MaxLimitOrderVolume),  0},
	{"MinLimitOrderVolume",  FieldType::Int64,  offsetof(Product, MinLimitOrderVolume),  0},
	{"SessionName",          FieldType::Char,   offsetof(Product, SessionName),          sizeof(Product::SessionName)},
};
static const int ProductPKIndices[] = {0, 1};
static void DeallocateProduct(void* r) { static_cast<Product*>(r)->Deallocate(); }
const TableSchema& Product::GetSchema()
{
	static const TableSchema schema = {"t_Product", ProductFields, 11, ProductPKIndices, 2, DeallocateProduct, nullptr, 0};
	return schema;
}

static const FieldDescriptor InstrumentFields[] = {
	{"ExchangeID",           FieldType::Char,   offsetof(Instrument, ExchangeID),           sizeof(Instrument::ExchangeID)},
	{"InstrumentID",         FieldType::Char,   offsetof(Instrument, InstrumentID),         sizeof(Instrument::InstrumentID)},
	{"ExchangeInstID",       FieldType::Char,   offsetof(Instrument, ExchangeInstID),       sizeof(Instrument::ExchangeInstID)},
	{"InstrumentName",       FieldType::Char,   offsetof(Instrument, InstrumentName),       sizeof(Instrument::InstrumentName)},
	{"ProductID",            FieldType::Char,   offsetof(Instrument, ProductID),            sizeof(Instrument::ProductID)},
	{"ProductClass",         FieldType::Int,    offsetof(Instrument, ProductClass),         0},
	{"InstrumentClass",      FieldType::Int,    offsetof(Instrument, InstrumentClass),      0},
	{"Rank",                 FieldType::Int,    offsetof(Instrument, Rank),                 0},
	{"VolumeMultiple",       FieldType::Int,    offsetof(Instrument, VolumeMultiple),       0},
	{"PriceTick",            FieldType::Double, offsetof(Instrument, PriceTick),            0},
	{"MaxMarketOrderVolume", FieldType::Int64,  offsetof(Instrument, MaxMarketOrderVolume), 0},
	{"MinMarketOrderVolume", FieldType::Int64,  offsetof(Instrument, MinMarketOrderVolume), 0},
	{"MaxLimitOrderVolume",  FieldType::Int64,  offsetof(Instrument, MaxLimitOrderVolume),  0},
	{"MinLimitOrderVolume",  FieldType::Int64,  offsetof(Instrument, MinLimitOrderVolume),  0},
	{"SessionName",          FieldType::Char,   offsetof(Instrument, SessionName),          sizeof(Instrument::SessionName)},
};
static const int InstrumentPKIndices[] = {0, 1};
static void DeallocateInstrument(void* r) { static_cast<Instrument*>(r)->Deallocate(); }
const TableSchema& Instrument::GetSchema()
{
	static const TableSchema schema = {"t_Instrument", InstrumentFields, 15, InstrumentPKIndices, 2, DeallocateInstrument, nullptr, 0};
	return schema;
}

static const FieldDescriptor PrimaryAccountFields[] = {
	{"PrimaryAccountID",   FieldType::Char,  offsetof(PrimaryAccount, PrimaryAccountID),   sizeof(PrimaryAccount::PrimaryAccountID)},
	{"PrimaryAccountName", FieldType::Char,  offsetof(PrimaryAccount, PrimaryAccountName), sizeof(PrimaryAccount::PrimaryAccountName)},
	{"AccountClass",       FieldType::Int,   offsetof(PrimaryAccount, AccountClass),       0},
	{"BrokerPassword",     FieldType::Char,  offsetof(PrimaryAccount, BrokerPassword),     sizeof(PrimaryAccount::BrokerPassword)},
	{"OfferID",            FieldType::Int,   offsetof(PrimaryAccount, OfferID),            0},
	{"IsAllowLogin",       FieldType::Bool,  offsetof(PrimaryAccount, IsAllowLogin),       0},
	{"IsSimulateAccount",  FieldType::Bool,  offsetof(PrimaryAccount, IsSimulateAccount),  0},
	{"LoginStatus",        FieldType::Int,   offsetof(PrimaryAccount, LoginStatus),        0},
	{"InitStatus",         FieldType::Int,   offsetof(PrimaryAccount, InitStatus),         0},
};
static const int PrimaryAccountPKIndices[] = {0};
static void DeallocatePrimaryAccount(void* r) { static_cast<PrimaryAccount*>(r)->Deallocate(); }
const TableSchema& PrimaryAccount::GetSchema()
{
	static const int kPrimaryAccountIdxOfferID[] = {4};
	static const IndexDefinition PrimaryAccountIndices[] = {
		{PrimaryAccountIndexOfferID::IndexID, kPrimaryAccountIdxOfferID, 1},
	};
	static const TableSchema schema = {"t_PrimaryAccount", PrimaryAccountFields, 9, PrimaryAccountPKIndices, 1, DeallocatePrimaryAccount, PrimaryAccountIndices, 1};
	return schema;
}

static const FieldDescriptor AccountFields[] = {
	{"AccountID",          FieldType::Char, offsetof(Account, AccountID),          sizeof(Account::AccountID)},
	{"AccountName",        FieldType::Char, offsetof(Account, AccountName),        sizeof(Account::AccountName)},
	{"AccountType",        FieldType::Int,  offsetof(Account, AccountType),        0},
	{"AccountStatus",      FieldType::Int,  offsetof(Account, AccountStatus),      0},
	{"Password",           FieldType::Char, offsetof(Account, Password),           sizeof(Account::Password)},
	{"TradeGroupID",       FieldType::Int,  offsetof(Account, TradeGroupID),       0},
	{"RiskGroupID",        FieldType::Int,  offsetof(Account, RiskGroupID),        0},
	{"CommissionGroupID",  FieldType::Int,  offsetof(Account, CommissionGroupID),  0},
};
static const int AccountPKIndices[] = {0};
static void DeallocateAccount(void* r) { static_cast<Account*>(r)->Deallocate(); }
const TableSchema& Account::GetSchema()
{
	static const TableSchema schema = {"t_Account", AccountFields, 8, AccountPKIndices, 1, DeallocateAccount, nullptr, 0};
	return schema;
}

static const FieldDescriptor CapitalFields[] = {
	{"TradingDay",             FieldType::Char,   offsetof(Capital, TradingDay),             sizeof(Capital::TradingDay)},
	{"AccountID",              FieldType::Char,   offsetof(Capital, AccountID),              sizeof(Capital::AccountID)},
	{"AccountType",            FieldType::Int,    offsetof(Capital, AccountType),            0},
	{"Balance",                FieldType::Double, offsetof(Capital, Balance),                0},
	{"PreBalance",             FieldType::Double, offsetof(Capital, PreBalance),             0},
	{"Available",              FieldType::Double, offsetof(Capital, Available),              0},
	{"MarketValue",            FieldType::Double, offsetof(Capital, MarketValue),            0},
	{"CashIn",                 FieldType::Double, offsetof(Capital, CashIn),                 0},
	{"CashOut",                FieldType::Double, offsetof(Capital, CashOut),                0},
	{"Margin",                 FieldType::Double, offsetof(Capital, Margin),                 0},
	{"Commission",             FieldType::Double, offsetof(Capital, Commission),             0},
	{"FrozenCash",             FieldType::Double, offsetof(Capital, FrozenCash),             0},
	{"FrozenMargin",           FieldType::Double, offsetof(Capital, FrozenMargin),           0},
	{"FrozenCommission",       FieldType::Double, offsetof(Capital, FrozenCommission),       0},
	{"CloseProfitByDate",      FieldType::Double, offsetof(Capital, CloseProfitByDate),      0},
	{"CloseProfitByTrade",     FieldType::Double, offsetof(Capital, CloseProfitByTrade),     0},
	{"PositionProfitByDate",   FieldType::Double, offsetof(Capital, PositionProfitByDate),   0},
	{"PositionProfitByTrade",  FieldType::Double, offsetof(Capital, PositionProfitByTrade),  0},
	{"Deposit",                FieldType::Double, offsetof(Capital, Deposit),                0},
	{"Withdraw",               FieldType::Double, offsetof(Capital, Withdraw),               0},
};
static const int CapitalPKIndices[] = {0, 1};
static void DeallocateCapital(void* r) { static_cast<Capital*>(r)->Deallocate(); }
const TableSchema& Capital::GetSchema()
{
	static const int kCapitalIdxTradingDay[] = {0};
	static const IndexDefinition CapitalIndices[] = {
		{CapitalIndexTradingDay::IndexID, kCapitalIdxTradingDay, 1},
	};
	static const TableSchema schema = {"t_Capital", CapitalFields, 20, CapitalPKIndices, 2, DeallocateCapital, CapitalIndices, 1};
	return schema;
}

static const FieldDescriptor PositionFields[] = {
	{"TradingDay",            FieldType::Char,   offsetof(Position, TradingDay),            sizeof(Position::TradingDay)},
	{"AccountID",             FieldType::Char,   offsetof(Position, AccountID),             sizeof(Position::AccountID)},
	{"AccountType",           FieldType::Int,    offsetof(Position, AccountType),           0},
	{"ExchangeID",            FieldType::Char,   offsetof(Position, ExchangeID),            sizeof(Position::ExchangeID)},
	{"InstrumentID",          FieldType::Char,   offsetof(Position, InstrumentID),          sizeof(Position::InstrumentID)},
	{"ProductClass",          FieldType::Int,    offsetof(Position, ProductClass),          0},
	{"PosiDirection",         FieldType::Int,    offsetof(Position, PosiDirection),         0},
	{"TotalPosition",         FieldType::Int64,  offsetof(Position, TotalPosition),         0},
	{"PositionFrozen",        FieldType::Int64,  offsetof(Position, PositionFrozen),        0},
	{"TodayPosition",         FieldType::Int64,  offsetof(Position, TodayPosition),         0},
	{"MarketValue",           FieldType::Double, offsetof(Position, MarketValue),           0},
	{"CashIn",                FieldType::Double, offsetof(Position, CashIn),                0},
	{"CashOut",               FieldType::Double, offsetof(Position, CashOut),               0},
	{"Margin",                FieldType::Double, offsetof(Position, Margin),                0},
	{"Commission",            FieldType::Double, offsetof(Position, Commission),            0},
	{"FrozenCash",            FieldType::Double, offsetof(Position, FrozenCash),            0},
	{"FrozenMargin",          FieldType::Double, offsetof(Position, FrozenMargin),          0},
	{"FrozenCommission",      FieldType::Double, offsetof(Position, FrozenCommission),      0},
	{"VolumeMultiple",        FieldType::Int,    offsetof(Position, VolumeMultiple),        0},
	{"CloseProfitByDate",     FieldType::Double, offsetof(Position, CloseProfitByDate),     0},
	{"CloseProfitByTrade",    FieldType::Double, offsetof(Position, CloseProfitByTrade),    0},
	{"PositionProfitByDate",  FieldType::Double, offsetof(Position, PositionProfitByDate),  0},
	{"PositionProfitByTrade", FieldType::Double, offsetof(Position, PositionProfitByTrade), 0},
	{"SettlementPrice",       FieldType::Double, offsetof(Position, SettlementPrice),       0},
	{"PreSettlementPrice",    FieldType::Double, offsetof(Position, PreSettlementPrice),    0},
};
static const int PositionPKIndices[] = {0, 1, 2, 3, 6};
static void DeallocatePosition(void* r) { static_cast<Position*>(r)->Deallocate(); }
const TableSchema& Position::GetSchema()
{
	static const int kPositionIdxAccount[] = {0, 1};
	static const int kPositionIdxTradingDay[] = {0};
	static const IndexDefinition PositionIndices[] = {
		{PositionIndexAccount::IndexID, kPositionIdxAccount, 2},
		{PositionIndexTradingDay::IndexID, kPositionIdxTradingDay, 1},
	};
	static const TableSchema schema = {"t_Position", PositionFields, 25, PositionPKIndices, 5, DeallocatePosition, PositionIndices, 2};
	return schema;
}

static const FieldDescriptor PositionDetailFields[] = {
	{"TradingDay",            FieldType::Char,   offsetof(PositionDetail, TradingDay),            sizeof(PositionDetail::TradingDay)},
	{"AccountID",             FieldType::Char,   offsetof(PositionDetail, AccountID),             sizeof(PositionDetail::AccountID)},
	{"AccountType",           FieldType::Int,    offsetof(PositionDetail, AccountType),           0},
	{"ExchangeID",            FieldType::Char,   offsetof(PositionDetail, ExchangeID),            sizeof(PositionDetail::ExchangeID)},
	{"InstrumentID",          FieldType::Char,   offsetof(PositionDetail, InstrumentID),          sizeof(PositionDetail::InstrumentID)},
	{"ProductClass",          FieldType::Int,    offsetof(PositionDetail, ProductClass),          0},
	{"PosiDirection",         FieldType::Int,    offsetof(PositionDetail, PosiDirection),         0},
	{"OpenDate",              FieldType::Char,   offsetof(PositionDetail, OpenDate),              sizeof(PositionDetail::OpenDate)},
	{"TradeID",               FieldType::Char,   offsetof(PositionDetail, TradeID),               sizeof(PositionDetail::TradeID)},
	{"Volume",                FieldType::Int64,  offsetof(PositionDetail, Volume),                0},
	{"OpenPrice",             FieldType::Double, offsetof(PositionDetail, OpenPrice),             0},
	{"MarketValue",           FieldType::Double, offsetof(PositionDetail, MarketValue),           0},
	{"CashIn",                FieldType::Double, offsetof(PositionDetail, CashIn),                0},
	{"CashOut",               FieldType::Double, offsetof(PositionDetail, CashOut),               0},
	{"Margin",                FieldType::Double, offsetof(PositionDetail, Margin),                0},
	{"Commission",            FieldType::Double, offsetof(PositionDetail, Commission),            0},
	{"VolumeMultiple",        FieldType::Int,    offsetof(PositionDetail, VolumeMultiple),        0},
	{"CloseProfitByDate",     FieldType::Double, offsetof(PositionDetail, CloseProfitByDate),     0},
	{"CloseProfitByTrade",    FieldType::Double, offsetof(PositionDetail, CloseProfitByTrade),    0},
	{"PositionProfitByDate",  FieldType::Double, offsetof(PositionDetail, PositionProfitByDate),  0},
	{"PositionProfitByTrade", FieldType::Double, offsetof(PositionDetail, PositionProfitByTrade), 0},
	{"SettlementPrice",       FieldType::Double, offsetof(PositionDetail, SettlementPrice),       0},
	{"PreSettlementPrice",    FieldType::Double, offsetof(PositionDetail, PreSettlementPrice),    0},
	{"CloseVolume",           FieldType::Int64,  offsetof(PositionDetail, CloseVolume),           0},
	{"CloseAmount",           FieldType::Double, offsetof(PositionDetail, CloseAmount),           0},
};
static const int PositionDetailPKIndices[] = {0, 1, 2, 3, 6, 7, 8};
static void DeallocatePositionDetail(void* r) { static_cast<PositionDetail*>(r)->Deallocate(); }
const TableSchema& PositionDetail::GetSchema()
{
	static const int kPositionDetailIdxTradeMatch[] = {0, 1, 2, 3, 6};
	static const int kPositionDetailIdxTradingDay[] = {0};
	static const IndexDefinition PositionDetailIndices[] = {
		{PositionDetailIndexTradeMatch::IndexID, kPositionDetailIdxTradeMatch, 5},
		{PositionDetailIndexTradingDay::IndexID, kPositionDetailIdxTradingDay, 1},
	};
	static const TableSchema schema = {"t_PositionDetail", PositionDetailFields, 25, PositionDetailPKIndices, 7, DeallocatePositionDetail, PositionDetailIndices, 2};
	return schema;
}

static const FieldDescriptor OrderFields[] = {
	{"TradingDay",            FieldType::Char,   offsetof(Order, TradingDay),            sizeof(Order::TradingDay)},
	{"AccountID",             FieldType::Char,   offsetof(Order, AccountID),             sizeof(Order::AccountID)},
	{"AccountType",           FieldType::Int,    offsetof(Order, AccountType),           0},
	{"ExchangeID",            FieldType::Char,   offsetof(Order, ExchangeID),            sizeof(Order::ExchangeID)},
	{"InstrumentID",          FieldType::Char,   offsetof(Order, InstrumentID),          sizeof(Order::InstrumentID)},
	{"ProductClass",          FieldType::Int,    offsetof(Order, ProductClass),          0},
	{"OrderID",               FieldType::Int,    offsetof(Order, OrderID),              0},
	{"OrderSysID",            FieldType::Char,   offsetof(Order, OrderSysID),            sizeof(Order::OrderSysID)},
	{"Direction",             FieldType::Int,    offsetof(Order, Direction),             0},
	{"OffsetFlag",            FieldType::Int,    offsetof(Order, OffsetFlag),            0},
	{"OrderPriceType",        FieldType::Int,    offsetof(Order, OrderPriceType),        0},
	{"Price",                 FieldType::Double, offsetof(Order, Price),                 0},
	{"Volume",                FieldType::Int64,  offsetof(Order, Volume),                0},
	{"VolumeTotal",           FieldType::Int64,  offsetof(Order, VolumeTotal),           0},
	{"VolumeTraded",          FieldType::Int64,  offsetof(Order, VolumeTraded),          0},
	{"VolumeMultiple",        FieldType::Int,    offsetof(Order, VolumeMultiple),        0},
	{"OrderStatus",           FieldType::Int,    offsetof(Order, OrderStatus),           0},
	{"OrderDate",             FieldType::Char,   offsetof(Order, OrderDate),             sizeof(Order::OrderDate)},
	{"OrderTime",             FieldType::Char,   offsetof(Order, OrderTime),             sizeof(Order::OrderTime)},
	{"CancelDate",            FieldType::Char,   offsetof(Order, CancelDate),            sizeof(Order::CancelDate)},
	{"CancelTime",            FieldType::Char,   offsetof(Order, CancelTime),            sizeof(Order::CancelTime)},
	{"SessionID",             FieldType::Int64,  offsetof(Order, SessionID),             0},
	{"ClientOrderID",         FieldType::Int,    offsetof(Order, ClientOrderID),         0},
	{"RequestID",             FieldType::Int,    offsetof(Order, RequestID),             0},
	{"OfferID",               FieldType::Int,    offsetof(Order, OfferID),               0},
	{"TradeGroupID",          FieldType::Int,    offsetof(Order, TradeGroupID),          0},
	{"RiskGroupID",           FieldType::Int,    offsetof(Order, RiskGroupID),           0},
	{"CommissionGroupID",     FieldType::Int,    offsetof(Order, CommissionGroupID),     0},
	{"FrozenCash",            FieldType::Double, offsetof(Order, FrozenCash),            0},
	{"FrozenMargin",          FieldType::Double, offsetof(Order, FrozenMargin),          0},
	{"FrozenCommission",      FieldType::Double, offsetof(Order, FrozenCommission),      0},
	{"RebuildMark",           FieldType::Bool,   offsetof(Order, RebuildMark),           0},
	{"IsForceClose",          FieldType::Bool,   offsetof(Order, IsForceClose),          0},
};
static const int OrderPKIndices[] = {0, 1, 2, 3, 6};
static void DeallocateOrder(void* r) { static_cast<Order*>(r)->Deallocate(); }
const TableSchema& Order::GetSchema()
{
	static const TableSchema schema = {"t_Order", OrderFields, 33, OrderPKIndices, 5, DeallocateOrder, nullptr, 0};
	return schema;
}

static const FieldDescriptor TradeFields[] = {
	{"TradingDay",      FieldType::Char,   offsetof(Trade, TradingDay),      sizeof(Trade::TradingDay)},
	{"AccountID",       FieldType::Char,   offsetof(Trade, AccountID),       sizeof(Trade::AccountID)},
	{"AccountType",     FieldType::Int,    offsetof(Trade, AccountType),     0},
	{"ExchangeID",      FieldType::Char,   offsetof(Trade, ExchangeID),      sizeof(Trade::ExchangeID)},
	{"InstrumentID",    FieldType::Char,   offsetof(Trade, InstrumentID),    sizeof(Trade::InstrumentID)},
	{"ProductClass",    FieldType::Int,    offsetof(Trade, ProductClass),    0},
	{"OrderID",         FieldType::Int,    offsetof(Trade, OrderID),         0},
	{"OrderSysID",      FieldType::Char,   offsetof(Trade, OrderSysID),      sizeof(Trade::OrderSysID)},
	{"TradeID",         FieldType::Char,   offsetof(Trade, TradeID),         sizeof(Trade::TradeID)},
	{"Direction",       FieldType::Int,    offsetof(Trade, Direction),       0},
	{"OffsetFlag",      FieldType::Int,    offsetof(Trade, OffsetFlag),      0},
	{"Price",           FieldType::Double, offsetof(Trade, Price),           0},
	{"Volume",          FieldType::Int64,  offsetof(Trade, Volume),          0},
	{"VolumeMultiple",  FieldType::Int,    offsetof(Trade, VolumeMultiple),  0},
	{"TradeAmount",     FieldType::Double, offsetof(Trade, TradeAmount),     0},
	{"Commission",      FieldType::Double, offsetof(Trade, Commission),      0},
	{"TradeDate",       FieldType::Char,   offsetof(Trade, TradeDate),       sizeof(Trade::TradeDate)},
	{"TradeTime",       FieldType::Char,   offsetof(Trade, TradeTime),       sizeof(Trade::TradeTime)},
};
static const int TradePKIndices[] = {0, 3, 8, 9};
static void DeallocateTrade(void* r) { static_cast<Trade*>(r)->Deallocate(); }
const TableSchema& Trade::GetSchema()
{
	static const TableSchema schema = {"t_Trade", TradeFields, 18, TradePKIndices, 4, DeallocateTrade, nullptr, 0};
	return schema;
}

}
