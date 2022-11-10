#pragma once

//布尔类型
typedef bool CBoolType;

//数量类型
typedef int CVolumeType;

//经纪公司代码类型
typedef long long CBrokerIDType;
//本地报单编号类型
typedef long long COrderLocalIDType;

//价格类型
typedef double CPriceType;

//账号类型
typedef char CAccountIDType[32];
//日期类型
typedef char CDateType[9];
//时间类型
typedef char CTimeType[9];
//机构代码类型
typedef char COrgIDType[16];
//账号名称类型
typedef char CAccountNameType[128];
//币种类型
typedef char CCurrencyIDType[8];
//交易所代码类型
typedef char CExchangeIDType[8];
//合约代码类型
typedef char CInstrumentIDType[32];
//系统委托编号类型
typedef char COrderSysIDType[32];
//经纪公司委托编号类型
typedef char CBrokerOrderIDType[32];

enum class CAccountTypeType : char
{
	//主账号
	Primary = '0',
	//子账号
	Sub = '1',
};
enum class CAccountClassType : char
{
	//期货账户
	Future = '0',
	//证券账户
	Security = '1',
	//衍生品账户
	Derivative = '2',
	//信用账户
	Credit = '3',
};
enum class CAccountStatusType : char
{
	//启用
	Normal = '0',
	//禁用
	Forbidden = '1',
};
enum class CDirectionType : char
{
	//买
	Buy = '0',
	//卖
	Sell = '1',
};
enum class COffsetFlagType : char
{
	//开仓
	Open = '0',
	//平仓
	Close = '1',
	//平今
	CloseToday = '3',
};
enum class CHedgeFlagType : char
{
	//投机
	Speculation = '0',
	//套利
	Arbitrage = '1',
	//套期保值
	Hedge = '2',
};
enum class COrderStatusType : char
{
	//经纪公司已接受
	BrokerAccepted = '0',
	//交易所已接受
	ExchangeAccepted = '1',
	//部分成交
	PartTraded = '2',
	//全部成交
	AllTraded = '3',
	//部分成交部分撤销
	PartTradedPartCanceled = '4',
	//撤销
	Canceled = '5',
	//经纪公司拒绝处理
	BrokerRefused = '6',
	//交易所拒绝处理
	ExchangeRefused = '7',
	//报盘发送失败
	OfferFailed = '8',
	//未知
	Unknown = 'a',
};
enum class COrderTypeType : char
{
	//普通单
	Normal = '0',
	//互换单
	Swap = '1',
	//组合报单
	Combination = '2',
};

