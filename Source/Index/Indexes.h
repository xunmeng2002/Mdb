#pragma once
#include "DataStruct.h"
#include "IndexComp.h"
#include <set>

using std::multiset;

class AccountIndexForPrimaryAccount
{
public:
	void Insert(Account* const record);
	void Erase(Account* const record);
	bool NeedUpdate(const Account* const oldRecord, const Account* const newRecord);

private:
	multiset<Account*, AccountLessForPrimaryAccount> m_Index;
};
class AccountIndexForBroker
{
public:
	void Insert(Account* const record);
	void Erase(Account* const record);
	bool NeedUpdate(const Account* const oldRecord, const Account* const newRecord);

private:
	multiset<Account*, AccountLessForBroker> m_Index;
};


