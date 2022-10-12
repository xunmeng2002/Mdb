#include "Indexes.h"


void AccountIndexPrimaryAccount::Insert(Account* const record)
{
	m_Index.insert(record);
}
void AccountIndexPrimaryAccount::Erase(Account* const record)
{
	m_Index.erase(record);
}
bool AccountIndexPrimaryAccount::NeedUpdate(const Account* const oldRecord, const Account* const newRecord)
{
	return !(AccountEqualForPrimaryAccountIndex()(oldRecord, newRecord));
}
AccountIndexPrimaryAccount::iterator AccountIndexPrimaryAccount::LowerBound(Account* const record)
{
	return m_Index.lower_bound(record);
}
AccountIndexPrimaryAccount::iterator AccountIndexPrimaryAccount::UpperBound(Account* const record)
{
	return m_Index.upper_bound(record);
}
std::pair<AccountIndexPrimaryAccount::iterator, AccountIndexPrimaryAccount::iterator> AccountIndexPrimaryAccount::EqualRange(Account* const record)
{
	return m_Index.equal_range(record);
}

void AccountIndexBroker::Insert(Account* const record)
{
	m_Index.insert(record);
}
void AccountIndexBroker::Erase(Account* const record)
{
	m_Index.erase(record);
}
bool AccountIndexBroker::NeedUpdate(const Account* const oldRecord, const Account* const newRecord)
{
	return !(AccountEqualForBrokerIndex()(oldRecord, newRecord));
}
AccountIndexBroker::iterator AccountIndexBroker::LowerBound(Account* const record)
{
	return m_Index.lower_bound(record);
}
AccountIndexBroker::iterator AccountIndexBroker::UpperBound(Account* const record)
{
	return m_Index.upper_bound(record);
}
std::pair<AccountIndexBroker::iterator, AccountIndexBroker::iterator> AccountIndexBroker::EqualRange(Account* const record)
{
	return m_Index.equal_range(record);
}


