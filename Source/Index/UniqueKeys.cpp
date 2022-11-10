#include "UniqueKeys.h"
#include <string.h>

using std::unordered_set;



AccountUniqueKeyPrimaryAccount::AccountUniqueKeyPrimaryAccount(size_t buckets)
	:m_Index(buckets), m_SelectAccount()
{
}
bool AccountUniqueKeyPrimaryAccount::Insert(Account* const record)
{
	return m_Index.insert(record).second;
}
bool AccountUniqueKeyPrimaryAccount::Erase(Account* const  record)
{
	return  m_Index.erase(record) > 0;
}
bool AccountUniqueKeyPrimaryAccount::CheckInsert(Account* const record)
{
	return m_Index.find(record) == m_Index.end();
}
bool AccountUniqueKeyPrimaryAccount::CheckUpdate(const Account* const oldRecord, const Account* const newRecord)
{
	return AccountEqualForPrimaryAccountUniqueKey()(oldRecord, newRecord);
}
const Account* AccountUniqueKeyPrimaryAccount::Select(const CBrokerIDType& BrokerID, const CAccountIDType& PrimaryAccountID, const CAccountClassType& AccountClass)
{
	m_SelectAccount.BrokerID = BrokerID;
	strcpy(m_SelectAccount.PrimaryAccountID, PrimaryAccountID);
	m_SelectAccount.AccountClass = AccountClass;

	auto it = m_Index.find(&m_SelectAccount);
	if (it == m_Index.end())
	{
		return nullptr;
	}
	return *it;
}


