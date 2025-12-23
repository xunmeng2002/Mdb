
CREATE TABLE IF NOT EXISTS t_TradingDay(
  PK int, 
  CurrTradingDay varchar, 
  PreTradingDay varchar, 
  PRIMARY KEY(PK)
);  -- '交易日'


CREATE TABLE IF NOT EXISTS t_Exchange(
  ExchangeID varchar, 
  ExchangeName varchar, 
  PRIMARY KEY(ExchangeID)
);  -- '交易所'


CREATE TABLE IF NOT EXISTS t_Product(
  ExchangeID varchar, 
  ProductID varchar, 
  ProductName varchar, 
  ProductClass int, 
  VolumeMultiple int, 
  PriceTick double, 
  MaxMarketOrderVolume bigint, 
  MinMarketOrderVolume bigint, 
  MaxLimitOrderVolume bigint, 
  MinLimitOrderVolume bigint, 
  SessionName varchar, 
  PRIMARY KEY(ExchangeID, ProductID)
);  -- '品种表'


CREATE TABLE IF NOT EXISTS t_Instrument(
  ExchangeID varchar, 
  InstrumentID varchar, 
  ExchangeInstID varchar, 
  InstrumentName varchar, 
  ProductID varchar, 
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
  PRIMARY KEY(ExchangeID, InstrumentID)
);  -- '合约'


CREATE TABLE IF NOT EXISTS t_PrimaryAccount(
  PrimaryAccountID varchar, 
  PrimaryAccountName varchar, 
  AccountClass int, 
  BrokerPassword varchar, 
  OfferID int, 
  IsAllowLogin int, 
  IsSimulateAccount int, 
  LoginStatus int, 
  InitStatus int, 
  PRIMARY KEY(PrimaryAccountID)
);  -- '主账户'
  CREATE INDEX PrimaryAccountOfferID ON t_PrimaryAccount(OfferID);


CREATE TABLE IF NOT EXISTS t_Account(
  AccountID varchar, 
  AccountName varchar, 
  AccountType int, 
  AccountStatus int, 
  Password varchar, 
  TradeGroupID int, 
  RiskGroupID int, 
  CommissionGroupID int, 
  PRIMARY KEY(AccountID)
);  -- '账户'


CREATE TABLE IF NOT EXISTS t_Capital(
  TradingDay varchar, 
  AccountID varchar, 
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
  PRIMARY KEY(TradingDay, AccountID)
);  -- '资金'
  CREATE INDEX CapitalTradingDay ON t_Capital(TradingDay);


CREATE TABLE IF NOT EXISTS t_Position(
  TradingDay varchar, 
  AccountID varchar, 
  AccountType int, 
  ExchangeID varchar, 
  InstrumentID varchar, 
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
  PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection)
);  -- '持仓'
  CREATE INDEX PositionAccount ON t_Position(TradingDay, AccountID);
  CREATE INDEX PositionTradingDay ON t_Position(TradingDay);


CREATE TABLE IF NOT EXISTS t_PositionDetail(
  TradingDay varchar, 
  AccountID varchar, 
  AccountType int, 
  ExchangeID varchar, 
  InstrumentID varchar, 
  ProductClass int, 
  PosiDirection int, 
  OpenDate varchar, 
  TradeID varchar, 
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
  PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection, OpenDate, TradeID)
);  -- '持仓明细'
  CREATE INDEX PositionDetailTradeMatch ON t_PositionDetail(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection);
  CREATE INDEX PositionDetailTradingDay ON t_PositionDetail(TradingDay);


CREATE TABLE IF NOT EXISTS t_Order(
  TradingDay varchar, 
  AccountID varchar, 
  AccountType int, 
  ExchangeID varchar, 
  InstrumentID varchar, 
  ProductClass int, 
  OrderID int, 
  OrderSysID varchar, 
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
  SessionID bigint, 
  ClientOrderID int, 
  RequestID int, 
  OfferID int, 
  TradeGroupID int, 
  RiskGroupID int, 
  CommissionGroupID int, 
  FrozenCash double, 
  FrozenMargin double, 
  FrozenCommission double, 
  RebuildMark int, 
  IsForceClose int, 
  UNIQUE (TradingDay, AccountID, ExchangeID, InstrumentID, SessionID, ClientOrderID), 
  PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, OrderID)
);  -- '委托'


CREATE TABLE IF NOT EXISTS t_Trade(
  TradingDay varchar, 
  AccountID varchar, 
  AccountType int, 
  ExchangeID varchar, 
  InstrumentID varchar, 
  ProductClass int, 
  OrderID int, 
  OrderSysID varchar, 
  TradeID varchar, 
  Direction int, 
  OffsetFlag int, 
  Price double, 
  Volume bigint, 
  VolumeMultiple int, 
  TradeAmount double, 
  Commission double, 
  TradeDate varchar, 
  TradeTime varchar, 
  PRIMARY KEY(TradingDay, ExchangeID, TradeID, Direction)
);  -- '成交'


