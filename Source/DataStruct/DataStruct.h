#pragma once
#include "DataType.h"
#include "DataEnum.h"


struct Account
{
public:
	//经纪公司代码
	CBrokerIDType BrokerID;
	//账户代码
	CAccountIDType AccountID;
	//交易日
	CDateType TradingDay;
	//机构代码
	COrgIDType OrgID;
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
	//默认币种
	CCurrencyIDType CurrencyID;
	//删除标志
	CBoolType DeleteFlag;
};

