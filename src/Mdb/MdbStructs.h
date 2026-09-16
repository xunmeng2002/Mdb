// 本文件由 ../Templates/Cpp/Mdb/MdbStructs.h.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#pragma once
#include <DBAdapters/DbInterface/Schema.h>
#include <Spark/Types.h>

using DbAdapters::TableSchema;

namespace Mdb
{
	struct TradingDay
	{
	public:
		static constexpr unsigned int TableId = 0x0001;
		//主键
		Int32Type PK;
		//当前交易日
		DateType CurrTradingDay;
		//昨交易日
		DateType PreTradingDay;
		
		static TradingDay* Allocate();
		void Deallocate();
		const char* GetString() const;
		const char* GetDebugString() const;
		static const TableSchema& GetSchema();
	};
	struct Exchange
	{
	public:
		static constexpr unsigned int TableId = 0x0002;
		//交易所代码
		ExchangeIdType ExchangeId;
		//交易所名称
		ExchangeNameType ExchangeName;
		
		static Exchange* Allocate();
		void Deallocate();
		const char* GetString() const;
		const char* GetDebugString() const;
		static const TableSchema& GetSchema();
	};
	struct Product
	{
	public:
		static constexpr unsigned int TableId = 0x0003;
		//交易所代码
		ExchangeIdType ExchangeId;
		//品种代码
		ProductIdType ProductId;
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
		void Deallocate();
		const char* GetString() const;
		const char* GetDebugString() const;
		static const TableSchema& GetSchema();
	};
	struct Instrument
	{
	public:
		static constexpr unsigned int TableId = 0x0004;
		//交易所代码
		ExchangeIdType ExchangeId;
		//合约代码
		InstrumentIdType InstrumentId;
		//交易所合约代码
		InstrumentIdType ExchangeInstId;
		//合约名称
		InstrumentNameType InstrumentName;
		//品种代码
		ProductIdType ProductId;
		//品种类型
		ProductClassType ProductClass;
		//合约类别
		InstrumentClassType InstrumentClass;
		//级别
		Int32Type Rank;
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
		void Deallocate();
		const char* GetString() const;
		const char* GetDebugString() const;
		static const TableSchema& GetSchema();
	};
	struct PrimaryAccount
	{
	public:
		static constexpr unsigned int TableId = 0x0005;
		//主账户代码
		AccountIdType PrimaryAccountId;
		//主账户名称
		AccountNameType PrimaryAccountName;
		//账户类别
		AccountClassType AccountClass;
		//经纪公司密码
		PasswordType BrokerPassword;
		//报盘代码
		OfferIdType OfferId;
		//是否允许登陆
		BoolType IsAllowLogin;
		//是否模拟账号
		BoolType IsSimulateAccount;
		//登录状态
		LoginStatusType LoginStatus;
		//初始化状态
		InitStatusType InitStatus;
		
		static PrimaryAccount* Allocate();
		void Deallocate();
		const char* GetString() const;
		const char* GetDebugString() const;
		static const TableSchema& GetSchema();
	};
	struct Account
	{
	public:
		static constexpr unsigned int TableId = 0x0006;
		//账户代码
		AccountIdType AccountId;
		//账户名称
		AccountNameType AccountName;
		//账户类型
		AccountTypeType AccountType;
		//账户状态
		AccountStatusType AccountStatus;
		//密码
		PasswordType Password;
		//交易组代码
		GroupIdType TradeGroupId;
		//交易组代码
		GroupIdType RiskGroupId;
		//交易组代码
		GroupIdType CommissionGroupId;
		
		static Account* Allocate();
		void Deallocate();
		const char* GetString() const;
		const char* GetDebugString() const;
		static const TableSchema& GetSchema();
	};
	struct Capital
	{
	public:
		static constexpr unsigned int TableId = 0x0007;
		//交易日
		DateType TradingDay;
		//账户代码
		AccountIdType AccountId;
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
		void Deallocate();
		const char* GetString() const;
		const char* GetDebugString() const;
		static const TableSchema& GetSchema();
	};
	struct Position
	{
	public:
		static constexpr unsigned int TableId = 0x0008;
		//交易日
		DateType TradingDay;
		//账户代码
		AccountIdType AccountId;
		//账户类型
		AccountTypeType AccountType;
		//交易所代码
		ExchangeIdType ExchangeId;
		//合约代码
		InstrumentIdType InstrumentId;
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
		void Deallocate();
		const char* GetString() const;
		const char* GetDebugString() const;
		static const TableSchema& GetSchema();
	};
	struct PositionDetail
	{
	public:
		static constexpr unsigned int TableId = 0x0009;
		//交易日
		DateType TradingDay;
		//账户代码
		AccountIdType AccountId;
		//账户类型
		AccountTypeType AccountType;
		//交易所代码
		ExchangeIdType ExchangeId;
		//合约代码
		InstrumentIdType InstrumentId;
		//品种类型
		ProductClassType ProductClass;
		//持仓方向
		PosiDirectionType PosiDirection;
		//开仓日期
		DateType OpenDate;
		//成交编号
		TradeIdType TradeId;
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
		void Deallocate();
		const char* GetString() const;
		const char* GetDebugString() const;
		static const TableSchema& GetSchema();
	};
	struct Order
	{
	public:
		static constexpr unsigned int TableId = 0x000A;
		//交易日
		DateType TradingDay;
		//账户代码
		AccountIdType AccountId;
		//账户类型
		AccountTypeType AccountType;
		//交易所代码
		ExchangeIdType ExchangeId;
		//合约代码
		InstrumentIdType InstrumentId;
		//品种类型
		ProductClassType ProductClass;
		//委托编号
		OrderIdType OrderId;
		//系统委托编号
		OrderSysIdType OrderSysId;
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
		SessionIdType SessionId;
		//客户端委托编号
		ClientOrderIdType ClientOrderId;
		//客户端请求编号
		RequestIdType RequestId;
		//报盘代码
		OfferIdType OfferId;
		//交易组代码
		GroupIdType TradeGroupId;
		//交易组代码
		GroupIdType RiskGroupId;
		//交易组代码
		GroupIdType CommissionGroupId;
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
		void Deallocate();
		const char* GetString() const;
		const char* GetDebugString() const;
		static const TableSchema& GetSchema();
	};
	struct Trade
	{
	public:
		static constexpr unsigned int TableId = 0x000B;
		//交易日
		DateType TradingDay;
		//账户代码
		AccountIdType AccountId;
		//账户类型
		AccountTypeType AccountType;
		//交易所代码
		ExchangeIdType ExchangeId;
		//合约代码
		InstrumentIdType InstrumentId;
		//品种类型
		ProductClassType ProductClass;
		//委托编号
		OrderIdType OrderId;
		//系统委托编号
		OrderSysIdType OrderSysId;
		//成交编号
		TradeIdType TradeId;
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
		void Deallocate();
		const char* GetString() const;
		const char* GetDebugString() const;
		static const TableSchema& GetSchema();
	};

	extern thread_local TradingDay CompareTradingDay;
	extern thread_local Exchange CompareExchange;
	extern thread_local Product CompareProduct;
	extern thread_local Instrument CompareInstrument;
	extern thread_local PrimaryAccount ComparePrimaryAccount;
	extern thread_local Account CompareAccount;
	extern thread_local Capital CompareCapital;
	extern thread_local Position ComparePosition;
	extern thread_local PositionDetail ComparePositionDetail;
	extern thread_local Order CompareOrder;
	extern thread_local Trade CompareTrade;

}

