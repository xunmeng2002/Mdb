#pragma once
#include "DataType.h"
#include "DataEnum.h"


struct Account
{
public:
	//���͹�˾����
	CBrokerIDType BrokerID;
	//�˻�����
	CAccountIDType AccountID;
	//������
	CDateType TradingDay;
	//��������
	COrgIDType OrgID;
	//�˻�����
	CAccountNameType AccountName;
	//�˻����
	CAccountClassType AccountClass;
	//�˻�����
	CAccountTypeType AccountType;
	//���˺Ŵ���
	CAccountIDType PrimaryAccountID;
	//�˻�״̬
	CAccountStatusType AccountStatus;
	//Ĭ�ϱ���
	CCurrencyIDType CurrencyID;
	//ɾ����־
	CBoolType DeleteFlag;
};

