#pragma once
#include "Types.h"

namespace mdb
{
	class TradingDay
	{
	public:
		static constexpr unsigned int TableID = 0x0001;
		//主键
		IntType PK;
		//当前交易日
		DateType CurrTradingDay;
		//上一交易日
		DateType PreTradingDay;
		
		static TradingDay* Allocate();
		void Free();
		const char* GetString() const;
		int GetSqlString(char* buff) const;
		const char* GetDebugString() const;
	};
	class Exchange
	{
	public:
		static constexpr unsigned int TableID = 0x0002;
		//交易所代码
		ExchangeIDType ExchangeID;
		//交易所名称
		ExchangeNameType ExchangeName;
		
		static Exchange* Allocate();
		void Free();
		const char* GetString() const;
		int GetSqlString(char* buff) const;
		const char* GetDebugString() const;
	};
	class Product
	{
	public:
		static constexpr unsigned int TableID = 0x0003;
		//交易所代码
		ExchangeIDType ExchangeID;
		//品种代码
		ProductIDType ProductID;
		//品种名称
		ProductNameType ProductName;
		//产品类型
		ProductClassType ProductClass;
		//数量乘数
		VolumeMultipleType VolumeMultiple;
		//报价单位
		PriceType PriceTick;
		//市价单最大下单量
		VolumeType MaxMarketOrderVolume;
		//市价单最小下单量
		VolumeType MinMarketOrderVolume;
		//限价单最大下单量
		VolumeType MaxLimitOrderVolume;
		//限价单最小下单量
		VolumeType MinLimitOrderVolume;
		//交易节名称
		SessionNameType SessionName;
		
		static Product* Allocate();
		void Free();
		const char* GetString() const;
		int GetSqlString(char* buff) const;
		const char* GetDebugString() const;
	};
	class Instrument
	{
	public:
		static constexpr unsigned int TableID = 0x0004;
		//交易所代码
		ExchangeIDType ExchangeID;
		//合约代码
		InstrumentIDType InstrumentID;
		//合约名称
		InstrumentNameType InstrumentName;
		//品种代码
		ProductIDType ProductID;
		//产品类型
		ProductClassType ProductClass;
		//合约数量乘数
		VolumeMultipleType VolumeMultiple;
		//报价单位
		PriceType PriceTick;
		//市价单最大下单量
		VolumeType MaxMarketOrderVolume;
		//市价单最小下单量
		VolumeType MinMarketOrderVolume;
		//限价单最大下单量
		VolumeType MaxLimitOrderVolume;
		//限价单最小下单量
		VolumeType MinLimitOrderVolume;
		//交易节名称
		SessionNameType SessionName;
		//交割年份
		IntType DeliveryYear;
		//交割月
		IntType DeliveryMonth;
		
		static Instrument* Allocate();
		void Free();
		const char* GetString() const;
		int GetSqlString(char* buff) const;
		const char* GetDebugString() const;
	};
	class Account
	{
	public:
		static constexpr unsigned int TableID = 0x0005;
		//交易日
		DateType TradingDay;
		//账户代码
		AccountIDType AccountID;
		//账户名称
		AccountNameType AccountName;
		//密码
		PasswordType Password;
		//上次结算准备金
		MoneyType PreBalance;
		//结算准备金
		MoneyType Balance;
		//逐日平仓盈亏
		MoneyType CloseProfitByDate;
		//逐日持仓盈亏
		MoneyType PositionProfitByDate;
		//逐笔持仓盈亏
		MoneyType PositionProfitByTrade;
		//期权权利金收入
		MoneyType PremiumIn;
		//期权权利金支出
		MoneyType PremiumOut;
		//市值
		MoneyType MarketValue;
		
		static Account* Allocate();
		void Free();
		const char* GetString() const;
		int GetSqlString(char* buff) const;
		const char* GetDebugString() const;
	};
	class Position
	{
	public:
		static constexpr unsigned int TableID = 0x0006;
		//交易日
		DateType TradingDay;
		//账户代码
		AccountIDType AccountID;
		//交易所代码
		ExchangeIDType ExchangeID;
		//合约代码
		InstrumentIDType InstrumentID;
		//产品类型
		ProductClassType ProductClass;
		//持仓方向
		PosiDirectionType PosiDirection;
		//总持仓数量
		VolumeType TotalPosition;
		//今仓
		VolumeType TodayPosition;
		//冻结持仓
		VolumeType FrozenPosition;
		//逐日盯市平仓盈亏
		MoneyType CloseProfitByDate;
		//逐笔对冲平仓盈亏
		MoneyType CloseProfitByTrade;
		//逐日盯市持仓盈亏
		MoneyType PositionProfitByDate;
		//逐笔对冲持仓盈亏
		MoneyType PositionProfitByTrade;
		//期权权利金收入
		MoneyType PremiumIn;
		//期权权利金支出
		MoneyType PremiumOut;
		//市值
		MoneyType MarketValue;
		//数量乘数
		VolumeMultipleType VolumeMultiple;
		//结算价
		PriceType SettlementPrice;
		//昨结算价
		PriceType PreSettlementPrice;
		
		static Position* Allocate();
		void Free();
		const char* GetString() const;
		int GetSqlString(char* buff) const;
		const char* GetDebugString() const;
	};
	class Order
	{
	public:
		static constexpr unsigned int TableID = 0x0007;
		//交易日
		DateType TradingDay;
		//账户代码
		AccountIDType AccountID;
		//交易所代码
		ExchangeIDType ExchangeID;
		//合约代码
		InstrumentIDType InstrumentID;
		//委托编号
		OrderIDType OrderID;
		//客户端委托编号
		OrderIDType ClientOrderID;
		//买卖方向
		DirectionType Direction;
		//开平标志
		OffsetFlagType OffsetFlag;
		//委托价格类型
		OrderPriceTypeType OrderPriceType;
		//委托价格
		PriceType Price;
		//委托数量
		VolumeType Volume;
		//剩余数量
		VolumeType VolumeRemain;
		//成交数量
		VolumeType VolumeTraded;
		//合约数量乘数
		VolumeMultipleType VolumeMultiple;
		//委托状态
		OrderStatusType OrderStatus;
		//委托日期
		DateType OrderDate;
		//委托时间
		TimeType OrderTime;
		//撤单日期
		DateType CancelDate;
		//撤单时间
		TimeType CancelTime;
		
		static Order* Allocate();
		void Free();
		const char* GetString() const;
		int GetSqlString(char* buff) const;
		const char* GetDebugString() const;
	};
	class Trade
	{
	public:
		static constexpr unsigned int TableID = 0x0008;
		//交易日
		DateType TradingDay;
		//账户代码
		AccountIDType AccountID;
		//交易所代码
		ExchangeIDType ExchangeID;
		//合约代码
		InstrumentIDType InstrumentID;
		//委托编号
		OrderIDType OrderID;
		//成交编号
		TradeIDType TradeID;
		//买卖方向
		DirectionType Direction;
		//开平标志
		OffsetFlagType OffsetFlag;
		//价格
		PriceType Price;
		//数量
		VolumeType Volume;
		//成交金额
		MoneyType TradeAmount;
		//期权权利金收入
		MoneyType PremiumIn;
		//期权权利金支出
		MoneyType PremiumOut;
		//成交日期
		DateType TradeDate;
		//成交时间
		TimeType TradeTime;
		
		static Trade* Allocate();
		void Free();
		const char* GetString() const;
		int GetSqlString(char* buff) const;
		const char* GetDebugString() const;
	};
	class MdTick
	{
	public:
		static constexpr unsigned int TableID = 0x0009;
		//交易日
		DateType TradingDay;
		//交易所代码
		ExchangeIDType ExchangeID;
		//合约代码
		InstrumentIDType InstrumentID;
		//最新价
		PriceType LastPrice;
		//昨结算价
		PriceType PreSettlementPrice;
		//昨收盘价
		PriceType PreClosePrice;
		//昨持仓量
		VolumeType PreOpenInterest;
		//今开盘价
		PriceType OpenPrice;
		//最高价
		PriceType HighestPrice;
		//最低价
		PriceType LowestPrice;
		//总成交量
		VolumeType Volume;
		//总成交金额
		MoneyType Turnover;
		//持仓量
		VolumeType OpenInterest;
		//涨停板价
		PriceType UpperLimitPrice;
		//跌停板价
		PriceType LowerLimitPrice;
		//最后修改时间
		TimeType UpdateTime;
		//最后修改毫秒
		IntType UpdateMillisec;
		//申卖价一
		PriceType AskPrice1;
		//申卖价二
		PriceType AskPrice2;
		//申卖价三
		PriceType AskPrice3;
		//申卖价四
		PriceType AskPrice4;
		//申卖价五
		PriceType AskPrice5;
		//申卖量一
		VolumeType AskVolume1;
		//申卖量二
		VolumeType AskVolume2;
		//申卖量三
		VolumeType AskVolume3;
		//申卖量四
		VolumeType AskVolume4;
		//申卖量五
		VolumeType AskVolume5;
		//申买价一
		PriceType BidPrice1;
		//申买价二
		PriceType BidPrice2;
		//申买价三
		PriceType BidPrice3;
		//申买价四
		PriceType BidPrice4;
		//申买价五
		PriceType BidPrice5;
		//申买量一
		VolumeType BidVolume1;
		//申买量二
		VolumeType BidVolume2;
		//申买量三
		VolumeType BidVolume3;
		//申买量四
		VolumeType BidVolume4;
		//申买量五
		VolumeType BidVolume5;
		//当日均价
		PriceType AveragePrice;
		
		static MdTick* Allocate();
		void Free();
		const char* GetString() const;
		int GetSqlString(char* buff) const;
		const char* GetDebugString() const;
	};

	extern thread_local TradingDay t_CompareTradingDay;
	extern thread_local Exchange t_CompareExchange;
	extern thread_local Product t_CompareProduct;
	extern thread_local Instrument t_CompareInstrument;
	extern thread_local Account t_CompareAccount;
	extern thread_local Position t_ComparePosition;
	extern thread_local Order t_CompareOrder;
	extern thread_local Trade t_CompareTrade;
	extern thread_local MdTick t_CompareMdTick;

}

