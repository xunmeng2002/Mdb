
CREATE TABLE IF NOT EXISTS t_TradingDay (
  PK int,  -- '主键'
  CurrTradingDay varchar,  -- '当前交易日'
  PreTradingDay varchar,  -- '昨交易日'
  PRIMARY KEY(PK)
);  -- '交易日'


CREATE TABLE IF NOT EXISTS t_Exchange (
  ExchangeID varchar,  -- '交易所代码'
  ExchangeName varchar,  -- '交易所名称'
  PRIMARY KEY(ExchangeID)
);  -- '交易所'


CREATE TABLE IF NOT EXISTS t_Product (
  ExchangeID varchar,  -- '交易所代码'
  ProductID varchar,  -- '品种代码'
  ProductName varchar,  -- '品种名称'
  ProductClass int,  -- '品种类型'
  VolumeMultiple int,  -- '合约乘数'
  PriceTick double,  -- '最小变动价位'
  MaxMarketOrderVolume bigint,  -- '市价最大下单量'
  MinMarketOrderVolume bigint,  -- '市价最小下单量'
  MaxLimitOrderVolume bigint,  -- '限价最大下单量'
  MinLimitOrderVolume bigint,  -- '限价最小下单量'
  SessionName varchar,  -- '交易节名称'
  PRIMARY KEY(ExchangeID, ProductID)
);  -- '品种表'


CREATE TABLE IF NOT EXISTS t_Instrument (
  ExchangeID varchar,  -- '交易所代码'
  InstrumentID varchar,  -- '合约代码'
  ExchangeInstID varchar,  -- '交易所合约代码'
  InstrumentName varchar,  -- '合约名称'
  ProductID varchar,  -- '品种代码'
  ProductClass int,  -- '品种类型'
  InstrumentClass int,  -- '合约类别'
  Rank int,  -- '级别'
  VolumeMultiple int,  -- '合约乘数'
  PriceTick double,  -- '最小变动价位'
  MaxMarketOrderVolume bigint,  -- '市价最大下单量'
  MinMarketOrderVolume bigint,  -- '市价最小下单量'
  MaxLimitOrderVolume bigint,  -- '限价最大下单量'
  MinLimitOrderVolume bigint,  -- '限价最小下单量'
  SessionName varchar,  -- '交易节名称'
  PRIMARY KEY(ExchangeID, InstrumentID)
);  -- '合约'


CREATE TABLE IF NOT EXISTS t_PrimaryAccount (
  PrimaryAccountID varchar,  -- '主账户代码'
  PrimaryAccountName varchar,  -- '主账户名称'
  AccountClass int,  -- '账户类别'
  BrokerPassword varchar,  -- '经纪公司密码'
  OfferID int,  -- '报盘代码'
  IsAllowLogin int,  -- '是否允许登陆'
  IsSimulateAccount int,  -- '是否模拟账号'
  LoginStatus int,  -- '登录状态'
  InitStatus int,  -- '初始化状态'
  PRIMARY KEY(PrimaryAccountID)
);  -- '主账户'
  CREATE INDEX PrimaryAccountOfferID ON t_PrimaryAccount(OfferID);


CREATE TABLE IF NOT EXISTS t_Account (
  AccountID varchar,  -- '账户代码'
  AccountName varchar,  -- '账户名称'
  AccountType int,  -- '账户类型'
  AccountStatus int,  -- '账户状态'
  Password varchar,  -- '密码'
  TradeGroupID int,  -- '交易组代码'
  RiskGroupID int,  -- '交易组代码'
  CommissionGroupID int,  -- '交易组代码'
  PRIMARY KEY(AccountID)
);  -- '账户'


CREATE TABLE IF NOT EXISTS t_Capital (
  TradingDay varchar,  -- '交易日'
  AccountID varchar,  -- '账户代码'
  AccountType int,  -- '账户类型'
  Balance double,  -- '权益'
  PreBalance double,  -- '上日权益'
  Available double,  -- '可用资金'
  MarketValue double,  -- '市值'
  CashIn double,  -- '现金收入'
  CashOut double,  -- '现金支出'
  Margin double,  -- '保证金'
  Commission double,  -- '手续费'
  FrozenCash double,  -- '冻结资金'
  FrozenMargin double,  -- '冻结保证金'
  FrozenCommission double,  -- '冻结手续费'
  CloseProfitByDate double,  -- '逐日平仓盈亏'
  CloseProfitByTrade double,  -- '逐笔平仓盈亏'
  PositionProfitByDate double,  -- '逐日持仓盈亏'
  PositionProfitByTrade double,  -- '逐笔持仓盈亏'
  Deposit double,  -- '入金'
  Withdraw double,  -- '出金'
  PRIMARY KEY(TradingDay, AccountID)
);  -- '资金'
  CREATE INDEX CapitalTradingDay ON t_Capital(TradingDay);


CREATE TABLE IF NOT EXISTS t_Position (
  TradingDay varchar,  -- '交易日'
  AccountID varchar,  -- '账户代码'
  AccountType int,  -- '账户类型'
  ExchangeID varchar,  -- '交易所代码'
  InstrumentID varchar,  -- '合约代码'
  ProductClass int,  -- '品种类型'
  PosiDirection int,  -- '持仓方向'
  TotalPosition bigint,  -- '持仓数量'
  PositionFrozen bigint,  -- '冻结持仓'
  TodayPosition bigint,  -- '今日持仓'
  MarketValue double,  -- '市值'
  CashIn double,  -- '现金收入'
  CashOut double,  -- '现金支出'
  Margin double,  -- '保证金'
  Commission double,  -- '手续费'
  FrozenCash double,  -- '冻结资金'
  FrozenMargin double,  -- '冻结保证金'
  FrozenCommission double,  -- '冻结手续费'
  VolumeMultiple int,  -- '合约乘数'
  CloseProfitByDate double,  -- '逐日平仓盈亏'
  CloseProfitByTrade double,  -- '逐笔平仓盈亏'
  PositionProfitByDate double,  -- '逐日持仓盈亏'
  PositionProfitByTrade double,  -- '逐笔持仓盈亏'
  SettlementPrice double,  -- '结算价'
  PreSettlementPrice double,  -- '昨结算价'
  PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection)
);  -- '持仓'
  CREATE INDEX PositionAccount ON t_Position(TradingDay, AccountID);
  CREATE INDEX PositionTradingDay ON t_Position(TradingDay);


CREATE TABLE IF NOT EXISTS t_PositionDetail (
  TradingDay varchar,  -- '交易日'
  AccountID varchar,  -- '账户代码'
  AccountType int,  -- '账户类型'
  ExchangeID varchar,  -- '交易所代码'
  InstrumentID varchar,  -- '合约代码'
  ProductClass int,  -- '品种类型'
  PosiDirection int,  -- '持仓方向'
  OpenDate varchar,  -- '开仓日期'
  TradeID varchar,  -- '成交编号'
  Volume bigint,  -- '委托数量'
  OpenPrice double,  -- '开盘价'
  MarketValue double,  -- '市值'
  CashIn double,  -- '现金收入'
  CashOut double,  -- '现金支出'
  Margin double,  -- '保证金'
  Commission double,  -- '手续费'
  VolumeMultiple int,  -- '合约乘数'
  CloseProfitByDate double,  -- '逐日平仓盈亏'
  CloseProfitByTrade double,  -- '逐笔平仓盈亏'
  PositionProfitByDate double,  -- '逐日持仓盈亏'
  PositionProfitByTrade double,  -- '逐笔持仓盈亏'
  SettlementPrice double,  -- '结算价'
  PreSettlementPrice double,  -- '昨结算价'
  CloseVolume bigint,  -- '平仓数量'
  CloseAmount double,  -- '平仓金额'
  PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection, OpenDate, TradeID)
);  -- '持仓明细'
  CREATE INDEX PositionDetailTradeMatch ON t_PositionDetail(TradingDay, AccountID, ExchangeID, InstrumentID, PosiDirection);
  CREATE INDEX PositionDetailTradingDay ON t_PositionDetail(TradingDay);


CREATE TABLE IF NOT EXISTS t_Order (
  TradingDay varchar,  -- '交易日'
  AccountID varchar,  -- '账户代码'
  AccountType int,  -- '账户类型'
  ExchangeID varchar,  -- '交易所代码'
  InstrumentID varchar,  -- '合约代码'
  ProductClass int,  -- '品种类型'
  OrderID int,  -- '委托编号'
  OrderSysID varchar,  -- '系统委托编号'
  Direction int,  -- '买卖方向'
  OffsetFlag int,  -- '开平标志'
  OrderPriceType int,  -- '委托价格类型'
  Price double,  -- '委托价格'
  Volume bigint,  -- '委托数量'
  VolumeTotal bigint,  -- '剩余数量'
  VolumeTraded bigint,  -- '成交数量'
  VolumeMultiple int,  -- '合约乘数'
  OrderStatus int,  -- '委托状态'
  OrderDate varchar,  -- '委托日期'
  OrderTime varchar,  -- '委托时间'
  CancelDate varchar,  -- '撤单日期'
  CancelTime varchar,  -- '撤单时间'
  SessionID bigint,  -- '会话编号'
  ClientOrderID int,  -- '客户端委托编号'
  RequestID int,  -- '客户端请求编号'
  OfferID int,  -- '报盘代码'
  TradeGroupID int,  -- '交易组代码'
  RiskGroupID int,  -- '交易组代码'
  CommissionGroupID int,  -- '交易组代码'
  FrozenCash double,  -- '冻结资金'
  FrozenMargin double,  -- '冻结保证金'
  FrozenCommission double,  -- '冻结手续费'
  RebuildMark int,  -- '重建标志'
  IsForceClose int,  -- '是否强平单'
  UNIQUE (OrderTradingDay, OrderAccountID, OrderExchangeID, OrderInstrumentID, OrderSessionID, OrderClientOrderID),
  PRIMARY KEY(TradingDay, AccountID, ExchangeID, InstrumentID, OrderID)
);  -- '委托'


CREATE TABLE IF NOT EXISTS t_Trade (
  TradingDay varchar,  -- '交易日'
  AccountID varchar,  -- '账户代码'
  AccountType int,  -- '账户类型'
  ExchangeID varchar,  -- '交易所代码'
  InstrumentID varchar,  -- '合约代码'
  ProductClass int,  -- '品种类型'
  OrderID int,  -- '委托编号'
  OrderSysID varchar,  -- '系统委托编号'
  TradeID varchar,  -- '成交编号'
  Direction int,  -- '买卖方向'
  OffsetFlag int,  -- '开平标志'
  Price double,  -- '委托价格'
  Volume bigint,  -- '委托数量'
  VolumeMultiple int,  -- '合约乘数'
  TradeAmount double,  -- '成交金额'
  Commission double,  -- '手续费'
  TradeDate varchar,  -- '成交日期'
  TradeTime varchar,  -- '成交时间'
  PRIMARY KEY(TradingDay, ExchangeID, TradeID, Direction)
);  -- '成交'



