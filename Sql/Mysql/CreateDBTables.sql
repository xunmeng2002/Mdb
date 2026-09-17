-- 本文件由 ../Templates/Sql/Mysql/CreateTables.sql.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py

CREATE TABLE IF NOT EXISTS `TradingDay` (
  `Pk` int COMMENT '主键',
  `CurrTradingDay` char(16) COMMENT '当前交易日',
  `PreTradingDay` char(16) COMMENT '昨交易日',

  PRIMARY KEY(Pk)
) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin' COMMENT='交易日';

CREATE TABLE IF NOT EXISTS `Exchange` (
  `ExchangeId` char(8) COMMENT '交易所代码',
  `ExchangeName` char(64) COMMENT '交易所名称',

  PRIMARY KEY(ExchangeId)
) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin' COMMENT='交易所';

CREATE TABLE IF NOT EXISTS `Product` (
  `ExchangeId` char(8) COMMENT '交易所代码',
  `ProductId` char(32) COMMENT '品种代码',
  `ProductName` char(32) COMMENT '品种名称',
  `ProductClass` int COMMENT '品种类型',
  `VolumeMultiple` int COMMENT '合约乘数',
  `PriceTick` decimal(24,8) COMMENT '最小变动价位',
  `MaxMarketOrderVolume` bigint COMMENT '市价最大下单量',
  `MinMarketOrderVolume` bigint COMMENT '市价最小下单量',
  `MaxLimitOrderVolume` bigint COMMENT '限价最大下单量',
  `MinLimitOrderVolume` bigint COMMENT '限价最小下单量',
  `SessionName` char(32) COMMENT '交易节名称',

  PRIMARY KEY(ExchangeId, ProductId)
) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin' COMMENT='品种表';

CREATE TABLE IF NOT EXISTS `Instrument` (
  `ExchangeId` char(8) COMMENT '交易所代码',
  `InstrumentId` char(32) COMMENT '合约代码',
  `ExchangeInstId` char(32) COMMENT '交易所合约代码',
  `InstrumentName` char(64) COMMENT '合约名称',
  `ProductId` char(32) COMMENT '品种代码',
  `ProductClass` int COMMENT '品种类型',
  `InstrumentClass` int COMMENT '合约类别',
  `Rank` int COMMENT '级别',
  `VolumeMultiple` int COMMENT '合约乘数',
  `PriceTick` decimal(24,8) COMMENT '最小变动价位',
  `MaxMarketOrderVolume` bigint COMMENT '市价最大下单量',
  `MinMarketOrderVolume` bigint COMMENT '市价最小下单量',
  `MaxLimitOrderVolume` bigint COMMENT '限价最大下单量',
  `MinLimitOrderVolume` bigint COMMENT '限价最小下单量',
  `SessionName` char(32) COMMENT '交易节名称',

  PRIMARY KEY(ExchangeId, InstrumentId)
) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin' COMMENT='合约';

CREATE TABLE IF NOT EXISTS `PrimaryAccount` (
  `PrimaryAccountId` char(32) COMMENT '主账户代码',
  `PrimaryAccountName` char(64) COMMENT '主账户名称',
  `AccountClass` int COMMENT '账户类别',
  `BrokerPassword` char(64) COMMENT '经纪公司密码',
  `OfferId` int COMMENT '报盘代码',
  `IsAllowLogin` bool COMMENT '是否允许登陆',
  `IsSimulateAccount` bool COMMENT '是否模拟账号',
  `LoginStatus` int COMMENT '登录状态',
  `InitStatus` int COMMENT '初始化状态',

  INDEX OfferId(OfferId),
  PRIMARY KEY(PrimaryAccountId)
) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin' COMMENT='主账户';

CREATE TABLE IF NOT EXISTS `Account` (
  `AccountId` char(32) COMMENT '账户代码',
  `AccountName` char(64) COMMENT '账户名称',
  `AccountType` int COMMENT '账户类型',
  `AccountStatus` int COMMENT '账户状态',
  `Password` char(64) COMMENT '密码',
  `TradeGroupId` int COMMENT '交易组代码',
  `RiskGroupId` int COMMENT '交易组代码',
  `CommissionGroupId` int COMMENT '交易组代码',

  PRIMARY KEY(AccountId)
) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin' COMMENT='账户';

CREATE TABLE IF NOT EXISTS `Capital` (
  `TradingDay` char(16) COMMENT '交易日',
  `AccountId` char(32) COMMENT '账户代码',
  `AccountType` int COMMENT '账户类型',
  `Balance` decimal(24,8) COMMENT '权益',
  `PreBalance` decimal(24,8) COMMENT '上日权益',
  `Available` decimal(24,8) COMMENT '可用资金',
  `MarketValue` decimal(24,8) COMMENT '市值',
  `CashIn` decimal(24,8) COMMENT '现金收入',
  `CashOut` decimal(24,8) COMMENT '现金支出',
  `Margin` decimal(24,8) COMMENT '保证金',
  `Commission` decimal(24,8) COMMENT '手续费',
  `FrozenCash` decimal(24,8) COMMENT '冻结资金',
  `FrozenMargin` decimal(24,8) COMMENT '冻结保证金',
  `FrozenCommission` decimal(24,8) COMMENT '冻结手续费',
  `CloseProfitByDate` decimal(24,8) COMMENT '逐日平仓盈亏',
  `CloseProfitByTrade` decimal(24,8) COMMENT '逐笔平仓盈亏',
  `PositionProfitByDate` decimal(24,8) COMMENT '逐日持仓盈亏',
  `PositionProfitByTrade` decimal(24,8) COMMENT '逐笔持仓盈亏',
  `Deposit` decimal(24,8) COMMENT '入金',
  `Withdraw` decimal(24,8) COMMENT '出金',

  INDEX TradingDay(TradingDay),
  PRIMARY KEY(TradingDay, AccountId)
) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin' COMMENT='资金';

CREATE TABLE IF NOT EXISTS `Position` (
  `TradingDay` char(16) COMMENT '交易日',
  `AccountId` char(32) COMMENT '账户代码',
  `AccountType` int COMMENT '账户类型',
  `ExchangeId` char(8) COMMENT '交易所代码',
  `InstrumentId` char(32) COMMENT '合约代码',
  `ProductClass` int COMMENT '品种类型',
  `PosiDirection` int COMMENT '持仓方向',
  `TotalPosition` bigint COMMENT '持仓数量',
  `PositionFrozen` bigint COMMENT '冻结持仓',
  `TodayPosition` bigint COMMENT '今日持仓',
  `MarketValue` decimal(24,8) COMMENT '市值',
  `CashIn` decimal(24,8) COMMENT '现金收入',
  `CashOut` decimal(24,8) COMMENT '现金支出',
  `Margin` decimal(24,8) COMMENT '保证金',
  `Commission` decimal(24,8) COMMENT '手续费',
  `FrozenCash` decimal(24,8) COMMENT '冻结资金',
  `FrozenMargin` decimal(24,8) COMMENT '冻结保证金',
  `FrozenCommission` decimal(24,8) COMMENT '冻结手续费',
  `VolumeMultiple` int COMMENT '合约乘数',
  `CloseProfitByDate` decimal(24,8) COMMENT '逐日平仓盈亏',
  `CloseProfitByTrade` decimal(24,8) COMMENT '逐笔平仓盈亏',
  `PositionProfitByDate` decimal(24,8) COMMENT '逐日持仓盈亏',
  `PositionProfitByTrade` decimal(24,8) COMMENT '逐笔持仓盈亏',
  `SettlementPrice` decimal(24,8) COMMENT '结算价',
  `PreSettlementPrice` decimal(24,8) COMMENT '昨结算价',

  INDEX Account(TradingDay, AccountId),
  INDEX TradingDay(TradingDay),
  PRIMARY KEY(TradingDay, AccountId, ExchangeId, InstrumentId, PosiDirection)
) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin' COMMENT='持仓';

CREATE TABLE IF NOT EXISTS `PositionDetail` (
  `TradingDay` char(16) COMMENT '交易日',
  `AccountId` char(32) COMMENT '账户代码',
  `AccountType` int COMMENT '账户类型',
  `ExchangeId` char(8) COMMENT '交易所代码',
  `InstrumentId` char(32) COMMENT '合约代码',
  `ProductClass` int COMMENT '品种类型',
  `PosiDirection` int COMMENT '持仓方向',
  `OpenDate` char(16) COMMENT '开仓日期',
  `TradeId` char(64) COMMENT '成交编号',
  `Volume` bigint COMMENT '委托数量',
  `OpenPrice` decimal(24,8) COMMENT '开盘价',
  `MarketValue` decimal(24,8) COMMENT '市值',
  `CashIn` decimal(24,8) COMMENT '现金收入',
  `CashOut` decimal(24,8) COMMENT '现金支出',
  `Margin` decimal(24,8) COMMENT '保证金',
  `Commission` decimal(24,8) COMMENT '手续费',
  `VolumeMultiple` int COMMENT '合约乘数',
  `CloseProfitByDate` decimal(24,8) COMMENT '逐日平仓盈亏',
  `CloseProfitByTrade` decimal(24,8) COMMENT '逐笔平仓盈亏',
  `PositionProfitByDate` decimal(24,8) COMMENT '逐日持仓盈亏',
  `PositionProfitByTrade` decimal(24,8) COMMENT '逐笔持仓盈亏',
  `SettlementPrice` decimal(24,8) COMMENT '结算价',
  `PreSettlementPrice` decimal(24,8) COMMENT '昨结算价',
  `CloseVolume` bigint COMMENT '平仓数量',
  `CloseAmount` decimal(24,8) COMMENT '平仓金额',

  INDEX TradeMatch(TradingDay, AccountId, ExchangeId, InstrumentId, PosiDirection),
  INDEX TradingDay(TradingDay),
  PRIMARY KEY(TradingDay, AccountId, ExchangeId, InstrumentId, PosiDirection, OpenDate, TradeId)
) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin' COMMENT='持仓明细';

CREATE TABLE IF NOT EXISTS `Order` (
  `TradingDay` char(16) COMMENT '交易日',
  `AccountId` char(32) COMMENT '账户代码',
  `AccountType` int COMMENT '账户类型',
  `ExchangeId` char(8) COMMENT '交易所代码',
  `InstrumentId` char(32) COMMENT '合约代码',
  `ProductClass` int COMMENT '品种类型',
  `OrderId` int COMMENT '委托编号',
  `OrderSysId` char(64) COMMENT '系统委托编号',
  `Direction` int COMMENT '买卖方向',
  `OffsetFlag` int COMMENT '开平标志',
  `OrderPriceType` int COMMENT '委托价格类型',
  `Price` decimal(24,8) COMMENT '委托价格',
  `Volume` bigint COMMENT '委托数量',
  `VolumeTotal` bigint COMMENT '剩余数量',
  `VolumeTraded` bigint COMMENT '成交数量',
  `VolumeMultiple` int COMMENT '合约乘数',
  `OrderStatus` int COMMENT '委托状态',
  `OrderDate` char(16) COMMENT '委托日期',
  `OrderTime` char(16) COMMENT '委托时间',
  `CancelDate` char(16) COMMENT '撤单日期',
  `CancelTime` char(16) COMMENT '撤单时间',
  `SessionId` bigint COMMENT '会话编号',
  `ClientOrderId` int COMMENT '客户端委托编号',
  `RequestId` int COMMENT '客户端请求编号',
  `OfferId` int COMMENT '报盘代码',
  `TradeGroupId` int COMMENT '交易组代码',
  `RiskGroupId` int COMMENT '交易组代码',
  `CommissionGroupId` int COMMENT '交易组代码',
  `FrozenCash` decimal(24,8) COMMENT '冻结资金',
  `FrozenMargin` decimal(24,8) COMMENT '冻结保证金',
  `FrozenCommission` decimal(24,8) COMMENT '冻结手续费',
  `RebuildMark` bool COMMENT '重建标志',
  `IsForceClose` bool COMMENT '是否强平单',

  UNIQUE ClientOrderId(TradingDay, AccountId, ExchangeId, InstrumentId, SessionId, ClientOrderId),
  PRIMARY KEY(TradingDay, AccountId, ExchangeId, InstrumentId, OrderId)
) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin' COMMENT='委托';

CREATE TABLE IF NOT EXISTS `Trade` (
  `TradingDay` char(16) COMMENT '交易日',
  `AccountId` char(32) COMMENT '账户代码',
  `AccountType` int COMMENT '账户类型',
  `ExchangeId` char(8) COMMENT '交易所代码',
  `InstrumentId` char(32) COMMENT '合约代码',
  `ProductClass` int COMMENT '品种类型',
  `OrderId` int COMMENT '委托编号',
  `OrderSysId` char(64) COMMENT '系统委托编号',
  `TradeId` char(64) COMMENT '成交编号',
  `Direction` int COMMENT '买卖方向',
  `OffsetFlag` int COMMENT '开平标志',
  `Price` decimal(24,8) COMMENT '委托价格',
  `Volume` bigint COMMENT '委托数量',
  `VolumeMultiple` int COMMENT '合约乘数',
  `TradeAmount` decimal(24,8) COMMENT '成交金额',
  `Commission` decimal(24,8) COMMENT '手续费',
  `TradeDate` char(16) COMMENT '成交日期',
  `TradeTime` char(16) COMMENT '成交时间',

  PRIMARY KEY(TradingDay, ExchangeId, TradeId, Direction)
) ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin' COMMENT='成交';


