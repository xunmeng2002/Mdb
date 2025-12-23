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
		//昨交易日
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
		//品种类型
		ProductClassType ProductClass;
		//合约乘数
		VolumeMultipleType VolumeMultiple;
		//最小变动价位
		PriceType PriceTick;
		//市价最大下单量
		VolumeType MaxMarketOrderVolume;
		//市价最小下单量
		VolumeType MinMarketOrderVolume;
		//限价最大下单量
		VolumeType MaxLimitOrderVolume;
		//限价最小下单量
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
		//交易所合约代码
		InstrumentIDType ExchangeInstID;
		//合约名称
		InstrumentNameType InstrumentName;
		//品种代码
		ProductIDType ProductID;
		//品种类型
		ProductClassType ProductClass;
		//合约类别
		InstrumentClassType InstrumentClass;
		//级别
		IntType Rank;
		//合约乘数
		VolumeMultipleType VolumeMultiple;
		//最小变动价位
		PriceType PriceTick;
		//市价最大下单量
		VolumeType MaxMarketOrderVolume;
		//市价最小下单量
		VolumeType MinMarketOrderVolume;
		//限价最大下单量
		VolumeType MaxLimitOrderVolume;
		//限价最小下单量
		VolumeType MinLimitOrderVolume;
		//交易节名称
		SessionNameType SessionName;
		
		static Instrument* Allocate();
		void Free();
		const char* GetString() const;
		int GetSqlString(char* buff) const;
		const char* GetDebugString() const;
	};
	class PrimaryAccount
	{
	public:
		static constexpr unsigned int TableID = 0x0005;
		//主账户代码
		AccountIDType PrimaryAccountID;
		//主账户名称
		AccountNameType PrimaryAccountName;
		//账户类别
		AccountClassType AccountClass;
		//经纪公司密码
		PasswordType BrokerPassword;
		//报盘代码
		OfferIDType OfferID;
		//是否允许登陆
		BoolType IsAllowLogin;
		//是否模拟账号
		BoolType IsSimulateAccount;
		//登录状态
		LoginStatusType LoginStatus;
		//初始化状态
		InitStatusType InitStatus;
		
		static PrimaryAccount* Allocate();
		void Free();
		const char* GetString() const;
		int GetSqlString(char* buff) const;
		const char* GetDebugString() const;
	};
	class Account
	{
	public:
		static constexpr unsigned int TableID = 0x0006;
		//账户代码
		AccountIDType AccountID;
		//账户名称
		AccountNameType AccountName;
		//账户类型
		AccountTypeType AccountType;
		//账户状态
		AccountStatusType AccountStatus;
		//密码
		PasswordType Password;
		//交易组代码
		GroupIDType TradeGroupID;
		//交易组代码
		GroupIDType RiskGroupID;
		//交易组代码
		GroupIDType CommissionGroupID;
		
		static Account* Allocate();
		void Free();
		const char* GetString() const;
		int GetSqlString(char* buff) const;
		const char* GetDebugString() const;
	};
	class Capital
	{
	public:
		static constexpr unsigned int TableID = 0x0007;
		//交易日
		DateType TradingDay;
		//账户代码
		AccountIDType AccountID;
		//账户类型
		AccountTypeType AccountType;
		//权益
		MoneyType Balance;
		//上日权益
		MoneyType PreBalance;
		//可用资金
		MoneyType Available;
		//市值
		MoneyType MarketValue;
		//现金收入
		MoneyType CashIn;
		//现金支出
		MoneyType CashOut;
		//保证金
		MoneyType Margin;
		//手续费
		MoneyType Commission;
		//冻结资金
		MoneyType FrozenCash;
		//冻结保证金
		MoneyType FrozenMargin;
		//冻结手续费
		MoneyType FrozenCommission;
		//逐日平仓盈亏
		MoneyType CloseProfitByDate;
		//逐笔平仓盈亏
		MoneyType CloseProfitByTrade;
		//逐日持仓盈亏
		MoneyType PositionProfitByDate;
		//逐笔持仓盈亏
		MoneyType PositionProfitByTrade;
		//入金
		MoneyType Deposit;
		//出金
		MoneyType Withdraw;
		
		static Capital* Allocate();
		void Free();
		const char* GetString() const;
		int GetSqlString(char* buff) const;
		const char* GetDebugString() const;
	};
	class Position
	{
	public:
		static constexpr unsigned int TableID = 0x0008;
		//交易日
		DateType TradingDay;
		//账户代码
		AccountIDType AccountID;
		//账户类型
		AccountTypeType AccountType;
		//交易所代码
		ExchangeIDType ExchangeID;
		//合约代码
		InstrumentIDType InstrumentID;
		//品种类型
		ProductClassType ProductClass;
		//持仓方向
		PosiDirectionType PosiDirection;
		//持仓数量
		VolumeType TotalPosition;
		//冻结持仓
		VolumeType PositionFrozen;
		//今日持仓
		VolumeType TodayPosition;
		//市值
		MoneyType MarketValue;
		//现金收入
		MoneyType CashIn;
		//现金支出
		MoneyType CashOut;
		//保证金
		MoneyType Margin;
		//手续费
		MoneyType Commission;
		//冻结资金
		MoneyType FrozenCash;
		//冻结保证金
		MoneyType FrozenMargin;
		//冻结手续费
		MoneyType FrozenCommission;
		//合约乘数
		VolumeMultipleType VolumeMultiple;
		//逐日平仓盈亏
		MoneyType CloseProfitByDate;
		//逐笔平仓盈亏
		MoneyType CloseProfitByTrade;
		//逐日持仓盈亏
		MoneyType PositionProfitByDate;
		//逐笔持仓盈亏
		MoneyType PositionProfitByTrade;
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
	class PositionDetail
	{
	public:
		static constexpr unsigned int TableID = 0x0009;
		//交易日
		DateType TradingDay;
		//账户代码
		AccountIDType AccountID;
		//账户类型
		AccountTypeType AccountType;
		//交易所代码
		ExchangeIDType ExchangeID;
		//合约代码
		InstrumentIDType InstrumentID;
		//品种类型
		ProductClassType ProductClass;
		//持仓方向
		PosiDirectionType PosiDirection;
		//开仓日期
		DateType OpenDate;
		//成交编号
		TradeIDType TradeID;
		//委托数量
		VolumeType Volume;
		//开盘价
		PriceType OpenPrice;
		//市值
		MoneyType MarketValue;
		//现金收入
		MoneyType CashIn;
		//现金支出
		MoneyType CashOut;
		//保证金
		MoneyType Margin;
		//手续费
		MoneyType Commission;
		//合约乘数
		VolumeMultipleType VolumeMultiple;
		//逐日平仓盈亏
		MoneyType CloseProfitByDate;
		//逐笔平仓盈亏
		MoneyType CloseProfitByTrade;
		//逐日持仓盈亏
		MoneyType PositionProfitByDate;
		//逐笔持仓盈亏
		MoneyType PositionProfitByTrade;
		//结算价
		PriceType SettlementPrice;
		//昨结算价
		PriceType PreSettlementPrice;
		//平仓数量
		VolumeType CloseVolume;
		//平仓金额
		MoneyType CloseAmount;
		
		static PositionDetail* Allocate();
		void Free();
		const char* GetString() const;
		int GetSqlString(char* buff) const;
		const char* GetDebugString() const;
	};
	class Order
	{
	public:
		static constexpr unsigned int TableID = 0x000A;
		//交易日
		DateType TradingDay;
		//账户代码
		AccountIDType AccountID;
		//账户类型
		AccountTypeType AccountType;
		//交易所代码
		ExchangeIDType ExchangeID;
		//合约代码
		InstrumentIDType InstrumentID;
		//品种类型
		ProductClassType ProductClass;
		//委托编号
		OrderIDType OrderID;
		//系统委托编号
		OrderSysIDType OrderSysID;
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
		VolumeType VolumeTotal;
		//成交数量
		VolumeType VolumeTraded;
		//合约乘数
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
		//会话编号
		SessionIDType SessionID;
		//客户端委托编号
		ClientOrderIDType ClientOrderID;
		//客户端请求编号
		RequestIDType RequestID;
		//报盘代码
		OfferIDType OfferID;
		//交易组代码
		GroupIDType TradeGroupID;
		//交易组代码
		GroupIDType RiskGroupID;
		//交易组代码
		GroupIDType CommissionGroupID;
		//冻结资金
		MoneyType FrozenCash;
		//冻结保证金
		MoneyType FrozenMargin;
		//冻结手续费
		MoneyType FrozenCommission;
		//重建标志
		BoolType RebuildMark;
		//是否强平单
		BoolType IsForceClose;
		
		static Order* Allocate();
		void Free();
		const char* GetString() const;
		int GetSqlString(char* buff) const;
		const char* GetDebugString() const;
	};
	class Trade
	{
	public:
		static constexpr unsigned int TableID = 0x000B;
		//交易日
		DateType TradingDay;
		//账户代码
		AccountIDType AccountID;
		//账户类型
		AccountTypeType AccountType;
		//交易所代码
		ExchangeIDType ExchangeID;
		//合约代码
		InstrumentIDType InstrumentID;
		//品种类型
		ProductClassType ProductClass;
		//委托编号
		OrderIDType OrderID;
		//系统委托编号
		OrderSysIDType OrderSysID;
		//成交编号
		TradeIDType TradeID;
		//买卖方向
		DirectionType Direction;
		//开平标志
		OffsetFlagType OffsetFlag;
		//委托价格
		PriceType Price;
		//委托数量
		VolumeType Volume;
		//合约乘数
		VolumeMultipleType VolumeMultiple;
		//成交金额
		MoneyType TradeAmount;
		//手续费
		MoneyType Commission;
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

	extern thread_local TradingDay t_CompareTradingDay;
	extern thread_local Exchange t_CompareExchange;
	extern thread_local Product t_CompareProduct;
	extern thread_local Instrument t_CompareInstrument;
	extern thread_local PrimaryAccount t_ComparePrimaryAccount;
	extern thread_local Account t_CompareAccount;
	extern thread_local Capital t_CompareCapital;
	extern thread_local Position t_ComparePosition;
	extern thread_local PositionDetail t_ComparePositionDetail;
	extern thread_local Order t_CompareOrder;
	extern thread_local Trade t_CompareTrade;

}

