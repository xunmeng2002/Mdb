#pragma once

constexpr const char* CreateMariaDBTradingDayTableSqlString = "CREATE TABLE IF NOT EXISTS t_TradingDay(PK int, CurrTradingDay char(9), PreTradingDay char(9), PRIMARY KEY(PK)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
constexpr const char* TruncateMariaDBTradingDayTableSqlString = "Truncate Table t_TradingDay;";

constexpr const char* CreateMariaDBExchangeTableSqlString = "CREATE TABLE IF NOT EXISTS t_Exchange(ExchangeID char(8), ExchangeName char(64), PRIMARY KEY(ExchangeID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
constexpr const char* TruncateMariaDBExchangeTableSqlString = "Truncate Table t_Exchange;";

constexpr const char* CreateMariaDBProductTableSqlString = "CREATE TABLE IF NOT EXISTS t_Product(ExchangeID char(8), ProductID char(32), ProductName char(32), ProductClass int, VolumeMultiple int, PriceTick double, MaxMarketOrderVolume bigint, MinMarketOrderVolume bigint, MaxLimitOrderVolume bigint, MinLimitOrderVolume bigint, SessionName char(32), PRIMARY KEY(ExchangeID, ProductID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
constexpr const char* TruncateMariaDBProductTableSqlString = "Truncate Table t_Product;";

constexpr const char* CreateMariaDBInstrumentTableSqlString = "CREATE TABLE IF NOT EXISTS t_Instrument(ExchangeID char(8), InstrumentID char(32), ExchangeInstID char(32), InstrumentName char(64), ProductID char(32), ProductClass int, InstrumentClass int, Rank int, VolumeMultiple int, PriceTick double, MaxMarketOrderVolume bigint, MinMarketOrderVolume bigint, MaxLimitOrderVolume bigint, MinLimitOrderVolume bigint, SessionName char(32), PRIMARY KEY(ExchangeID, InstrumentID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
constexpr const char* TruncateMariaDBInstrumentTableSqlString = "Truncate Table t_Instrument;";

constexpr const char* CreateMariaDBPrimaryAccountTableSqlString = "CREATE TABLE IF NOT EXISTS t_PrimaryAccount(PrimaryAccountID char(32), PrimaryAccountName char(64), AccountClass int, BrokerPassword char(64), OfferID int, IsAllowLogin bool, IsSimulateAccount bool, LoginStatus int, InitStatus int, INDEX OfferID(OfferID), PRIMARY KEY(PrimaryAccountID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
constexpr const char* TruncateMariaDBPrimaryAccountTableSqlString = "Truncate Table t_PrimaryAccount;";

constexpr const char* CreateMariaDBAccountTableSqlString = "CREATE TABLE IF NOT EXISTS t_Account(AccountID char(32), AccountName char(64), AccountType int, AccountStatus int, Password char(64), TradeGroupID int, RiskGroupID int, CommissionGroupID int, PRIMARY KEY(AccountID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
constexpr const char* TruncateMariaDBAccountTableSqlString = "Truncate Table t_Account;";

constexpr const char* CreateMariaDBCapitalTableSqlString = "CREATE TABLE IF NOT EXISTS t_Capital(TradingDay char(9), AccountID char(32), AccountType int, Balance double, PreBalance double, Available double, MarketValue double, CashIn double, CashOut double, Margin double, Commission double, FrozenCash double, FrozenMargin double, FrozenCommission double, CloseProfitByDate double, CloseProfitByTrade double, PositionProfitByDate double, PositionProfitByTrade double, Deposit double, Withdraw double, INDEX TradingDay(TradingDay), PRIMARY KEY(TradingDay, AccountID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
constexpr const char* TruncateMariaDBCapitalTableSqlString = "Truncate Table t_Capital;";

constexpr const char* CreateMariaDBPositionTableSqlString = "CREATE TABLE IF NOT EXISTS t_Position(TradingDay char(9), AccountID char(32), AccountType int, ExchangeID char(8), InstrumentID char(32), ProductClass int, PosiDirection int, TotalPosition bigint, PositionFrozen bigint, TodayPosition bigint, MarketValue double, CashIn double, CashOut double, Margin double, Commission double, FrozenCash double, FrozenMargin double, FrozenCommission double, VolumeMultiple int, CloseProfitByDate double, CloseProfitByTrade double, PositionProfitByDate double, PositionProfitByTrade double, SettlementPrice double, PreSettlementPrice double, INDEX Account(TradingDay, AccountID), INDEX TradingDay(TradingDay), PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
constexpr const char* TruncateMariaDBPositionTableSqlString = "Truncate Table t_Position;";

constexpr const char* CreateMariaDBPositionDetailTableSqlString = "CREATE TABLE IF NOT EXISTS t_PositionDetail(TradingDay char(9), AccountID char(32), AccountType int, ExchangeID char(8), InstrumentID char(32), ProductClass int, PosiDirection int, OpenDate char(9), TradeID char(64), Volume bigint, OpenPrice double, MarketValue double, CashIn double, CashOut double, Margin double, Commission double, VolumeMultiple int, CloseProfitByDate double, CloseProfitByTrade double, PositionProfitByDate double, PositionProfitByTrade double, SettlementPrice double, PreSettlementPrice double, CloseVolume bigint, CloseAmount double, INDEX TradeMatch(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection), INDEX TradingDay(TradingDay), PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection, OpenDate, TradeID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
constexpr const char* TruncateMariaDBPositionDetailTableSqlString = "Truncate Table t_PositionDetail;";

constexpr const char* CreateMariaDBOrderTableSqlString = "CREATE TABLE IF NOT EXISTS t_Order(TradingDay char(9), AccountID char(32), AccountType int, ExchangeID char(8), InstrumentID char(32), ProductClass int, OrderID int, OrderSysID char(64), Direction int, OffsetFlag int, OrderPriceType int, Price double, Volume bigint, VolumeTotal bigint, VolumeTraded bigint, VolumeMultiple int, OrderStatus int, OrderDate char(9), OrderTime char(9), CancelDate char(9), CancelTime char(9), SessionID bigint, ClientOrderID int, RequestID int, OfferID int, TradeGroupID int, RiskGroupID int, CommissionGroupID int, FrozenCash double, FrozenMargin double, FrozenCommission double, RebuildMark bool, IsForceClose bool, PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, OrderID)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
constexpr const char* TruncateMariaDBOrderTableSqlString = "Truncate Table t_Order;";

constexpr const char* CreateMariaDBTradeTableSqlString = "CREATE TABLE IF NOT EXISTS t_Trade(TradingDay char(9), AccountID char(32), AccountType int, ExchangeID char(8), InstrumentID char(32), ProductClass int, OrderID int, OrderSysID char(64), TradeID char(64), Direction int, OffsetFlag int, Price double, Volume bigint, VolumeMultiple int, TradeAmount double, Commission double, TradeDate char(9), TradeTime char(9), PRIMARY KEY(TradingDay, ExchangeID, TradeID, Direction)) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin';";
constexpr const char* TruncateMariaDBTradeTableSqlString = "Truncate Table t_Trade;";

