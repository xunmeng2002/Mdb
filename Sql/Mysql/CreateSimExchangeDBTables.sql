
CREATE TABLE IF NOT EXISTS `t_TradingDay` (
  `PK` int COMMENT '主键',
  `CurrTradingDay` char(9) COMMENT '当前交易日',
  `PreTradingDay` char(9) COMMENT '上一交易日',

  PRIMARY KEY(PK)
) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin' COMMENT='交易日';

CREATE TABLE IF NOT EXISTS `t_Exchange` (
  `ExchangeID` char(8) COMMENT '交易所代码',
  `ExchangeName` char(64) COMMENT '交易所名称',

  PRIMARY KEY(ExchangeID)
) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin' COMMENT='交易所';

CREATE TABLE IF NOT EXISTS `t_Product` (
  `ExchangeID` char(8) COMMENT '交易所代码',
  `ProductID` char(32) COMMENT '品种代码',
  `ProductName` char(32) COMMENT '品种名称',
  `ProductClass` int COMMENT '产品类型',
  `VolumeMultiple` int COMMENT '数量乘数',
  `PriceTick` decimal(24,8) COMMENT '报价单位',
  `MaxMarketOrderVolume` int COMMENT '市价单最大下单量',
  `MinMarketOrderVolume` int COMMENT '市价单最小下单量',
  `MaxLimitOrderVolume` int COMMENT '限价单最大下单量',
  `MinLimitOrderVolume` int COMMENT '限价单最小下单量',
  `SessionName` char(32) COMMENT '交易节名称',

  PRIMARY KEY(ExchangeID, ProductID)
) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin' COMMENT='品种表';

CREATE TABLE IF NOT EXISTS `t_Instrument` (
  `ExchangeID` char(8) COMMENT '交易所代码',
  `InstrumentID` char(32) COMMENT '合约代码',
  `InstrumentName` char(64) COMMENT '合约名称',
  `ProductID` char(32) COMMENT '品种代码',
  `ProductClass` int COMMENT '产品类型',
  `VolumeMultiple` int COMMENT '合约数量乘数',
  `PriceTick` decimal(24,8) COMMENT '报价单位',
  `MaxMarketOrderVolume` int COMMENT '市价单最大下单量',
  `MinMarketOrderVolume` int COMMENT '市价单最小下单量',
  `MaxLimitOrderVolume` int COMMENT '限价单最大下单量',
  `MinLimitOrderVolume` int COMMENT '限价单最小下单量',
  `SessionName` char(32) COMMENT '交易节名称',
  `DeliveryYear` int COMMENT '交割年份',
  `DeliveryMonth` int COMMENT '交割月',

  PRIMARY KEY(ExchangeID, InstrumentID)
) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin' COMMENT='合约';

CREATE TABLE IF NOT EXISTS `t_Account` (
  `TradingDay` char(9) COMMENT '交易日',
  `AccountID` char(32) COMMENT '账户代码',
  `AccountName` char(64) COMMENT '账户名称',
  `Password` char(64) COMMENT '密码',
  `PreBalance` decimal(24,8) COMMENT '上次结算准备金',
  `Balance` decimal(24,8) COMMENT '结算准备金',
  `CloseProfitByDate` decimal(24,8) COMMENT '逐日平仓盈亏',
  `PositionProfitByDate` decimal(24,8) COMMENT '逐日持仓盈亏',
  `PositionProfitByTrade` decimal(24,8) COMMENT '逐笔持仓盈亏',
  `PremiumIn` decimal(24,8) COMMENT '期权权利金收入',
  `PremiumOut` decimal(24,8) COMMENT '期权权利金支出',
  `MarketValue` decimal(24,8) COMMENT '市值',

  PRIMARY KEY(AccountID)
) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin' COMMENT='账户';

CREATE TABLE IF NOT EXISTS `t_Position` (
  `TradingDay` char(9) COMMENT '交易日',
  `AccountID` char(32) COMMENT '账户代码',
  `ExchangeID` char(8) COMMENT '交易所代码',
  `InstrumentID` char(32) COMMENT '合约代码',
  `ProductClass` int COMMENT '产品类型',
  `PosiDirection` int COMMENT '持仓方向',
  `TotalPosition` int COMMENT '总持仓数量',
  `TodayPosition` int COMMENT '今仓',
  `FrozenPosition` int COMMENT '冻结持仓',
  `CloseProfitByDate` decimal(24,8) COMMENT '逐日盯市平仓盈亏',
  `CloseProfitByTrade` decimal(24,8) COMMENT '逐笔对冲平仓盈亏',
  `PositionProfitByDate` decimal(24,8) COMMENT '逐日盯市持仓盈亏',
  `PositionProfitByTrade` decimal(24,8) COMMENT '逐笔对冲持仓盈亏',
  `PremiumIn` decimal(24,8) COMMENT '期权权利金收入',
  `PremiumOut` decimal(24,8) COMMENT '期权权利金支出',
  `MarketValue` decimal(24,8) COMMENT '市值',
  `VolumeMultiple` int COMMENT '数量乘数',
  `SettlementPrice` decimal(24,8) COMMENT '结算价',
  `PreSettlementPrice` decimal(24,8) COMMENT '昨结算价',

  INDEX Account(TradingDay, AccountID),
  PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection)
) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin' COMMENT='持仓';

CREATE TABLE IF NOT EXISTS `t_Order` (
  `TradingDay` char(9) COMMENT '交易日',
  `AccountID` char(32) COMMENT '账户代码',
  `ExchangeID` char(8) COMMENT '交易所代码',
  `InstrumentID` char(32) COMMENT '合约代码',
  `OrderID` int COMMENT '委托编号',
  `ClientOrderID` int COMMENT '客户端委托编号',
  `Direction` int COMMENT '买卖方向',
  `OffsetFlag` int COMMENT '开平标志',
  `OrderPriceType` int COMMENT '委托价格类型',
  `Price` decimal(24,8) COMMENT '委托价格',
  `Volume` int COMMENT '委托数量',
  `VolumeRemain` int COMMENT '剩余数量',
  `VolumeTraded` int COMMENT '成交数量',
  `VolumeMultiple` int COMMENT '合约数量乘数',
  `OrderStatus` int COMMENT '委托状态',
  `OrderDate` char(9) COMMENT '委托日期',
  `OrderTime` char(9) COMMENT '委托时间',
  `CancelDate` char(9) COMMENT '撤单日期',
  `CancelTime` char(9) COMMENT '撤单时间',

  UNIQUE ClientOrderID(TradingDay, AccountID, ExchangeID, InstrumentID, ClientOrderID),
  PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, OrderID)
) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin' COMMENT='委托';

CREATE TABLE IF NOT EXISTS `t_Trade` (
  `TradingDay` char(9) COMMENT '交易日',
  `AccountID` char(32) COMMENT '账户代码',
  `ExchangeID` char(8) COMMENT '交易所代码',
  `InstrumentID` char(32) COMMENT '合约代码',
  `OrderID` int COMMENT '委托编号',
  `TradeID` char(64) COMMENT '成交编号',
  `Direction` int COMMENT '买卖方向',
  `OffsetFlag` int COMMENT '开平标志',
  `Price` decimal(24,8) COMMENT '价格',
  `Volume` int COMMENT '数量',
  `TradeAmount` decimal(24,8) COMMENT '成交金额',
  `PremiumIn` decimal(24,8) COMMENT '期权权利金收入',
  `PremiumOut` decimal(24,8) COMMENT '期权权利金支出',
  `TradeDate` char(9) COMMENT '成交日期',
  `TradeTime` char(9) COMMENT '成交时间',

  PRIMARY KEY(TradingDay, ExchangeID, TradeID, Direction)
) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin' COMMENT='成交';

CREATE TABLE IF NOT EXISTS `t_MdTick` (
  `TradingDay` char(9) COMMENT '交易日',
  `ExchangeID` char(8) COMMENT '交易所代码',
  `InstrumentID` char(32) COMMENT '合约代码',
  `LastPrice` decimal(24,8) COMMENT '最新价',
  `PreSettlementPrice` decimal(24,8) COMMENT '昨结算价',
  `PreClosePrice` decimal(24,8) COMMENT '昨收盘价',
  `PreOpenInterest` int COMMENT '昨持仓量',
  `OpenPrice` decimal(24,8) COMMENT '今开盘价',
  `HighestPrice` decimal(24,8) COMMENT '最高价',
  `LowestPrice` decimal(24,8) COMMENT '最低价',
  `Volume` int COMMENT '总成交量',
  `Turnover` decimal(24,8) COMMENT '总成交金额',
  `OpenInterest` int COMMENT '持仓量',
  `UpperLimitPrice` decimal(24,8) COMMENT '涨停板价',
  `LowerLimitPrice` decimal(24,8) COMMENT '跌停板价',
  `UpdateTime` char(9) COMMENT '最后修改时间',
  `UpdateMillisec` int COMMENT '最后修改毫秒',
  `AskPrice1` decimal(24,8) COMMENT '申卖价一',
  `AskPrice2` decimal(24,8) COMMENT '申卖价二',
  `AskPrice3` decimal(24,8) COMMENT '申卖价三',
  `AskPrice4` decimal(24,8) COMMENT '申卖价四',
  `AskPrice5` decimal(24,8) COMMENT '申卖价五',
  `AskVolume1` int COMMENT '申卖量一',
  `AskVolume2` int COMMENT '申卖量二',
  `AskVolume3` int COMMENT '申卖量三',
  `AskVolume4` int COMMENT '申卖量四',
  `AskVolume5` int COMMENT '申卖量五',
  `BidPrice1` decimal(24,8) COMMENT '申买价一',
  `BidPrice2` decimal(24,8) COMMENT '申买价二',
  `BidPrice3` decimal(24,8) COMMENT '申买价三',
  `BidPrice4` decimal(24,8) COMMENT '申买价四',
  `BidPrice5` decimal(24,8) COMMENT '申买价五',
  `BidVolume1` int COMMENT '申买量一',
  `BidVolume2` int COMMENT '申买量二',
  `BidVolume3` int COMMENT '申买量三',
  `BidVolume4` int COMMENT '申买量四',
  `BidVolume5` int COMMENT '申买量五',
  `AveragePrice` decimal(24,8) COMMENT '当日均价',

  PRIMARY KEY(TradingDay, ExchangeID, InstrumentID)
) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin' COMMENT='Tick行情';


