
CREATE TABLE IF NOT EXISTS t_TradingDay(
  `PK` int, 
  `CurrTradingDay` text, 
  `PreTradingDay` text, 
  PRIMARY KEY(PK)
);


CREATE TABLE IF NOT EXISTS t_Exchange(
  `ExchangeID` text, 
  `ExchangeName` text, 
  PRIMARY KEY(ExchangeID)
);


CREATE TABLE IF NOT EXISTS t_Product(
  `ExchangeID` text, 
  `ProductID` text, 
  `ProductName` text, 
  `ProductClass` int, 
  `VolumeMultiple` int, 
  `PriceTick` double, 
  `MaxMarketOrderVolume` bigint, 
  `MinMarketOrderVolume` bigint, 
  `MaxLimitOrderVolume` bigint, 
  `MinLimitOrderVolume` bigint, 
  `SessionName` text, 
  PRIMARY KEY(ExchangeID, ProductID)
);


CREATE TABLE IF NOT EXISTS t_Instrument(
  `ExchangeID` text, 
  `InstrumentID` text, 
  `ExchangeInstID` text, 
  `InstrumentName` text, 
  `ProductID` text, 
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
  PRIMARY KEY(ExchangeID, InstrumentID)
);


CREATE TABLE IF NOT EXISTS t_PrimaryAccount(
  `PrimaryAccountID` text, 
  `PrimaryAccountName` text, 
  `AccountClass` int, 
  `BrokerPassword` text, 
  `OfferID` int, 
  `IsAllowLogin` int, 
  `IsSimulateAccount` int, 
  `LoginStatus` int, 
  `InitStatus` int, 
  PRIMARY KEY(PrimaryAccountID)
);
CREATE INDEX PrimaryAccountOfferID ON t_PrimaryAccount(OfferID);


CREATE TABLE IF NOT EXISTS t_Account(
  `AccountID` text, 
  `AccountName` text, 
  `AccountType` int, 
  `AccountStatus` int, 
  `Password` text, 
  `TradeGroupID` int, 
  `RiskGroupID` int, 
  `CommissionGroupID` int, 
  PRIMARY KEY(AccountID)
);


CREATE TABLE IF NOT EXISTS t_Capital(
  `TradingDay` text, 
  `AccountID` text, 
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
  PRIMARY KEY(TradingDay, AccountID)
);
CREATE INDEX CapitalTradingDay ON t_Capital(TradingDay);


CREATE TABLE IF NOT EXISTS t_Position(
  `TradingDay` text, 
  `AccountID` text, 
  `AccountType` int, 
  `ExchangeID` text, 
  `InstrumentID` text, 
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
  PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection)
);
CREATE INDEX PositionAccount ON t_Position(TradingDay, AccountID);
CREATE INDEX PositionTradingDay ON t_Position(TradingDay);


CREATE TABLE IF NOT EXISTS t_PositionDetail(
  `TradingDay` text, 
  `AccountID` text, 
  `AccountType` int, 
  `ExchangeID` text, 
  `InstrumentID` text, 
  `ProductClass` int, 
  `PosiDirection` int, 
  `OpenDate` text, 
  `TradeID` text, 
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
  PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection, OpenDate, TradeID)
);
CREATE INDEX PositionDetailTradeMatch ON t_PositionDetail(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection);
CREATE INDEX PositionDetailTradingDay ON t_PositionDetail(TradingDay);


CREATE TABLE IF NOT EXISTS t_Order(
  `TradingDay` text, 
  `AccountID` text, 
  `AccountType` int, 
  `ExchangeID` text, 
  `InstrumentID` text, 
  `ProductClass` int, 
  `OrderID` int, 
  `OrderSysID` text, 
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
  `SessionID` bigint, 
  `ClientOrderID` int, 
  `RequestID` int, 
  `OfferID` int, 
  `TradeGroupID` int, 
  `RiskGroupID` int, 
  `CommissionGroupID` int, 
  `FrozenCash` double, 
  `FrozenMargin` double, 
  `FrozenCommission` double, 
  `RebuildMark` int, 
  `IsForceClose` int, 
  UNIQUE (TradingDay, AccountID, ExchangeID, InstrumentID, SessionID, ClientOrderID), 
  PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, OrderID)
);


CREATE TABLE IF NOT EXISTS t_Trade(
  `TradingDay` text, 
  `AccountID` text, 
  `AccountType` int, 
  `ExchangeID` text, 
  `InstrumentID` text, 
  `ProductClass` int, 
  `OrderID` int, 
  `OrderSysID` text, 
  `TradeID` text, 
  `Direction` int, 
  `OffsetFlag` int, 
  `Price` double, 
  `Volume` bigint, 
  `VolumeMultiple` int, 
  `TradeAmount` double, 
  `Commission` double, 
  `TradeDate` text, 
  `TradeTime` text, 
  PRIMARY KEY(TradingDay, ExchangeID, TradeID, Direction)
);


