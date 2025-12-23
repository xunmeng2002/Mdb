#pragma once

constexpr const char* CreateMysqlDBTradingDayTableSqlString = "CREATE TABLE IF NOT EXISTS t_TradingDay(PK int, CurrTradingDay char(9), PreTradingDay char(9), PRIMARY KEY(PK)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
constexpr const char* TruncateMysqlDBTradingDayTableSqlString = "Truncate Table t_TradingDay;";

constexpr const char* CreateMysqlDBExchangeTableSqlString = "CREATE TABLE IF NOT EXISTS t_Exchange(ExchangeID char(8), ExchangeName char(64), PRIMARY KEY(ExchangeID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
constexpr const char* TruncateMysqlDBExchangeTableSqlString = "Truncate Table t_Exchange;";

constexpr const char* CreateMysqlDBProductTableSqlString = "CREATE TABLE IF NOT EXISTS t_Product(ExchangeID char(8), ProductID char(32), ProductName char(32), ProductClass int, VolumeMultiple int, PriceTick double, MaxMarketOrderVolume bigint, MinMarketOrderVolume bigint, MaxLimitOrderVolume bigint, MinLimitOrderVolume bigint, SessionName char(32), PRIMARY KEY(ExchangeID, ProductID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
constexpr const char* TruncateMysqlDBProductTableSqlString = "Truncate Table t_Product;";

constexpr const char* CreateMysqlDBInstrumentTableSqlString = "CREATE TABLE IF NOT EXISTS t_Instrument(ExchangeID char(8), InstrumentID char(32), ExchangeInstID char(32), InstrumentName char(64), ProductID char(32), ProductClass int, InstrumentClass int, Rank int, VolumeMultiple int, PriceTick double, MaxMarketOrderVolume bigint, MinMarketOrderVolume bigint, MaxLimitOrderVolume bigint, MinLimitOrderVolume bigint, SessionName char(32), PRIMARY KEY(ExchangeID, InstrumentID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
constexpr const char* TruncateMysqlDBInstrumentTableSqlString = "Truncate Table t_Instrument;";

constexpr const char* CreateMysqlDBPrimaryAccountTableSqlString = "CREATE TABLE IF NOT EXISTS t_PrimaryAccount(PrimaryAccountID char(32), PrimaryAccountName char(64), AccountClass int, BrokerPassword char(64), OfferID int, IsAllowLogin bool, IsSimulateAccount bool, LoginStatus int, InitStatus int, INDEX OfferID(OfferID), PRIMARY KEY(PrimaryAccountID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
constexpr const char* TruncateMysqlDBPrimaryAccountTableSqlString = "Truncate Table t_PrimaryAccount;";

constexpr const char* CreateMysqlDBAccountTableSqlString = "CREATE TABLE IF NOT EXISTS t_Account(AccountID char(32), AccountName char(64), AccountType int, AccountStatus int, Password char(64), TradeGroupID int, RiskGroupID int, CommissionGroupID int, PRIMARY KEY(AccountID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
constexpr const char* TruncateMysqlDBAccountTableSqlString = "Truncate Table t_Account;";

constexpr const char* CreateMysqlDBCapitalTableSqlString = "CREATE TABLE IF NOT EXISTS t_Capital(TradingDay char(9), AccountID char(32), AccountType int, Balance double, PreBalance double, Available double, MarketValue double, CashIn double, CashOut double, Margin double, Commission double, FrozenCash double, FrozenMargin double, FrozenCommission double, CloseProfitByDate double, CloseProfitByTrade double, PositionProfitByDate double, PositionProfitByTrade double, Deposit double, Withdraw double, INDEX TradingDay(TradingDay), PRIMARY KEY(TradingDay, AccountID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
constexpr const char* TruncateMysqlDBCapitalTableSqlString = "Truncate Table t_Capital;";

constexpr const char* CreateMysqlDBPositionTableSqlString = "CREATE TABLE IF NOT EXISTS t_Position(TradingDay char(9), AccountID char(32), AccountType int, ExchangeID char(8), InstrumentID char(32), ProductClass int, PosiDirection int, TotalPosition bigint, PositionFrozen bigint, TodayPosition bigint, MarketValue double, CashIn double, CashOut double, Margin double, Commission double, FrozenCash double, FrozenMargin double, FrozenCommission double, VolumeMultiple int, CloseProfitByDate double, CloseProfitByTrade double, PositionProfitByDate double, PositionProfitByTrade double, SettlementPrice double, PreSettlementPrice double, INDEX Account(TradingDay, AccountID), INDEX TradingDay(TradingDay), PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
constexpr const char* TruncateMysqlDBPositionTableSqlString = "Truncate Table t_Position;";

constexpr const char* CreateMysqlDBPositionDetailTableSqlString = "CREATE TABLE IF NOT EXISTS t_PositionDetail(TradingDay char(9), AccountID char(32), AccountType int, ExchangeID char(8), InstrumentID char(32), ProductClass int, PosiDirection int, OpenDate char(9), TradeID char(64), Volume bigint, OpenPrice double, MarketValue double, CashIn double, CashOut double, Margin double, Commission double, VolumeMultiple int, CloseProfitByDate double, CloseProfitByTrade double, PositionProfitByDate double, PositionProfitByTrade double, SettlementPrice double, PreSettlementPrice double, CloseVolume bigint, CloseAmount double, INDEX TradeMatch(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection), INDEX TradingDay(TradingDay), PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection, OpenDate, TradeID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
constexpr const char* TruncateMysqlDBPositionDetailTableSqlString = "Truncate Table t_PositionDetail;";

constexpr const char* CreateMysqlDBOrderTableSqlString = "CREATE TABLE IF NOT EXISTS t_Order(TradingDay char(9), AccountID char(32), AccountType int, ExchangeID char(8), InstrumentID char(32), ProductClass int, OrderID int, OrderSysID char(64), Direction int, OffsetFlag int, OrderPriceType int, Price double, Volume bigint, VolumeTotal bigint, VolumeTraded bigint, VolumeMultiple int, OrderStatus int, OrderDate char(9), OrderTime char(9), CancelDate char(9), CancelTime char(9), SessionID bigint, ClientOrderID int, RequestID int, OfferID int, TradeGroupID int, RiskGroupID int, CommissionGroupID int, FrozenCash double, FrozenMargin double, FrozenCommission double, RebuildMark bool, IsForceClose bool, PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, OrderID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
constexpr const char* TruncateMysqlDBOrderTableSqlString = "Truncate Table t_Order;";

constexpr const char* CreateMysqlDBTradeTableSqlString = "CREATE TABLE IF NOT EXISTS t_Trade(TradingDay char(9), AccountID char(32), AccountType int, ExchangeID char(8), InstrumentID char(32), ProductClass int, OrderID int, OrderSysID char(64), TradeID char(64), Direction int, OffsetFlag int, Price double, Volume bigint, VolumeMultiple int, TradeAmount double, Commission double, TradeDate char(9), TradeTime char(9), PRIMARY KEY(TradingDay, ExchangeID, TradeID, Direction)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
constexpr const char* TruncateMysqlDBTradeTableSqlString = "Truncate Table t_Trade;";

