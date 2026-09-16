-- 本文件由 ../Templates/Sql/Sqlite/CreateTables.sql.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py

CREATE TABLE IF NOT EXISTS TradingDay(
  `PK` int, 
  `CurrTradingDay` text, 
  `PreTradingDay` text, 
  PRIMARY KEY(PK)
);


CREATE TABLE IF NOT EXISTS Exchange(
  `ExchangeId` text, 
  `ExchangeName` text, 
  PRIMARY KEY(ExchangeId)
);


CREATE TABLE IF NOT EXISTS Product(
  `ExchangeId` text, 
  `ProductId` text, 
  `ProductName` text, 
  `ProductClass` int, 
  `VolumeMultiple` int, 
  `PriceTick` double, 
  `MaxMarketOrderVolume` bigint, 
  `MinMarketOrderVolume` bigint, 
  `MaxLimitOrderVolume` bigint, 
  `MinLimitOrderVolume` bigint, 
  `SessionName` text, 
  PRIMARY KEY(ExchangeId, ProductId)
);


CREATE TABLE IF NOT EXISTS Instrument(
  `ExchangeId` text, 
  `InstrumentId` text, 
  `ExchangeInstId` text, 
  `InstrumentName` text, 
  `ProductId` text, 
  `ProductClass` int, 
  `InstrumentClass` int, 
  `Rank` int, 
  `VolumeMultiple` int, 
  `PriceTick` double, 
  `MaxMarketOrderVolume` bigint, 
  `MinMarketOrderVolume` bigint, 
  `MaxLimitOrderVolume` bigint, 
  `MinLimitOrderVolume` bigint, 
  `SessionName` text, 
  PRIMARY KEY(ExchangeId, InstrumentId)
);


CREATE TABLE IF NOT EXISTS PrimaryAccount(
  `PrimaryAccountId` text, 
  `PrimaryAccountName` text, 
  `AccountClass` int, 
  `BrokerPassword` text, 
  `OfferId` int, 
  `IsAllowLogin` int, 
  `IsSimulateAccount` int, 
  `LoginStatus` int, 
  `InitStatus` int, 
  PRIMARY KEY(PrimaryAccountId)
);
CREATE INDEX PrimaryAccountOfferId ON PrimaryAccount(OfferId);


CREATE TABLE IF NOT EXISTS Account(
  `AccountId` text, 
  `AccountName` text, 
  `AccountType` int, 
  `AccountStatus` int, 
  `Password` text, 
  `TradeGroupId` int, 
  `RiskGroupId` int, 
  `CommissionGroupId` int, 
  PRIMARY KEY(AccountId)
);


CREATE TABLE IF NOT EXISTS Capital(
  `TradingDay` text, 
  `AccountId` text, 
  `AccountType` int, 
  `Balance` double, 
  `PreBalance` double, 
  `Available` double, 
  `MarketValue` double, 
  `CashIn` double, 
  `CashOut` double, 
  `Margin` double, 
  `Commission` double, 
  `FrozenCash` double, 
  `FrozenMargin` double, 
  `FrozenCommission` double, 
  `CloseProfitByDate` double, 
  `CloseProfitByTrade` double, 
  `PositionProfitByDate` double, 
  `PositionProfitByTrade` double, 
  `Deposit` double, 
  `Withdraw` double, 
  PRIMARY KEY(TradingDay, AccountId)
);
CREATE INDEX CapitalTradingDay ON Capital(TradingDay);


CREATE TABLE IF NOT EXISTS Position(
  `TradingDay` text, 
  `AccountId` text, 
  `AccountType` int, 
  `ExchangeId` text, 
  `InstrumentId` text, 
  `ProductClass` int, 
  `PosiDirection` int, 
  `TotalPosition` bigint, 
  `PositionFrozen` bigint, 
  `TodayPosition` bigint, 
  `MarketValue` double, 
  `CashIn` double, 
  `CashOut` double, 
  `Margin` double, 
  `Commission` double, 
  `FrozenCash` double, 
  `FrozenMargin` double, 
  `FrozenCommission` double, 
  `VolumeMultiple` int, 
  `CloseProfitByDate` double, 
  `CloseProfitByTrade` double, 
  `PositionProfitByDate` double, 
  `PositionProfitByTrade` double, 
  `SettlementPrice` double, 
  `PreSettlementPrice` double, 
  PRIMARY KEY(TradingDay, AccountId, ExchangeId, InstrumentId, PosiDirection)
);
CREATE INDEX PositionAccount ON Position(TradingDay, AccountId);
CREATE INDEX PositionTradingDay ON Position(TradingDay);


CREATE TABLE IF NOT EXISTS PositionDetail(
  `TradingDay` text, 
  `AccountId` text, 
  `AccountType` int, 
  `ExchangeId` text, 
  `InstrumentId` text, 
  `ProductClass` int, 
  `PosiDirection` int, 
  `OpenDate` text, 
  `TradeId` text, 
  `Volume` bigint, 
  `OpenPrice` double, 
  `MarketValue` double, 
  `CashIn` double, 
  `CashOut` double, 
  `Margin` double, 
  `Commission` double, 
  `VolumeMultiple` int, 
  `CloseProfitByDate` double, 
  `CloseProfitByTrade` double, 
  `PositionProfitByDate` double, 
  `PositionProfitByTrade` double, 
  `SettlementPrice` double, 
  `PreSettlementPrice` double, 
  `CloseVolume` bigint, 
  `CloseAmount` double, 
  PRIMARY KEY(TradingDay, AccountId, ExchangeId, InstrumentId, PosiDirection, OpenDate, TradeId)
);
CREATE INDEX PositionDetailTradeMatch ON PositionDetail(TradingDay, AccountId, ExchangeId, InstrumentId, PosiDirection);
CREATE INDEX PositionDetailTradingDay ON PositionDetail(TradingDay);


CREATE TABLE IF NOT EXISTS Order(
  `TradingDay` text, 
  `AccountId` text, 
  `AccountType` int, 
  `ExchangeId` text, 
  `InstrumentId` text, 
  `ProductClass` int, 
  `OrderId` int, 
  `OrderSysId` text, 
  `Direction` int, 
  `OffsetFlag` int, 
  `OrderPriceType` int, 
  `Price` double, 
  `Volume` bigint, 
  `VolumeTotal` bigint, 
  `VolumeTraded` bigint, 
  `VolumeMultiple` int, 
  `OrderStatus` int, 
  `OrderDate` text, 
  `OrderTime` text, 
  `CancelDate` text, 
  `CancelTime` text, 
  `SessionId` bigint, 
  `ClientOrderId` int, 
  `RequestId` int, 
  `OfferId` int, 
  `TradeGroupId` int, 
  `RiskGroupId` int, 
  `CommissionGroupId` int, 
  `FrozenCash` double, 
  `FrozenMargin` double, 
  `FrozenCommission` double, 
  `RebuildMark` int, 
  `IsForceClose` int, 
  UNIQUE (TradingDay, AccountId, ExchangeId, InstrumentId, SessionId, ClientOrderId), 
  PRIMARY KEY(TradingDay, AccountId, ExchangeId, InstrumentId, OrderId)
);


CREATE TABLE IF NOT EXISTS Trade(
  `TradingDay` text, 
  `AccountId` text, 
  `AccountType` int, 
  `ExchangeId` text, 
  `InstrumentId` text, 
  `ProductClass` int, 
  `OrderId` int, 
  `OrderSysId` text, 
  `TradeId` text, 
  `Direction` int, 
  `OffsetFlag` int, 
  `Price` double, 
  `Volume` bigint, 
  `VolumeMultiple` int, 
  `TradeAmount` double, 
  `Commission` double, 
  `TradeDate` text, 
  `TradeTime` text, 
  PRIMARY KEY(TradingDay, ExchangeId, TradeId, Direction)
);


