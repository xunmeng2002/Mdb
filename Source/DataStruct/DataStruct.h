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

