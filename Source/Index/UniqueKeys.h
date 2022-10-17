#pragma once
#include <unordered_set>
#include "DataStruct.h"
#include "UniqueKeyComp.h"


class AccountUniqueKeyPrimaryAccount
{
	friend class AccountTable;
public:
	AccountUniqueKeyPrimaryAccount(size_t buckets = 1000);
	bool Insert(Account* const record);
	bool Erase(Account* const record);
	bool CheckInsert(Account* const record);
	bool CheckUpdate(const Account* const oldRecord, const Account* const newRecord);
	const Account* Select(const CBrokerIDType& BrokerID, const CAccountIDType& PrimaryAccountID, const CAccountClassType& AccountClass);

private:
	Account m_SelectAccount;
	std::unordered_set<Account*, AccountHashForPrimaryAccountUniqueKey, AccountEqualForPrimaryAccountUniqueKey> m_Index;
};

