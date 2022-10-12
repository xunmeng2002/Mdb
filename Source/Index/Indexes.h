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

class OrderIndexPrimaryAccount
{
	using iterator = std::multiset<Order*, OrderLessForPrimaryAccountIndex>::iterator;
public:
	void Insert(Order* const record);
	void Erase(Order* const record);
	bool NeedUpdate(const Order* const oldRecord, const Order* const newRecord);
	iterator LowerBound(Order* const record);
	iterator UpperBound(Order* const record);
	std::pair<iterator, iterator> EqualRange(Order* const record);

private:
	multiset<Order*, OrderLessForPrimaryAccountIndex> m_Index;
};
class OrderIndexInstrument
{
	using iterator = std::multiset<Order*, OrderLessForInstrumentIndex>::iterator;
public:
	void Insert(Order* const record);
	void Erase(Order* const record);
	bool NeedUpdate(const Order* const oldRecord, const Order* const newRecord);
	iterator LowerBound(Order* const record);
	iterator UpperBound(Order* const record);
	std::pair<iterator, iterator> EqualRange(Order* const record);

private:
	multiset<Order*, OrderLessForInstrumentIndex> m_Index;
};


