-- 本文件由 ../Templates/Sql/Duckdb/CreateTables.sql.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py

CREATE TABLE IF NOT EXISTS TradingDay(
  PK int, 
  CurrTradingDay varchar, 
  PreTradingDay varchar, 
  PRIMARY KEY(PK)
);  -- '交易日'


CREATE TABLE IF NOT EXISTS Exchange(
  ExchangeId varchar, 
  ExchangeName varchar, 
  PRIMARY KEY(ExchangeId)
);  -- '交易所'


CREATE TABLE IF NOT EXISTS Product(
  ExchangeId varchar, 
  ProductId varchar, 
  ProductName varchar, 
  ProductClass int, 
  VolumeMultiple int, 
  PriceTick double, 
  MaxMarketOrderVolume bigint, 
  MinMarketOrderVolume bigint, 
  MaxLimitOrderVolume bigint, 
  MinLimitOrderVolume bigint, 
  SessionName varchar, 
  PRIMARY KEY(ExchangeId, ProductId)
);  -- '品种表'


CREATE TABLE IF NOT EXISTS Instrument(
  ExchangeId varchar, 
  InstrumentId varchar, 
  ExchangeInstId varchar, 
  InstrumentName varchar, 
  ProductId varchar, 
  ProductClass int, 
  InstrumentClass int, 
  Rank int, 
  VolumeMultiple int, 
  PriceTick double, 
  MaxMarketOrderVolume bigint, 
  MinMarketOrderVolume bigint, 
  MaxLimitOrderVolume bigint, 
  MinLimitOrderVolume bigint, 
  SessionName varchar, 
  PRIMARY KEY(ExchangeId, InstrumentId)
);  -- '合约'


CREATE TABLE IF NOT EXISTS PrimaryAccount(
  PrimaryAccountId varchar, 
  PrimaryAccountName varchar, 
  AccountClass int, 
  BrokerPassword varchar, 
  OfferId int, 
  IsAllowLogin int, 
  IsSimulateAccount int, 
  LoginStatus int, 
  InitStatus int, 
  PRIMARY KEY(PrimaryAccountId)
);  -- '主账户'
  CREATE INDEX PrimaryAccountOfferId ON PrimaryAccount(OfferId);


CREATE TABLE IF NOT EXISTS Account(
  AccountId varchar, 
  AccountName varchar, 
  AccountType int, 
  AccountStatus int, 
  Password varchar, 
  TradeGroupId int, 
  RiskGroupId int, 
  CommissionGroupId int, 
  PRIMARY KEY(AccountId)
);  -- '账户'


CREATE TABLE IF NOT EXISTS Capital(
  TradingDay varchar, 
  AccountId varchar, 
  AccountType int, 
  Balance double, 
  PreBalance double, 
  Available double, 
  MarketValue double, 
  CashIn double, 
  CashOut double, 
  Margin double, 
  Commission double, 
  FrozenCash double, 
  FrozenMargin double, 
  FrozenCommission double, 
  CloseProfitByDate double, 
  CloseProfitByTrade double, 
  PositionProfitByDate double, 
  PositionProfitByTrade double, 
  Deposit double, 
  Withdraw double, 
  PRIMARY KEY(TradingDay, AccountId)
);  -- '资金'
  CREATE INDEX CapitalTradingDay ON Capital(TradingDay);


CREATE TABLE IF NOT EXISTS Position(
  TradingDay varchar, 
  AccountId varchar, 
  AccountType int, 
  ExchangeId varchar, 
  InstrumentId varchar, 
  ProductClass int, 
  PosiDirection int, 
  TotalPosition bigint, 
  PositionFrozen bigint, 
  TodayPosition bigint, 
  MarketValue double, 
  CashIn double, 
  CashOut double, 
  Margin double, 
  Commission double, 
  FrozenCash double, 
  FrozenMargin double, 
  FrozenCommission double, 
  VolumeMultiple int, 
  CloseProfitByDate double, 
  CloseProfitByTrade double, 
  PositionProfitByDate double, 
  PositionProfitByTrade double, 
  SettlementPrice double, 
  PreSettlementPrice double, 
  PRIMARY KEY(TradingDay, AccountId, ExchangeId, InstrumentId, PosiDirection)
);  -- '持仓'
  CREATE INDEX PositionAccount ON Position(TradingDay, AccountId);
  CREATE INDEX PositionTradingDay ON Position(TradingDay);


CREATE TABLE IF NOT EXISTS PositionDetail(
  TradingDay varchar, 
  AccountId varchar, 
  AccountType int, 
  ExchangeId varchar, 
  InstrumentId varchar, 
  ProductClass int, 
  PosiDirection int, 
  OpenDate varchar, 
  TradeId varchar, 
  Volume bigint, 
  OpenPrice double, 
  MarketValue double, 
  CashIn double, 
  CashOut double, 
  Margin double, 
  Commission double, 
  VolumeMultiple int, 
  CloseProfitByDate double, 
  CloseProfitByTrade double, 
  PositionProfitByDate double, 
  PositionProfitByTrade double, 
  SettlementPrice double, 
  PreSettlementPrice double, 
  CloseVolume bigint, 
  CloseAmount double, 
  PRIMARY KEY(TradingDay, AccountId, ExchangeId, InstrumentId, PosiDirection, OpenDate, TradeId)
);  -- '持仓明细'
  CREATE INDEX PositionDetailTradeMatch ON PositionDetail(TradingDay, AccountId, ExchangeId, InstrumentId, PosiDirection);
  CREATE INDEX PositionDetailTradingDay ON PositionDetail(TradingDay);


CREATE TABLE IF NOT EXISTS Order(
  TradingDay varchar, 
  AccountId varchar, 
  AccountType int, 
  ExchangeId varchar, 
  InstrumentId varchar, 
  ProductClass int, 
  OrderId int, 
  OrderSysId varchar, 
  Direction int, 
  OffsetFlag int, 
  OrderPriceType int, 
  Price double, 
  Volume bigint, 
  VolumeTotal bigint, 
  VolumeTraded bigint, 
  VolumeMultiple int, 
  OrderStatus int, 
  OrderDate varchar, 
  OrderTime varchar, 
  CancelDate varchar, 
  CancelTime varchar, 
  SessionId bigint, 
  ClientOrderId int, 
  RequestId int, 
  OfferId int, 
  TradeGroupId int, 
  RiskGroupId int, 
  CommissionGroupId int, 
  FrozenCash double, 
  FrozenMargin double, 
  FrozenCommission double, 
  RebuildMark int, 
  IsForceClose int, 
  UNIQUE (TradingDay, AccountId, ExchangeId, InstrumentId, SessionId, ClientOrderId), 
  PRIMARY KEY(TradingDay, AccountId, ExchangeId, InstrumentId, OrderId)
);  -- '委托'


CREATE TABLE IF NOT EXISTS Trade(
  TradingDay varchar, 
  AccountId varchar, 
  AccountType int, 
  ExchangeId varchar, 
  InstrumentId varchar, 
  ProductClass int, 
  OrderId int, 
  OrderSysId varchar, 
  TradeId varchar, 
  Direction int, 
  OffsetFlag int, 
  Price double, 
  Volume bigint, 
  VolumeMultiple int, 
  TradeAmount double, 
  Commission double, 
  TradeDate varchar, 
  TradeTime varchar, 
  PRIMARY KEY(TradingDay, ExchangeId, TradeId, Direction)
);  -- '成交'


