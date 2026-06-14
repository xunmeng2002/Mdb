#include <Mdb/SqliteWrapper/SqliteWrapper.h>
#include <PersonalLib/TemplateLib/TemplateLib.h>
#include <PersonalLib/Core/Core.h>
#include <sqlite3.h>
#include <string.h>
#include <cstring>
#include <chrono>

using namespace mdb;
using namespace std;
using namespace std::chrono;

static void SetStatementForTradingDayRecord(sqlite3_stmt* statement, TradingDay* record)
{
	sqlite3_bind_int(statement, 1, record->PK);
	sqlite3_bind_text(statement, 2, record->CurrTradingDay, sizeof(record->CurrTradingDay), nullptr);
	sqlite3_bind_text(statement, 3, record->PreTradingDay, sizeof(record->PreTradingDay), nullptr);
}
static void SetStatementForTradingDayRecordUpdate(sqlite3_stmt* statement, TradingDay* record)
{
	sqlite3_bind_text(statement, 1, record->CurrTradingDay, sizeof(record->CurrTradingDay), nullptr);
	sqlite3_bind_text(statement, 2, record->PreTradingDay, sizeof(record->PreTradingDay), nullptr);
	sqlite3_bind_int(statement, 3, record->PK);
}
static void SetStatementForTradingDayPrimaryKey(sqlite3_stmt* statement, const IntType& PK)
{
	sqlite3_bind_int(statement, 1, PK);
}
static void ParseRecord(sqlite3_stmt* statement, std::list<TradingDay*>& records)
{
	TradingDay* record = TradingDay::Allocate();
	record->PK = sqlite3_column_int(statement, 0);
	Utility::Strcpy(record->CurrTradingDay, (const char*)sqlite3_column_text(statement, 1));
	Utility::Strcpy(record->PreTradingDay, (const char*)sqlite3_column_text(statement, 2));
	records.push_back(record);
}
static void SetStatementForExchangeRecord(sqlite3_stmt* statement, Exchange* record)
{
	sqlite3_bind_text(statement, 1, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 2, record->ExchangeName, sizeof(record->ExchangeName), nullptr);
}
static void SetStatementForExchangeRecordUpdate(sqlite3_stmt* statement, Exchange* record)
{
	sqlite3_bind_text(statement, 1, record->ExchangeName, sizeof(record->ExchangeName), nullptr);
	sqlite3_bind_text(statement, 2, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
}
static void SetStatementForExchangePrimaryKey(sqlite3_stmt* statement, const ExchangeIDType& ExchangeID)
{
	sqlite3_bind_text(statement, 1, ExchangeID, sizeof(ExchangeID), nullptr);
}
static void ParseRecord(sqlite3_stmt* statement, std::list<Exchange*>& records)
{
	Exchange* record = Exchange::Allocate();
	Utility::Strcpy(record->ExchangeID, (const char*)sqlite3_column_text(statement, 0));
	Utility::Strcpy(record->ExchangeName, (const char*)sqlite3_column_text(statement, 1));
	records.push_back(record);
}
static void SetStatementForProductRecord(sqlite3_stmt* statement, Product* record)
{
	sqlite3_bind_text(statement, 1, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 2, record->ProductID, sizeof(record->ProductID), nullptr);
	sqlite3_bind_text(statement, 3, record->ProductName, sizeof(record->ProductName), nullptr);
	sqlite3_bind_int(statement, 4, int(record->ProductClass));
	sqlite3_bind_int(statement, 5, record->VolumeMultiple);
	sqlite3_bind_double(statement, 6, record->PriceTick);
	sqlite3_bind_int64(statement, 7, record->MaxMarketOrderVolume);
	sqlite3_bind_int64(statement, 8, record->MinMarketOrderVolume);
	sqlite3_bind_int64(statement, 9, record->MaxLimitOrderVolume);
	sqlite3_bind_int64(statement, 10, record->MinLimitOrderVolume);
	sqlite3_bind_text(statement, 11, record->SessionName, sizeof(record->SessionName), nullptr);
}
static void SetStatementForProductRecordUpdate(sqlite3_stmt* statement, Product* record)
{
	sqlite3_bind_text(statement, 1, record->ProductName, sizeof(record->ProductName), nullptr);
	sqlite3_bind_int(statement, 2, int(record->ProductClass));
	sqlite3_bind_int(statement, 3, record->VolumeMultiple);
	sqlite3_bind_double(statement, 4, record->PriceTick);
	sqlite3_bind_int64(statement, 5, record->MaxMarketOrderVolume);
	sqlite3_bind_int64(statement, 6, record->MinMarketOrderVolume);
	sqlite3_bind_int64(statement, 7, record->MaxLimitOrderVolume);
	sqlite3_bind_int64(statement, 8, record->MinLimitOrderVolume);
	sqlite3_bind_text(statement, 9, record->SessionName, sizeof(record->SessionName), nullptr);
	sqlite3_bind_text(statement, 10, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 11, record->ProductID, sizeof(record->ProductID), nullptr);
}
static void SetStatementForProductPrimaryKey(sqlite3_stmt* statement, const ExchangeIDType& ExchangeID, const ProductIDType& ProductID)
{
	sqlite3_bind_text(statement, 1, ExchangeID, sizeof(ExchangeID), nullptr);
	sqlite3_bind_text(statement, 2, ProductID, sizeof(ProductID), nullptr);
}
static void ParseRecord(sqlite3_stmt* statement, std::list<Product*>& records)
{
	Product* record = Product::Allocate();
	Utility::Strcpy(record->ExchangeID, (const char*)sqlite3_column_text(statement, 0));
	Utility::Strcpy(record->ProductID, (const char*)sqlite3_column_text(statement, 1));
	Utility::Strcpy(record->ProductName, (const char*)sqlite3_column_text(statement, 2));
	record->ProductClass = ProductClassType(sqlite3_column_int(statement, 3));
	record->VolumeMultiple = sqlite3_column_int(statement, 4);
	record->PriceTick = sqlite3_column_double(statement, 5);
	record->MaxMarketOrderVolume = sqlite3_column_int64(statement, 6);
	record->MinMarketOrderVolume = sqlite3_column_int64(statement, 7);
	record->MaxLimitOrderVolume = sqlite3_column_int64(statement, 8);
	record->MinLimitOrderVolume = sqlite3_column_int64(statement, 9);
	Utility::Strcpy(record->SessionName, (const char*)sqlite3_column_text(statement, 10));
	records.push_back(record);
}
static void SetStatementForInstrumentRecord(sqlite3_stmt* statement, Instrument* record)
{
	sqlite3_bind_text(statement, 1, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 2, record->InstrumentID, sizeof(record->InstrumentID), nullptr);
	sqlite3_bind_text(statement, 3, record->ExchangeInstID, sizeof(record->ExchangeInstID), nullptr);
	sqlite3_bind_text(statement, 4, record->InstrumentName, sizeof(record->InstrumentName), nullptr);
	sqlite3_bind_text(statement, 5, record->ProductID, sizeof(record->ProductID), nullptr);
	sqlite3_bind_int(statement, 6, int(record->ProductClass));
	sqlite3_bind_int(statement, 7, int(record->InstrumentClass));
	sqlite3_bind_int(statement, 8, record->Rank);
	sqlite3_bind_int(statement, 9, record->VolumeMultiple);
	sqlite3_bind_double(statement, 10, record->PriceTick);
	sqlite3_bind_int64(statement, 11, record->MaxMarketOrderVolume);
	sqlite3_bind_int64(statement, 12, record->MinMarketOrderVolume);
	sqlite3_bind_int64(statement, 13, record->MaxLimitOrderVolume);
	sqlite3_bind_int64(statement, 14, record->MinLimitOrderVolume);
	sqlite3_bind_text(statement, 15, record->SessionName, sizeof(record->SessionName), nullptr);
}
static void SetStatementForInstrumentRecordUpdate(sqlite3_stmt* statement, Instrument* record)
{
	sqlite3_bind_text(statement, 1, record->ExchangeInstID, sizeof(record->ExchangeInstID), nullptr);
	sqlite3_bind_text(statement, 2, record->InstrumentName, sizeof(record->InstrumentName), nullptr);
	sqlite3_bind_text(statement, 3, record->ProductID, sizeof(record->ProductID), nullptr);
	sqlite3_bind_int(statement, 4, int(record->ProductClass));
	sqlite3_bind_int(statement, 5, int(record->InstrumentClass));
	sqlite3_bind_int(statement, 6, record->Rank);
	sqlite3_bind_int(statement, 7, record->VolumeMultiple);
	sqlite3_bind_double(statement, 8, record->PriceTick);
	sqlite3_bind_int64(statement, 9, record->MaxMarketOrderVolume);
	sqlite3_bind_int64(statement, 10, record->MinMarketOrderVolume);
	sqlite3_bind_int64(statement, 11, record->MaxLimitOrderVolume);
	sqlite3_bind_int64(statement, 12, record->MinLimitOrderVolume);
	sqlite3_bind_text(statement, 13, record->SessionName, sizeof(record->SessionName), nullptr);
	sqlite3_bind_text(statement, 14, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 15, record->InstrumentID, sizeof(record->InstrumentID), nullptr);
}
static void SetStatementForInstrumentPrimaryKey(sqlite3_stmt* statement, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID)
{
	sqlite3_bind_text(statement, 1, ExchangeID, sizeof(ExchangeID), nullptr);
	sqlite3_bind_text(statement, 2, InstrumentID, sizeof(InstrumentID), nullptr);
}
static void ParseRecord(sqlite3_stmt* statement, std::list<Instrument*>& records)
{
	Instrument* record = Instrument::Allocate();
	Utility::Strcpy(record->ExchangeID, (const char*)sqlite3_column_text(statement, 0));
	Utility::Strcpy(record->InstrumentID, (const char*)sqlite3_column_text(statement, 1));
	Utility::Strcpy(record->ExchangeInstID, (const char*)sqlite3_column_text(statement, 2));
	Utility::Strcpy(record->InstrumentName, (const char*)sqlite3_column_text(statement, 3));
	Utility::Strcpy(record->ProductID, (const char*)sqlite3_column_text(statement, 4));
	record->ProductClass = ProductClassType(sqlite3_column_int(statement, 5));
	record->InstrumentClass = InstrumentClassType(sqlite3_column_int(statement, 6));
	record->Rank = sqlite3_column_int(statement, 7);
	record->VolumeMultiple = sqlite3_column_int(statement, 8);
	record->PriceTick = sqlite3_column_double(statement, 9);
	record->MaxMarketOrderVolume = sqlite3_column_int64(statement, 10);
	record->MinMarketOrderVolume = sqlite3_column_int64(statement, 11);
	record->MaxLimitOrderVolume = sqlite3_column_int64(statement, 12);
	record->MinLimitOrderVolume = sqlite3_column_int64(statement, 13);
	Utility::Strcpy(record->SessionName, (const char*)sqlite3_column_text(statement, 14));
	records.push_back(record);
}
static void SetStatementForPrimaryAccountRecord(sqlite3_stmt* statement, PrimaryAccount* record)
{
	sqlite3_bind_text(statement, 1, record->PrimaryAccountID, sizeof(record->PrimaryAccountID), nullptr);
	sqlite3_bind_text(statement, 2, record->PrimaryAccountName, sizeof(record->PrimaryAccountName), nullptr);
	sqlite3_bind_int(statement, 3, int(record->AccountClass));
	sqlite3_bind_text(statement, 4, record->BrokerPassword, sizeof(record->BrokerPassword), nullptr);
	sqlite3_bind_int(statement, 5, record->OfferID);
	sqlite3_bind_int(statement, 6, record->IsAllowLogin);
	sqlite3_bind_int(statement, 7, record->IsSimulateAccount);
	sqlite3_bind_int(statement, 8, int(record->LoginStatus));
	sqlite3_bind_int(statement, 9, int(record->InitStatus));
}
static void SetStatementForPrimaryAccountRecordUpdate(sqlite3_stmt* statement, PrimaryAccount* record)
{
	sqlite3_bind_text(statement, 1, record->PrimaryAccountName, sizeof(record->PrimaryAccountName), nullptr);
	sqlite3_bind_int(statement, 2, int(record->AccountClass));
	sqlite3_bind_text(statement, 3, record->BrokerPassword, sizeof(record->BrokerPassword), nullptr);
	sqlite3_bind_int(statement, 4, record->OfferID);
	sqlite3_bind_int(statement, 5, record->IsAllowLogin);
	sqlite3_bind_int(statement, 6, record->IsSimulateAccount);
	sqlite3_bind_int(statement, 7, int(record->LoginStatus));
	sqlite3_bind_int(statement, 8, int(record->InitStatus));
	sqlite3_bind_text(statement, 9, record->PrimaryAccountID, sizeof(record->PrimaryAccountID), nullptr);
}
static void SetStatementForPrimaryAccountPrimaryKey(sqlite3_stmt* statement, const AccountIDType& PrimaryAccountID)
{
	sqlite3_bind_text(statement, 1, PrimaryAccountID, sizeof(PrimaryAccountID), nullptr);
}
static void SetStatementForPrimaryAccountIndexOfferID(sqlite3_stmt* statement, PrimaryAccount* record)
{
	sqlite3_bind_int(statement, 1, record->OfferID);
}
static void ParseRecord(sqlite3_stmt* statement, std::list<PrimaryAccount*>& records)
{
	PrimaryAccount* record = PrimaryAccount::Allocate();
	Utility::Strcpy(record->PrimaryAccountID, (const char*)sqlite3_column_text(statement, 0));
	Utility::Strcpy(record->PrimaryAccountName, (const char*)sqlite3_column_text(statement, 1));
	record->AccountClass = AccountClassType(sqlite3_column_int(statement, 2));
	Utility::Strcpy(record->BrokerPassword, (const char*)sqlite3_column_text(statement, 3));
	record->OfferID = sqlite3_column_int(statement, 4);
	record->IsAllowLogin = sqlite3_column_int(statement, 5);
	record->IsSimulateAccount = sqlite3_column_int(statement, 6);
	record->LoginStatus = LoginStatusType(sqlite3_column_int(statement, 7));
	record->InitStatus = InitStatusType(sqlite3_column_int(statement, 8));
	records.push_back(record);
}
static void SetStatementForAccountRecord(sqlite3_stmt* statement, Account* record)
{
	sqlite3_bind_text(statement, 1, record->AccountID, sizeof(record->AccountID), nullptr);
	sqlite3_bind_text(statement, 2, record->AccountName, sizeof(record->AccountName), nullptr);
	sqlite3_bind_int(statement, 3, int(record->AccountType));
	sqlite3_bind_int(statement, 4, int(record->AccountStatus));
	sqlite3_bind_text(statement, 5, record->Password, sizeof(record->Password), nullptr);
	sqlite3_bind_int(statement, 6, record->TradeGroupID);
	sqlite3_bind_int(statement, 7, record->RiskGroupID);
	sqlite3_bind_int(statement, 8, record->CommissionGroupID);
}
static void SetStatementForAccountRecordUpdate(sqlite3_stmt* statement, Account* record)
{
	sqlite3_bind_text(statement, 1, record->AccountName, sizeof(record->AccountName), nullptr);
	sqlite3_bind_int(statement, 2, int(record->AccountType));
	sqlite3_bind_int(statement, 3, int(record->AccountStatus));
	sqlite3_bind_text(statement, 4, record->Password, sizeof(record->Password), nullptr);
	sqlite3_bind_int(statement, 5, record->TradeGroupID);
	sqlite3_bind_int(statement, 6, record->RiskGroupID);
	sqlite3_bind_int(statement, 7, record->CommissionGroupID);
	sqlite3_bind_text(statement, 8, record->AccountID, sizeof(record->AccountID), nullptr);
}
static void SetStatementForAccountPrimaryKey(sqlite3_stmt* statement, const AccountIDType& AccountID)
{
	sqlite3_bind_text(statement, 1, AccountID, sizeof(AccountID), nullptr);
}
static void ParseRecord(sqlite3_stmt* statement, std::list<Account*>& records)
{
	Account* record = Account::Allocate();
	Utility::Strcpy(record->AccountID, (const char*)sqlite3_column_text(statement, 0));
	Utility::Strcpy(record->AccountName, (const char*)sqlite3_column_text(statement, 1));
	record->AccountType = AccountTypeType(sqlite3_column_int(statement, 2));
	record->AccountStatus = AccountStatusType(sqlite3_column_int(statement, 3));
	Utility::Strcpy(record->Password, (const char*)sqlite3_column_text(statement, 4));
	record->TradeGroupID = sqlite3_column_int(statement, 5);
	record->RiskGroupID = sqlite3_column_int(statement, 6);
	record->CommissionGroupID = sqlite3_column_int(statement, 7);
	records.push_back(record);
}
static void SetStatementForCapitalRecord(sqlite3_stmt* statement, Capital* record)
{
	sqlite3_bind_text(statement, 1, record->TradingDay, sizeof(record->TradingDay), nullptr);
	sqlite3_bind_text(statement, 2, record->AccountID, sizeof(record->AccountID), nullptr);
	sqlite3_bind_int(statement, 3, int(record->AccountType));
	sqlite3_bind_double(statement, 4, record->Balance);
	sqlite3_bind_double(statement, 5, record->PreBalance);
	sqlite3_bind_double(statement, 6, record->Available);
	sqlite3_bind_double(statement, 7, record->MarketValue);
	sqlite3_bind_double(statement, 8, record->CashIn);
	sqlite3_bind_double(statement, 9, record->CashOut);
	sqlite3_bind_double(statement, 10, record->Margin);
	sqlite3_bind_double(statement, 11, record->Commission);
	sqlite3_bind_double(statement, 12, record->FrozenCash);
	sqlite3_bind_double(statement, 13, record->FrozenMargin);
	sqlite3_bind_double(statement, 14, record->FrozenCommission);
	sqlite3_bind_double(statement, 15, record->CloseProfitByDate);
	sqlite3_bind_double(statement, 16, record->CloseProfitByTrade);
	sqlite3_bind_double(statement, 17, record->PositionProfitByDate);
	sqlite3_bind_double(statement, 18, record->PositionProfitByTrade);
	sqlite3_bind_double(statement, 19, record->Deposit);
	sqlite3_bind_double(statement, 20, record->Withdraw);
}
static void SetStatementForCapitalRecordUpdate(sqlite3_stmt* statement, Capital* record)
{
	sqlite3_bind_int(statement, 1, int(record->AccountType));
	sqlite3_bind_double(statement, 2, record->Balance);
	sqlite3_bind_double(statement, 3, record->PreBalance);
	sqlite3_bind_double(statement, 4, record->Available);
	sqlite3_bind_double(statement, 5, record->MarketValue);
	sqlite3_bind_double(statement, 6, record->CashIn);
	sqlite3_bind_double(statement, 7, record->CashOut);
	sqlite3_bind_double(statement, 8, record->Margin);
	sqlite3_bind_double(statement, 9, record->Commission);
	sqlite3_bind_double(statement, 10, record->FrozenCash);
	sqlite3_bind_double(statement, 11, record->FrozenMargin);
	sqlite3_bind_double(statement, 12, record->FrozenCommission);
	sqlite3_bind_double(statement, 13, record->CloseProfitByDate);
	sqlite3_bind_double(statement, 14, record->CloseProfitByTrade);
	sqlite3_bind_double(statement, 15, record->PositionProfitByDate);
	sqlite3_bind_double(statement, 16, record->PositionProfitByTrade);
	sqlite3_bind_double(statement, 17, record->Deposit);
	sqlite3_bind_double(statement, 18, record->Withdraw);
	sqlite3_bind_text(statement, 19, record->TradingDay, sizeof(record->TradingDay), nullptr);
	sqlite3_bind_text(statement, 20, record->AccountID, sizeof(record->AccountID), nullptr);
}
static void SetStatementForCapitalPrimaryKey(sqlite3_stmt* statement, const DateType& TradingDay, const AccountIDType& AccountID)
{
	sqlite3_bind_text(statement, 1, TradingDay, sizeof(TradingDay), nullptr);
	sqlite3_bind_text(statement, 2, AccountID, sizeof(AccountID), nullptr);
}
static void SetStatementForCapitalIndexTradingDay(sqlite3_stmt* statement, Capital* record)
{
	sqlite3_bind_text(statement, 1, record->TradingDay, sizeof(record->TradingDay), nullptr);
}
static void ParseRecord(sqlite3_stmt* statement, std::list<Capital*>& records)
{
	Capital* record = Capital::Allocate();
	Utility::Strcpy(record->TradingDay, (const char*)sqlite3_column_text(statement, 0));
	Utility::Strcpy(record->AccountID, (const char*)sqlite3_column_text(statement, 1));
	record->AccountType = AccountTypeType(sqlite3_column_int(statement, 2));
	record->Balance = sqlite3_column_double(statement, 3);
	record->PreBalance = sqlite3_column_double(statement, 4);
	record->Available = sqlite3_column_double(statement, 5);
	record->MarketValue = sqlite3_column_double(statement, 6);
	record->CashIn = sqlite3_column_double(statement, 7);
	record->CashOut = sqlite3_column_double(statement, 8);
	record->Margin = sqlite3_column_double(statement, 9);
	record->Commission = sqlite3_column_double(statement, 10);
	record->FrozenCash = sqlite3_column_double(statement, 11);
	record->FrozenMargin = sqlite3_column_double(statement, 12);
	record->FrozenCommission = sqlite3_column_double(statement, 13);
	record->CloseProfitByDate = sqlite3_column_double(statement, 14);
	record->CloseProfitByTrade = sqlite3_column_double(statement, 15);
	record->PositionProfitByDate = sqlite3_column_double(statement, 16);
	record->PositionProfitByTrade = sqlite3_column_double(statement, 17);
	record->Deposit = sqlite3_column_double(statement, 18);
	record->Withdraw = sqlite3_column_double(statement, 19);
	records.push_back(record);
}
static void SetStatementForPositionRecord(sqlite3_stmt* statement, Position* record)
{
	sqlite3_bind_text(statement, 1, record->TradingDay, sizeof(record->TradingDay), nullptr);
	sqlite3_bind_text(statement, 2, record->AccountID, sizeof(record->AccountID), nullptr);
	sqlite3_bind_int(statement, 3, int(record->AccountType));
	sqlite3_bind_text(statement, 4, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 5, record->InstrumentID, sizeof(record->InstrumentID), nullptr);
	sqlite3_bind_int(statement, 6, int(record->ProductClass));
	sqlite3_bind_int(statement, 7, int(record->PosiDirection));
	sqlite3_bind_int64(statement, 8, record->TotalPosition);
	sqlite3_bind_int64(statement, 9, record->PositionFrozen);
	sqlite3_bind_int64(statement, 10, record->TodayPosition);
	sqlite3_bind_double(statement, 11, record->MarketValue);
	sqlite3_bind_double(statement, 12, record->CashIn);
	sqlite3_bind_double(statement, 13, record->CashOut);
	sqlite3_bind_double(statement, 14, record->Margin);
	sqlite3_bind_double(statement, 15, record->Commission);
	sqlite3_bind_double(statement, 16, record->FrozenCash);
	sqlite3_bind_double(statement, 17, record->FrozenMargin);
	sqlite3_bind_double(statement, 18, record->FrozenCommission);
	sqlite3_bind_int(statement, 19, record->VolumeMultiple);
	sqlite3_bind_double(statement, 20, record->CloseProfitByDate);
	sqlite3_bind_double(statement, 21, record->CloseProfitByTrade);
	sqlite3_bind_double(statement, 22, record->PositionProfitByDate);
	sqlite3_bind_double(statement, 23, record->PositionProfitByTrade);
	sqlite3_bind_double(statement, 24, record->SettlementPrice);
	sqlite3_bind_double(statement, 25, record->PreSettlementPrice);
}
static void SetStatementForPositionRecordUpdate(sqlite3_stmt* statement, Position* record)
{
	sqlite3_bind_int(statement, 1, int(record->AccountType));
	sqlite3_bind_int(statement, 2, int(record->ProductClass));
	sqlite3_bind_int64(statement, 3, record->TotalPosition);
	sqlite3_bind_int64(statement, 4, record->PositionFrozen);
	sqlite3_bind_int64(statement, 5, record->TodayPosition);
	sqlite3_bind_double(statement, 6, record->MarketValue);
	sqlite3_bind_double(statement, 7, record->CashIn);
	sqlite3_bind_double(statement, 8, record->CashOut);
	sqlite3_bind_double(statement, 9, record->Margin);
	sqlite3_bind_double(statement, 10, record->Commission);
	sqlite3_bind_double(statement, 11, record->FrozenCash);
	sqlite3_bind_double(statement, 12, record->FrozenMargin);
	sqlite3_bind_double(statement, 13, record->FrozenCommission);
	sqlite3_bind_int(statement, 14, record->VolumeMultiple);
	sqlite3_bind_double(statement, 15, record->CloseProfitByDate);
	sqlite3_bind_double(statement, 16, record->CloseProfitByTrade);
	sqlite3_bind_double(statement, 17, record->PositionProfitByDate);
	sqlite3_bind_double(statement, 18, record->PositionProfitByTrade);
	sqlite3_bind_double(statement, 19, record->SettlementPrice);
	sqlite3_bind_double(statement, 20, record->PreSettlementPrice);
	sqlite3_bind_text(statement, 21, record->TradingDay, sizeof(record->TradingDay), nullptr);
	sqlite3_bind_text(statement, 22, record->AccountID, sizeof(record->AccountID), nullptr);
	sqlite3_bind_text(statement, 23, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 24, record->InstrumentID, sizeof(record->InstrumentID), nullptr);
	sqlite3_bind_int(statement, 25, int(record->PosiDirection));
}
static void SetStatementForPositionPrimaryKey(sqlite3_stmt* statement, const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection)
{
	sqlite3_bind_text(statement, 1, TradingDay, sizeof(TradingDay), nullptr);
	sqlite3_bind_text(statement, 2, AccountID, sizeof(AccountID), nullptr);
	sqlite3_bind_text(statement, 3, ExchangeID, sizeof(ExchangeID), nullptr);
	sqlite3_bind_text(statement, 4, InstrumentID, sizeof(InstrumentID), nullptr);
	sqlite3_bind_int(statement, 5, int(PosiDirection));
}
static void SetStatementForPositionIndexAccount(sqlite3_stmt* statement, Position* record)
{
	sqlite3_bind_text(statement, 1, record->TradingDay, sizeof(record->TradingDay), nullptr);
	sqlite3_bind_text(statement, 2, record->AccountID, sizeof(record->AccountID), nullptr);
}
static void SetStatementForPositionIndexTradingDay(sqlite3_stmt* statement, Position* record)
{
	sqlite3_bind_text(statement, 1, record->TradingDay, sizeof(record->TradingDay), nullptr);
}
static void ParseRecord(sqlite3_stmt* statement, std::list<Position*>& records)
{
	Position* record = Position::Allocate();
	Utility::Strcpy(record->TradingDay, (const char*)sqlite3_column_text(statement, 0));
	Utility::Strcpy(record->AccountID, (const char*)sqlite3_column_text(statement, 1));
	record->AccountType = AccountTypeType(sqlite3_column_int(statement, 2));
	Utility::Strcpy(record->ExchangeID, (const char*)sqlite3_column_text(statement, 3));
	Utility::Strcpy(record->InstrumentID, (const char*)sqlite3_column_text(statement, 4));
	record->ProductClass = ProductClassType(sqlite3_column_int(statement, 5));
	record->PosiDirection = PosiDirectionType(sqlite3_column_int(statement, 6));
	record->TotalPosition = sqlite3_column_int64(statement, 7);
	record->PositionFrozen = sqlite3_column_int64(statement, 8);
	record->TodayPosition = sqlite3_column_int64(statement, 9);
	record->MarketValue = sqlite3_column_double(statement, 10);
	record->CashIn = sqlite3_column_double(statement, 11);
	record->CashOut = sqlite3_column_double(statement, 12);
	record->Margin = sqlite3_column_double(statement, 13);
	record->Commission = sqlite3_column_double(statement, 14);
	record->FrozenCash = sqlite3_column_double(statement, 15);
	record->FrozenMargin = sqlite3_column_double(statement, 16);
	record->FrozenCommission = sqlite3_column_double(statement, 17);
	record->VolumeMultiple = sqlite3_column_int(statement, 18);
	record->CloseProfitByDate = sqlite3_column_double(statement, 19);
	record->CloseProfitByTrade = sqlite3_column_double(statement, 20);
	record->PositionProfitByDate = sqlite3_column_double(statement, 21);
	record->PositionProfitByTrade = sqlite3_column_double(statement, 22);
	record->SettlementPrice = sqlite3_column_double(statement, 23);
	record->PreSettlementPrice = sqlite3_column_double(statement, 24);
	records.push_back(record);
}
static void SetStatementForPositionDetailRecord(sqlite3_stmt* statement, PositionDetail* record)
{
	sqlite3_bind_text(statement, 1, record->TradingDay, sizeof(record->TradingDay), nullptr);
	sqlite3_bind_text(statement, 2, record->AccountID, sizeof(record->AccountID), nullptr);
	sqlite3_bind_int(statement, 3, int(record->AccountType));
	sqlite3_bind_text(statement, 4, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 5, record->InstrumentID, sizeof(record->InstrumentID), nullptr);
	sqlite3_bind_int(statement, 6, int(record->ProductClass));
	sqlite3_bind_int(statement, 7, int(record->PosiDirection));
	sqlite3_bind_text(statement, 8, record->OpenDate, sizeof(record->OpenDate), nullptr);
	sqlite3_bind_text(statement, 9, record->TradeID, sizeof(record->TradeID), nullptr);
	sqlite3_bind_int64(statement, 10, record->Volume);
	sqlite3_bind_double(statement, 11, record->OpenPrice);
	sqlite3_bind_double(statement, 12, record->MarketValue);
	sqlite3_bind_double(statement, 13, record->CashIn);
	sqlite3_bind_double(statement, 14, record->CashOut);
	sqlite3_bind_double(statement, 15, record->Margin);
	sqlite3_bind_double(statement, 16, record->Commission);
	sqlite3_bind_int(statement, 17, record->VolumeMultiple);
	sqlite3_bind_double(statement, 18, record->CloseProfitByDate);
	sqlite3_bind_double(statement, 19, record->CloseProfitByTrade);
	sqlite3_bind_double(statement, 20, record->PositionProfitByDate);
	sqlite3_bind_double(statement, 21, record->PositionProfitByTrade);
	sqlite3_bind_double(statement, 22, record->SettlementPrice);
	sqlite3_bind_double(statement, 23, record->PreSettlementPrice);
	sqlite3_bind_int64(statement, 24, record->CloseVolume);
	sqlite3_bind_double(statement, 25, record->CloseAmount);
}
static void SetStatementForPositionDetailRecordUpdate(sqlite3_stmt* statement, PositionDetail* record)
{
	sqlite3_bind_int(statement, 1, int(record->AccountType));
	sqlite3_bind_int(statement, 2, int(record->ProductClass));
	sqlite3_bind_int64(statement, 3, record->Volume);
	sqlite3_bind_double(statement, 4, record->OpenPrice);
	sqlite3_bind_double(statement, 5, record->MarketValue);
	sqlite3_bind_double(statement, 6, record->CashIn);
	sqlite3_bind_double(statement, 7, record->CashOut);
	sqlite3_bind_double(statement, 8, record->Margin);
	sqlite3_bind_double(statement, 9, record->Commission);
	sqlite3_bind_int(statement, 10, record->VolumeMultiple);
	sqlite3_bind_double(statement, 11, record->CloseProfitByDate);
	sqlite3_bind_double(statement, 12, record->CloseProfitByTrade);
	sqlite3_bind_double(statement, 13, record->PositionProfitByDate);
	sqlite3_bind_double(statement, 14, record->PositionProfitByTrade);
	sqlite3_bind_double(statement, 15, record->SettlementPrice);
	sqlite3_bind_double(statement, 16, record->PreSettlementPrice);
	sqlite3_bind_int64(statement, 17, record->CloseVolume);
	sqlite3_bind_double(statement, 18, record->CloseAmount);
	sqlite3_bind_text(statement, 19, record->TradingDay, sizeof(record->TradingDay), nullptr);
	sqlite3_bind_text(statement, 20, record->AccountID, sizeof(record->AccountID), nullptr);
	sqlite3_bind_text(statement, 21, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 22, record->InstrumentID, sizeof(record->InstrumentID), nullptr);
	sqlite3_bind_int(statement, 23, int(record->PosiDirection));
	sqlite3_bind_text(statement, 24, record->OpenDate, sizeof(record->OpenDate), nullptr);
	sqlite3_bind_text(statement, 25, record->TradeID, sizeof(record->TradeID), nullptr);
}
static void SetStatementForPositionDetailPrimaryKey(sqlite3_stmt* statement, const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const PosiDirectionType& PosiDirection, const DateType& OpenDate, const TradeIDType& TradeID)
{
	sqlite3_bind_text(statement, 1, TradingDay, sizeof(TradingDay), nullptr);
	sqlite3_bind_text(statement, 2, AccountID, sizeof(AccountID), nullptr);
	sqlite3_bind_text(statement, 3, ExchangeID, sizeof(ExchangeID), nullptr);
	sqlite3_bind_text(statement, 4, InstrumentID, sizeof(InstrumentID), nullptr);
	sqlite3_bind_int(statement, 5, int(PosiDirection));
	sqlite3_bind_text(statement, 6, OpenDate, sizeof(OpenDate), nullptr);
	sqlite3_bind_text(statement, 7, TradeID, sizeof(TradeID), nullptr);
}
static void SetStatementForPositionDetailIndexTradeMatch(sqlite3_stmt* statement, PositionDetail* record)
{
	sqlite3_bind_text(statement, 1, record->TradingDay, sizeof(record->TradingDay), nullptr);
	sqlite3_bind_text(statement, 2, record->AccountID, sizeof(record->AccountID), nullptr);
	sqlite3_bind_text(statement, 3, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 4, record->InstrumentID, sizeof(record->InstrumentID), nullptr);
	sqlite3_bind_int(statement, 5, int(record->PosiDirection));
}
static void SetStatementForPositionDetailIndexTradingDay(sqlite3_stmt* statement, PositionDetail* record)
{
	sqlite3_bind_text(statement, 1, record->TradingDay, sizeof(record->TradingDay), nullptr);
}
static void ParseRecord(sqlite3_stmt* statement, std::list<PositionDetail*>& records)
{
	PositionDetail* record = PositionDetail::Allocate();
	Utility::Strcpy(record->TradingDay, (const char*)sqlite3_column_text(statement, 0));
	Utility::Strcpy(record->AccountID, (const char*)sqlite3_column_text(statement, 1));
	record->AccountType = AccountTypeType(sqlite3_column_int(statement, 2));
	Utility::Strcpy(record->ExchangeID, (const char*)sqlite3_column_text(statement, 3));
	Utility::Strcpy(record->InstrumentID, (const char*)sqlite3_column_text(statement, 4));
	record->ProductClass = ProductClassType(sqlite3_column_int(statement, 5));
	record->PosiDirection = PosiDirectionType(sqlite3_column_int(statement, 6));
	Utility::Strcpy(record->OpenDate, (const char*)sqlite3_column_text(statement, 7));
	Utility::Strcpy(record->TradeID, (const char*)sqlite3_column_text(statement, 8));
	record->Volume = sqlite3_column_int64(statement, 9);
	record->OpenPrice = sqlite3_column_double(statement, 10);
	record->MarketValue = sqlite3_column_double(statement, 11);
	record->CashIn = sqlite3_column_double(statement, 12);
	record->CashOut = sqlite3_column_double(statement, 13);
	record->Margin = sqlite3_column_double(statement, 14);
	record->Commission = sqlite3_column_double(statement, 15);
	record->VolumeMultiple = sqlite3_column_int(statement, 16);
	record->CloseProfitByDate = sqlite3_column_double(statement, 17);
	record->CloseProfitByTrade = sqlite3_column_double(statement, 18);
	record->PositionProfitByDate = sqlite3_column_double(statement, 19);
	record->PositionProfitByTrade = sqlite3_column_double(statement, 20);
	record->SettlementPrice = sqlite3_column_double(statement, 21);
	record->PreSettlementPrice = sqlite3_column_double(statement, 22);
	record->CloseVolume = sqlite3_column_int64(statement, 23);
	record->CloseAmount = sqlite3_column_double(statement, 24);
	records.push_back(record);
}
static void SetStatementForOrderRecord(sqlite3_stmt* statement, Order* record)
{
	sqlite3_bind_text(statement, 1, record->TradingDay, sizeof(record->TradingDay), nullptr);
	sqlite3_bind_text(statement, 2, record->AccountID, sizeof(record->AccountID), nullptr);
	sqlite3_bind_int(statement, 3, int(record->AccountType));
	sqlite3_bind_text(statement, 4, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 5, record->InstrumentID, sizeof(record->InstrumentID), nullptr);
	sqlite3_bind_int(statement, 6, int(record->ProductClass));
	sqlite3_bind_int(statement, 7, record->OrderID);
	sqlite3_bind_text(statement, 8, record->OrderSysID, sizeof(record->OrderSysID), nullptr);
	sqlite3_bind_int(statement, 9, int(record->Direction));
	sqlite3_bind_int(statement, 10, int(record->OffsetFlag));
	sqlite3_bind_int(statement, 11, int(record->OrderPriceType));
	sqlite3_bind_double(statement, 12, record->Price);
	sqlite3_bind_int64(statement, 13, record->Volume);
	sqlite3_bind_int64(statement, 14, record->VolumeTotal);
	sqlite3_bind_int64(statement, 15, record->VolumeTraded);
	sqlite3_bind_int(statement, 16, record->VolumeMultiple);
	sqlite3_bind_int(statement, 17, int(record->OrderStatus));
	sqlite3_bind_text(statement, 18, record->OrderDate, sizeof(record->OrderDate), nullptr);
	sqlite3_bind_text(statement, 19, record->OrderTime, sizeof(record->OrderTime), nullptr);
	sqlite3_bind_text(statement, 20, record->CancelDate, sizeof(record->CancelDate), nullptr);
	sqlite3_bind_text(statement, 21, record->CancelTime, sizeof(record->CancelTime), nullptr);
	sqlite3_bind_int64(statement, 22, record->SessionID);
	sqlite3_bind_int(statement, 23, record->ClientOrderID);
	sqlite3_bind_int(statement, 24, record->RequestID);
	sqlite3_bind_int(statement, 25, record->OfferID);
	sqlite3_bind_int(statement, 26, record->TradeGroupID);
	sqlite3_bind_int(statement, 27, record->RiskGroupID);
	sqlite3_bind_int(statement, 28, record->CommissionGroupID);
	sqlite3_bind_double(statement, 29, record->FrozenCash);
	sqlite3_bind_double(statement, 30, record->FrozenMargin);
	sqlite3_bind_double(statement, 31, record->FrozenCommission);
	sqlite3_bind_int(statement, 32, record->RebuildMark);
	sqlite3_bind_int(statement, 33, record->IsForceClose);
}
static void SetStatementForOrderRecordUpdate(sqlite3_stmt* statement, Order* record)
{
	sqlite3_bind_int(statement, 1, int(record->AccountType));
	sqlite3_bind_int(statement, 2, int(record->ProductClass));
	sqlite3_bind_text(statement, 3, record->OrderSysID, sizeof(record->OrderSysID), nullptr);
	sqlite3_bind_int(statement, 4, int(record->Direction));
	sqlite3_bind_int(statement, 5, int(record->OffsetFlag));
	sqlite3_bind_int(statement, 6, int(record->OrderPriceType));
	sqlite3_bind_double(statement, 7, record->Price);
	sqlite3_bind_int64(statement, 8, record->Volume);
	sqlite3_bind_int64(statement, 9, record->VolumeTotal);
	sqlite3_bind_int64(statement, 10, record->VolumeTraded);
	sqlite3_bind_int(statement, 11, record->VolumeMultiple);
	sqlite3_bind_int(statement, 12, int(record->OrderStatus));
	sqlite3_bind_text(statement, 13, record->OrderDate, sizeof(record->OrderDate), nullptr);
	sqlite3_bind_text(statement, 14, record->OrderTime, sizeof(record->OrderTime), nullptr);
	sqlite3_bind_text(statement, 15, record->CancelDate, sizeof(record->CancelDate), nullptr);
	sqlite3_bind_text(statement, 16, record->CancelTime, sizeof(record->CancelTime), nullptr);
	sqlite3_bind_int64(statement, 17, record->SessionID);
	sqlite3_bind_int(statement, 18, record->ClientOrderID);
	sqlite3_bind_int(statement, 19, record->RequestID);
	sqlite3_bind_int(statement, 20, record->OfferID);
	sqlite3_bind_int(statement, 21, record->TradeGroupID);
	sqlite3_bind_int(statement, 22, record->RiskGroupID);
	sqlite3_bind_int(statement, 23, record->CommissionGroupID);
	sqlite3_bind_double(statement, 24, record->FrozenCash);
	sqlite3_bind_double(statement, 25, record->FrozenMargin);
	sqlite3_bind_double(statement, 26, record->FrozenCommission);
	sqlite3_bind_int(statement, 27, record->RebuildMark);
	sqlite3_bind_int(statement, 28, record->IsForceClose);
	sqlite3_bind_text(statement, 29, record->TradingDay, sizeof(record->TradingDay), nullptr);
	sqlite3_bind_text(statement, 30, record->AccountID, sizeof(record->AccountID), nullptr);
	sqlite3_bind_text(statement, 31, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 32, record->InstrumentID, sizeof(record->InstrumentID), nullptr);
	sqlite3_bind_int(statement, 33, record->OrderID);
}
static void SetStatementForOrderPrimaryKey(sqlite3_stmt* statement, const DateType& TradingDay, const AccountIDType& AccountID, const ExchangeIDType& ExchangeID, const InstrumentIDType& InstrumentID, const OrderIDType& OrderID)
{
	sqlite3_bind_text(statement, 1, TradingDay, sizeof(TradingDay), nullptr);
	sqlite3_bind_text(statement, 2, AccountID, sizeof(AccountID), nullptr);
	sqlite3_bind_text(statement, 3, ExchangeID, sizeof(ExchangeID), nullptr);
	sqlite3_bind_text(statement, 4, InstrumentID, sizeof(InstrumentID), nullptr);
	sqlite3_bind_int(statement, 5, OrderID);
}
static void ParseRecord(sqlite3_stmt* statement, std::list<Order*>& records)
{
	Order* record = Order::Allocate();
	Utility::Strcpy(record->TradingDay, (const char*)sqlite3_column_text(statement, 0));
	Utility::Strcpy(record->AccountID, (const char*)sqlite3_column_text(statement, 1));
	record->AccountType = AccountTypeType(sqlite3_column_int(statement, 2));
	Utility::Strcpy(record->ExchangeID, (const char*)sqlite3_column_text(statement, 3));
	Utility::Strcpy(record->InstrumentID, (const char*)sqlite3_column_text(statement, 4));
	record->ProductClass = ProductClassType(sqlite3_column_int(statement, 5));
	record->OrderID = sqlite3_column_int(statement, 6);
	Utility::Strcpy(record->OrderSysID, (const char*)sqlite3_column_text(statement, 7));
	record->Direction = DirectionType(sqlite3_column_int(statement, 8));
	record->OffsetFlag = OffsetFlagType(sqlite3_column_int(statement, 9));
	record->OrderPriceType = OrderPriceTypeType(sqlite3_column_int(statement, 10));
	record->Price = sqlite3_column_double(statement, 11);
	record->Volume = sqlite3_column_int64(statement, 12);
	record->VolumeTotal = sqlite3_column_int64(statement, 13);
	record->VolumeTraded = sqlite3_column_int64(statement, 14);
	record->VolumeMultiple = sqlite3_column_int(statement, 15);
	record->OrderStatus = OrderStatusType(sqlite3_column_int(statement, 16));
	Utility::Strcpy(record->OrderDate, (const char*)sqlite3_column_text(statement, 17));
	Utility::Strcpy(record->OrderTime, (const char*)sqlite3_column_text(statement, 18));
	Utility::Strcpy(record->CancelDate, (const char*)sqlite3_column_text(statement, 19));
	Utility::Strcpy(record->CancelTime, (const char*)sqlite3_column_text(statement, 20));
	record->SessionID = sqlite3_column_int64(statement, 21);
	record->ClientOrderID = sqlite3_column_int(statement, 22);
	record->RequestID = sqlite3_column_int(statement, 23);
	record->OfferID = sqlite3_column_int(statement, 24);
	record->TradeGroupID = sqlite3_column_int(statement, 25);
	record->RiskGroupID = sqlite3_column_int(statement, 26);
	record->CommissionGroupID = sqlite3_column_int(statement, 27);
	record->FrozenCash = sqlite3_column_double(statement, 28);
	record->FrozenMargin = sqlite3_column_double(statement, 29);
	record->FrozenCommission = sqlite3_column_double(statement, 30);
	record->RebuildMark = sqlite3_column_int(statement, 31);
	record->IsForceClose = sqlite3_column_int(statement, 32);
	records.push_back(record);
}
static void SetStatementForTradeRecord(sqlite3_stmt* statement, Trade* record)
{
	sqlite3_bind_text(statement, 1, record->TradingDay, sizeof(record->TradingDay), nullptr);
	sqlite3_bind_text(statement, 2, record->AccountID, sizeof(record->AccountID), nullptr);
	sqlite3_bind_int(statement, 3, int(record->AccountType));
	sqlite3_bind_text(statement, 4, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 5, record->InstrumentID, sizeof(record->InstrumentID), nullptr);
	sqlite3_bind_int(statement, 6, int(record->ProductClass));
	sqlite3_bind_int(statement, 7, record->OrderID);
	sqlite3_bind_text(statement, 8, record->OrderSysID, sizeof(record->OrderSysID), nullptr);
	sqlite3_bind_text(statement, 9, record->TradeID, sizeof(record->TradeID), nullptr);
	sqlite3_bind_int(statement, 10, int(record->Direction));
	sqlite3_bind_int(statement, 11, int(record->OffsetFlag));
	sqlite3_bind_double(statement, 12, record->Price);
	sqlite3_bind_int64(statement, 13, record->Volume);
	sqlite3_bind_int(statement, 14, record->VolumeMultiple);
	sqlite3_bind_double(statement, 15, record->TradeAmount);
	sqlite3_bind_double(statement, 16, record->Commission);
	sqlite3_bind_text(statement, 17, record->TradeDate, sizeof(record->TradeDate), nullptr);
	sqlite3_bind_text(statement, 18, record->TradeTime, sizeof(record->TradeTime), nullptr);
}
static void SetStatementForTradeRecordUpdate(sqlite3_stmt* statement, Trade* record)
{
	sqlite3_bind_text(statement, 1, record->AccountID, sizeof(record->AccountID), nullptr);
	sqlite3_bind_int(statement, 2, int(record->AccountType));
	sqlite3_bind_text(statement, 3, record->InstrumentID, sizeof(record->InstrumentID), nullptr);
	sqlite3_bind_int(statement, 4, int(record->ProductClass));
	sqlite3_bind_int(statement, 5, record->OrderID);
	sqlite3_bind_text(statement, 6, record->OrderSysID, sizeof(record->OrderSysID), nullptr);
	sqlite3_bind_int(statement, 7, int(record->OffsetFlag));
	sqlite3_bind_double(statement, 8, record->Price);
	sqlite3_bind_int64(statement, 9, record->Volume);
	sqlite3_bind_int(statement, 10, record->VolumeMultiple);
	sqlite3_bind_double(statement, 11, record->TradeAmount);
	sqlite3_bind_double(statement, 12, record->Commission);
	sqlite3_bind_text(statement, 13, record->TradeDate, sizeof(record->TradeDate), nullptr);
	sqlite3_bind_text(statement, 14, record->TradeTime, sizeof(record->TradeTime), nullptr);
	sqlite3_bind_text(statement, 15, record->TradingDay, sizeof(record->TradingDay), nullptr);
	sqlite3_bind_text(statement, 16, record->ExchangeID, sizeof(record->ExchangeID), nullptr);
	sqlite3_bind_text(statement, 17, record->TradeID, sizeof(record->TradeID), nullptr);
	sqlite3_bind_int(statement, 18, int(record->Direction));
}
static void SetStatementForTradePrimaryKey(sqlite3_stmt* statement, const DateType& TradingDay, const ExchangeIDType& ExchangeID, const TradeIDType& TradeID, const DirectionType& Direction)
{
	sqlite3_bind_text(statement, 1, TradingDay, sizeof(TradingDay), nullptr);
	sqlite3_bind_text(statement, 2, ExchangeID, sizeof(ExchangeID), nullptr);
	sqlite3_bind_text(statement, 3, TradeID, sizeof(TradeID), nullptr);
	sqlite3_bind_int(statement, 4, int(Direction));
}
static void ParseRecord(sqlite3_stmt* statement, std::list<Trade*>& records)
{
	Trade* record = Trade::Allocate();
	Utility::Strcpy(record->TradingDay, (const char*)sqlite3_column_text(statement, 0));
	Utility::Strcpy(record->AccountID, (const char*)sqlite3_column_text(statement, 1));
	record->AccountType = AccountTypeType(sqlite3_column_int(statement, 2));
	Utility::Strcpy(record->ExchangeID, (const char*)sqlite3_column_text(statement, 3));
	Utility::Strcpy(record->InstrumentID, (const char*)sqlite3_column_text(statement, 4));
	record->ProductClass = ProductClassType(sqlite3_column_int(statement, 5));
	record->OrderID = sqlite3_column_int(statement, 6);
	Utility::Strcpy(record->OrderSysID, (const char*)sqlite3_column_text(statement, 7));
	Utility::Strcpy(record->TradeID, (const char*)sqlite3_column_text(statement, 8));
	record->Direction = DirectionType(sqlite3_column_int(statement, 9));
	record->OffsetFlag = OffsetFlagType(sqlite3_column_int(statement, 10));
	record->Price = sqlite3_column_double(statement, 11);
	record->Volume = sqlite3_column_int64(statement, 12);
	record->VolumeMultiple = sqlite3_column_int(statement, 13);
	record->TradeAmount = sqlite3_column_double(statement, 14);
	record->Commission = sqlite3_column_double(statement, 15);
	Utility::Strcpy(record->TradeDate, (const char*)sqlite3_column_text(statement, 16));
	Utility::Strcpy(record->TradeTime, (const char*)sqlite3_column_text(statement, 17));
	records.push_back(record);
}


class SqliteWrapper::Impl
{
public:
Impl(const std::string& dbName)
	:m_DBName(dbName)
{
	m_SqlBuff = new char[BuffSize];
	
	m_DB = nullptr;
	m_TradingDayInsertStatement = nullptr;
	m_TradingDayDeleteStatement = nullptr;
	m_TradingDayUpdateStatement = nullptr;
	m_TradingDaySelectStatement = nullptr;
	m_TradingDayTruncateStatement = nullptr;
	m_ExchangeInsertStatement = nullptr;
	m_ExchangeDeleteStatement = nullptr;
	m_ExchangeUpdateStatement = nullptr;
	m_ExchangeSelectStatement = nullptr;
	m_ExchangeTruncateStatement = nullptr;
	m_ProductInsertStatement = nullptr;
	m_ProductDeleteStatement = nullptr;
	m_ProductUpdateStatement = nullptr;
	m_ProductSelectStatement = nullptr;
	m_ProductTruncateStatement = nullptr;
	m_InstrumentInsertStatement = nullptr;
	m_InstrumentDeleteStatement = nullptr;
	m_InstrumentUpdateStatement = nullptr;
	m_InstrumentSelectStatement = nullptr;
	m_InstrumentTruncateStatement = nullptr;
	m_PrimaryAccountInsertStatement = nullptr;
	m_PrimaryAccountDeleteStatement = nullptr;
	m_PrimaryAccountDeleteByOfferIDIndexStatement = nullptr;
	m_PrimaryAccountUpdateStatement = nullptr;
	m_PrimaryAccountSelectStatement = nullptr;
	m_PrimaryAccountTruncateStatement = nullptr;
	m_AccountInsertStatement = nullptr;
	m_AccountDeleteStatement = nullptr;
	m_AccountUpdateStatement = nullptr;
	m_AccountSelectStatement = nullptr;
	m_AccountTruncateStatement = nullptr;
	m_CapitalInsertStatement = nullptr;
	m_CapitalDeleteStatement = nullptr;
	m_CapitalDeleteByTradingDayIndexStatement = nullptr;
	m_CapitalUpdateStatement = nullptr;
	m_CapitalSelectStatement = nullptr;
	m_CapitalTruncateStatement = nullptr;
	m_PositionInsertStatement = nullptr;
	m_PositionDeleteStatement = nullptr;
	m_PositionDeleteByAccountIndexStatement = nullptr;
	m_PositionDeleteByTradingDayIndexStatement = nullptr;
	m_PositionUpdateStatement = nullptr;
	m_PositionSelectStatement = nullptr;
	m_PositionTruncateStatement = nullptr;
	m_PositionDetailInsertStatement = nullptr;
	m_PositionDetailDeleteStatement = nullptr;
	m_PositionDetailDeleteByTradeMatchIndexStatement = nullptr;
	m_PositionDetailDeleteByTradingDayIndexStatement = nullptr;
	m_PositionDetailUpdateStatement = nullptr;
	m_PositionDetailSelectStatement = nullptr;
	m_PositionDetailTruncateStatement = nullptr;
	m_OrderInsertStatement = nullptr;
	m_OrderDeleteStatement = nullptr;
	m_OrderUpdateStatement = nullptr;
	m_OrderSelectStatement = nullptr;
	m_OrderTruncateStatement = nullptr;
	m_TradeInsertStatement = nullptr;
	m_TradeDeleteStatement = nullptr;
	m_TradeUpdateStatement = nullptr;
	m_TradeSelectStatement = nullptr;
	m_TradeTruncateStatement = nullptr;
}
~Impl()
{
	DisConnect();
}
bool Connect()
{
	int result = sqlite3_open(m_DBName.c_str(), &m_DB);
	if (result != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "Open Sqlite Failed.");
		return false;
	}
	WriteLog(LogLevel::Info, "Open Sqlite Success.");
	return true;
}
void DisConnect()
{
	if (m_DB != nullptr)
	{
		sqlite3_close(m_DB);
		m_DB = nullptr;
	}
	if (m_TradingDayInsertStatement != nullptr)
	{
		sqlite3_finalize(m_TradingDayInsertStatement);
		m_TradingDayInsertStatement = nullptr;
	}
	if (m_TradingDayDeleteStatement != nullptr)
	{
		sqlite3_finalize(m_TradingDayDeleteStatement);
		m_TradingDayDeleteStatement = nullptr;
	}
	if (m_TradingDayUpdateStatement != nullptr)
	{
		sqlite3_finalize(m_TradingDayUpdateStatement);
		m_TradingDayUpdateStatement = nullptr;
	}
	if (m_TradingDaySelectStatement != nullptr)
	{
		sqlite3_finalize(m_TradingDaySelectStatement);
		m_TradingDaySelectStatement = nullptr;
	}
	if (m_TradingDayTruncateStatement != nullptr)
	{
		sqlite3_finalize(m_TradingDayTruncateStatement);
		m_TradingDayTruncateStatement = nullptr;
	}
	if (m_ExchangeInsertStatement != nullptr)
	{
		sqlite3_finalize(m_ExchangeInsertStatement);
		m_ExchangeInsertStatement = nullptr;
	}
	if (m_ExchangeDeleteStatement != nullptr)
	{
		sqlite3_finalize(m_ExchangeDeleteStatement);
		m_ExchangeDeleteStatement = nullptr;
	}
	if (m_ExchangeUpdateStatement != nullptr)
	{
		sqlite3_finalize(m_ExchangeUpdateStatement);
		m_ExchangeUpdateStatement = nullptr;
	}
	if (m_ExchangeSelectStatement != nullptr)
	{
		sqlite3_finalize(m_ExchangeSelectStatement);
		m_ExchangeSelectStatement = nullptr;
	}
	if (m_ExchangeTruncateStatement != nullptr)
	{
		sqlite3_finalize(m_ExchangeTruncateStatement);
		m_ExchangeTruncateStatement = nullptr;
	}
	if (m_ProductInsertStatement != nullptr)
	{
		sqlite3_finalize(m_ProductInsertStatement);
		m_ProductInsertStatement = nullptr;
	}
	if (m_ProductDeleteStatement != nullptr)
	{
		sqlite3_finalize(m_ProductDeleteStatement);
		m_ProductDeleteStatement = nullptr;
	}
	if (m_ProductUpdateStatement != nullptr)
	{
		sqlite3_finalize(m_ProductUpdateStatement);
		m_ProductUpdateStatement = nullptr;
	}
	if (m_ProductSelectStatement != nullptr)
	{
		sqlite3_finalize(m_ProductSelectStatement);
		m_ProductSelectStatement = nullptr;
	}
	if (m_ProductTruncateStatement != nullptr)
	{
		sqlite3_finalize(m_ProductTruncateStatement);
		m_ProductTruncateStatement = nullptr;
	}
	if (m_InstrumentInsertStatement != nullptr)
	{
		sqlite3_finalize(m_InstrumentInsertStatement);
		m_InstrumentInsertStatement = nullptr;
	}
	if (m_InstrumentDeleteStatement != nullptr)
	{
		sqlite3_finalize(m_InstrumentDeleteStatement);
		m_InstrumentDeleteStatement = nullptr;
	}
	if (m_InstrumentUpdateStatement != nullptr)
	{
		sqlite3_finalize(m_InstrumentUpdateStatement);
		m_InstrumentUpdateStatement = nullptr;
	}
	if (m_InstrumentSelectStatement != nullptr)
	{
		sqlite3_finalize(m_InstrumentSelectStatement);
		m_InstrumentSelectStatement = nullptr;
	}
	if (m_InstrumentTruncateStatement != nullptr)
	{
		sqlite3_finalize(m_InstrumentTruncateStatement);
		m_InstrumentTruncateStatement = nullptr;
	}
	if (m_PrimaryAccountInsertStatement != nullptr)
	{
		sqlite3_finalize(m_PrimaryAccountInsertStatement);
		m_PrimaryAccountInsertStatement = nullptr;
	}
	if (m_PrimaryAccountDeleteStatement != nullptr)
	{
		sqlite3_finalize(m_PrimaryAccountDeleteStatement);
		m_PrimaryAccountDeleteStatement = nullptr;
	}
	if (m_PrimaryAccountDeleteByOfferIDIndexStatement != nullptr)
	{
		sqlite3_finalize(m_PrimaryAccountDeleteByOfferIDIndexStatement);
		m_PrimaryAccountDeleteByOfferIDIndexStatement = nullptr;
	}
	if (m_PrimaryAccountUpdateStatement != nullptr)
	{
		sqlite3_finalize(m_PrimaryAccountUpdateStatement);
		m_PrimaryAccountUpdateStatement = nullptr;
	}
	if (m_PrimaryAccountSelectStatement != nullptr)
	{
		sqlite3_finalize(m_PrimaryAccountSelectStatement);
		m_PrimaryAccountSelectStatement = nullptr;
	}
	if (m_PrimaryAccountTruncateStatement != nullptr)
	{
		sqlite3_finalize(m_PrimaryAccountTruncateStatement);
		m_PrimaryAccountTruncateStatement = nullptr;
	}
	if (m_AccountInsertStatement != nullptr)
	{
		sqlite3_finalize(m_AccountInsertStatement);
		m_AccountInsertStatement = nullptr;
	}
	if (m_AccountDeleteStatement != nullptr)
	{
		sqlite3_finalize(m_AccountDeleteStatement);
		m_AccountDeleteStatement = nullptr;
	}
	if (m_AccountUpdateStatement != nullptr)
	{
		sqlite3_finalize(m_AccountUpdateStatement);
		m_AccountUpdateStatement = nullptr;
	}
	if (m_AccountSelectStatement != nullptr)
	{
		sqlite3_finalize(m_AccountSelectStatement);
		m_AccountSelectStatement = nullptr;
	}
	if (m_AccountTruncateStatement != nullptr)
	{
		sqlite3_finalize(m_AccountTruncateStatement);
		m_AccountTruncateStatement = nullptr;
	}
	if (m_CapitalInsertStatement != nullptr)
	{
		sqlite3_finalize(m_CapitalInsertStatement);
		m_CapitalInsertStatement = nullptr;
	}
	if (m_CapitalDeleteStatement != nullptr)
	{
		sqlite3_finalize(m_CapitalDeleteStatement);
		m_CapitalDeleteStatement = nullptr;
	}
	if (m_CapitalDeleteByTradingDayIndexStatement != nullptr)
	{
		sqlite3_finalize(m_CapitalDeleteByTradingDayIndexStatement);
		m_CapitalDeleteByTradingDayIndexStatement = nullptr;
	}
	if (m_CapitalUpdateStatement != nullptr)
	{
		sqlite3_finalize(m_CapitalUpdateStatement);
		m_CapitalUpdateStatement = nullptr;
	}
	if (m_CapitalSelectStatement != nullptr)
	{
		sqlite3_finalize(m_CapitalSelectStatement);
		m_CapitalSelectStatement = nullptr;
	}
	if (m_CapitalTruncateStatement != nullptr)
	{
		sqlite3_finalize(m_CapitalTruncateStatement);
		m_CapitalTruncateStatement = nullptr;
	}
	if (m_PositionInsertStatement != nullptr)
	{
		sqlite3_finalize(m_PositionInsertStatement);
		m_PositionInsertStatement = nullptr;
	}
	if (m_PositionDeleteStatement != nullptr)
	{
		sqlite3_finalize(m_PositionDeleteStatement);
		m_PositionDeleteStatement = nullptr;
	}
	if (m_PositionDeleteByAccountIndexStatement != nullptr)
	{
		sqlite3_finalize(m_PositionDeleteByAccountIndexStatement);
		m_PositionDeleteByAccountIndexStatement = nullptr;
	}
	if (m_PositionDeleteByTradingDayIndexStatement != nullptr)
	{
		sqlite3_finalize(m_PositionDeleteByTradingDayIndexStatement);
		m_PositionDeleteByTradingDayIndexStatement = nullptr;
	}
	if (m_PositionUpdateStatement != nullptr)
	{
		sqlite3_finalize(m_PositionUpdateStatement);
		m_PositionUpdateStatement = nullptr;
	}
	if (m_PositionSelectStatement != nullptr)
	{
		sqlite3_finalize(m_PositionSelectStatement);
		m_PositionSelectStatement = nullptr;
	}
	if (m_PositionTruncateStatement != nullptr)
	{
		sqlite3_finalize(m_PositionTruncateStatement);
		m_PositionTruncateStatement = nullptr;
	}
	if (m_PositionDetailInsertStatement != nullptr)
	{
		sqlite3_finalize(m_PositionDetailInsertStatement);
		m_PositionDetailInsertStatement = nullptr;
	}
	if (m_PositionDetailDeleteStatement != nullptr)
	{
		sqlite3_finalize(m_PositionDetailDeleteStatement);
		m_PositionDetailDeleteStatement = nullptr;
	}
	if (m_PositionDetailDeleteByTradeMatchIndexStatement != nullptr)
	{
		sqlite3_finalize(m_PositionDetailDeleteByTradeMatchIndexStatement);
		m_PositionDetailDeleteByTradeMatchIndexStatement = nullptr;
	}
	if (m_PositionDetailDeleteByTradingDayIndexStatement != nullptr)
	{
		sqlite3_finalize(m_PositionDetailDeleteByTradingDayIndexStatement);
		m_PositionDetailDeleteByTradingDayIndexStatement = nullptr;
	}
	if (m_PositionDetailUpdateStatement != nullptr)
	{
		sqlite3_finalize(m_PositionDetailUpdateStatement);
		m_PositionDetailUpdateStatement = nullptr;
	}
	if (m_PositionDetailSelectStatement != nullptr)
	{
		sqlite3_finalize(m_PositionDetailSelectStatement);
		m_PositionDetailSelectStatement = nullptr;
	}
	if (m_PositionDetailTruncateStatement != nullptr)
	{
		sqlite3_finalize(m_PositionDetailTruncateStatement);
		m_PositionDetailTruncateStatement = nullptr;
	}
	if (m_OrderInsertStatement != nullptr)
	{
		sqlite3_finalize(m_OrderInsertStatement);
		m_OrderInsertStatement = nullptr;
	}
	if (m_OrderDeleteStatement != nullptr)
	{
		sqlite3_finalize(m_OrderDeleteStatement);
		m_OrderDeleteStatement = nullptr;
	}
	if (m_OrderUpdateStatement != nullptr)
	{
		sqlite3_finalize(m_OrderUpdateStatement);
		m_OrderUpdateStatement = nullptr;
	}
	if (m_OrderSelectStatement != nullptr)
	{
		sqlite3_finalize(m_OrderSelectStatement);
		m_OrderSelectStatement = nullptr;
	}
	if (m_OrderTruncateStatement != nullptr)
	{
		sqlite3_finalize(m_OrderTruncateStatement);
		m_OrderTruncateStatement = nullptr;
	}
	if (m_TradeInsertStatement != nullptr)
	{
		sqlite3_finalize(m_TradeInsertStatement);
		m_TradeInsertStatement = nullptr;
	}
	if (m_TradeDeleteStatement != nullptr)
	{
		sqlite3_finalize(m_TradeDeleteStatement);
		m_TradeDeleteStatement = nullptr;
	}
	if (m_TradeUpdateStatement != nullptr)
	{
		sqlite3_finalize(m_TradeUpdateStatement);
		m_TradeUpdateStatement = nullptr;
	}
	if (m_TradeSelectStatement != nullptr)
	{
		sqlite3_finalize(m_TradeSelectStatement);
		m_TradeSelectStatement = nullptr;
	}
	if (m_TradeTruncateStatement != nullptr)
	{
		sqlite3_finalize(m_TradeTruncateStatement);
		m_TradeTruncateStatement = nullptr;
	}
}
void Exec(const char* sql)
{
	char* t_ErrorMsg;
	auto ret = sqlite3_exec(m_DB, sql, nullptr, nullptr, &t_ErrorMsg);
	if (ret != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "Sqlitedb::Exec Failed. Sql:%s, Error: %s\n", sql, t_ErrorMsg);
		sqlite3_free(t_ErrorMsg);
	}
}


void CreateTradingDay()
{
	auto start = steady_clock::now();
	char* t_ErrorMsg;
	const char* sql = "CREATE TABLE IF NOT EXISTS t_TradingDay(`PK` int, `CurrTradingDay` text, `PreTradingDay` text, PRIMARY KEY(PK));";
	auto rc = sqlite3_exec(m_DB, sql, nullptr, nullptr, &t_ErrorMsg);
	if (rc != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "CreateTradingDay failed, ErrorMsg:%s", t_ErrorMsg);
		sqlite3_free(t_ErrorMsg);
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateTradingDay Spend:%lldms, sql:%s", duration, sql);
}
void DropTradingDay()
{
	auto start = steady_clock::now();
	char* t_ErrorMsg;
	const char* sql = "DROP TABLE IF EXISTS t_TradingDay;";
	auto rc = sqlite3_exec(m_DB, sql, nullptr, nullptr, &t_ErrorMsg);
	if (rc != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "DropTradingDay failed, ErrorMsg:%s", t_ErrorMsg);
		sqlite3_free(t_ErrorMsg);
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropTradingDay Spend:%lldms, sql:%s", duration, sql);
}
void InsertTradingDay(TradingDay* record)
{
	auto start = steady_clock::now();
	if (m_TradingDayInsertStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "insert into t_TradingDay Values(?, ?, ?);", -1, &m_TradingDayInsertStatement, nullptr);
	}
	SetStatementForTradingDayRecord(m_TradingDayInsertStatement, record);
	
	auto rc = sqlite3_step(m_TradingDayInsertStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "InsertTradingDay failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_TradingDayInsertStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertTradingDay Spend:%lldms", duration);
	}
}
void BatchInsertTradingDay(std::list<TradingDay*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_TradingDay Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	char* t_ErrorMsg;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > BuffSize - 1024)
		{
			m_SqlBuff[n - 1] = ';';
			auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
			if (ret != SQLITE_OK)
			{
				WriteLog(LogLevel::Warning, "BatchInsertTradingDay Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
				sqlite3_free(t_ErrorMsg);
				return;
			}
			
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_TradingDay Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';

	auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
	if (ret != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "BatchInsertTradingDay Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
		sqlite3_free(t_ErrorMsg);
		return;
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertTradingDay RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void DeleteTradingDay(TradingDay* record)
{
	auto start = steady_clock::now();
	if (m_TradingDayDeleteStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_TradingDay where PK = ?;", -1, &m_TradingDayDeleteStatement, nullptr);
	}
	SetStatementForTradingDayPrimaryKey(m_TradingDayDeleteStatement, record->PK);

	auto rc = sqlite3_step(m_TradingDayDeleteStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeleteTradingDay failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_TradingDayDeleteStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteTradingDay Spend:%lldms", duration);
	}
}
void UpdateTradingDay(TradingDay* record)
{
	auto start = steady_clock::now();
	if (m_TradingDayUpdateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "update t_TradingDay set CurrTradingDay = ?, PreTradingDay = ? where PK = ?;", -1, &m_TradingDayUpdateStatement, nullptr);
	}
	SetStatementForTradingDayRecordUpdate(m_TradingDayUpdateStatement, record);
	
	auto rc = sqlite3_step(m_TradingDayUpdateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "UpdateTradingDay failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_TradingDayUpdateStatement);
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateTradingDay Spend:%lldms", duration);
	}
}
void SelectTradingDay(std::list<TradingDay*>& records)
{
	auto start = steady_clock::now();
	if (m_TradingDaySelectStatement == nullptr)
	{
		if (sqlite3_prepare_v2(m_DB, "select * from t_TradingDay;", -1, &m_TradingDaySelectStatement, nullptr) != SQLITE_OK)
		{
			WriteLog(LogLevel::Warning, "SelectTradingDay prepare failed: %s", sqlite3_errmsg(m_DB));
			return;
		}
	}
	while (sqlite3_step(m_TradingDaySelectStatement) == SQLITE_ROW)
	{
		ParseRecord(m_TradingDaySelectStatement, records);
	}
	sqlite3_reset(m_TradingDaySelectStatement);
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectTradingDay Spend:%lldms", duration);
	}
}
void SelectTradingDayWithSql(const char* sql, std::list<TradingDay*>& records)
{
	auto start = steady_clock::now();
	sqlite3_stmt* stmt = nullptr;
	if (sqlite3_prepare_v2(m_DB, sql, -1, &stmt, nullptr) != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "SelectTradingDayWithSql prepare failed: %s", sqlite3_errmsg(m_DB));
		return;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		ParseRecord(stmt, records);
	}
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectTradingDayWithSql Spend:%lldms, sql:%s", duration, sql);
	}
}
void TruncateTradingDay()
{
	auto start = steady_clock::now();
	if (m_TradingDayTruncateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_TradingDay;", -1, &m_TradingDayTruncateStatement, nullptr);
	}

	auto rc = sqlite3_step(m_TradingDayTruncateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "TruncateTradingDay failed, ErrorMsg:%s", sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_TradingDayTruncateStatement);
	
	WriteLog(LogLevel::Info, "TruncateTradingDay Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void CreateExchange()
{
	auto start = steady_clock::now();
	char* t_ErrorMsg;
	const char* sql = "CREATE TABLE IF NOT EXISTS t_Exchange(`ExchangeID` text, `ExchangeName` text, PRIMARY KEY(ExchangeID));";
	auto rc = sqlite3_exec(m_DB, sql, nullptr, nullptr, &t_ErrorMsg);
	if (rc != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "CreateExchange failed, ErrorMsg:%s", t_ErrorMsg);
		sqlite3_free(t_ErrorMsg);
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateExchange Spend:%lldms, sql:%s", duration, sql);
}
void DropExchange()
{
	auto start = steady_clock::now();
	char* t_ErrorMsg;
	const char* sql = "DROP TABLE IF EXISTS t_Exchange;";
	auto rc = sqlite3_exec(m_DB, sql, nullptr, nullptr, &t_ErrorMsg);
	if (rc != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "DropExchange failed, ErrorMsg:%s", t_ErrorMsg);
		sqlite3_free(t_ErrorMsg);
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropExchange Spend:%lldms, sql:%s", duration, sql);
}
void InsertExchange(Exchange* record)
{
	auto start = steady_clock::now();
	if (m_ExchangeInsertStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "insert into t_Exchange Values(?, ?);", -1, &m_ExchangeInsertStatement, nullptr);
	}
	SetStatementForExchangeRecord(m_ExchangeInsertStatement, record);
	
	auto rc = sqlite3_step(m_ExchangeInsertStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "InsertExchange failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_ExchangeInsertStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertExchange Spend:%lldms", duration);
	}
}
void BatchInsertExchange(std::list<Exchange*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_Exchange Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	char* t_ErrorMsg;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > BuffSize - 1024)
		{
			m_SqlBuff[n - 1] = ';';
			auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
			if (ret != SQLITE_OK)
			{
				WriteLog(LogLevel::Warning, "BatchInsertExchange Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
				sqlite3_free(t_ErrorMsg);
				return;
			}
			
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_Exchange Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';

	auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
	if (ret != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "BatchInsertExchange Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
		sqlite3_free(t_ErrorMsg);
		return;
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertExchange RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void DeleteExchange(Exchange* record)
{
	auto start = steady_clock::now();
	if (m_ExchangeDeleteStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Exchange where ExchangeID = ?;", -1, &m_ExchangeDeleteStatement, nullptr);
	}
	SetStatementForExchangePrimaryKey(m_ExchangeDeleteStatement, record->ExchangeID);

	auto rc = sqlite3_step(m_ExchangeDeleteStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeleteExchange failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_ExchangeDeleteStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteExchange Spend:%lldms", duration);
	}
}
void UpdateExchange(Exchange* record)
{
	auto start = steady_clock::now();
	if (m_ExchangeUpdateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "update t_Exchange set ExchangeName = ? where ExchangeID = ?;", -1, &m_ExchangeUpdateStatement, nullptr);
	}
	SetStatementForExchangeRecordUpdate(m_ExchangeUpdateStatement, record);
	
	auto rc = sqlite3_step(m_ExchangeUpdateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "UpdateExchange failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_ExchangeUpdateStatement);
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateExchange Spend:%lldms", duration);
	}
}
void SelectExchange(std::list<Exchange*>& records)
{
	auto start = steady_clock::now();
	if (m_ExchangeSelectStatement == nullptr)
	{
		if (sqlite3_prepare_v2(m_DB, "select * from t_Exchange;", -1, &m_ExchangeSelectStatement, nullptr) != SQLITE_OK)
		{
			WriteLog(LogLevel::Warning, "SelectExchange prepare failed: %s", sqlite3_errmsg(m_DB));
			return;
		}
	}
	while (sqlite3_step(m_ExchangeSelectStatement) == SQLITE_ROW)
	{
		ParseRecord(m_ExchangeSelectStatement, records);
	}
	sqlite3_reset(m_ExchangeSelectStatement);
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectExchange Spend:%lldms", duration);
	}
}
void SelectExchangeWithSql(const char* sql, std::list<Exchange*>& records)
{
	auto start = steady_clock::now();
	sqlite3_stmt* stmt = nullptr;
	if (sqlite3_prepare_v2(m_DB, sql, -1, &stmt, nullptr) != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "SelectExchangeWithSql prepare failed: %s", sqlite3_errmsg(m_DB));
		return;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		ParseRecord(stmt, records);
	}
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectExchangeWithSql Spend:%lldms, sql:%s", duration, sql);
	}
}
void TruncateExchange()
{
	auto start = steady_clock::now();
	if (m_ExchangeTruncateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Exchange;", -1, &m_ExchangeTruncateStatement, nullptr);
	}

	auto rc = sqlite3_step(m_ExchangeTruncateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "TruncateExchange failed, ErrorMsg:%s", sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_ExchangeTruncateStatement);
	
	WriteLog(LogLevel::Info, "TruncateExchange Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void CreateProduct()
{
	auto start = steady_clock::now();
	char* t_ErrorMsg;
	const char* sql = "CREATE TABLE IF NOT EXISTS t_Product(`ExchangeID` text, `ProductID` text, `ProductName` text, `ProductClass` int, `VolumeMultiple` int, `PriceTick` double, `MaxMarketOrderVolume` bigint, `MinMarketOrderVolume` bigint, `MaxLimitOrderVolume` bigint, `MinLimitOrderVolume` bigint, `SessionName` text, PRIMARY KEY(ExchangeID, ProductID));";
	auto rc = sqlite3_exec(m_DB, sql, nullptr, nullptr, &t_ErrorMsg);
	if (rc != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "CreateProduct failed, ErrorMsg:%s", t_ErrorMsg);
		sqlite3_free(t_ErrorMsg);
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateProduct Spend:%lldms, sql:%s", duration, sql);
}
void DropProduct()
{
	auto start = steady_clock::now();
	char* t_ErrorMsg;
	const char* sql = "DROP TABLE IF EXISTS t_Product;";
	auto rc = sqlite3_exec(m_DB, sql, nullptr, nullptr, &t_ErrorMsg);
	if (rc != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "DropProduct failed, ErrorMsg:%s", t_ErrorMsg);
		sqlite3_free(t_ErrorMsg);
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropProduct Spend:%lldms, sql:%s", duration, sql);
}
void InsertProduct(Product* record)
{
	auto start = steady_clock::now();
	if (m_ProductInsertStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "insert into t_Product Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);", -1, &m_ProductInsertStatement, nullptr);
	}
	SetStatementForProductRecord(m_ProductInsertStatement, record);
	
	auto rc = sqlite3_step(m_ProductInsertStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "InsertProduct failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_ProductInsertStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertProduct Spend:%lldms", duration);
	}
}
void BatchInsertProduct(std::list<Product*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_Product Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	char* t_ErrorMsg;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > BuffSize - 1024)
		{
			m_SqlBuff[n - 1] = ';';
			auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
			if (ret != SQLITE_OK)
			{
				WriteLog(LogLevel::Warning, "BatchInsertProduct Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
				sqlite3_free(t_ErrorMsg);
				return;
			}
			
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_Product Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';

	auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
	if (ret != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "BatchInsertProduct Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
		sqlite3_free(t_ErrorMsg);
		return;
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertProduct RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void DeleteProduct(Product* record)
{
	auto start = steady_clock::now();
	if (m_ProductDeleteStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Product where ExchangeID = ? and ProductID = ?;", -1, &m_ProductDeleteStatement, nullptr);
	}
	SetStatementForProductPrimaryKey(m_ProductDeleteStatement, record->ExchangeID, record->ProductID);

	auto rc = sqlite3_step(m_ProductDeleteStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeleteProduct failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_ProductDeleteStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteProduct Spend:%lldms", duration);
	}
}
void UpdateProduct(Product* record)
{
	auto start = steady_clock::now();
	if (m_ProductUpdateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "update t_Product set ProductName = ?, ProductClass = ?, VolumeMultiple = ?, PriceTick = ?, MaxMarketOrderVolume = ?, MinMarketOrderVolume = ?, MaxLimitOrderVolume = ?, MinLimitOrderVolume = ?, SessionName = ? where ExchangeID = ? and ProductID = ?;", -1, &m_ProductUpdateStatement, nullptr);
	}
	SetStatementForProductRecordUpdate(m_ProductUpdateStatement, record);
	
	auto rc = sqlite3_step(m_ProductUpdateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "UpdateProduct failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_ProductUpdateStatement);
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateProduct Spend:%lldms", duration);
	}
}
void SelectProduct(std::list<Product*>& records)
{
	auto start = steady_clock::now();
	if (m_ProductSelectStatement == nullptr)
	{
		if (sqlite3_prepare_v2(m_DB, "select * from t_Product;", -1, &m_ProductSelectStatement, nullptr) != SQLITE_OK)
		{
			WriteLog(LogLevel::Warning, "SelectProduct prepare failed: %s", sqlite3_errmsg(m_DB));
			return;
		}
	}
	while (sqlite3_step(m_ProductSelectStatement) == SQLITE_ROW)
	{
		ParseRecord(m_ProductSelectStatement, records);
	}
	sqlite3_reset(m_ProductSelectStatement);
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectProduct Spend:%lldms", duration);
	}
}
void SelectProductWithSql(const char* sql, std::list<Product*>& records)
{
	auto start = steady_clock::now();
	sqlite3_stmt* stmt = nullptr;
	if (sqlite3_prepare_v2(m_DB, sql, -1, &stmt, nullptr) != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "SelectProductWithSql prepare failed: %s", sqlite3_errmsg(m_DB));
		return;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		ParseRecord(stmt, records);
	}
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectProductWithSql Spend:%lldms, sql:%s", duration, sql);
	}
}
void TruncateProduct()
{
	auto start = steady_clock::now();
	if (m_ProductTruncateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Product;", -1, &m_ProductTruncateStatement, nullptr);
	}

	auto rc = sqlite3_step(m_ProductTruncateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "TruncateProduct failed, ErrorMsg:%s", sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_ProductTruncateStatement);
	
	WriteLog(LogLevel::Info, "TruncateProduct Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void CreateInstrument()
{
	auto start = steady_clock::now();
	char* t_ErrorMsg;
	const char* sql = "CREATE TABLE IF NOT EXISTS t_Instrument(`ExchangeID` text, `InstrumentID` text, `ExchangeInstID` text, `InstrumentName` text, `ProductID` text, `ProductClass` int, `InstrumentClass` int, `Rank` int, `VolumeMultiple` int, `PriceTick` double, `MaxMarketOrderVolume` bigint, `MinMarketOrderVolume` bigint, `MaxLimitOrderVolume` bigint, `MinLimitOrderVolume` bigint, `SessionName` text, PRIMARY KEY(ExchangeID, InstrumentID));";
	auto rc = sqlite3_exec(m_DB, sql, nullptr, nullptr, &t_ErrorMsg);
	if (rc != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "CreateInstrument failed, ErrorMsg:%s", t_ErrorMsg);
		sqlite3_free(t_ErrorMsg);
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateInstrument Spend:%lldms, sql:%s", duration, sql);
}
void DropInstrument()
{
	auto start = steady_clock::now();
	char* t_ErrorMsg;
	const char* sql = "DROP TABLE IF EXISTS t_Instrument;";
	auto rc = sqlite3_exec(m_DB, sql, nullptr, nullptr, &t_ErrorMsg);
	if (rc != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "DropInstrument failed, ErrorMsg:%s", t_ErrorMsg);
		sqlite3_free(t_ErrorMsg);
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropInstrument Spend:%lldms, sql:%s", duration, sql);
}
void InsertInstrument(Instrument* record)
{
	auto start = steady_clock::now();
	if (m_InstrumentInsertStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "insert into t_Instrument Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);", -1, &m_InstrumentInsertStatement, nullptr);
	}
	SetStatementForInstrumentRecord(m_InstrumentInsertStatement, record);
	
	auto rc = sqlite3_step(m_InstrumentInsertStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "InsertInstrument failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_InstrumentInsertStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertInstrument Spend:%lldms", duration);
	}
}
void BatchInsertInstrument(std::list<Instrument*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_Instrument Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	char* t_ErrorMsg;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > BuffSize - 1024)
		{
			m_SqlBuff[n - 1] = ';';
			auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
			if (ret != SQLITE_OK)
			{
				WriteLog(LogLevel::Warning, "BatchInsertInstrument Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
				sqlite3_free(t_ErrorMsg);
				return;
			}
			
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_Instrument Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';

	auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
	if (ret != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "BatchInsertInstrument Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
		sqlite3_free(t_ErrorMsg);
		return;
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertInstrument RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void DeleteInstrument(Instrument* record)
{
	auto start = steady_clock::now();
	if (m_InstrumentDeleteStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Instrument where ExchangeID = ? and InstrumentID = ?;", -1, &m_InstrumentDeleteStatement, nullptr);
	}
	SetStatementForInstrumentPrimaryKey(m_InstrumentDeleteStatement, record->ExchangeID, record->InstrumentID);

	auto rc = sqlite3_step(m_InstrumentDeleteStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeleteInstrument failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_InstrumentDeleteStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteInstrument Spend:%lldms", duration);
	}
}
void UpdateInstrument(Instrument* record)
{
	auto start = steady_clock::now();
	if (m_InstrumentUpdateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "update t_Instrument set ExchangeInstID = ?, InstrumentName = ?, ProductID = ?, ProductClass = ?, InstrumentClass = ?, Rank = ?, VolumeMultiple = ?, PriceTick = ?, MaxMarketOrderVolume = ?, MinMarketOrderVolume = ?, MaxLimitOrderVolume = ?, MinLimitOrderVolume = ?, SessionName = ? where ExchangeID = ? and InstrumentID = ?;", -1, &m_InstrumentUpdateStatement, nullptr);
	}
	SetStatementForInstrumentRecordUpdate(m_InstrumentUpdateStatement, record);
	
	auto rc = sqlite3_step(m_InstrumentUpdateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "UpdateInstrument failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_InstrumentUpdateStatement);
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateInstrument Spend:%lldms", duration);
	}
}
void SelectInstrument(std::list<Instrument*>& records)
{
	auto start = steady_clock::now();
	if (m_InstrumentSelectStatement == nullptr)
	{
		if (sqlite3_prepare_v2(m_DB, "select * from t_Instrument;", -1, &m_InstrumentSelectStatement, nullptr) != SQLITE_OK)
		{
			WriteLog(LogLevel::Warning, "SelectInstrument prepare failed: %s", sqlite3_errmsg(m_DB));
			return;
		}
	}
	while (sqlite3_step(m_InstrumentSelectStatement) == SQLITE_ROW)
	{
		ParseRecord(m_InstrumentSelectStatement, records);
	}
	sqlite3_reset(m_InstrumentSelectStatement);
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectInstrument Spend:%lldms", duration);
	}
}
void SelectInstrumentWithSql(const char* sql, std::list<Instrument*>& records)
{
	auto start = steady_clock::now();
	sqlite3_stmt* stmt = nullptr;
	if (sqlite3_prepare_v2(m_DB, sql, -1, &stmt, nullptr) != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "SelectInstrumentWithSql prepare failed: %s", sqlite3_errmsg(m_DB));
		return;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		ParseRecord(stmt, records);
	}
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectInstrumentWithSql Spend:%lldms, sql:%s", duration, sql);
	}
}
void TruncateInstrument()
{
	auto start = steady_clock::now();
	if (m_InstrumentTruncateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Instrument;", -1, &m_InstrumentTruncateStatement, nullptr);
	}

	auto rc = sqlite3_step(m_InstrumentTruncateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "TruncateInstrument failed, ErrorMsg:%s", sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_InstrumentTruncateStatement);
	
	WriteLog(LogLevel::Info, "TruncateInstrument Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void CreatePrimaryAccount()
{
	auto start = steady_clock::now();
	char* t_ErrorMsg;
	const char* sql = "CREATE TABLE IF NOT EXISTS t_PrimaryAccount(`PrimaryAccountID` text, `PrimaryAccountName` text, `AccountClass` int, `BrokerPassword` text, `OfferID` int, `IsAllowLogin` int, `IsSimulateAccount` int, `LoginStatus` int, `InitStatus` int, PRIMARY KEY(PrimaryAccountID));CREATE INDEX IF NOT EXISTS PrimaryAccountOfferID ON t_PrimaryAccount(OfferID);";
	auto rc = sqlite3_exec(m_DB, sql, nullptr, nullptr, &t_ErrorMsg);
	if (rc != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "CreatePrimaryAccount failed, ErrorMsg:%s", t_ErrorMsg);
		sqlite3_free(t_ErrorMsg);
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreatePrimaryAccount Spend:%lldms, sql:%s", duration, sql);
}
void DropPrimaryAccount()
{
	auto start = steady_clock::now();
	char* t_ErrorMsg;
	const char* sql = "DROP INDEX PrimaryAccountOfferID;DROP TABLE IF EXISTS t_PrimaryAccount;";
	auto rc = sqlite3_exec(m_DB, sql, nullptr, nullptr, &t_ErrorMsg);
	if (rc != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "DropPrimaryAccount failed, ErrorMsg:%s", t_ErrorMsg);
		sqlite3_free(t_ErrorMsg);
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropPrimaryAccount Spend:%lldms, sql:%s", duration, sql);
}
void InsertPrimaryAccount(PrimaryAccount* record)
{
	auto start = steady_clock::now();
	if (m_PrimaryAccountInsertStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "insert into t_PrimaryAccount Values(?, ?, ?, ?, ?, ?, ?, ?, ?);", -1, &m_PrimaryAccountInsertStatement, nullptr);
	}
	SetStatementForPrimaryAccountRecord(m_PrimaryAccountInsertStatement, record);
	
	auto rc = sqlite3_step(m_PrimaryAccountInsertStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "InsertPrimaryAccount failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_PrimaryAccountInsertStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertPrimaryAccount Spend:%lldms", duration);
	}
}
void BatchInsertPrimaryAccount(std::list<PrimaryAccount*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_PrimaryAccount Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	char* t_ErrorMsg;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > BuffSize - 1024)
		{
			m_SqlBuff[n - 1] = ';';
			auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
			if (ret != SQLITE_OK)
			{
				WriteLog(LogLevel::Warning, "BatchInsertPrimaryAccount Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
				sqlite3_free(t_ErrorMsg);
				return;
			}
			
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_PrimaryAccount Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';

	auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
	if (ret != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "BatchInsertPrimaryAccount Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
		sqlite3_free(t_ErrorMsg);
		return;
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertPrimaryAccount RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void DeletePrimaryAccount(PrimaryAccount* record)
{
	auto start = steady_clock::now();
	if (m_PrimaryAccountDeleteStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_PrimaryAccount where PrimaryAccountID = ?;", -1, &m_PrimaryAccountDeleteStatement, nullptr);
	}
	SetStatementForPrimaryAccountPrimaryKey(m_PrimaryAccountDeleteStatement, record->PrimaryAccountID);

	auto rc = sqlite3_step(m_PrimaryAccountDeleteStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeletePrimaryAccount failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_PrimaryAccountDeleteStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePrimaryAccount Spend:%lldms", duration);
	}
}
void DeletePrimaryAccountByOfferIDIndex(PrimaryAccount* record)
{
	auto start = steady_clock::now();
	if (m_PrimaryAccountDeleteByOfferIDIndexStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_PrimaryAccount where OfferID = ?;", -1, &m_PrimaryAccountDeleteByOfferIDIndexStatement, nullptr);
	}
	SetStatementForPrimaryAccountIndexOfferID(m_PrimaryAccountDeleteByOfferIDIndexStatement, record);
	
	auto rc = sqlite3_step(m_PrimaryAccountDeleteByOfferIDIndexStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeletePrimaryAccountByOfferIDIndex failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_PrimaryAccountDeleteByOfferIDIndexStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePrimaryAccountByOfferIDIndex Spend:%lldms", duration);
	}
}
void UpdatePrimaryAccount(PrimaryAccount* record)
{
	auto start = steady_clock::now();
	if (m_PrimaryAccountUpdateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "update t_PrimaryAccount set PrimaryAccountName = ?, AccountClass = ?, BrokerPassword = ?, OfferID = ?, IsAllowLogin = ?, IsSimulateAccount = ?, LoginStatus = ?, InitStatus = ? where PrimaryAccountID = ?;", -1, &m_PrimaryAccountUpdateStatement, nullptr);
	}
	SetStatementForPrimaryAccountRecordUpdate(m_PrimaryAccountUpdateStatement, record);
	
	auto rc = sqlite3_step(m_PrimaryAccountUpdateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "UpdatePrimaryAccount failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_PrimaryAccountUpdateStatement);
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdatePrimaryAccount Spend:%lldms", duration);
	}
}
void SelectPrimaryAccount(std::list<PrimaryAccount*>& records)
{
	auto start = steady_clock::now();
	if (m_PrimaryAccountSelectStatement == nullptr)
	{
		if (sqlite3_prepare_v2(m_DB, "select * from t_PrimaryAccount;", -1, &m_PrimaryAccountSelectStatement, nullptr) != SQLITE_OK)
		{
			WriteLog(LogLevel::Warning, "SelectPrimaryAccount prepare failed: %s", sqlite3_errmsg(m_DB));
			return;
		}
	}
	while (sqlite3_step(m_PrimaryAccountSelectStatement) == SQLITE_ROW)
	{
		ParseRecord(m_PrimaryAccountSelectStatement, records);
	}
	sqlite3_reset(m_PrimaryAccountSelectStatement);
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectPrimaryAccount Spend:%lldms", duration);
	}
}
void SelectPrimaryAccountWithSql(const char* sql, std::list<PrimaryAccount*>& records)
{
	auto start = steady_clock::now();
	sqlite3_stmt* stmt = nullptr;
	if (sqlite3_prepare_v2(m_DB, sql, -1, &stmt, nullptr) != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "SelectPrimaryAccountWithSql prepare failed: %s", sqlite3_errmsg(m_DB));
		return;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		ParseRecord(stmt, records);
	}
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectPrimaryAccountWithSql Spend:%lldms, sql:%s", duration, sql);
	}
}
void TruncatePrimaryAccount()
{
	auto start = steady_clock::now();
	if (m_PrimaryAccountTruncateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_PrimaryAccount;", -1, &m_PrimaryAccountTruncateStatement, nullptr);
	}

	auto rc = sqlite3_step(m_PrimaryAccountTruncateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "TruncatePrimaryAccount failed, ErrorMsg:%s", sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_PrimaryAccountTruncateStatement);
	
	WriteLog(LogLevel::Info, "TruncatePrimaryAccount Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void CreateAccount()
{
	auto start = steady_clock::now();
	char* t_ErrorMsg;
	const char* sql = "CREATE TABLE IF NOT EXISTS t_Account(`AccountID` text, `AccountName` text, `AccountType` int, `AccountStatus` int, `Password` text, `TradeGroupID` int, `RiskGroupID` int, `CommissionGroupID` int, PRIMARY KEY(AccountID));";
	auto rc = sqlite3_exec(m_DB, sql, nullptr, nullptr, &t_ErrorMsg);
	if (rc != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "CreateAccount failed, ErrorMsg:%s", t_ErrorMsg);
		sqlite3_free(t_ErrorMsg);
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateAccount Spend:%lldms, sql:%s", duration, sql);
}
void DropAccount()
{
	auto start = steady_clock::now();
	char* t_ErrorMsg;
	const char* sql = "DROP TABLE IF EXISTS t_Account;";
	auto rc = sqlite3_exec(m_DB, sql, nullptr, nullptr, &t_ErrorMsg);
	if (rc != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "DropAccount failed, ErrorMsg:%s", t_ErrorMsg);
		sqlite3_free(t_ErrorMsg);
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropAccount Spend:%lldms, sql:%s", duration, sql);
}
void InsertAccount(Account* record)
{
	auto start = steady_clock::now();
	if (m_AccountInsertStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "insert into t_Account Values(?, ?, ?, ?, ?, ?, ?, ?);", -1, &m_AccountInsertStatement, nullptr);
	}
	SetStatementForAccountRecord(m_AccountInsertStatement, record);
	
	auto rc = sqlite3_step(m_AccountInsertStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "InsertAccount failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_AccountInsertStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertAccount Spend:%lldms", duration);
	}
}
void BatchInsertAccount(std::list<Account*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_Account Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	char* t_ErrorMsg;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > BuffSize - 1024)
		{
			m_SqlBuff[n - 1] = ';';
			auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
			if (ret != SQLITE_OK)
			{
				WriteLog(LogLevel::Warning, "BatchInsertAccount Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
				sqlite3_free(t_ErrorMsg);
				return;
			}
			
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_Account Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';

	auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
	if (ret != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "BatchInsertAccount Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
		sqlite3_free(t_ErrorMsg);
		return;
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertAccount RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void DeleteAccount(Account* record)
{
	auto start = steady_clock::now();
	if (m_AccountDeleteStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Account where AccountID = ?;", -1, &m_AccountDeleteStatement, nullptr);
	}
	SetStatementForAccountPrimaryKey(m_AccountDeleteStatement, record->AccountID);

	auto rc = sqlite3_step(m_AccountDeleteStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeleteAccount failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_AccountDeleteStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteAccount Spend:%lldms", duration);
	}
}
void UpdateAccount(Account* record)
{
	auto start = steady_clock::now();
	if (m_AccountUpdateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "update t_Account set AccountName = ?, AccountType = ?, AccountStatus = ?, Password = ?, TradeGroupID = ?, RiskGroupID = ?, CommissionGroupID = ? where AccountID = ?;", -1, &m_AccountUpdateStatement, nullptr);
	}
	SetStatementForAccountRecordUpdate(m_AccountUpdateStatement, record);
	
	auto rc = sqlite3_step(m_AccountUpdateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "UpdateAccount failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_AccountUpdateStatement);
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateAccount Spend:%lldms", duration);
	}
}
void SelectAccount(std::list<Account*>& records)
{
	auto start = steady_clock::now();
	if (m_AccountSelectStatement == nullptr)
	{
		if (sqlite3_prepare_v2(m_DB, "select * from t_Account;", -1, &m_AccountSelectStatement, nullptr) != SQLITE_OK)
		{
			WriteLog(LogLevel::Warning, "SelectAccount prepare failed: %s", sqlite3_errmsg(m_DB));
			return;
		}
	}
	while (sqlite3_step(m_AccountSelectStatement) == SQLITE_ROW)
	{
		ParseRecord(m_AccountSelectStatement, records);
	}
	sqlite3_reset(m_AccountSelectStatement);
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectAccount Spend:%lldms", duration);
	}
}
void SelectAccountWithSql(const char* sql, std::list<Account*>& records)
{
	auto start = steady_clock::now();
	sqlite3_stmt* stmt = nullptr;
	if (sqlite3_prepare_v2(m_DB, sql, -1, &stmt, nullptr) != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "SelectAccountWithSql prepare failed: %s", sqlite3_errmsg(m_DB));
		return;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		ParseRecord(stmt, records);
	}
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectAccountWithSql Spend:%lldms, sql:%s", duration, sql);
	}
}
void TruncateAccount()
{
	auto start = steady_clock::now();
	if (m_AccountTruncateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Account;", -1, &m_AccountTruncateStatement, nullptr);
	}

	auto rc = sqlite3_step(m_AccountTruncateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "TruncateAccount failed, ErrorMsg:%s", sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_AccountTruncateStatement);
	
	WriteLog(LogLevel::Info, "TruncateAccount Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void CreateCapital()
{
	auto start = steady_clock::now();
	char* t_ErrorMsg;
	const char* sql = "CREATE TABLE IF NOT EXISTS t_Capital(`TradingDay` text, `AccountID` text, `AccountType` int, `Balance` double, `PreBalance` double, `Available` double, `MarketValue` double, `CashIn` double, `CashOut` double, `Margin` double, `Commission` double, `FrozenCash` double, `FrozenMargin` double, `FrozenCommission` double, `CloseProfitByDate` double, `CloseProfitByTrade` double, `PositionProfitByDate` double, `PositionProfitByTrade` double, `Deposit` double, `Withdraw` double, PRIMARY KEY(TradingDay, AccountID));CREATE INDEX IF NOT EXISTS CapitalTradingDay ON t_Capital(TradingDay);";
	auto rc = sqlite3_exec(m_DB, sql, nullptr, nullptr, &t_ErrorMsg);
	if (rc != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "CreateCapital failed, ErrorMsg:%s", t_ErrorMsg);
		sqlite3_free(t_ErrorMsg);
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateCapital Spend:%lldms, sql:%s", duration, sql);
}
void DropCapital()
{
	auto start = steady_clock::now();
	char* t_ErrorMsg;
	const char* sql = "DROP INDEX CapitalTradingDay;DROP TABLE IF EXISTS t_Capital;";
	auto rc = sqlite3_exec(m_DB, sql, nullptr, nullptr, &t_ErrorMsg);
	if (rc != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "DropCapital failed, ErrorMsg:%s", t_ErrorMsg);
		sqlite3_free(t_ErrorMsg);
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropCapital Spend:%lldms, sql:%s", duration, sql);
}
void InsertCapital(Capital* record)
{
	auto start = steady_clock::now();
	if (m_CapitalInsertStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "insert into t_Capital Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);", -1, &m_CapitalInsertStatement, nullptr);
	}
	SetStatementForCapitalRecord(m_CapitalInsertStatement, record);
	
	auto rc = sqlite3_step(m_CapitalInsertStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "InsertCapital failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_CapitalInsertStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertCapital Spend:%lldms", duration);
	}
}
void BatchInsertCapital(std::list<Capital*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_Capital Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	char* t_ErrorMsg;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > BuffSize - 1024)
		{
			m_SqlBuff[n - 1] = ';';
			auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
			if (ret != SQLITE_OK)
			{
				WriteLog(LogLevel::Warning, "BatchInsertCapital Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
				sqlite3_free(t_ErrorMsg);
				return;
			}
			
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_Capital Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';

	auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
	if (ret != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "BatchInsertCapital Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
		sqlite3_free(t_ErrorMsg);
		return;
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertCapital RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void DeleteCapital(Capital* record)
{
	auto start = steady_clock::now();
	if (m_CapitalDeleteStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Capital where TradingDay = ? and AccountID = ?;", -1, &m_CapitalDeleteStatement, nullptr);
	}
	SetStatementForCapitalPrimaryKey(m_CapitalDeleteStatement, record->TradingDay, record->AccountID);

	auto rc = sqlite3_step(m_CapitalDeleteStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeleteCapital failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_CapitalDeleteStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteCapital Spend:%lldms", duration);
	}
}
void DeleteCapitalByTradingDayIndex(Capital* record)
{
	auto start = steady_clock::now();
	if (m_CapitalDeleteByTradingDayIndexStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Capital where TradingDay = ?;", -1, &m_CapitalDeleteByTradingDayIndexStatement, nullptr);
	}
	SetStatementForCapitalIndexTradingDay(m_CapitalDeleteByTradingDayIndexStatement, record);
	
	auto rc = sqlite3_step(m_CapitalDeleteByTradingDayIndexStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeleteCapitalByTradingDayIndex failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_CapitalDeleteByTradingDayIndexStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteCapitalByTradingDayIndex Spend:%lldms", duration);
	}
}
void UpdateCapital(Capital* record)
{
	auto start = steady_clock::now();
	if (m_CapitalUpdateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "update t_Capital set AccountType = ?, Balance = ?, PreBalance = ?, Available = ?, MarketValue = ?, CashIn = ?, CashOut = ?, Margin = ?, Commission = ?, FrozenCash = ?, FrozenMargin = ?, FrozenCommission = ?, CloseProfitByDate = ?, CloseProfitByTrade = ?, PositionProfitByDate = ?, PositionProfitByTrade = ?, Deposit = ?, Withdraw = ? where TradingDay = ? and AccountID = ?;", -1, &m_CapitalUpdateStatement, nullptr);
	}
	SetStatementForCapitalRecordUpdate(m_CapitalUpdateStatement, record);
	
	auto rc = sqlite3_step(m_CapitalUpdateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "UpdateCapital failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_CapitalUpdateStatement);
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateCapital Spend:%lldms", duration);
	}
}
void SelectCapital(std::list<Capital*>& records)
{
	auto start = steady_clock::now();
	if (m_CapitalSelectStatement == nullptr)
	{
		if (sqlite3_prepare_v2(m_DB, "select * from t_Capital;", -1, &m_CapitalSelectStatement, nullptr) != SQLITE_OK)
		{
			WriteLog(LogLevel::Warning, "SelectCapital prepare failed: %s", sqlite3_errmsg(m_DB));
			return;
		}
	}
	while (sqlite3_step(m_CapitalSelectStatement) == SQLITE_ROW)
	{
		ParseRecord(m_CapitalSelectStatement, records);
	}
	sqlite3_reset(m_CapitalSelectStatement);
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectCapital Spend:%lldms", duration);
	}
}
void SelectCapitalWithSql(const char* sql, std::list<Capital*>& records)
{
	auto start = steady_clock::now();
	sqlite3_stmt* stmt = nullptr;
	if (sqlite3_prepare_v2(m_DB, sql, -1, &stmt, nullptr) != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "SelectCapitalWithSql prepare failed: %s", sqlite3_errmsg(m_DB));
		return;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		ParseRecord(stmt, records);
	}
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectCapitalWithSql Spend:%lldms, sql:%s", duration, sql);
	}
}
void TruncateCapital()
{
	auto start = steady_clock::now();
	if (m_CapitalTruncateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Capital;", -1, &m_CapitalTruncateStatement, nullptr);
	}

	auto rc = sqlite3_step(m_CapitalTruncateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "TruncateCapital failed, ErrorMsg:%s", sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_CapitalTruncateStatement);
	
	WriteLog(LogLevel::Info, "TruncateCapital Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void CreatePosition()
{
	auto start = steady_clock::now();
	char* t_ErrorMsg;
	const char* sql = "CREATE TABLE IF NOT EXISTS t_Position(`TradingDay` text, `AccountID` text, `AccountType` int, `ExchangeID` text, `InstrumentID` text, `ProductClass` int, `PosiDirection` int, `TotalPosition` bigint, `PositionFrozen` bigint, `TodayPosition` bigint, `MarketValue` double, `CashIn` double, `CashOut` double, `Margin` double, `Commission` double, `FrozenCash` double, `FrozenMargin` double, `FrozenCommission` double, `VolumeMultiple` int, `CloseProfitByDate` double, `CloseProfitByTrade` double, `PositionProfitByDate` double, `PositionProfitByTrade` double, `SettlementPrice` double, `PreSettlementPrice` double, PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection));CREATE INDEX IF NOT EXISTS PositionAccount ON t_Position(TradingDay, AccountID);CREATE INDEX IF NOT EXISTS PositionTradingDay ON t_Position(TradingDay);";
	auto rc = sqlite3_exec(m_DB, sql, nullptr, nullptr, &t_ErrorMsg);
	if (rc != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "CreatePosition failed, ErrorMsg:%s", t_ErrorMsg);
		sqlite3_free(t_ErrorMsg);
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreatePosition Spend:%lldms, sql:%s", duration, sql);
}
void DropPosition()
{
	auto start = steady_clock::now();
	char* t_ErrorMsg;
	const char* sql = "DROP INDEX PositionAccount;DROP INDEX PositionTradingDay;DROP TABLE IF EXISTS t_Position;";
	auto rc = sqlite3_exec(m_DB, sql, nullptr, nullptr, &t_ErrorMsg);
	if (rc != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "DropPosition failed, ErrorMsg:%s", t_ErrorMsg);
		sqlite3_free(t_ErrorMsg);
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropPosition Spend:%lldms, sql:%s", duration, sql);
}
void InsertPosition(Position* record)
{
	auto start = steady_clock::now();
	if (m_PositionInsertStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "insert into t_Position Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);", -1, &m_PositionInsertStatement, nullptr);
	}
	SetStatementForPositionRecord(m_PositionInsertStatement, record);
	
	auto rc = sqlite3_step(m_PositionInsertStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "InsertPosition failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_PositionInsertStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertPosition Spend:%lldms", duration);
	}
}
void BatchInsertPosition(std::list<Position*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_Position Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	char* t_ErrorMsg;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > BuffSize - 1024)
		{
			m_SqlBuff[n - 1] = ';';
			auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
			if (ret != SQLITE_OK)
			{
				WriteLog(LogLevel::Warning, "BatchInsertPosition Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
				sqlite3_free(t_ErrorMsg);
				return;
			}
			
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_Position Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';

	auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
	if (ret != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "BatchInsertPosition Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
		sqlite3_free(t_ErrorMsg);
		return;
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertPosition RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void DeletePosition(Position* record)
{
	auto start = steady_clock::now();
	if (m_PositionDeleteStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Position where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and PosiDirection = ?;", -1, &m_PositionDeleteStatement, nullptr);
	}
	SetStatementForPositionPrimaryKey(m_PositionDeleteStatement, record->TradingDay, record->AccountID, record->ExchangeID, record->InstrumentID, record->PosiDirection);

	auto rc = sqlite3_step(m_PositionDeleteStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeletePosition failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_PositionDeleteStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePosition Spend:%lldms", duration);
	}
}
void DeletePositionByAccountIndex(Position* record)
{
	auto start = steady_clock::now();
	if (m_PositionDeleteByAccountIndexStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Position where TradingDay = ? and AccountID = ?;", -1, &m_PositionDeleteByAccountIndexStatement, nullptr);
	}
	SetStatementForPositionIndexAccount(m_PositionDeleteByAccountIndexStatement, record);
	
	auto rc = sqlite3_step(m_PositionDeleteByAccountIndexStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeletePositionByAccountIndex failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_PositionDeleteByAccountIndexStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePositionByAccountIndex Spend:%lldms", duration);
	}
}
void DeletePositionByTradingDayIndex(Position* record)
{
	auto start = steady_clock::now();
	if (m_PositionDeleteByTradingDayIndexStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Position where TradingDay = ?;", -1, &m_PositionDeleteByTradingDayIndexStatement, nullptr);
	}
	SetStatementForPositionIndexTradingDay(m_PositionDeleteByTradingDayIndexStatement, record);
	
	auto rc = sqlite3_step(m_PositionDeleteByTradingDayIndexStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeletePositionByTradingDayIndex failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_PositionDeleteByTradingDayIndexStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePositionByTradingDayIndex Spend:%lldms", duration);
	}
}
void UpdatePosition(Position* record)
{
	auto start = steady_clock::now();
	if (m_PositionUpdateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "update t_Position set AccountType = ?, ProductClass = ?, TotalPosition = ?, PositionFrozen = ?, TodayPosition = ?, MarketValue = ?, CashIn = ?, CashOut = ?, Margin = ?, Commission = ?, FrozenCash = ?, FrozenMargin = ?, FrozenCommission = ?, VolumeMultiple = ?, CloseProfitByDate = ?, CloseProfitByTrade = ?, PositionProfitByDate = ?, PositionProfitByTrade = ?, SettlementPrice = ?, PreSettlementPrice = ? where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and PosiDirection = ?;", -1, &m_PositionUpdateStatement, nullptr);
	}
	SetStatementForPositionRecordUpdate(m_PositionUpdateStatement, record);
	
	auto rc = sqlite3_step(m_PositionUpdateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "UpdatePosition failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_PositionUpdateStatement);
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdatePosition Spend:%lldms", duration);
	}
}
void SelectPosition(std::list<Position*>& records)
{
	auto start = steady_clock::now();
	if (m_PositionSelectStatement == nullptr)
	{
		if (sqlite3_prepare_v2(m_DB, "select * from t_Position;", -1, &m_PositionSelectStatement, nullptr) != SQLITE_OK)
		{
			WriteLog(LogLevel::Warning, "SelectPosition prepare failed: %s", sqlite3_errmsg(m_DB));
			return;
		}
	}
	while (sqlite3_step(m_PositionSelectStatement) == SQLITE_ROW)
	{
		ParseRecord(m_PositionSelectStatement, records);
	}
	sqlite3_reset(m_PositionSelectStatement);
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectPosition Spend:%lldms", duration);
	}
}
void SelectPositionWithSql(const char* sql, std::list<Position*>& records)
{
	auto start = steady_clock::now();
	sqlite3_stmt* stmt = nullptr;
	if (sqlite3_prepare_v2(m_DB, sql, -1, &stmt, nullptr) != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "SelectPositionWithSql prepare failed: %s", sqlite3_errmsg(m_DB));
		return;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		ParseRecord(stmt, records);
	}
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectPositionWithSql Spend:%lldms, sql:%s", duration, sql);
	}
}
void TruncatePosition()
{
	auto start = steady_clock::now();
	if (m_PositionTruncateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Position;", -1, &m_PositionTruncateStatement, nullptr);
	}

	auto rc = sqlite3_step(m_PositionTruncateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "TruncatePosition failed, ErrorMsg:%s", sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_PositionTruncateStatement);
	
	WriteLog(LogLevel::Info, "TruncatePosition Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void CreatePositionDetail()
{
	auto start = steady_clock::now();
	char* t_ErrorMsg;
	const char* sql = "CREATE TABLE IF NOT EXISTS t_PositionDetail(`TradingDay` text, `AccountID` text, `AccountType` int, `ExchangeID` text, `InstrumentID` text, `ProductClass` int, `PosiDirection` int, `OpenDate` text, `TradeID` text, `Volume` bigint, `OpenPrice` double, `MarketValue` double, `CashIn` double, `CashOut` double, `Margin` double, `Commission` double, `VolumeMultiple` int, `CloseProfitByDate` double, `CloseProfitByTrade` double, `PositionProfitByDate` double, `PositionProfitByTrade` double, `SettlementPrice` double, `PreSettlementPrice` double, `CloseVolume` bigint, `CloseAmount` double, PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection, OpenDate, TradeID));CREATE INDEX IF NOT EXISTS PositionDetailTradeMatch ON t_PositionDetail(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection);CREATE INDEX IF NOT EXISTS PositionDetailTradingDay ON t_PositionDetail(TradingDay);";
	auto rc = sqlite3_exec(m_DB, sql, nullptr, nullptr, &t_ErrorMsg);
	if (rc != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "CreatePositionDetail failed, ErrorMsg:%s", t_ErrorMsg);
		sqlite3_free(t_ErrorMsg);
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreatePositionDetail Spend:%lldms, sql:%s", duration, sql);
}
void DropPositionDetail()
{
	auto start = steady_clock::now();
	char* t_ErrorMsg;
	const char* sql = "DROP INDEX PositionDetailTradeMatch;DROP INDEX PositionDetailTradingDay;DROP TABLE IF EXISTS t_PositionDetail;";
	auto rc = sqlite3_exec(m_DB, sql, nullptr, nullptr, &t_ErrorMsg);
	if (rc != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "DropPositionDetail failed, ErrorMsg:%s", t_ErrorMsg);
		sqlite3_free(t_ErrorMsg);
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropPositionDetail Spend:%lldms, sql:%s", duration, sql);
}
void InsertPositionDetail(PositionDetail* record)
{
	auto start = steady_clock::now();
	if (m_PositionDetailInsertStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "insert into t_PositionDetail Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);", -1, &m_PositionDetailInsertStatement, nullptr);
	}
	SetStatementForPositionDetailRecord(m_PositionDetailInsertStatement, record);
	
	auto rc = sqlite3_step(m_PositionDetailInsertStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "InsertPositionDetail failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_PositionDetailInsertStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertPositionDetail Spend:%lldms", duration);
	}
}
void BatchInsertPositionDetail(std::list<PositionDetail*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_PositionDetail Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	char* t_ErrorMsg;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > BuffSize - 1024)
		{
			m_SqlBuff[n - 1] = ';';
			auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
			if (ret != SQLITE_OK)
			{
				WriteLog(LogLevel::Warning, "BatchInsertPositionDetail Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
				sqlite3_free(t_ErrorMsg);
				return;
			}
			
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_PositionDetail Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';

	auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
	if (ret != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "BatchInsertPositionDetail Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
		sqlite3_free(t_ErrorMsg);
		return;
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertPositionDetail RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void DeletePositionDetail(PositionDetail* record)
{
	auto start = steady_clock::now();
	if (m_PositionDetailDeleteStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_PositionDetail where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and PosiDirection = ? and OpenDate = ? and TradeID = ?;", -1, &m_PositionDetailDeleteStatement, nullptr);
	}
	SetStatementForPositionDetailPrimaryKey(m_PositionDetailDeleteStatement, record->TradingDay, record->AccountID, record->ExchangeID, record->InstrumentID, record->PosiDirection, record->OpenDate, record->TradeID);

	auto rc = sqlite3_step(m_PositionDetailDeleteStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeletePositionDetail failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_PositionDetailDeleteStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePositionDetail Spend:%lldms", duration);
	}
}
void DeletePositionDetailByTradeMatchIndex(PositionDetail* record)
{
	auto start = steady_clock::now();
	if (m_PositionDetailDeleteByTradeMatchIndexStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_PositionDetail where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and PosiDirection = ?;", -1, &m_PositionDetailDeleteByTradeMatchIndexStatement, nullptr);
	}
	SetStatementForPositionDetailIndexTradeMatch(m_PositionDetailDeleteByTradeMatchIndexStatement, record);
	
	auto rc = sqlite3_step(m_PositionDetailDeleteByTradeMatchIndexStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeletePositionDetailByTradeMatchIndex failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_PositionDetailDeleteByTradeMatchIndexStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePositionDetailByTradeMatchIndex Spend:%lldms", duration);
	}
}
void DeletePositionDetailByTradingDayIndex(PositionDetail* record)
{
	auto start = steady_clock::now();
	if (m_PositionDetailDeleteByTradingDayIndexStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_PositionDetail where TradingDay = ?;", -1, &m_PositionDetailDeleteByTradingDayIndexStatement, nullptr);
	}
	SetStatementForPositionDetailIndexTradingDay(m_PositionDetailDeleteByTradingDayIndexStatement, record);
	
	auto rc = sqlite3_step(m_PositionDetailDeleteByTradingDayIndexStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeletePositionDetailByTradingDayIndex failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_PositionDetailDeleteByTradingDayIndexStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeletePositionDetailByTradingDayIndex Spend:%lldms", duration);
	}
}
void UpdatePositionDetail(PositionDetail* record)
{
	auto start = steady_clock::now();
	if (m_PositionDetailUpdateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "update t_PositionDetail set AccountType = ?, ProductClass = ?, Volume = ?, OpenPrice = ?, MarketValue = ?, CashIn = ?, CashOut = ?, Margin = ?, Commission = ?, VolumeMultiple = ?, CloseProfitByDate = ?, CloseProfitByTrade = ?, PositionProfitByDate = ?, PositionProfitByTrade = ?, SettlementPrice = ?, PreSettlementPrice = ?, CloseVolume = ?, CloseAmount = ? where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and PosiDirection = ? and OpenDate = ? and TradeID = ?;", -1, &m_PositionDetailUpdateStatement, nullptr);
	}
	SetStatementForPositionDetailRecordUpdate(m_PositionDetailUpdateStatement, record);
	
	auto rc = sqlite3_step(m_PositionDetailUpdateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "UpdatePositionDetail failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_PositionDetailUpdateStatement);
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdatePositionDetail Spend:%lldms", duration);
	}
}
void SelectPositionDetail(std::list<PositionDetail*>& records)
{
	auto start = steady_clock::now();
	if (m_PositionDetailSelectStatement == nullptr)
	{
		if (sqlite3_prepare_v2(m_DB, "select * from t_PositionDetail;", -1, &m_PositionDetailSelectStatement, nullptr) != SQLITE_OK)
		{
			WriteLog(LogLevel::Warning, "SelectPositionDetail prepare failed: %s", sqlite3_errmsg(m_DB));
			return;
		}
	}
	while (sqlite3_step(m_PositionDetailSelectStatement) == SQLITE_ROW)
	{
		ParseRecord(m_PositionDetailSelectStatement, records);
	}
	sqlite3_reset(m_PositionDetailSelectStatement);
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectPositionDetail Spend:%lldms", duration);
	}
}
void SelectPositionDetailWithSql(const char* sql, std::list<PositionDetail*>& records)
{
	auto start = steady_clock::now();
	sqlite3_stmt* stmt = nullptr;
	if (sqlite3_prepare_v2(m_DB, sql, -1, &stmt, nullptr) != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "SelectPositionDetailWithSql prepare failed: %s", sqlite3_errmsg(m_DB));
		return;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		ParseRecord(stmt, records);
	}
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectPositionDetailWithSql Spend:%lldms, sql:%s", duration, sql);
	}
}
void TruncatePositionDetail()
{
	auto start = steady_clock::now();
	if (m_PositionDetailTruncateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_PositionDetail;", -1, &m_PositionDetailTruncateStatement, nullptr);
	}

	auto rc = sqlite3_step(m_PositionDetailTruncateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "TruncatePositionDetail failed, ErrorMsg:%s", sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_PositionDetailTruncateStatement);
	
	WriteLog(LogLevel::Info, "TruncatePositionDetail Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void CreateOrder()
{
	auto start = steady_clock::now();
	char* t_ErrorMsg;
	const char* sql = "CREATE TABLE IF NOT EXISTS t_Order(`TradingDay` text, `AccountID` text, `AccountType` int, `ExchangeID` text, `InstrumentID` text, `ProductClass` int, `OrderID` int, `OrderSysID` text, `Direction` int, `OffsetFlag` int, `OrderPriceType` int, `Price` double, `Volume` bigint, `VolumeTotal` bigint, `VolumeTraded` bigint, `VolumeMultiple` int, `OrderStatus` int, `OrderDate` text, `OrderTime` text, `CancelDate` text, `CancelTime` text, `SessionID` bigint, `ClientOrderID` int, `RequestID` int, `OfferID` int, `TradeGroupID` int, `RiskGroupID` int, `CommissionGroupID` int, `FrozenCash` double, `FrozenMargin` double, `FrozenCommission` double, `RebuildMark` int, `IsForceClose` int, UNIQUE (TradingDay, AccountID, ExchangeID, InstrumentID, SessionID, ClientOrderID), PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, OrderID));";
	auto rc = sqlite3_exec(m_DB, sql, nullptr, nullptr, &t_ErrorMsg);
	if (rc != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "CreateOrder failed, ErrorMsg:%s", t_ErrorMsg);
		sqlite3_free(t_ErrorMsg);
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateOrder Spend:%lldms, sql:%s", duration, sql);
}
void DropOrder()
{
	auto start = steady_clock::now();
	char* t_ErrorMsg;
	const char* sql = "DROP TABLE IF EXISTS t_Order;";
	auto rc = sqlite3_exec(m_DB, sql, nullptr, nullptr, &t_ErrorMsg);
	if (rc != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "DropOrder failed, ErrorMsg:%s", t_ErrorMsg);
		sqlite3_free(t_ErrorMsg);
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropOrder Spend:%lldms, sql:%s", duration, sql);
}
void InsertOrder(Order* record)
{
	auto start = steady_clock::now();
	if (m_OrderInsertStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "insert into t_Order Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);", -1, &m_OrderInsertStatement, nullptr);
	}
	SetStatementForOrderRecord(m_OrderInsertStatement, record);
	
	auto rc = sqlite3_step(m_OrderInsertStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "InsertOrder failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_OrderInsertStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertOrder Spend:%lldms", duration);
	}
}
void BatchInsertOrder(std::list<Order*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_Order Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	char* t_ErrorMsg;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > BuffSize - 1024)
		{
			m_SqlBuff[n - 1] = ';';
			auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
			if (ret != SQLITE_OK)
			{
				WriteLog(LogLevel::Warning, "BatchInsertOrder Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
				sqlite3_free(t_ErrorMsg);
				return;
			}
			
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_Order Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';

	auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
	if (ret != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "BatchInsertOrder Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
		sqlite3_free(t_ErrorMsg);
		return;
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertOrder RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void DeleteOrder(Order* record)
{
	auto start = steady_clock::now();
	if (m_OrderDeleteStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Order where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and OrderID = ?;", -1, &m_OrderDeleteStatement, nullptr);
	}
	SetStatementForOrderPrimaryKey(m_OrderDeleteStatement, record->TradingDay, record->AccountID, record->ExchangeID, record->InstrumentID, record->OrderID);

	auto rc = sqlite3_step(m_OrderDeleteStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeleteOrder failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_OrderDeleteStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteOrder Spend:%lldms", duration);
	}
}
void UpdateOrder(Order* record)
{
	auto start = steady_clock::now();
	if (m_OrderUpdateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "update t_Order set AccountType = ?, ProductClass = ?, OrderSysID = ?, Direction = ?, OffsetFlag = ?, OrderPriceType = ?, Price = ?, Volume = ?, VolumeTotal = ?, VolumeTraded = ?, VolumeMultiple = ?, OrderStatus = ?, OrderDate = ?, OrderTime = ?, CancelDate = ?, CancelTime = ?, SessionID = ?, ClientOrderID = ?, RequestID = ?, OfferID = ?, TradeGroupID = ?, RiskGroupID = ?, CommissionGroupID = ?, FrozenCash = ?, FrozenMargin = ?, FrozenCommission = ?, RebuildMark = ?, IsForceClose = ? where TradingDay = ? and AccountID = ? and ExchangeID = ? and InstrumentID = ? and OrderID = ?;", -1, &m_OrderUpdateStatement, nullptr);
	}
	SetStatementForOrderRecordUpdate(m_OrderUpdateStatement, record);
	
	auto rc = sqlite3_step(m_OrderUpdateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "UpdateOrder failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_OrderUpdateStatement);
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateOrder Spend:%lldms", duration);
	}
}
void SelectOrder(std::list<Order*>& records)
{
	auto start = steady_clock::now();
	if (m_OrderSelectStatement == nullptr)
	{
		if (sqlite3_prepare_v2(m_DB, "select * from t_Order;", -1, &m_OrderSelectStatement, nullptr) != SQLITE_OK)
		{
			WriteLog(LogLevel::Warning, "SelectOrder prepare failed: %s", sqlite3_errmsg(m_DB));
			return;
		}
	}
	while (sqlite3_step(m_OrderSelectStatement) == SQLITE_ROW)
	{
		ParseRecord(m_OrderSelectStatement, records);
	}
	sqlite3_reset(m_OrderSelectStatement);
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectOrder Spend:%lldms", duration);
	}
}
void SelectOrderWithSql(const char* sql, std::list<Order*>& records)
{
	auto start = steady_clock::now();
	sqlite3_stmt* stmt = nullptr;
	if (sqlite3_prepare_v2(m_DB, sql, -1, &stmt, nullptr) != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "SelectOrderWithSql prepare failed: %s", sqlite3_errmsg(m_DB));
		return;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		ParseRecord(stmt, records);
	}
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectOrderWithSql Spend:%lldms, sql:%s", duration, sql);
	}
}
void TruncateOrder()
{
	auto start = steady_clock::now();
	if (m_OrderTruncateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Order;", -1, &m_OrderTruncateStatement, nullptr);
	}

	auto rc = sqlite3_step(m_OrderTruncateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "TruncateOrder failed, ErrorMsg:%s", sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_OrderTruncateStatement);
	
	WriteLog(LogLevel::Info, "TruncateOrder Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void CreateTrade()
{
	auto start = steady_clock::now();
	char* t_ErrorMsg;
	const char* sql = "CREATE TABLE IF NOT EXISTS t_Trade(`TradingDay` text, `AccountID` text, `AccountType` int, `ExchangeID` text, `InstrumentID` text, `ProductClass` int, `OrderID` int, `OrderSysID` text, `TradeID` text, `Direction` int, `OffsetFlag` int, `Price` double, `Volume` bigint, `VolumeMultiple` int, `TradeAmount` double, `Commission` double, `TradeDate` text, `TradeTime` text, PRIMARY KEY(TradingDay, ExchangeID, TradeID, Direction));";
	auto rc = sqlite3_exec(m_DB, sql, nullptr, nullptr, &t_ErrorMsg);
	if (rc != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "CreateTrade failed, ErrorMsg:%s", t_ErrorMsg);
		sqlite3_free(t_ErrorMsg);
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "CreateTrade Spend:%lldms, sql:%s", duration, sql);
}
void DropTrade()
{
	auto start = steady_clock::now();
	char* t_ErrorMsg;
	const char* sql = "DROP TABLE IF EXISTS t_Trade;";
	auto rc = sqlite3_exec(m_DB, sql, nullptr, nullptr, &t_ErrorMsg);
	if (rc != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "DropTrade failed, ErrorMsg:%s", t_ErrorMsg);
		sqlite3_free(t_ErrorMsg);
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Info, "DropTrade Spend:%lldms, sql:%s", duration, sql);
}
void InsertTrade(Trade* record)
{
	auto start = steady_clock::now();
	if (m_TradeInsertStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "insert into t_Trade Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);", -1, &m_TradeInsertStatement, nullptr);
	}
	SetStatementForTradeRecord(m_TradeInsertStatement, record);
	
	auto rc = sqlite3_step(m_TradeInsertStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "InsertTrade failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_TradeInsertStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "InsertTrade Spend:%lldms", duration);
	}
}
void BatchInsertTrade(std::list<Trade*>* records)
{
	auto start = steady_clock::now();
	memset(m_SqlBuff, 0, BuffSize);
	strcpy(m_SqlBuff, "Insert into t_Trade Values");
	int n = (int)strlen(m_SqlBuff);
	int i = 0;
	char* t_ErrorMsg;
	for (auto it = records->begin(); it != records->end(); ++it, ++i)
	{
		if (n > BuffSize - 1024)
		{
			m_SqlBuff[n - 1] = ';';
			auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
			if (ret != SQLITE_OK)
			{
				WriteLog(LogLevel::Warning, "BatchInsertTrade Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
				sqlite3_free(t_ErrorMsg);
				return;
			}
			
			memset(m_SqlBuff, 0, BuffSize);
			strcpy(m_SqlBuff, "Insert into t_Trade Values");
			n = (int)strlen(m_SqlBuff);
		}
		n += (*it)->GetSqlString(m_SqlBuff + n);
	}
	m_SqlBuff[n - 1] = ';';

	auto ret = sqlite3_exec(m_DB, m_SqlBuff, nullptr, nullptr, &t_ErrorMsg);
	if (ret != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "BatchInsertTrade Failed. Error: %s, Sql:[%s]", t_ErrorMsg, m_SqlBuff);
		sqlite3_free(t_ErrorMsg);
		return;
	}
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	WriteLog(LogLevel::Warning, "BatchInsertTrade RecordSize:%lld, Spend:%lldms", records->size(), duration);
}
void DeleteTrade(Trade* record)
{
	auto start = steady_clock::now();
	if (m_TradeDeleteStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Trade where TradingDay = ? and ExchangeID = ? and TradeID = ? and Direction = ?;", -1, &m_TradeDeleteStatement, nullptr);
	}
	SetStatementForTradePrimaryKey(m_TradeDeleteStatement, record->TradingDay, record->ExchangeID, record->TradeID, record->Direction);

	auto rc = sqlite3_step(m_TradeDeleteStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "DeleteTrade failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_TradeDeleteStatement);

	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "DeleteTrade Spend:%lldms", duration);
	}
}
void UpdateTrade(Trade* record)
{
	auto start = steady_clock::now();
	if (m_TradeUpdateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "update t_Trade set AccountID = ?, AccountType = ?, InstrumentID = ?, ProductClass = ?, OrderID = ?, OrderSysID = ?, OffsetFlag = ?, Price = ?, Volume = ?, VolumeMultiple = ?, TradeAmount = ?, Commission = ?, TradeDate = ?, TradeTime = ? where TradingDay = ? and ExchangeID = ? and TradeID = ? and Direction = ?;", -1, &m_TradeUpdateStatement, nullptr);
	}
	SetStatementForTradeRecordUpdate(m_TradeUpdateStatement, record);
	
	auto rc = sqlite3_step(m_TradeUpdateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "UpdateTrade failed: %s, ErrorMsg:%s", record->GetDebugString(), sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_TradeUpdateStatement);
	
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "UpdateTrade Spend:%lldms", duration);
	}
}
void SelectTrade(std::list<Trade*>& records)
{
	auto start = steady_clock::now();
	if (m_TradeSelectStatement == nullptr)
	{
		if (sqlite3_prepare_v2(m_DB, "select * from t_Trade;", -1, &m_TradeSelectStatement, nullptr) != SQLITE_OK)
		{
			WriteLog(LogLevel::Warning, "SelectTrade prepare failed: %s", sqlite3_errmsg(m_DB));
			return;
		}
	}
	while (sqlite3_step(m_TradeSelectStatement) == SQLITE_ROW)
	{
		ParseRecord(m_TradeSelectStatement, records);
	}
	sqlite3_reset(m_TradeSelectStatement);
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectTrade Spend:%lldms", duration);
	}
}
void SelectTradeWithSql(const char* sql, std::list<Trade*>& records)
{
	auto start = steady_clock::now();
	sqlite3_stmt* stmt = nullptr;
	if (sqlite3_prepare_v2(m_DB, sql, -1, &stmt, nullptr) != SQLITE_OK)
	{
		WriteLog(LogLevel::Warning, "SelectTradeWithSql prepare failed: %s", sqlite3_errmsg(m_DB));
		return;
	}
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		ParseRecord(stmt, records);
	}
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	auto duration = TimeUtility::GetDuration<chrono::milliseconds>(start);
	if (duration >= 100)
	{
		WriteLog(LogLevel::Warning, "SelectTradeWithSql Spend:%lldms, sql:%s", duration, sql);
	}
}
void TruncateTrade()
{
	auto start = steady_clock::now();
	if (m_TradeTruncateStatement == nullptr)
	{
		sqlite3_prepare_v2(m_DB, "delete from t_Trade;", -1, &m_TradeTruncateStatement, nullptr);
	}

	auto rc = sqlite3_step(m_TradeTruncateStatement);
	if (rc != SQLITE_DONE)
	{
		WriteLog(LogLevel::Warning, "TruncateTrade failed, ErrorMsg:%s", sqlite3_errmsg(m_DB));
	}
	sqlite3_reset(m_TradeTruncateStatement);
	
	WriteLog(LogLevel::Info, "TruncateTrade Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}


public:
	std::string m_DBName;
	sqlite3* m_DB;
	char* m_SqlBuff;

	sqlite3_stmt* m_TradingDayInsertStatement;
	sqlite3_stmt* m_TradingDayDeleteStatement;
	sqlite3_stmt* m_TradingDayUpdateStatement;
	sqlite3_stmt* m_TradingDaySelectStatement;
	sqlite3_stmt* m_TradingDayTruncateStatement;
	sqlite3_stmt* m_ExchangeInsertStatement;
	sqlite3_stmt* m_ExchangeDeleteStatement;
	sqlite3_stmt* m_ExchangeUpdateStatement;
	sqlite3_stmt* m_ExchangeSelectStatement;
	sqlite3_stmt* m_ExchangeTruncateStatement;
	sqlite3_stmt* m_ProductInsertStatement;
	sqlite3_stmt* m_ProductDeleteStatement;
	sqlite3_stmt* m_ProductUpdateStatement;
	sqlite3_stmt* m_ProductSelectStatement;
	sqlite3_stmt* m_ProductTruncateStatement;
	sqlite3_stmt* m_InstrumentInsertStatement;
	sqlite3_stmt* m_InstrumentDeleteStatement;
	sqlite3_stmt* m_InstrumentUpdateStatement;
	sqlite3_stmt* m_InstrumentSelectStatement;
	sqlite3_stmt* m_InstrumentTruncateStatement;
	sqlite3_stmt* m_PrimaryAccountInsertStatement;
	sqlite3_stmt* m_PrimaryAccountDeleteStatement;
	sqlite3_stmt* m_PrimaryAccountDeleteByOfferIDIndexStatement;
	sqlite3_stmt* m_PrimaryAccountUpdateStatement;
	sqlite3_stmt* m_PrimaryAccountSelectStatement;
	sqlite3_stmt* m_PrimaryAccountTruncateStatement;
	sqlite3_stmt* m_AccountInsertStatement;
	sqlite3_stmt* m_AccountDeleteStatement;
	sqlite3_stmt* m_AccountUpdateStatement;
	sqlite3_stmt* m_AccountSelectStatement;
	sqlite3_stmt* m_AccountTruncateStatement;
	sqlite3_stmt* m_CapitalInsertStatement;
	sqlite3_stmt* m_CapitalDeleteStatement;
	sqlite3_stmt* m_CapitalDeleteByTradingDayIndexStatement;
	sqlite3_stmt* m_CapitalUpdateStatement;
	sqlite3_stmt* m_CapitalSelectStatement;
	sqlite3_stmt* m_CapitalTruncateStatement;
	sqlite3_stmt* m_PositionInsertStatement;
	sqlite3_stmt* m_PositionDeleteStatement;
	sqlite3_stmt* m_PositionDeleteByAccountIndexStatement;
	sqlite3_stmt* m_PositionDeleteByTradingDayIndexStatement;
	sqlite3_stmt* m_PositionUpdateStatement;
	sqlite3_stmt* m_PositionSelectStatement;
	sqlite3_stmt* m_PositionTruncateStatement;
	sqlite3_stmt* m_PositionDetailInsertStatement;
	sqlite3_stmt* m_PositionDetailDeleteStatement;
	sqlite3_stmt* m_PositionDetailDeleteByTradeMatchIndexStatement;
	sqlite3_stmt* m_PositionDetailDeleteByTradingDayIndexStatement;
	sqlite3_stmt* m_PositionDetailUpdateStatement;
	sqlite3_stmt* m_PositionDetailSelectStatement;
	sqlite3_stmt* m_PositionDetailTruncateStatement;
	sqlite3_stmt* m_OrderInsertStatement;
	sqlite3_stmt* m_OrderDeleteStatement;
	sqlite3_stmt* m_OrderUpdateStatement;
	sqlite3_stmt* m_OrderSelectStatement;
	sqlite3_stmt* m_OrderTruncateStatement;
	sqlite3_stmt* m_TradeInsertStatement;
	sqlite3_stmt* m_TradeDeleteStatement;
	sqlite3_stmt* m_TradeUpdateStatement;
	sqlite3_stmt* m_TradeSelectStatement;
	sqlite3_stmt* m_TradeTruncateStatement;
};


SqliteWrapper::SqliteWrapper(const std::string& dbName)
	:m_DBName(dbName), m_Impl(std::unique_ptr<Impl>(new Impl(dbName)))
{
}
SqliteWrapper::~SqliteWrapper()
{
	DisConnect();
}
bool SqliteWrapper::Connect()
{
	return m_Impl->Connect();
}
void SqliteWrapper::DisConnect()
{
	m_Impl->DisConnect();
}
void SqliteWrapper::InitDB()
{
	Exec("Truncate Table t_TradingDay;");
	Exec("Insert Into t_TradingDay select * from Init.t_TradingDay;");
	Exec("Truncate Table t_Exchange;");
	Exec("Insert Into t_Exchange select * from Init.t_Exchange;");
	Exec("Truncate Table t_Product;");
	Exec("Insert Into t_Product select * from Init.t_Product;");
	Exec("Truncate Table t_Instrument;");
	Exec("Insert Into t_Instrument select * from Init.t_Instrument;");
	Exec("Truncate Table t_PrimaryAccount;");
	Exec("Insert Into t_PrimaryAccount select * from Init.t_PrimaryAccount;");
	Exec("Truncate Table t_Account;");
	Exec("Insert Into t_Account select * from Init.t_Account;");
	Exec("Truncate Table t_Capital;");
	Exec("Insert Into t_Capital select * from Init.t_Capital;");
	Exec("Truncate Table t_Position;");
	Exec("Insert Into t_Position select * from Init.t_Position;");
	Exec("Truncate Table t_PositionDetail;");
	Exec("Insert Into t_PositionDetail select * from Init.t_PositionDetail;");
	Exec("Truncate Table t_Order;");
	Exec("Insert Into t_Order select * from Init.t_Order;");
	Exec("Truncate Table t_Trade;");
	Exec("Insert Into t_Trade select * from Init.t_Trade;");
}
void SqliteWrapper::CreateTables()
{
	CreateTradingDay();
	CreateExchange();
	CreateProduct();
	CreateInstrument();
	CreatePrimaryAccount();
	CreateAccount();
	CreateCapital();
	CreatePosition();
	CreatePositionDetail();
	CreateOrder();
	CreateTrade();
}
void SqliteWrapper::DropTables()
{
	DropTradingDay();
	DropExchange();
	DropProduct();
	DropInstrument();
	DropPrimaryAccount();
	DropAccount();
	DropCapital();
	DropPosition();
	DropPositionDetail();
	DropOrder();
	DropTrade();
}
void SqliteWrapper::TruncateTables()
{
	TruncateTradingDay();
	TruncateExchange();
	TruncateProduct();
	TruncatePrimaryAccount();
	TruncateAccount();
	TruncateCapital();
	TruncatePosition();
	TruncatePositionDetail();
	TruncateOrder();
	TruncateTrade();
}
void SqliteWrapper::TruncateSessionTables()
{
	auto start = steady_clock::now();
	WriteLog(LogLevel::Info, "TruncateSessionTables Spend:%lldms", TimeUtility::GetDuration<chrono::milliseconds>(start));
}
void SqliteWrapper::Exec(const char* sql)
{
	m_Impl->Exec(sql);
}

void SqliteWrapper::CreateTradingDay()
{
	m_Impl->CreateTradingDay();
}
void SqliteWrapper::DropTradingDay()
{
	m_Impl->DropTradingDay();
}
void SqliteWrapper::InsertTradingDay(TradingDay* record)
{
	m_Impl->InsertTradingDay(record);
}
void SqliteWrapper::BatchInsertTradingDay(std::list<TradingDay*>* records)
{
	m_Impl->BatchInsertTradingDay(records);
}
void SqliteWrapper::DeleteTradingDay(TradingDay* record)
{
	m_Impl->DeleteTradingDay(record);
}
void SqliteWrapper::UpdateTradingDay(TradingDay* record)
{
	m_Impl->UpdateTradingDay(record);
}
void SqliteWrapper::SelectTradingDay(std::list<TradingDay*>& records)
{
	m_Impl->SelectTradingDay(records);
}
void SqliteWrapper::SelectTradingDayWithSql(const char* sql, std::list<TradingDay*>& records)
{
	m_Impl->SelectTradingDayWithSql(sql, records);
}
void SqliteWrapper::TruncateTradingDay()
{
	m_Impl->TruncateTradingDay();
}
void SqliteWrapper::CreateExchange()
{
	m_Impl->CreateExchange();
}
void SqliteWrapper::DropExchange()
{
	m_Impl->DropExchange();
}
void SqliteWrapper::InsertExchange(Exchange* record)
{
	m_Impl->InsertExchange(record);
}
void SqliteWrapper::BatchInsertExchange(std::list<Exchange*>* records)
{
	m_Impl->BatchInsertExchange(records);
}
void SqliteWrapper::DeleteExchange(Exchange* record)
{
	m_Impl->DeleteExchange(record);
}
void SqliteWrapper::UpdateExchange(Exchange* record)
{
	m_Impl->UpdateExchange(record);
}
void SqliteWrapper::SelectExchange(std::list<Exchange*>& records)
{
	m_Impl->SelectExchange(records);
}
void SqliteWrapper::SelectExchangeWithSql(const char* sql, std::list<Exchange*>& records)
{
	m_Impl->SelectExchangeWithSql(sql, records);
}
void SqliteWrapper::TruncateExchange()
{
	m_Impl->TruncateExchange();
}
void SqliteWrapper::CreateProduct()
{
	m_Impl->CreateProduct();
}
void SqliteWrapper::DropProduct()
{
	m_Impl->DropProduct();
}
void SqliteWrapper::InsertProduct(Product* record)
{
	m_Impl->InsertProduct(record);
}
void SqliteWrapper::BatchInsertProduct(std::list<Product*>* records)
{
	m_Impl->BatchInsertProduct(records);
}
void SqliteWrapper::DeleteProduct(Product* record)
{
	m_Impl->DeleteProduct(record);
}
void SqliteWrapper::UpdateProduct(Product* record)
{
	m_Impl->UpdateProduct(record);
}
void SqliteWrapper::SelectProduct(std::list<Product*>& records)
{
	m_Impl->SelectProduct(records);
}
void SqliteWrapper::SelectProductWithSql(const char* sql, std::list<Product*>& records)
{
	m_Impl->SelectProductWithSql(sql, records);
}
void SqliteWrapper::TruncateProduct()
{
	m_Impl->TruncateProduct();
}
void SqliteWrapper::CreateInstrument()
{
	m_Impl->CreateInstrument();
}
void SqliteWrapper::DropInstrument()
{
	m_Impl->DropInstrument();
}
void SqliteWrapper::InsertInstrument(Instrument* record)
{
	m_Impl->InsertInstrument(record);
}
void SqliteWrapper::BatchInsertInstrument(std::list<Instrument*>* records)
{
	m_Impl->BatchInsertInstrument(records);
}
void SqliteWrapper::DeleteInstrument(Instrument* record)
{
	m_Impl->DeleteInstrument(record);
}
void SqliteWrapper::UpdateInstrument(Instrument* record)
{
	m_Impl->UpdateInstrument(record);
}
void SqliteWrapper::SelectInstrument(std::list<Instrument*>& records)
{
	m_Impl->SelectInstrument(records);
}
void SqliteWrapper::SelectInstrumentWithSql(const char* sql, std::list<Instrument*>& records)
{
	m_Impl->SelectInstrumentWithSql(sql, records);
}
void SqliteWrapper::TruncateInstrument()
{
	m_Impl->TruncateInstrument();
}
void SqliteWrapper::CreatePrimaryAccount()
{
	m_Impl->CreatePrimaryAccount();
}
void SqliteWrapper::DropPrimaryAccount()
{
	m_Impl->DropPrimaryAccount();
}
void SqliteWrapper::InsertPrimaryAccount(PrimaryAccount* record)
{
	m_Impl->InsertPrimaryAccount(record);
}
void SqliteWrapper::BatchInsertPrimaryAccount(std::list<PrimaryAccount*>* records)
{
	m_Impl->BatchInsertPrimaryAccount(records);
}
void SqliteWrapper::DeletePrimaryAccount(PrimaryAccount* record)
{
	m_Impl->DeletePrimaryAccount(record);
}
void SqliteWrapper::DeletePrimaryAccountByOfferIDIndex(PrimaryAccount* record)
{
	m_Impl->DeletePrimaryAccountByOfferIDIndex(record);
}
void SqliteWrapper::UpdatePrimaryAccount(PrimaryAccount* record)
{
	m_Impl->UpdatePrimaryAccount(record);
}
void SqliteWrapper::SelectPrimaryAccount(std::list<PrimaryAccount*>& records)
{
	m_Impl->SelectPrimaryAccount(records);
}
void SqliteWrapper::SelectPrimaryAccountWithSql(const char* sql, std::list<PrimaryAccount*>& records)
{
	m_Impl->SelectPrimaryAccountWithSql(sql, records);
}
void SqliteWrapper::TruncatePrimaryAccount()
{
	m_Impl->TruncatePrimaryAccount();
}
void SqliteWrapper::CreateAccount()
{
	m_Impl->CreateAccount();
}
void SqliteWrapper::DropAccount()
{
	m_Impl->DropAccount();
}
void SqliteWrapper::InsertAccount(Account* record)
{
	m_Impl->InsertAccount(record);
}
void SqliteWrapper::BatchInsertAccount(std::list<Account*>* records)
{
	m_Impl->BatchInsertAccount(records);
}
void SqliteWrapper::DeleteAccount(Account* record)
{
	m_Impl->DeleteAccount(record);
}
void SqliteWrapper::UpdateAccount(Account* record)
{
	m_Impl->UpdateAccount(record);
}
void SqliteWrapper::SelectAccount(std::list<Account*>& records)
{
	m_Impl->SelectAccount(records);
}
void SqliteWrapper::SelectAccountWithSql(const char* sql, std::list<Account*>& records)
{
	m_Impl->SelectAccountWithSql(sql, records);
}
void SqliteWrapper::TruncateAccount()
{
	m_Impl->TruncateAccount();
}
void SqliteWrapper::CreateCapital()
{
	m_Impl->CreateCapital();
}
void SqliteWrapper::DropCapital()
{
	m_Impl->DropCapital();
}
void SqliteWrapper::InsertCapital(Capital* record)
{
	m_Impl->InsertCapital(record);
}
void SqliteWrapper::BatchInsertCapital(std::list<Capital*>* records)
{
	m_Impl->BatchInsertCapital(records);
}
void SqliteWrapper::DeleteCapital(Capital* record)
{
	m_Impl->DeleteCapital(record);
}
void SqliteWrapper::DeleteCapitalByTradingDayIndex(Capital* record)
{
	m_Impl->DeleteCapitalByTradingDayIndex(record);
}
void SqliteWrapper::UpdateCapital(Capital* record)
{
	m_Impl->UpdateCapital(record);
}
void SqliteWrapper::SelectCapital(std::list<Capital*>& records)
{
	m_Impl->SelectCapital(records);
}
void SqliteWrapper::SelectCapitalWithSql(const char* sql, std::list<Capital*>& records)
{
	m_Impl->SelectCapitalWithSql(sql, records);
}
void SqliteWrapper::TruncateCapital()
{
	m_Impl->TruncateCapital();
}
void SqliteWrapper::CreatePosition()
{
	m_Impl->CreatePosition();
}
void SqliteWrapper::DropPosition()
{
	m_Impl->DropPosition();
}
void SqliteWrapper::InsertPosition(Position* record)
{
	m_Impl->InsertPosition(record);
}
void SqliteWrapper::BatchInsertPosition(std::list<Position*>* records)
{
	m_Impl->BatchInsertPosition(records);
}
void SqliteWrapper::DeletePosition(Position* record)
{
	m_Impl->DeletePosition(record);
}
void SqliteWrapper::DeletePositionByAccountIndex(Position* record)
{
	m_Impl->DeletePositionByAccountIndex(record);
}
void SqliteWrapper::DeletePositionByTradingDayIndex(Position* record)
{
	m_Impl->DeletePositionByTradingDayIndex(record);
}
void SqliteWrapper::UpdatePosition(Position* record)
{
	m_Impl->UpdatePosition(record);
}
void SqliteWrapper::SelectPosition(std::list<Position*>& records)
{
	m_Impl->SelectPosition(records);
}
void SqliteWrapper::SelectPositionWithSql(const char* sql, std::list<Position*>& records)
{
	m_Impl->SelectPositionWithSql(sql, records);
}
void SqliteWrapper::TruncatePosition()
{
	m_Impl->TruncatePosition();
}
void SqliteWrapper::CreatePositionDetail()
{
	m_Impl->CreatePositionDetail();
}
void SqliteWrapper::DropPositionDetail()
{
	m_Impl->DropPositionDetail();
}
void SqliteWrapper::InsertPositionDetail(PositionDetail* record)
{
	m_Impl->InsertPositionDetail(record);
}
void SqliteWrapper::BatchInsertPositionDetail(std::list<PositionDetail*>* records)
{
	m_Impl->BatchInsertPositionDetail(records);
}
void SqliteWrapper::DeletePositionDetail(PositionDetail* record)
{
	m_Impl->DeletePositionDetail(record);
}
void SqliteWrapper::DeletePositionDetailByTradeMatchIndex(PositionDetail* record)
{
	m_Impl->DeletePositionDetailByTradeMatchIndex(record);
}
void SqliteWrapper::DeletePositionDetailByTradingDayIndex(PositionDetail* record)
{
	m_Impl->DeletePositionDetailByTradingDayIndex(record);
}
void SqliteWrapper::UpdatePositionDetail(PositionDetail* record)
{
	m_Impl->UpdatePositionDetail(record);
}
void SqliteWrapper::SelectPositionDetail(std::list<PositionDetail*>& records)
{
	m_Impl->SelectPositionDetail(records);
}
void SqliteWrapper::SelectPositionDetailWithSql(const char* sql, std::list<PositionDetail*>& records)
{
	m_Impl->SelectPositionDetailWithSql(sql, records);
}
void SqliteWrapper::TruncatePositionDetail()
{
	m_Impl->TruncatePositionDetail();
}
void SqliteWrapper::CreateOrder()
{
	m_Impl->CreateOrder();
}
void SqliteWrapper::DropOrder()
{
	m_Impl->DropOrder();
}
void SqliteWrapper::InsertOrder(Order* record)
{
	m_Impl->InsertOrder(record);
}
void SqliteWrapper::BatchInsertOrder(std::list<Order*>* records)
{
	m_Impl->BatchInsertOrder(records);
}
void SqliteWrapper::DeleteOrder(Order* record)
{
	m_Impl->DeleteOrder(record);
}
void SqliteWrapper::UpdateOrder(Order* record)
{
	m_Impl->UpdateOrder(record);
}
void SqliteWrapper::SelectOrder(std::list<Order*>& records)
{
	m_Impl->SelectOrder(records);
}
void SqliteWrapper::SelectOrderWithSql(const char* sql, std::list<Order*>& records)
{
	m_Impl->SelectOrderWithSql(sql, records);
}
void SqliteWrapper::TruncateOrder()
{
	m_Impl->TruncateOrder();
}
void SqliteWrapper::CreateTrade()
{
	m_Impl->CreateTrade();
}
void SqliteWrapper::DropTrade()
{
	m_Impl->DropTrade();
}
void SqliteWrapper::InsertTrade(Trade* record)
{
	m_Impl->InsertTrade(record);
}
void SqliteWrapper::BatchInsertTrade(std::list<Trade*>* records)
{
	m_Impl->BatchInsertTrade(records);
}
void SqliteWrapper::DeleteTrade(Trade* record)
{
	m_Impl->DeleteTrade(record);
}
void SqliteWrapper::UpdateTrade(Trade* record)
{
	m_Impl->UpdateTrade(record);
}
void SqliteWrapper::SelectTrade(std::list<Trade*>& records)
{
	m_Impl->SelectTrade(records);
}
void SqliteWrapper::SelectTradeWithSql(const char* sql, std::list<Trade*>& records)
{
	m_Impl->SelectTradeWithSql(sql, records);
}
void SqliteWrapper::TruncateTrade()
{
	m_Impl->TruncateTrade();
}

