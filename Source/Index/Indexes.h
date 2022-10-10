#pragma once
#include "DataStruct.h"
#include "IndexComp.h"
#include <set>

using std::multiset;

class AccountIndexPrimaryAccount
{
public:
	void Insert(Account* const record);
	void Erase(Account* const record);
	bool NeedUpdate(const Account* const oldRecord, const Account* const newRecord);

private:
	multiset<Account*, AccountLessForPrimaryAccountIndex> m_Index;
};
class AccountIndexBroker
{
public:
	void Insert(Account* const record);
	void Erase(Account* const record);
	bool NeedUpdate(const Account* const oldRecord, const Account* const newRecord);

private:
	multiset<Account*, AccountLessForBrokerIndex> m_Index;
};


