#pragma once
#include "DataStruct.h"
#include "IndexComp.h"
#include <set>

using std::multiset;

class AccountIndexPrimaryAccount
{
	using iterator = std::multiset<Account*, AccountLessForPrimaryAccountIndex>::iterator;
public:
	void Insert(Account* const record);
	void Erase(Account* const record);
	bool NeedUpdate(const Account* const oldRecord, const Account* const newRecord);
	iterator LowerBound(Account* const record);
	iterator UpperBound(Account* const record);
	std::pair<iterator, iterator> EqualRange(Account* const record);

private:
	multiset<Account*, AccountLessForPrimaryAccountIndex> m_Index;
};
class AccountIndexBroker
{
	using iterator = std::multiset<Account*, AccountLessForBrokerIndex>::iterator;
public:
	void Insert(Account* const record);
	void Erase(Account* const record);
	bool NeedUpdate(const Account* const oldRecord, const Account* const newRecord);
	iterator LowerBound(Account* const record);
	iterator UpperBound(Account* const record);
	std::pair<iterator, iterator> EqualRange(Account* const record);

private:
	multiset<Account*, AccountLessForBrokerIndex> m_Index;
};


