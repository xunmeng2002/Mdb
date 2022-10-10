#pragma once
#include <unordered_set>
#include "DataStruct.h"
#include "PrimaryKeyComp.h"


class AccountPrimaryKeyForDefault
{
	friend class AccountTable;
public:
	AccountPrimaryKeyForDefault(size_t buckets = 1000);
	bool Insert(Account* const record);
	bool Erase(Account* const record);
	bool CheckUpdate(const Account* const oldRecord, const Account* const newRecord);
	const Account* Select(const CBrokerIDType& BrokerID, const CAccountIDType& AccountID, const CAccountClassType& AccountClass);

private:
	Account m_SelectAccount;
	std::unordered_set<Account*, AccountHashForDefault, AccountEqualForDefault> m_Index;
};

