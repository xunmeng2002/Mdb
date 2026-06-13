#pragma once

constexpr const char* CreateDuckDBTradingDayTableSqlString = "CREATE TABLE IF NOT EXISTS t_TradingDay(PK int, CurrTradingDay varchar, PreTradingDay varchar, PRIMARY KEY(PK));";
constexpr const char* TruncateDuckDBTradingDayTableSqlString = "Delete From t_TradingDay;";

constexpr const char* CreateDuckDBExchangeTableSqlString = "CREATE TABLE IF NOT EXISTS t_Exchange(ExchangeID varchar, ExchangeName varchar, PRIMARY KEY(ExchangeID));";
constexpr const char* TruncateDuckDBExchangeTableSqlString = "Delete From t_Exchange;";

constexpr const char* CreateDuckDBProductTableSqlString = "CREATE TABLE IF NOT EXISTS t_Product(ExchangeID varchar, ProductID varchar, ProductName varchar, ProductClass int, VolumeMultiple int, PriceTick double, MaxMarketOrderVolume bigint, MinMarketOrderVolume bigint, MaxLimitOrderVolume bigint, MinLimitOrderVolume bigint, SessionName varchar, PRIMARY KEY(ExchangeID, ProductID));";
constexpr const char* TruncateDuckDBProductTableSqlString = "Delete From t_Product;";

constexpr const char* CreateDuckDBInstrumentTableSqlString = "CREATE TABLE IF NOT EXISTS t_Instrument(ExchangeID varchar, InstrumentID varchar, ExchangeInstID varchar, InstrumentName varchar, ProductID varchar, ProductClass int, InstrumentClass int, Rank int, VolumeMultiple int, PriceTick double, MaxMarketOrderVolume bigint, MinMarketOrderVolume bigint, MaxLimitOrderVolume bigint, MinLimitOrderVolume bigint, SessionName varchar, PRIMARY KEY(ExchangeID, InstrumentID));";
constexpr const char* TruncateDuckDBInstrumentTableSqlString = "Delete From t_Instrument;";

constexpr const char* CreateDuckDBPrimaryAccountTableSqlString = "CREATE TABLE IF NOT EXISTS t_PrimaryAccount(PrimaryAccountID varchar, PrimaryAccountName varchar, AccountClass int, BrokerPassword varchar, OfferID int, IsAllowLogin bool, IsSimulateAccount bool, LoginStatus int, InitStatus int, PRIMARY KEY(PrimaryAccountID)); CREATE INDEX IF NOT EXISTS OfferIDIndex ON t_PrimaryAccount(OfferID);";
constexpr const char* TruncateDuckDBPrimaryAccountTableSqlString = "Delete From t_PrimaryAccount;";

constexpr const char* CreateDuckDBAccountTableSqlString = "CREATE TABLE IF NOT EXISTS t_Account(AccountID varchar, AccountName varchar, AccountType int, AccountStatus int, Password varchar, TradeGroupID int, RiskGroupID int, CommissionGroupID int, PRIMARY KEY(AccountID));";
constexpr const char* TruncateDuckDBAccountTableSqlString = "Delete From t_Account;";

constexpr const char* CreateDuckDBCapitalTableSqlString = "CREATE TABLE IF NOT EXISTS t_Capital(TradingDay varchar, AccountID varchar, AccountType int, Balance double, PreBalance double, Available double, MarketValue double, CashIn double, CashOut double, Margin double, Commission double, FrozenCash double, FrozenMargin double, FrozenCommission double, CloseProfitByDate double, CloseProfitByTrade double, PositionProfitByDate double, PositionProfitByTrade double, Deposit double, Withdraw double, PRIMARY KEY(TradingDay, AccountID)); CREATE INDEX IF NOT EXISTS TradingDayIndex ON t_Capital(TradingDay);";
constexpr const char* TruncateDuckDBCapitalTableSqlString = "Delete From t_Capital;";

constexpr const char* CreateDuckDBPositionTableSqlString = "CREATE TABLE IF NOT EXISTS t_Position(TradingDay varchar, AccountID varchar, AccountType int, ExchangeID varchar, InstrumentID varchar, ProductClass int, PosiDirection int, TotalPosition bigint, PositionFrozen bigint, TodayPosition bigint, MarketValue double, CashIn double, CashOut double, Margin double, Commission double, FrozenCash double, FrozenMargin double, FrozenCommission double, VolumeMultiple int, CloseProfitByDate double, CloseProfitByTrade double, PositionProfitByDate double, PositionProfitByTrade double, SettlementPrice double, PreSettlementPrice double, PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection)); CREATE INDEX IF NOT EXISTS AccountIndex ON t_Position(TradingDay, AccountID); CREATE INDEX IF NOT EXISTS TradingDayIndex ON t_Position(TradingDay);";
constexpr const char* TruncateDuckDBPositionTableSqlString = "Delete From t_Position;";

constexpr const char* CreateDuckDBPositionDetailTableSqlString = "CREATE TABLE IF NOT EXISTS t_PositionDetail(TradingDay varchar, AccountID varchar, AccountType int, ExchangeID varchar, InstrumentID varchar, ProductClass int, PosiDirection int, OpenDate varchar, TradeID varchar, Volume bigint, OpenPrice double, MarketValue double, CashIn double, CashOut double, Margin double, Commission double, VolumeMultiple int, CloseProfitByDate double, CloseProfitByTrade double, PositionProfitByDate double, PositionProfitByTrade double, SettlementPrice double, PreSettlementPrice double, CloseVolume bigint, CloseAmount double, PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection, OpenDate, TradeID)); CREATE INDEX IF NOT EXISTS TradeMatchIndex ON t_PositionDetail(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection); CREATE INDEX IF NOT EXISTS TradingDayIndex ON t_PositionDetail(TradingDay);";
constexpr const char* TruncateDuckDBPositionDetailTableSqlString = "Delete From t_PositionDetail;";

constexpr const char* CreateDuckDBOrderTableSqlString = "CREATE TABLE IF NOT EXISTS t_Order(TradingDay varchar, AccountID varchar, AccountType int, ExchangeID varchar, InstrumentID varchar, ProductClass int, OrderID int, OrderSysID varchar, Direction int, OffsetFlag int, OrderPriceType int, Price double, Volume bigint, VolumeTotal bigint, VolumeTraded bigint, VolumeMultiple int, OrderStatus int, OrderDate varchar, OrderTime varchar, CancelDate varchar, CancelTime varchar, SessionID bigint, ClientOrderID int, RequestID int, OfferID int, TradeGroupID int, RiskGroupID int, CommissionGroupID int, FrozenCash double, FrozenMargin double, FrozenCommission double, RebuildMark bool, IsForceClose bool, PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, OrderID));";
constexpr const char* TruncateDuckDBOrderTableSqlString = "Delete From t_Order;";

constexpr const char* CreateDuckDBTradeTableSqlString = "CREATE TABLE IF NOT EXISTS t_Trade(TradingDay varchar, AccountID varchar, AccountType int, ExchangeID varchar, InstrumentID varchar, ProductClass int, OrderID int, OrderSysID varchar, TradeID varchar, Direction int, OffsetFlag int, Price double, Volume bigint, VolumeMultiple int, TradeAmount double, Commission double, TradeDate varchar, TradeTime varchar, PRIMARY KEY(TradingDay, ExchangeID, TradeID, Direction));";
constexpr const char* TruncateDuckDBTradeTableSqlString = "Delete From t_Trade;";

