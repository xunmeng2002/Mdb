#pragma once
#include <unordered_set>
#include "DataStruct.h"
#include "PrimaryKeyComp.h"




class AccountPrimaryKey
{
	friend class AccountTable;
public:
	AccountPrimaryKey(size_t buckets = 1000);

	bool Insert(Account* const account);
	bool Erase(Account* const account);
	bool CheckUpdate(const Account* const oldAccount, const Account* const newAccount);
	const Account* Select(const CBrokerIDType& brokerID, const CAccountIDType& accountID, const CAccountClassType& accountClass);

private:
	Account m_SelectAccount;
	std::unordered_set<Account*, AccountHashForPrimaryKey, AccountEqualForPrimaryKey> m_Index;
};

