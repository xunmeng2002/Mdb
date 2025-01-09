
CREATE TABLE IF NOT EXISTS `t_TradingDay` (
  `PK` int,  -- '主键'
  `CurrTradingDay` text,  -- '当前交易日'
  `PreTradingDay` text,  -- '上一交易日'
  PRIMARY KEY(PK)
);  -- '交易日'


CREATE TABLE IF NOT EXISTS `t_Exchange` (
  `ExchangeID` text,  -- '交易所代码'
  `ExchangeName` text,  -- '交易所名称'
  PRIMARY KEY(ExchangeID)
);  -- '交易所'


CREATE TABLE IF NOT EXISTS `t_Product` (
  `ExchangeID` text,  -- '交易所代码'
  `ProductID` text,  -- '品种代码'
  `ProductName` text,  -- '品种名称'
  `ProductClass` int,  -- '产品类型'
  `VolumeMultiple` int,  -- '数量乘数'
  `PriceTick` double,  -- '报价单位'
  `MaxMarketOrderVolume` int,  -- '市价单最大下单量'
  `MinMarketOrderVolume` int,  -- '市价单最小下单量'
  `MaxLimitOrderVolume` int,  -- '限价单最大下单量'
  `MinLimitOrderVolume` int,  -- '限价单最小下单量'
  `SessionName` text,  -- '交易节名称'
  PRIMARY KEY(ExchangeID, ProductID)
);  -- '品种表'


CREATE TABLE IF NOT EXISTS `t_Instrument` (
  `ExchangeID` text,  -- '交易所代码'
  `InstrumentID` text,  -- '合约代码'
  `InstrumentName` text,  -- '合约名称'
  `ProductID` text,  -- '品种代码'
  `ProductClass` int,  -- '产品类型'
  `VolumeMultiple` int,  -- '合约数量乘数'
  `PriceTick` double,  -- '报价单位'
  `MaxMarketOrderVolume` int,  -- '市价单最大下单量'
  `MinMarketOrderVolume` int,  -- '市价单最小下单量'
  `MaxLimitOrderVolume` int,  -- '限价单最大下单量'
  `MinLimitOrderVolume` int,  -- '限价单最小下单量'
  `SessionName` text,  -- '交易节名称'
  `DeliveryYear` int,  -- '交割年份'
  `DeliveryMonth` int,  -- '交割月'
  PRIMARY KEY(ExchangeID, InstrumentID)
);  -- '合约'


CREATE TABLE IF NOT EXISTS `t_Account` (
  `TradingDay` text,  -- '交易日'
  `AccountID` text,  -- '账户代码'
  `AccountName` text,  -- '账户名称'
  `Password` text,  -- '密码'
  `PreBalance` double,  -- '上次结算准备金'
  `Balance` double,  -- '结算准备金'
  `CloseProfitByDate` double,  -- '逐日平仓盈亏'
  `PositionProfitByDate` double,  -- '逐日持仓盈亏'
  `PositionProfitByTrade` double,  -- '逐笔持仓盈亏'
  `PremiumIn` double,  -- '期权权利金收入'
  `PremiumOut` double,  -- '期权权利金支出'
  `MarketValue` double,  -- '市值'
  PRIMARY KEY(AccountID)
);  -- '账户'


CREATE TABLE IF NOT EXISTS `t_Position` (
  `TradingDay` text,  -- '交易日'
  `AccountID` text,  -- '账户代码'
  `ExchangeID` text,  -- '交易所代码'
  `InstrumentID` text,  -- '合约代码'
  `ProductClass` int,  -- '产品类型'
  `PosiDirection` int,  -- '持仓方向'
  `TotalPosition` int,  -- '总持仓数量'
  `TodayPosition` int,  -- '今仓'
  `FrozenPosition` int,  -- '冻结持仓'
  `CloseProfitByDate` double,  -- '逐日盯市平仓盈亏'
  `CloseProfitByTrade` double,  -- '逐笔对冲平仓盈亏'
  `PositionProfitByDate` double,  -- '逐日盯市持仓盈亏'
  `PositionProfitByTrade` double,  -- '逐笔对冲持仓盈亏'
  `PremiumIn` double,  -- '期权权利金收入'
  `PremiumOut` double,  -- '期权权利金支出'
  `MarketValue` double,  -- '市值'
  `VolumeMultiple` int,  -- '数量乘数'
  `SettlementPrice` double,  -- '结算价'
  `PreSettlementPrice` double,  -- '昨结算价'
  PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection)
);  -- '持仓'
  CREATE INDEX Account ON t_Position(TradingDay, AccountID);


CREATE TABLE IF NOT EXISTS `t_Order` (
  `TradingDay` text,  -- '交易日'
  `AccountID` text,  -- '账户代码'
  `ExchangeID` text,  -- '交易所代码'
  `InstrumentID` text,  -- '合约代码'
  `OrderID` int,  -- '委托编号'
  `ClientOrderID` int,  -- '客户端委托编号'
  `Direction` int,  -- '买卖方向'
  `OffsetFlag` int,  -- '开平标志'
  `OrderPriceType` int,  -- '委托价格类型'
  `Price` double,  -- '委托价格'
  `Volume` int,  -- '委托数量'
  `VolumeRemain` int,  -- '剩余数量'
  `VolumeTraded` int,  -- '成交数量'
  `VolumeMultiple` int,  -- '合约数量乘数'
  `OrderStatus` int,  -- '委托状态'
  `OrderDate` text,  -- '委托日期'
  `OrderTime` text,  -- '委托时间'
  `CancelDate` text,  -- '撤单日期'
  `CancelTime` text,  -- '撤单时间'
  UNIQUE (TradingDay, AccountID, ExchangeID, InstrumentID, ClientOrderID),
  PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, OrderID)
);  -- '委托'


CREATE TABLE IF NOT EXISTS `t_Trade` (
  `TradingDay` text,  -- '交易日'
  `AccountID` text,  -- '账户代码'
  `ExchangeID` text,  -- '交易所代码'
  `InstrumentID` text,  -- '合约代码'
  `OrderID` int,  -- '委托编号'
  `TradeID` text,  -- '成交编号'
  `Direction` int,  -- '买卖方向'
  `OffsetFlag` int,  -- '开平标志'
  `Price` double,  -- '价格'
  `Volume` int,  -- '数量'
  `TradeAmount` double,  -- '成交金额'
  `PremiumIn` double,  -- '期权权利金收入'
  `PremiumOut` double,  -- '期权权利金支出'
  `TradeDate` text,  -- '成交日期'
  `TradeTime` text,  -- '成交时间'
  PRIMARY KEY(TradingDay, ExchangeID, TradeID, Direction)
);  -- '成交'


CREATE TABLE IF NOT EXISTS `t_MdTick` (
  `TradingDay` text,  -- '交易日'
  `ExchangeID` text,  -- '交易所代码'
  `InstrumentID` text,  -- '合约代码'
  `LastPrice` double,  -- '最新价'
  `PreSettlementPrice` double,  -- '昨结算价'
  `PreClosePrice` double,  -- '昨收盘价'
  `PreOpenInterest` int,  -- '昨持仓量'
  `OpenPrice` double,  -- '今开盘价'
  `HighestPrice` double,  -- '最高价'
  `LowestPrice` double,  -- '最低价'
  `Volume` int,  -- '总成交量'
  `Turnover` double,  -- '总成交金额'
  `OpenInterest` int,  -- '持仓量'
  `UpperLimitPrice` double,  -- '涨停板价'
  `LowerLimitPrice` double,  -- '跌停板价'
  `UpdateTime` text,  -- '最后修改时间'
  `UpdateMillisec` int,  -- '最后修改毫秒'
  `AskPrice1` double,  -- '申卖价一'
  `AskPrice2` double,  -- '申卖价二'
  `AskPrice3` double,  -- '申卖价三'
  `AskPrice4` double,  -- '申卖价四'
  `AskPrice5` double,  -- '申卖价五'
  `AskVolume1` int,  -- '申卖量一'
  `AskVolume2` int,  -- '申卖量二'
  `AskVolume3` int,  -- '申卖量三'
  `AskVolume4` int,  -- '申卖量四'
  `AskVolume5` int,  -- '申卖量五'
  `BidPrice1` double,  -- '申买价一'
  `BidPrice2` double,  -- '申买价二'
  `BidPrice3` double,  -- '申买价三'
  `BidPrice4` double,  -- '申买价四'
  `BidPrice5` double,  -- '申买价五'
  `BidVolume1` int,  -- '申买量一'
  `BidVolume2` int,  -- '申买量二'
  `BidVolume3` int,  -- '申买量三'
  `BidVolume4` int,  -- '申买量四'
  `BidVolume5` int,  -- '申买量五'
  `AveragePrice` double,  -- '当日均价'
  PRIMARY KEY(TradingDay, ExchangeID, InstrumentID)
);  -- 'Tick行情'



