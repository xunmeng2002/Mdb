#include "Indexes.h"


void AccountIndexForPrimaryAccount::Insert(Account* const record)
{
	m_Index.insert(record);
}
void AccountIndexForPrimaryAccount::Erase(Account* const record)
{
	m_Index.erase(record);
}
bool AccountIndexForPrimaryAccount::NeedUpdate(const Account* const oldRecord, const Account* const newRecord)
{
	return !(AccountEqualForPrimaryAccount()(oldRecord, newRecord));
}

void AccountIndexForBroker::Insert(Account* const record)
{
	m_Index.insert(record);
}
void AccountIndexForBroker::Erase(Account* const record)
{
	m_Index.erase(record);
}
bool AccountIndexForBroker::NeedUpdate(const Account* const oldRecord, const Account* const newRecord)
{
	return !(AccountEqualForBroker()(oldRecord, newRecord));
}



