#pragma once
#include <unordered_set>
#include "DataStruct.h"
#include "PrimaryKeyComp.h"


class AccountPrimaryKeyDefault
{
	friend class AccountTable;
public:
	AccountPrimaryKeyDefault(size_t buckets = 1000);
	bool Insert(Account* const record);
	bool Erase(Account* const record);
	bool CheckInsert(Account* const record);
	bool CheckUpdate(const Account* const oldRecord, const Account* const newRecord);
	const Account* Select(const CBrokerIDType& BrokerID, const CAccountIDType& AccountID, const CAccountClassType& AccountClass);

private:
	Account m_SelectAccount;
	std::unordered_set<Account*, AccountHashForDefaultPrimaryKey, AccountEqualForDefaultPrimaryKey> m_Index;
};

class AccountPrimaryKeyPrimaryAccount
{
	friend class AccountTable;
public:
	AccountPrimaryKeyPrimaryAccount(size_t buckets = 1000);
	bool Insert(Account* const record);
	bool Erase(Account* const record);
	bool CheckInsert(Account* const record);
	bool CheckUpdate(const Account* const oldRecord, const Account* const newRecord);
	const Account* Select(const CBrokerIDType& BrokerID, const CAccountIDType& PrimaryAccountID, const CAccountClassType& AccountClass);

private:
	Account m_SelectAccount;
	std::unordered_set<Account*, AccountHashForPrimaryAccountPrimaryKey, AccountEqualForPrimaryAccountPrimaryKey> m_Index;
};

class OrderPrimaryKeyDefault
{
	friend class OrderTable;
public:
	OrderPrimaryKeyDefault(size_t buckets = 1000);
	bool Insert(Order* const record);
	bool Erase(Order* const record);
	bool CheckInsert(Order* const record);
	bool CheckUpdate(const Order* const oldRecord, const Order* const newRecord);
	const Order* Select(const CBrokerIDType& BrokerID, const CAccountIDType& AccountID, const CAccountClassType& AccountClass, const CDateType& InsertDate, const COrderLocalIDType& OrderLocalID);

private:
	Order m_SelectOrder;
	std::unordered_set<Order*, OrderHashForDefaultPrimaryKey, OrderEqualForDefaultPrimaryKey> m_Index;
};

