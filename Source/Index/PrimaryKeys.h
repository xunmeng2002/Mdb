#pragma once
#include <unordered_set>
#include "DataStruct.h"
#include "PrimaryKeyComp.h"


class AccountPrimaryKey
{
	friend class AccountTable;
public:
	AccountPrimaryKey(size_t buckets = 1000);
	bool Insert(Account* const record);
	bool Erase(Account* const record);
	bool CheckInsert(Account* const record);
	bool CheckUpdate(const Account* const oldRecord, const Account* const newRecord);
	const Account* Select(const CBrokerIDType& BrokerID, const CAccountIDType& AccountID, const CAccountClassType& AccountClass);

private:
	Account m_SelectAccount;
	std::unordered_set<Account*, AccountHashForAccountPrimaryKey, AccountEqualForAccountPrimaryKey> m_Index;
};

class OrderPrimaryKey
{
	friend class OrderTable;
public:
	OrderPrimaryKey(size_t buckets = 1000);
	bool Insert(Order* const record);
	bool Erase(Order* const record);
	bool CheckInsert(Order* const record);
	bool CheckUpdate(const Order* const oldRecord, const Order* const newRecord);
	const Order* Select(const CBrokerIDType& BrokerID, const CAccountIDType& AccountID, const CAccountClassType& AccountClass, const CDateType& InsertDate, const COrderLocalIDType& OrderLocalID);

private:
	Order m_SelectOrder;
	std::unordered_set<Order*, OrderHashForOrderPrimaryKey, OrderEqualForOrderPrimaryKey> m_Index;
};

