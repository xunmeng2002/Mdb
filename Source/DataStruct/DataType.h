#pragma once

typedef bool CBoolType;

typedef int CVolumeType;

typedef long long CBrokerIDType;
typedef long long COrderLocalIDType;

typedef double CPriceType;

typedef char CAccountIDType[32];
typedef char CDateType[9];
typedef char CTimeType[9];
typedef char COrgIDType[16];
typedef char CAccountNameType[128];
typedef char CCurrencyIDType[8];
typedef char CExchangeIDType[8];
typedef char CInstrumentIDType[32];
typedef char COrderSysIDType[32];
typedef char CBrokerOrderIDType[32];

enum class CAccountTypeType : char
{
	Primary = '0',
	Sub = '1',
};
enum class CAccountClassType : char
{
	Future = '0',
	Security = '1',
	Derivative = '2',
	Credit = '3',
};
enum class CAccountStatusType : char
{
	Normal = '0',
	Forbidden = '1',
};
enum class CDirectionType : char
{
	Buy = '0',
	Sell = '1',
};
enum class COffsetFlagType : char
{
	Open = '0',
	Close = '1',
	CloseToday = '3',
};
enum class CHedgeFlagType : char
{
	Speculation = '0',
	Arbitrage = '1',
	Hedge = '2',
};
enum class COrderStatusType : char
{
	BrokerAccepted = '0',
	ExchangeAccepted = '1',
	PartTraded = '2',
	AllTraded = '3',
	PartTradedPartCanceled = '4',
	Canceled = '5',
	BrokerRefused = '6',
	ExchangeRefused = '7',
	OfferFailed = '8',
	Unknown = 'a',
};
enum class COrderTypeType : char
{
	Normal = '0',
	Swap = '1',
	Combination = '2',
};

