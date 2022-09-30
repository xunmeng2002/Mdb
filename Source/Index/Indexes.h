#pragma once
#include "DataStruct.h"
#include "IndexComp.h"
#include <set>

using std::multiset;


class AccountIndexForPrimaryAccount
{
public:
	void Insert(Account* const account);
	void Erase(Account* const account);
	bool NeedUpdate(const Account* const oldAccount, const Account* const newAccount);

private:
	multiset<Account*, AccountLessForPrimaryAccount> m_Index;
};


