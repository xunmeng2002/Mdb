#pragma once

constexpr const char* CreateSqliteDBTradingDayTableSqlString = "CREATE TABLE IF NOT EXISTS t_TradingDay(PK int, CurrTradingDay char(9), PreTradingDay char(9), PRIMARY KEY(PK));";
constexpr const char* TruncateSqliteDBTradingDayTableSqlString = "Delete From t_TradingDay;";

constexpr const char* CreateSqliteDBExchangeTableSqlString = "CREATE TABLE IF NOT EXISTS t_Exchange(ExchangeID char(8), ExchangeName char(64), PRIMARY KEY(ExchangeID));";
constexpr const char* TruncateSqliteDBExchangeTableSqlString = "Delete From t_Exchange;";

constexpr const char* CreateSqliteDBProductTableSqlString = "CREATE TABLE IF NOT EXISTS t_Product(ExchangeID char(8), ProductID char(32), ProductName char(32), ProductClass int, VolumeMultiple int, PriceTick double, MaxMarketOrderVolume bigint, MinMarketOrderVolume bigint, MaxLimitOrderVolume bigint, MinLimitOrderVolume bigint, SessionName char(32), PRIMARY KEY(ExchangeID, ProductID));";
constexpr const char* TruncateSqliteDBProductTableSqlString = "Delete From t_Product;";

constexpr const char* CreateSqliteDBInstrumentTableSqlString = "CREATE TABLE IF NOT EXISTS t_Instrument(ExchangeID char(8), InstrumentID char(32), ExchangeInstID char(32), InstrumentName char(64), ProductID char(32), ProductClass int, InstrumentClass int, Rank int, VolumeMultiple int, PriceTick double, MaxMarketOrderVolume bigint, MinMarketOrderVolume bigint, MaxLimitOrderVolume bigint, MinLimitOrderVolume bigint, SessionName char(32), PRIMARY KEY(ExchangeID, InstrumentID));";
constexpr const char* TruncateSqliteDBInstrumentTableSqlString = "Delete From t_Instrument;";

constexpr const char* CreateSqliteDBPrimaryAccountTableSqlString = "CREATE TABLE IF NOT EXISTS t_PrimaryAccount(PrimaryAccountID char(32), PrimaryAccountName char(64), AccountClass int, BrokerPassword char(64), OfferID int, IsAllowLogin bool, IsSimulateAccount bool, LoginStatus int, InitStatus int, PRIMARY KEY(PrimaryAccountID)); CREATE INDEX IF NOT EXISTS OfferIDIndex ON t_PrimaryAccount(OfferID);";
constexpr const char* TruncateSqliteDBPrimaryAccountTableSqlString = "Delete From t_PrimaryAccount;";

constexpr const char* CreateSqliteDBAccountTableSqlString = "CREATE TABLE IF NOT EXISTS t_Account(AccountID char(32), AccountName char(64), AccountType int, AccountStatus int, Password char(64), TradeGroupID int, RiskGroupID int, CommissionGroupID int, PRIMARY KEY(AccountID));";
constexpr const char* TruncateSqliteDBAccountTableSqlString = "Delete From t_Account;";

constexpr const char* CreateSqliteDBCapitalTableSqlString = "CREATE TABLE IF NOT EXISTS t_Capital(TradingDay char(9), AccountID char(32), AccountType int, Balance double, PreBalance double, Available double, MarketValue double, CashIn double, CashOut double, Margin double, Commission double, FrozenCash double, FrozenMargin double, FrozenCommission double, CloseProfitByDate double, CloseProfitByTrade double, PositionProfitByDate double, PositionProfitByTrade double, Deposit double, Withdraw double, PRIMARY KEY(TradingDay, AccountID)); CREATE INDEX IF NOT EXISTS TradingDayIndex ON t_Capital(TradingDay);";
constexpr const char* TruncateSqliteDBCapitalTableSqlString = "Delete From t_Capital;";

constexpr const char* CreateSqliteDBPositionTableSqlString = "CREATE TABLE IF NOT EXISTS t_Position(TradingDay char(9), AccountID char(32), AccountType int, ExchangeID char(8), InstrumentID char(32), ProductClass int, PosiDirection int, TotalPosition bigint, PositionFrozen bigint, TodayPosition bigint, MarketValue double, CashIn double, CashOut double, Margin double, Commission double, FrozenCash double, FrozenMargin double, FrozenCommission double, VolumeMultiple int, CloseProfitByDate double, CloseProfitByTrade double, PositionProfitByDate double, PositionProfitByTrade double, SettlementPrice double, PreSettlementPrice double, PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection)); CREATE INDEX IF NOT EXISTS AccountIndex ON t_Position(TradingDay, AccountID); CREATE INDEX IF NOT EXISTS TradingDayIndex ON t_Position(TradingDay);";
constexpr const char* TruncateSqliteDBPositionTableSqlString = "Delete From t_Position;";

constexpr const char* CreateSqliteDBPositionDetailTableSqlString = "CREATE TABLE IF NOT EXISTS t_PositionDetail(TradingDay char(9), AccountID char(32), AccountType int, ExchangeID char(8), InstrumentID char(32), ProductClass int, PosiDirection int, OpenDate char(9), TradeID char(64), Volume bigint, OpenPrice double, MarketValue double, CashIn double, CashOut double, Margin double, Commission double, VolumeMultiple int, CloseProfitByDate double, CloseProfitByTrade double, PositionProfitByDate double, PositionProfitByTrade double, SettlementPrice double, PreSettlementPrice double, CloseVolume bigint, CloseAmount double, PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection, OpenDate, TradeID)); CREATE INDEX IF NOT EXISTS TradeMatchIndex ON t_PositionDetail(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection); CREATE INDEX IF NOT EXISTS TradingDayIndex ON t_PositionDetail(TradingDay);";
constexpr const char* TruncateSqliteDBPositionDetailTableSqlString = "Delete From t_PositionDetail;";

constexpr const char* CreateSqliteDBOrderTableSqlString = "CREATE TABLE IF NOT EXISTS t_Order(TradingDay char(9), AccountID char(32), AccountType int, ExchangeID char(8), InstrumentID char(32), ProductClass int, OrderID int, OrderSysID char(64), Direction int, OffsetFlag int, OrderPriceType int, Price double, Volume bigint, VolumeTotal bigint, VolumeTraded bigint, VolumeMultiple int, OrderStatus int, OrderDate char(9), OrderTime char(9), CancelDate char(9), CancelTime char(9), SessionID bigint, ClientOrderID int, RequestID int, OfferID int, TradeGroupID int, RiskGroupID int, CommissionGroupID int, FrozenCash double, FrozenMargin double, FrozenCommission double, RebuildMark bool, IsForceClose bool, PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, OrderID));";
constexpr const char* TruncateSqliteDBOrderTableSqlString = "Delete From t_Order;";

constexpr const char* CreateSqliteDBTradeTableSqlString = "CREATE TABLE IF NOT EXISTS t_Trade(TradingDay char(9), AccountID char(32), AccountType int, ExchangeID char(8), InstrumentID char(32), ProductClass int, OrderID int, OrderSysID char(64), TradeID char(64), Direction int, OffsetFlag int, Price double, Volume bigint, VolumeMultiple int, TradeAmount double, Commission double, TradeDate char(9), TradeTime char(9), PRIMARY KEY(TradingDay, ExchangeID, TradeID, Direction));";
constexpr const char* TruncateSqliteDBTradeTableSqlString = "Delete From t_Trade;";

