#pragma once
#include "DataType.h"

struct Account
{
public:
	//机构代码
	COrgIDType OrgID;
	//经纪公司代码
	CBrokerIDType BrokerID;
	//账户代码
	CAccountIDType AccountID;
	//账户名称
	CAccountNameType AccountName;
	//账户类别
	CAccountClassType AccountClass;
	//账户类型
	CAccountTypeType AccountType;
	//主账号代码
	CAccountIDType PrimaryAccountID;
	//账户状态
	CAccountStatusType AccountStatus;
	//币种代码
	CCurrencyIDType CurrencyID;
	//删除标志
	CBoolType DeleteFlag;
	
	const char* GetString() const;
	const char* GetDebugString() const;
};
struct Order
{
public:
	//机构代码
	COrgIDType OrgID;
	//经纪公司代码
	CBrokerIDType BrokerID;
	//账户代码
	CAccountIDType AccountID;
	//主账号代码
	CAccountIDType PrimaryAccountID;
	//账户类别
	CAccountClassType AccountClass;
	//账户类型
	CAccountTypeType AccountType;
	//交易所代码
	CExchangeIDType ExchangeID;
	//合约代码
	CInstrumentIDType InstrumentID;
	//买卖方向
	CDirectionType Direction;
	//开平标志
	COffsetFlagType OffsetFlag;
	//组合投机套保标志
	CHedgeFlagType HedgeFlag;
	//报单编号
	COrderSysIDType OrderSysID;
	//本地报单编号
	COrderLocalIDType OrderLocalID;
	//柜台本地报单编号
	CBrokerOrderIDType BrokerOrderID;
	//报单状态
	COrderStatusType OrderStatus;
	//报单类型
	COrderTypeType OrderType;
	//委托数量
	CVolumeType Volume;
	//成交数量
	CVolumeType VolumeTraded;
	//报单日期
	CDateType InsertDate;
	//委托时间
	CTimeType InsertTime;
	
	const char* GetString() const;
	const char* GetDebugString() const;
};

