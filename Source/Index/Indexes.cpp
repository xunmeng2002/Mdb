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



