// 本文件由 ../Templates/Cpp/Mdb/MdbStructs.cpp.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#include "MdbStructs.h"
#include "MdbIndexes.h"
#include <Spark/TemplateLib/TemplateLib.h>
#include <string>
#include <cstring>

using namespace Spark;
using namespace DbAdapters;
namespace Mdb
{
	thread_local char MdbDataStringBuffer[10240];

	static const FieldDescriptor TradingDayFields[] = {
		{"PK", FieldType::Int32, offsetof(TradingDay, PK), 0},
		{"CurrTradingDay", FieldType::Char, offsetof(TradingDay, CurrTradingDay), sizeof(TradingDay::CurrTradingDay)},
		{"PreTradingDay", FieldType::Char, offsetof(TradingDay, PreTradingDay), sizeof(TradingDay::PreTradingDay)},
	};
	static const int TradingDayPKIndices[] = { 0 };
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
		snprintf(MdbDataStringBuffer, sizeof(MdbDataStringBuffer), "%d,%s,%s",
			PK, CurrTradingDay, PreTradingDay);
		return MdbDataStringBuffer;
	}
	const char* TradingDay::GetDebugString() const
	{
		snprintf(MdbDataStringBuffer, sizeof(MdbDataStringBuffer), "TradingDay:PK:[%d], CurrTradingDay:[%s], PreTradingDay:[%s]",
			PK, CurrTradingDay, PreTradingDay);
		return MdbDataStringBuffer;
	}
	static void DeallocateTradingDay(void* r) { static_cast<TradingDay*>(r)->Deallocate(); }
	const TableSchema& TradingDay::GetSchema()
	{
		static const TableSchema schema = {"TradingDay", TradingDayFields, 3, TradingDayPKIndices, 1, DeallocateTradingDay, nullptr, 0};
		return schema;
	}
	static const FieldDescriptor ExchangeFields[] = {
		{"ExchangeId", FieldType::Char, offsetof(Exchange, ExchangeId), sizeof(Exchange::ExchangeId)},
		{"ExchangeName", FieldType::Char, offsetof(Exchange, ExchangeName), sizeof(Exchange::ExchangeName)},
	};
	static const int ExchangePKIndices[] = { 0 };
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
		snprintf(MdbDataStringBuffer, sizeof(MdbDataStringBuffer), "%s,%s",
			ExchangeId, ExchangeName);
		return MdbDataStringBuffer;
	}
	const char* Exchange::GetDebugString() const
	{
		snprintf(MdbDataStringBuffer, sizeof(MdbDataStringBuffer), "Exchange:ExchangeId:[%s], ExchangeName:[%s]",
			ExchangeId, ExchangeName);
		return MdbDataStringBuffer;
	}
	static void DeallocateExchange(void* r) { static_cast<Exchange*>(r)->Deallocate(); }
	const TableSchema& Exchange::GetSchema()
	{
		static const TableSchema schema = {"Exchange", ExchangeFields, 2, ExchangePKIndices, 1, DeallocateExchange, nullptr, 0};
		return schema;
	}
	static const FieldDescriptor ProductFields[] = {
		{"ExchangeId", FieldType::Char, offsetof(Product, ExchangeId), sizeof(Product::ExchangeId)},
		{"ProductId", FieldType::Char, offsetof(Product, ProductId), sizeof(Product::ProductId)},
		{"ProductName", FieldType::Char, offsetof(Product, ProductName), sizeof(Product::ProductName)},
		{"ProductClass", FieldType::Int32, offsetof(Product, ProductClass), 0},
		{"VolumeMultiple", FieldType::Int32, offsetof(Product, VolumeMultiple), 0},
		{"PriceTick", FieldType::Double, offsetof(Product, PriceTick), 0},
		{"MaxMarketOrderVolume", FieldType::Int64, offsetof(Product, MaxMarketOrderVolume), 0},
		{"MinMarketOrderVolume", FieldType::Int64, offsetof(Product, MinMarketOrderVolume), 0},
		{"MaxLimitOrderVolume", FieldType::Int64, offsetof(Product, MaxLimitOrderVolume), 0},
		{"MinLimitOrderVolume", FieldType::Int64, offsetof(Product, MinLimitOrderVolume), 0},
		{"SessionName", FieldType::Char, offsetof(Product, SessionName), sizeof(Product::SessionName)},
	};
	static const int ProductPKIndices[] = { 0, 1 };
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
		snprintf(MdbDataStringBuffer, sizeof(MdbDataStringBuffer), "%s,%s,%s,%d,%d,%f,%lld,%lld,%lld,%lld,%s",
			ExchangeId, ProductId, ProductName, static_cast<int>(ProductClass), VolumeMultiple, PriceTick, MaxMarketOrderVolume, MinMarketOrderVolume, MaxLimitOrderVolume, MinLimitOrderVolume, SessionName);
		return MdbDataStringBuffer;
	}
	const char* Product::GetDebugString() const
	{
		snprintf(MdbDataStringBuffer, sizeof(MdbDataStringBuffer), "Product:ExchangeId:[%s], ProductId:[%s], ProductName:[%s], ProductClass:[%d], VolumeMultiple:[%d], PriceTick:[%f], MaxMarketOrderVolume:[%lld], MinMarketOrderVolume:[%lld], MaxLimitOrderVolume:[%lld], MinLimitOrderVolume:[%lld], SessionName:[%s]",
			ExchangeId, ProductId, ProductName, static_cast<int>(ProductClass), VolumeMultiple, PriceTick, MaxMarketOrderVolume, MinMarketOrderVolume, MaxLimitOrderVolume, MinLimitOrderVolume, SessionName);
		return MdbDataStringBuffer;
	}
	static void DeallocateProduct(void* r) { static_cast<Product*>(r)->Deallocate(); }
	const TableSchema& Product::GetSchema()
	{
		static const TableSchema schema = {"Product", ProductFields, 11, ProductPKIndices, 2, DeallocateProduct, nullptr, 0};
		return schema;
	}
	static const FieldDescriptor InstrumentFields[] = {
		{"ExchangeId", FieldType::Char, offsetof(Instrument, ExchangeId), sizeof(Instrument::ExchangeId)},
		{"InstrumentId", FieldType::Char, offsetof(Instrument, InstrumentId), sizeof(Instrument::InstrumentId)},
		{"ExchangeInstId", FieldType::Char, offsetof(Instrument, ExchangeInstId), sizeof(Instrument::ExchangeInstId)},
		{"InstrumentName", FieldType::Char, offsetof(Instrument, InstrumentName), sizeof(Instrument::InstrumentName)},
		{"ProductId", FieldType::Char, offsetof(Instrument, ProductId), sizeof(Instrument::ProductId)},
		{"ProductClass", FieldType::Int32, offsetof(Instrument, ProductClass), 0},
		{"InstrumentClass", FieldType::Int32, offsetof(Instrument, InstrumentClass), 0},
		{"Rank", FieldType::Int32, offsetof(Instrument, Rank), 0},
		{"VolumeMultiple", FieldType::Int32, offsetof(Instrument, VolumeMultiple), 0},
		{"PriceTick", FieldType::Double, offsetof(Instrument, PriceTick), 0},
		{"MaxMarketOrderVolume", FieldType::Int64, offsetof(Instrument, MaxMarketOrderVolume), 0},
		{"MinMarketOrderVolume", FieldType::Int64, offsetof(Instrument, MinMarketOrderVolume), 0},
		{"MaxLimitOrderVolume", FieldType::Int64, offsetof(Instrument, MaxLimitOrderVolume), 0},
		{"MinLimitOrderVolume", FieldType::Int64, offsetof(Instrument, MinLimitOrderVolume), 0},
		{"SessionName", FieldType::Char, offsetof(Instrument, SessionName), sizeof(Instrument::SessionName)},
	};
	static const int InstrumentPKIndices[] = { 0, 1 };
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
		snprintf(MdbDataStringBuffer, sizeof(MdbDataStringBuffer), "%s,%s,%s,%s,%s,%d,%d,%d,%d,%f,%lld,%lld,%lld,%lld,%s",
			ExchangeId, InstrumentId, ExchangeInstId, InstrumentName, ProductId, static_cast<int>(ProductClass), static_cast<int>(InstrumentClass), Rank, VolumeMultiple, PriceTick, MaxMarketOrderVolume, MinMarketOrderVolume, MaxLimitOrderVolume, MinLimitOrderVolume, SessionName);
		return MdbDataStringBuffer;
	}
	const char* Instrument::GetDebugString() const
	{
		snprintf(MdbDataStringBuffer, sizeof(MdbDataStringBuffer), "Instrument:ExchangeId:[%s], InstrumentId:[%s], ExchangeInstId:[%s], InstrumentName:[%s], ProductId:[%s], ProductClass:[%d], InstrumentClass:[%d], Rank:[%d], VolumeMultiple:[%d], PriceTick:[%f], MaxMarketOrderVolume:[%lld], MinMarketOrderVolume:[%lld], MaxLimitOrderVolume:[%lld], MinLimitOrderVolume:[%lld], SessionName:[%s]",
			ExchangeId, InstrumentId, ExchangeInstId, InstrumentName, ProductId, static_cast<int>(ProductClass), static_cast<int>(InstrumentClass), Rank, VolumeMultiple, PriceTick, MaxMarketOrderVolume, MinMarketOrderVolume, MaxLimitOrderVolume, MinLimitOrderVolume, SessionName);
		return MdbDataStringBuffer;
	}
	static void DeallocateInstrument(void* r) { static_cast<Instrument*>(r)->Deallocate(); }
	const TableSchema& Instrument::GetSchema()
	{
		static const TableSchema schema = {"Instrument", InstrumentFields, 15, InstrumentPKIndices, 2, DeallocateInstrument, nullptr, 0};
		return schema;
	}
	static const FieldDescriptor PrimaryAccountFields[] = {
		{"PrimaryAccountId", FieldType::Char, offsetof(PrimaryAccount, PrimaryAccountId), sizeof(PrimaryAccount::PrimaryAccountId)},
		{"PrimaryAccountName", FieldType::Char, offsetof(PrimaryAccount, PrimaryAccountName), sizeof(PrimaryAccount::PrimaryAccountName)},
		{"AccountClass", FieldType::Int32, offsetof(PrimaryAccount, AccountClass), 0},
		{"BrokerPassword", FieldType::Char, offsetof(PrimaryAccount, BrokerPassword), sizeof(PrimaryAccount::BrokerPassword)},
		{"OfferId", FieldType::Int32, offsetof(PrimaryAccount, OfferId), 0},
		{"IsAllowLogin", FieldType::Bool, offsetof(PrimaryAccount, IsAllowLogin), 0},
		{"IsSimulateAccount", FieldType::Bool, offsetof(PrimaryAccount, IsSimulateAccount), 0},
		{"LoginStatus", FieldType::Int32, offsetof(PrimaryAccount, LoginStatus), 0},
		{"InitStatus", FieldType::Int32, offsetof(PrimaryAccount, InitStatus), 0},
	};
	static const int PrimaryAccountPKIndices[] = { 0 };
	static const int kPrimaryAccountIdxOfferId[] = { 4 };
	static const IndexDefinition PrimaryAccountIndices[] = {
		{PrimaryAccountIndexOfferId::IndexID, kPrimaryAccountIdxOfferId, 1},
	};
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
		snprintf(MdbDataStringBuffer, sizeof(MdbDataStringBuffer), "%s,%s,%d,%s,%d,%d,%d,%d,%d",
			PrimaryAccountId, PrimaryAccountName, static_cast<int>(AccountClass), BrokerPassword, OfferId, IsAllowLogin, IsSimulateAccount, static_cast<int>(LoginStatus), static_cast<int>(InitStatus));
		return MdbDataStringBuffer;
	}
	const char* PrimaryAccount::GetDebugString() const
	{
		snprintf(MdbDataStringBuffer, sizeof(MdbDataStringBuffer), "PrimaryAccount:PrimaryAccountId:[%s], PrimaryAccountName:[%s], AccountClass:[%d], BrokerPassword:[%s], OfferId:[%d], IsAllowLogin:[%d], IsSimulateAccount:[%d], LoginStatus:[%d], InitStatus:[%d]",
			PrimaryAccountId, PrimaryAccountName, static_cast<int>(AccountClass), BrokerPassword, OfferId, IsAllowLogin, IsSimulateAccount, static_cast<int>(LoginStatus), static_cast<int>(InitStatus));
		return MdbDataStringBuffer;
	}
	static void DeallocatePrimaryAccount(void* r) { static_cast<PrimaryAccount*>(r)->Deallocate(); }
	const TableSchema& PrimaryAccount::GetSchema()
	{
		static const TableSchema schema = {"PrimaryAccount", PrimaryAccountFields, 9, PrimaryAccountPKIndices, 1, DeallocatePrimaryAccount, PrimaryAccountIndices, 1};
		return schema;
	}
	static const FieldDescriptor AccountFields[] = {
		{"AccountId", FieldType::Char, offsetof(Account, AccountId), sizeof(Account::AccountId)},
		{"AccountName", FieldType::Char, offsetof(Account, AccountName), sizeof(Account::AccountName)},
		{"AccountType", FieldType::Int32, offsetof(Account, AccountType), 0},
		{"AccountStatus", FieldType::Int32, offsetof(Account, AccountStatus), 0},
		{"Password", FieldType::Char, offsetof(Account, Password), sizeof(Account::Password)},
		{"TradeGroupId", FieldType::Int32, offsetof(Account, TradeGroupId), 0},
		{"RiskGroupId", FieldType::Int32, offsetof(Account, RiskGroupId), 0},
		{"CommissionGroupId", FieldType::Int32, offsetof(Account, CommissionGroupId), 0},
	};
	static const int AccountPKIndices[] = { 0 };
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
		snprintf(MdbDataStringBuffer, sizeof(MdbDataStringBuffer), "%s,%s,%d,%d,%s,%d,%d,%d",
			AccountId, AccountName, static_cast<int>(AccountType), static_cast<int>(AccountStatus), Password, TradeGroupId, RiskGroupId, CommissionGroupId);
		return MdbDataStringBuffer;
	}
	const char* Account::GetDebugString() const
	{
		snprintf(MdbDataStringBuffer, sizeof(MdbDataStringBuffer), "Account:AccountId:[%s], AccountName:[%s], AccountType:[%d], AccountStatus:[%d], Password:[%s], TradeGroupId:[%d], RiskGroupId:[%d], CommissionGroupId:[%d]",
			AccountId, AccountName, static_cast<int>(AccountType), static_cast<int>(AccountStatus), Password, TradeGroupId, RiskGroupId, CommissionGroupId);
		return MdbDataStringBuffer;
	}
	static void DeallocateAccount(void* r) { static_cast<Account*>(r)->Deallocate(); }
	const TableSchema& Account::GetSchema()
	{
		static const TableSchema schema = {"Account", AccountFields, 8, AccountPKIndices, 1, DeallocateAccount, nullptr, 0};
		return schema;
	}
	static const FieldDescriptor CapitalFields[] = {
		{"TradingDay", FieldType::Char, offsetof(Capital, TradingDay), sizeof(Capital::TradingDay)},
		{"AccountId", FieldType::Char, offsetof(Capital, AccountId), sizeof(Capital::AccountId)},
		{"AccountType", FieldType::Int32, offsetof(Capital, AccountType), 0},
		{"Balance", FieldType::Double, offsetof(Capital, Balance), 0},
		{"PreBalance", FieldType::Double, offsetof(Capital, PreBalance), 0},
		{"Available", FieldType::Double, offsetof(Capital, Available), 0},
		{"MarketValue", FieldType::Double, offsetof(Capital, MarketValue), 0},
		{"CashIn", FieldType::Double, offsetof(Capital, CashIn), 0},
		{"CashOut", FieldType::Double, offsetof(Capital, CashOut), 0},
		{"Margin", FieldType::Double, offsetof(Capital, Margin), 0},
		{"Commission", FieldType::Double, offsetof(Capital, Commission), 0},
		{"FrozenCash", FieldType::Double, offsetof(Capital, FrozenCash), 0},
		{"FrozenMargin", FieldType::Double, offsetof(Capital, FrozenMargin), 0},
		{"FrozenCommission", FieldType::Double, offsetof(Capital, FrozenCommission), 0},
		{"CloseProfitByDate", FieldType::Double, offsetof(Capital, CloseProfitByDate), 0},
		{"CloseProfitByTrade", FieldType::Double, offsetof(Capital, CloseProfitByTrade), 0},
		{"PositionProfitByDate", FieldType::Double, offsetof(Capital, PositionProfitByDate), 0},
		{"PositionProfitByTrade", FieldType::Double, offsetof(Capital, PositionProfitByTrade), 0},
		{"Deposit", FieldType::Double, offsetof(Capital, Deposit), 0},
		{"Withdraw", FieldType::Double, offsetof(Capital, Withdraw), 0},
	};
	static const int CapitalPKIndices[] = { 0, 1 };
	static const int kCapitalIdxTradingDay[] = { 0 };
	static const IndexDefinition CapitalIndices[] = {
		{CapitalIndexTradingDay::IndexID, kCapitalIdxTradingDay, 1},
	};
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
		snprintf(MdbDataStringBuffer, sizeof(MdbDataStringBuffer), "%s,%s,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
			TradingDay, AccountId, static_cast<int>(AccountType), Balance, PreBalance, Available, MarketValue, CashIn, CashOut, Margin, Commission, FrozenCash, FrozenMargin, FrozenCommission, CloseProfitByDate, CloseProfitByTrade, PositionProfitByDate, PositionProfitByTrade, Deposit, Withdraw);
		return MdbDataStringBuffer;
	}
	const char* Capital::GetDebugString() const
	{
		snprintf(MdbDataStringBuffer, sizeof(MdbDataStringBuffer), "Capital:TradingDay:[%s], AccountId:[%s], AccountType:[%d], Balance:[%f], PreBalance:[%f], Available:[%f], MarketValue:[%f], CashIn:[%f], CashOut:[%f], Margin:[%f], Commission:[%f], FrozenCash:[%f], FrozenMargin:[%f], FrozenCommission:[%f], CloseProfitByDate:[%f], CloseProfitByTrade:[%f], PositionProfitByDate:[%f], PositionProfitByTrade:[%f], Deposit:[%f], Withdraw:[%f]",
			TradingDay, AccountId, static_cast<int>(AccountType), Balance, PreBalance, Available, MarketValue, CashIn, CashOut, Margin, Commission, FrozenCash, FrozenMargin, FrozenCommission, CloseProfitByDate, CloseProfitByTrade, PositionProfitByDate, PositionProfitByTrade, Deposit, Withdraw);
		return MdbDataStringBuffer;
	}
	static void DeallocateCapital(void* r) { static_cast<Capital*>(r)->Deallocate(); }
	const TableSchema& Capital::GetSchema()
	{
		static const TableSchema schema = {"Capital", CapitalFields, 20, CapitalPKIndices, 2, DeallocateCapital, CapitalIndices, 1};
		return schema;
	}
	static const FieldDescriptor PositionFields[] = {
		{"TradingDay", FieldType::Char, offsetof(Position, TradingDay), sizeof(Position::TradingDay)},
		{"AccountId", FieldType::Char, offsetof(Position, AccountId), sizeof(Position::AccountId)},
		{"AccountType", FieldType::Int32, offsetof(Position, AccountType), 0},
		{"ExchangeId", FieldType::Char, offsetof(Position, ExchangeId), sizeof(Position::ExchangeId)},
		{"InstrumentId", FieldType::Char, offsetof(Position, InstrumentId), sizeof(Position::InstrumentId)},
		{"ProductClass", FieldType::Int32, offsetof(Position, ProductClass), 0},
		{"PosiDirection", FieldType::Int32, offsetof(Position, PosiDirection), 0},
		{"TotalPosition", FieldType::Int64, offsetof(Position, TotalPosition), 0},
		{"PositionFrozen", FieldType::Int64, offsetof(Position, PositionFrozen), 0},
		{"TodayPosition", FieldType::Int64, offsetof(Position, TodayPosition), 0},
		{"MarketValue", FieldType::Double, offsetof(Position, MarketValue), 0},
		{"CashIn", FieldType::Double, offsetof(Position, CashIn), 0},
		{"CashOut", FieldType::Double, offsetof(Position, CashOut), 0},
		{"Margin", FieldType::Double, offsetof(Position, Margin), 0},
		{"Commission", FieldType::Double, offsetof(Position, Commission), 0},
		{"FrozenCash", FieldType::Double, offsetof(Position, FrozenCash), 0},
		{"FrozenMargin", FieldType::Double, offsetof(Position, FrozenMargin), 0},
		{"FrozenCommission", FieldType::Double, offsetof(Position, FrozenCommission), 0},
		{"VolumeMultiple", FieldType::Int32, offsetof(Position, VolumeMultiple), 0},
		{"CloseProfitByDate", FieldType::Double, offsetof(Position, CloseProfitByDate), 0},
		{"CloseProfitByTrade", FieldType::Double, offsetof(Position, CloseProfitByTrade), 0},
		{"PositionProfitByDate", FieldType::Double, offsetof(Position, PositionProfitByDate), 0},
		{"PositionProfitByTrade", FieldType::Double, offsetof(Position, PositionProfitByTrade), 0},
		{"SettlementPrice", FieldType::Double, offsetof(Position, SettlementPrice), 0},
		{"PreSettlementPrice", FieldType::Double, offsetof(Position, PreSettlementPrice), 0},
	};
	static const int PositionPKIndices[] = { 0, 1, 3, 4, 6 };
	static const int kPositionIdxAccount[] = { 0, 1 };
	static const int kPositionIdxTradingDay[] = { 0 };
	static const IndexDefinition PositionIndices[] = {
		{PositionIndexAccount::IndexID, kPositionIdxAccount, 2},
		{PositionIndexTradingDay::IndexID, kPositionIdxTradingDay, 1},
	};
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
		snprintf(MdbDataStringBuffer, sizeof(MdbDataStringBuffer), "%s,%s,%d,%s,%s,%d,%d,%lld,%lld,%lld,%f,%f,%f,%f,%f,%f,%f,%f,%d,%f,%f,%f,%f,%f,%f",
			TradingDay, AccountId, static_cast<int>(AccountType), ExchangeId, InstrumentId, static_cast<int>(ProductClass), static_cast<int>(PosiDirection), TotalPosition, PositionFrozen, TodayPosition, MarketValue, CashIn, CashOut, Margin, Commission, FrozenCash, FrozenMargin, FrozenCommission, VolumeMultiple, CloseProfitByDate, CloseProfitByTrade, PositionProfitByDate, PositionProfitByTrade, SettlementPrice, PreSettlementPrice);
		return MdbDataStringBuffer;
	}
	const char* Position::GetDebugString() const
	{
		snprintf(MdbDataStringBuffer, sizeof(MdbDataStringBuffer), "Position:TradingDay:[%s], AccountId:[%s], AccountType:[%d], ExchangeId:[%s], InstrumentId:[%s], ProductClass:[%d], PosiDirection:[%d], TotalPosition:[%lld], PositionFrozen:[%lld], TodayPosition:[%lld], MarketValue:[%f], CashIn:[%f], CashOut:[%f], Margin:[%f], Commission:[%f], FrozenCash:[%f], FrozenMargin:[%f], FrozenCommission:[%f], VolumeMultiple:[%d], CloseProfitByDate:[%f], CloseProfitByTrade:[%f], PositionProfitByDate:[%f], PositionProfitByTrade:[%f], SettlementPrice:[%f], PreSettlementPrice:[%f]",
			TradingDay, AccountId, static_cast<int>(AccountType), ExchangeId, InstrumentId, static_cast<int>(ProductClass), static_cast<int>(PosiDirection), TotalPosition, PositionFrozen, TodayPosition, MarketValue, CashIn, CashOut, Margin, Commission, FrozenCash, FrozenMargin, FrozenCommission, VolumeMultiple, CloseProfitByDate, CloseProfitByTrade, PositionProfitByDate, PositionProfitByTrade, SettlementPrice, PreSettlementPrice);
		return MdbDataStringBuffer;
	}
	static void DeallocatePosition(void* r) { static_cast<Position*>(r)->Deallocate(); }
	const TableSchema& Position::GetSchema()
	{
		static const TableSchema schema = {"Position", PositionFields, 25, PositionPKIndices, 5, DeallocatePosition, PositionIndices, 2};
		return schema;
	}
	static const FieldDescriptor PositionDetailFields[] = {
		{"TradingDay", FieldType::Char, offsetof(PositionDetail, TradingDay), sizeof(PositionDetail::TradingDay)},
		{"AccountId", FieldType::Char, offsetof(PositionDetail, AccountId), sizeof(PositionDetail::AccountId)},
		{"AccountType", FieldType::Int32, offsetof(PositionDetail, AccountType), 0},
		{"ExchangeId", FieldType::Char, offsetof(PositionDetail, ExchangeId), sizeof(PositionDetail::ExchangeId)},
		{"InstrumentId", FieldType::Char, offsetof(PositionDetail, InstrumentId), sizeof(PositionDetail::InstrumentId)},
		{"ProductClass", FieldType::Int32, offsetof(PositionDetail, ProductClass), 0},
		{"PosiDirection", FieldType::Int32, offsetof(PositionDetail, PosiDirection), 0},
		{"OpenDate", FieldType::Char, offsetof(PositionDetail, OpenDate), sizeof(PositionDetail::OpenDate)},
		{"TradeId", FieldType::Char, offsetof(PositionDetail, TradeId), sizeof(PositionDetail::TradeId)},
		{"Volume", FieldType::Int64, offsetof(PositionDetail, Volume), 0},
		{"OpenPrice", FieldType::Double, offsetof(PositionDetail, OpenPrice), 0},
		{"MarketValue", FieldType::Double, offsetof(PositionDetail, MarketValue), 0},
		{"CashIn", FieldType::Double, offsetof(PositionDetail, CashIn), 0},
		{"CashOut", FieldType::Double, offsetof(PositionDetail, CashOut), 0},
		{"Margin", FieldType::Double, offsetof(PositionDetail, Margin), 0},
		{"Commission", FieldType::Double, offsetof(PositionDetail, Commission), 0},
		{"VolumeMultiple", FieldType::Int32, offsetof(PositionDetail, VolumeMultiple), 0},
		{"CloseProfitByDate", FieldType::Double, offsetof(PositionDetail, CloseProfitByDate), 0},
		{"CloseProfitByTrade", FieldType::Double, offsetof(PositionDetail, CloseProfitByTrade), 0},
		{"PositionProfitByDate", FieldType::Double, offsetof(PositionDetail, PositionProfitByDate), 0},
		{"PositionProfitByTrade", FieldType::Double, offsetof(PositionDetail, PositionProfitByTrade), 0},
		{"SettlementPrice", FieldType::Double, offsetof(PositionDetail, SettlementPrice), 0},
		{"PreSettlementPrice", FieldType::Double, offsetof(PositionDetail, PreSettlementPrice), 0},
		{"CloseVolume", FieldType::Int64, offsetof(PositionDetail, CloseVolume), 0},
		{"CloseAmount", FieldType::Double, offsetof(PositionDetail, CloseAmount), 0},
	};
	static const int PositionDetailPKIndices[] = { 0, 1, 3, 4, 6, 7, 8 };
	static const int kPositionDetailIdxTradeMatch[] = { 0, 1, 3, 4, 6 };
	static const int kPositionDetailIdxTradingDay[] = { 0 };
	static const IndexDefinition PositionDetailIndices[] = {
		{PositionDetailIndexTradeMatch::IndexID, kPositionDetailIdxTradeMatch, 5},
		{PositionDetailIndexTradingDay::IndexID, kPositionDetailIdxTradingDay, 1},
	};
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
		snprintf(MdbDataStringBuffer, sizeof(MdbDataStringBuffer), "%s,%s,%d,%s,%s,%d,%d,%s,%s,%lld,%f,%f,%f,%f,%f,%f,%d,%f,%f,%f,%f,%f,%f,%lld,%f",
			TradingDay, AccountId, static_cast<int>(AccountType), ExchangeId, InstrumentId, static_cast<int>(ProductClass), static_cast<int>(PosiDirection), OpenDate, TradeId, Volume, OpenPrice, MarketValue, CashIn, CashOut, Margin, Commission, VolumeMultiple, CloseProfitByDate, CloseProfitByTrade, PositionProfitByDate, PositionProfitByTrade, SettlementPrice, PreSettlementPrice, CloseVolume, CloseAmount);
		return MdbDataStringBuffer;
	}
	const char* PositionDetail::GetDebugString() const
	{
		snprintf(MdbDataStringBuffer, sizeof(MdbDataStringBuffer), "PositionDetail:TradingDay:[%s], AccountId:[%s], AccountType:[%d], ExchangeId:[%s], InstrumentId:[%s], ProductClass:[%d], PosiDirection:[%d], OpenDate:[%s], TradeId:[%s], Volume:[%lld], OpenPrice:[%f], MarketValue:[%f], CashIn:[%f], CashOut:[%f], Margin:[%f], Commission:[%f], VolumeMultiple:[%d], CloseProfitByDate:[%f], CloseProfitByTrade:[%f], PositionProfitByDate:[%f], PositionProfitByTrade:[%f], SettlementPrice:[%f], PreSettlementPrice:[%f], CloseVolume:[%lld], CloseAmount:[%f]",
			TradingDay, AccountId, static_cast<int>(AccountType), ExchangeId, InstrumentId, static_cast<int>(ProductClass), static_cast<int>(PosiDirection), OpenDate, TradeId, Volume, OpenPrice, MarketValue, CashIn, CashOut, Margin, Commission, VolumeMultiple, CloseProfitByDate, CloseProfitByTrade, PositionProfitByDate, PositionProfitByTrade, SettlementPrice, PreSettlementPrice, CloseVolume, CloseAmount);
		return MdbDataStringBuffer;
	}
	static void DeallocatePositionDetail(void* r) { static_cast<PositionDetail*>(r)->Deallocate(); }
	const TableSchema& PositionDetail::GetSchema()
	{
		static const TableSchema schema = {"PositionDetail", PositionDetailFields, 25, PositionDetailPKIndices, 7, DeallocatePositionDetail, PositionDetailIndices, 2};
		return schema;
	}
	static const FieldDescriptor OrderFields[] = {
		{"TradingDay", FieldType::Char, offsetof(Order, TradingDay), sizeof(Order::TradingDay)},
		{"AccountId", FieldType::Char, offsetof(Order, AccountId), sizeof(Order::AccountId)},
		{"AccountType", FieldType::Int32, offsetof(Order, AccountType), 0},
		{"ExchangeId", FieldType::Char, offsetof(Order, ExchangeId), sizeof(Order::ExchangeId)},
		{"InstrumentId", FieldType::Char, offsetof(Order, InstrumentId), sizeof(Order::InstrumentId)},
		{"ProductClass", FieldType::Int32, offsetof(Order, ProductClass), 0},
		{"OrderId", FieldType::Int32, offsetof(Order, OrderId), 0},
		{"OrderSysId", FieldType::Char, offsetof(Order, OrderSysId), sizeof(Order::OrderSysId)},
		{"Direction", FieldType::Int32, offsetof(Order, Direction), 0},
		{"OffsetFlag", FieldType::Int32, offsetof(Order, OffsetFlag), 0},
		{"OrderPriceType", FieldType::Int32, offsetof(Order, OrderPriceType), 0},
		{"Price", FieldType::Double, offsetof(Order, Price), 0},
		{"Volume", FieldType::Int64, offsetof(Order, Volume), 0},
		{"VolumeTotal", FieldType::Int64, offsetof(Order, VolumeTotal), 0},
		{"VolumeTraded", FieldType::Int64, offsetof(Order, VolumeTraded), 0},
		{"VolumeMultiple", FieldType::Int32, offsetof(Order, VolumeMultiple), 0},
		{"OrderStatus", FieldType::Int32, offsetof(Order, OrderStatus), 0},
		{"OrderDate", FieldType::Char, offsetof(Order, OrderDate), sizeof(Order::OrderDate)},
		{"OrderTime", FieldType::Char, offsetof(Order, OrderTime), sizeof(Order::OrderTime)},
		{"CancelDate", FieldType::Char, offsetof(Order, CancelDate), sizeof(Order::CancelDate)},
		{"CancelTime", FieldType::Char, offsetof(Order, CancelTime), sizeof(Order::CancelTime)},
		{"SessionId", FieldType::Int64, offsetof(Order, SessionId), 0},
		{"ClientOrderId", FieldType::Int32, offsetof(Order, ClientOrderId), 0},
		{"RequestId", FieldType::Int32, offsetof(Order, RequestId), 0},
		{"OfferId", FieldType::Int32, offsetof(Order, OfferId), 0},
		{"TradeGroupId", FieldType::Int32, offsetof(Order, TradeGroupId), 0},
		{"RiskGroupId", FieldType::Int32, offsetof(Order, RiskGroupId), 0},
		{"CommissionGroupId", FieldType::Int32, offsetof(Order, CommissionGroupId), 0},
		{"FrozenCash", FieldType::Double, offsetof(Order, FrozenCash), 0},
		{"FrozenMargin", FieldType::Double, offsetof(Order, FrozenMargin), 0},
		{"FrozenCommission", FieldType::Double, offsetof(Order, FrozenCommission), 0},
		{"RebuildMark", FieldType::Bool, offsetof(Order, RebuildMark), 0},
		{"IsForceClose", FieldType::Bool, offsetof(Order, IsForceClose), 0},
	};
	static const int OrderPKIndices[] = { 0, 1, 3, 4, 6 };
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
		snprintf(MdbDataStringBuffer, sizeof(MdbDataStringBuffer), "%s,%s,%d,%s,%s,%d,%d,%s,%d,%d,%d,%f,%lld,%lld,%lld,%d,%d,%s,%s,%s,%s,%lld,%d,%d,%d,%d,%d,%d,%f,%f,%f,%d,%d",
			TradingDay, AccountId, static_cast<int>(AccountType), ExchangeId, InstrumentId, static_cast<int>(ProductClass), OrderId, OrderSysId, static_cast<int>(Direction), static_cast<int>(OffsetFlag), static_cast<int>(OrderPriceType), Price, Volume, VolumeTotal, VolumeTraded, VolumeMultiple, static_cast<int>(OrderStatus), OrderDate, OrderTime, CancelDate, CancelTime, SessionId, ClientOrderId, RequestId, OfferId, TradeGroupId, RiskGroupId, CommissionGroupId, FrozenCash, FrozenMargin, FrozenCommission, RebuildMark, IsForceClose);
		return MdbDataStringBuffer;
	}
	const char* Order::GetDebugString() const
	{
		snprintf(MdbDataStringBuffer, sizeof(MdbDataStringBuffer), "Order:TradingDay:[%s], AccountId:[%s], AccountType:[%d], ExchangeId:[%s], InstrumentId:[%s], ProductClass:[%d], OrderId:[%d], OrderSysId:[%s], Direction:[%d], OffsetFlag:[%d], OrderPriceType:[%d], Price:[%f], Volume:[%lld], VolumeTotal:[%lld], VolumeTraded:[%lld], VolumeMultiple:[%d], OrderStatus:[%d], OrderDate:[%s], OrderTime:[%s], CancelDate:[%s], CancelTime:[%s], SessionId:[%lld], ClientOrderId:[%d], RequestId:[%d], OfferId:[%d], TradeGroupId:[%d], RiskGroupId:[%d], CommissionGroupId:[%d], FrozenCash:[%f], FrozenMargin:[%f], FrozenCommission:[%f], RebuildMark:[%d], IsForceClose:[%d]",
			TradingDay, AccountId, static_cast<int>(AccountType), ExchangeId, InstrumentId, static_cast<int>(ProductClass), OrderId, OrderSysId, static_cast<int>(Direction), static_cast<int>(OffsetFlag), static_cast<int>(OrderPriceType), Price, Volume, VolumeTotal, VolumeTraded, VolumeMultiple, static_cast<int>(OrderStatus), OrderDate, OrderTime, CancelDate, CancelTime, SessionId, ClientOrderId, RequestId, OfferId, TradeGroupId, RiskGroupId, CommissionGroupId, FrozenCash, FrozenMargin, FrozenCommission, RebuildMark, IsForceClose);
		return MdbDataStringBuffer;
	}
	static void DeallocateOrder(void* r) { static_cast<Order*>(r)->Deallocate(); }
	const TableSchema& Order::GetSchema()
	{
		static const TableSchema schema = {"Order", OrderFields, 33, OrderPKIndices, 5, DeallocateOrder, nullptr, 0};
		return schema;
	}
	static const FieldDescriptor TradeFields[] = {
		{"TradingDay", FieldType::Char, offsetof(Trade, TradingDay), sizeof(Trade::TradingDay)},
		{"AccountId", FieldType::Char, offsetof(Trade, AccountId), sizeof(Trade::AccountId)},
		{"AccountType", FieldType::Int32, offsetof(Trade, AccountType), 0},
		{"ExchangeId", FieldType::Char, offsetof(Trade, ExchangeId), sizeof(Trade::ExchangeId)},
		{"InstrumentId", FieldType::Char, offsetof(Trade, InstrumentId), sizeof(Trade::InstrumentId)},
		{"ProductClass", FieldType::Int32, offsetof(Trade, ProductClass), 0},
		{"OrderId", FieldType::Int32, offsetof(Trade, OrderId), 0},
		{"OrderSysId", FieldType::Char, offsetof(Trade, OrderSysId), sizeof(Trade::OrderSysId)},
		{"TradeId", FieldType::Char, offsetof(Trade, TradeId), sizeof(Trade::TradeId)},
		{"Direction", FieldType::Int32, offsetof(Trade, Direction), 0},
		{"OffsetFlag", FieldType::Int32, offsetof(Trade, OffsetFlag), 0},
		{"Price", FieldType::Double, offsetof(Trade, Price), 0},
		{"Volume", FieldType::Int64, offsetof(Trade, Volume), 0},
		{"VolumeMultiple", FieldType::Int32, offsetof(Trade, VolumeMultiple), 0},
		{"TradeAmount", FieldType::Double, offsetof(Trade, TradeAmount), 0},
		{"Commission", FieldType::Double, offsetof(Trade, Commission), 0},
		{"TradeDate", FieldType::Char, offsetof(Trade, TradeDate), sizeof(Trade::TradeDate)},
		{"TradeTime", FieldType::Char, offsetof(Trade, TradeTime), sizeof(Trade::TradeTime)},
	};
	static const int TradePKIndices[] = { 0, 3, 8, 9 };
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
		snprintf(MdbDataStringBuffer, sizeof(MdbDataStringBuffer), "%s,%s,%d,%s,%s,%d,%d,%s,%s,%d,%d,%f,%lld,%d,%f,%f,%s,%s",
			TradingDay, AccountId, static_cast<int>(AccountType), ExchangeId, InstrumentId, static_cast<int>(ProductClass), OrderId, OrderSysId, TradeId, static_cast<int>(Direction), static_cast<int>(OffsetFlag), Price, Volume, VolumeMultiple, TradeAmount, Commission, TradeDate, TradeTime);
		return MdbDataStringBuffer;
	}
	const char* Trade::GetDebugString() const
	{
		snprintf(MdbDataStringBuffer, sizeof(MdbDataStringBuffer), "Trade:TradingDay:[%s], AccountId:[%s], AccountType:[%d], ExchangeId:[%s], InstrumentId:[%s], ProductClass:[%d], OrderId:[%d], OrderSysId:[%s], TradeId:[%s], Direction:[%d], OffsetFlag:[%d], Price:[%f], Volume:[%lld], VolumeMultiple:[%d], TradeAmount:[%f], Commission:[%f], TradeDate:[%s], TradeTime:[%s]",
			TradingDay, AccountId, static_cast<int>(AccountType), ExchangeId, InstrumentId, static_cast<int>(ProductClass), OrderId, OrderSysId, TradeId, static_cast<int>(Direction), static_cast<int>(OffsetFlag), Price, Volume, VolumeMultiple, TradeAmount, Commission, TradeDate, TradeTime);
		return MdbDataStringBuffer;
	}
	static void DeallocateTrade(void* r) { static_cast<Trade*>(r)->Deallocate(); }
	const TableSchema& Trade::GetSchema()
	{
		static const TableSchema schema = {"Trade", TradeFields, 18, TradePKIndices, 4, DeallocateTrade, nullptr, 0};
		return schema;
	}

	thread_local TradingDay CompareTradingDay;
	thread_local Exchange CompareExchange;
	thread_local Product CompareProduct;
	thread_local Instrument CompareInstrument;
	thread_local PrimaryAccount ComparePrimaryAccount;
	thread_local Account CompareAccount;
	thread_local Capital CompareCapital;
	thread_local Position ComparePosition;
	thread_local PositionDetail ComparePositionDetail;
	thread_local Order CompareOrder;
	thread_local Trade CompareTrade;

}
