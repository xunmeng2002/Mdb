#include "PrimaryKeys.h"

using std::unordered_set;



AccountPrimaryKeyForDefault::AccountPrimaryKeyForDefault(size_t buckets)
	:m_Index(buckets), m_SelectAccount()
{
}
bool AccountPrimaryKeyForDefault::Insert(Account* const record)
{
	return m_Index.insert(record).second;
}
bool AccountPrimaryKeyForDefault::Erase(Account* const  record)
{
	return  m_Index.erase(record) > 0;
}
bool AccountPrimaryKeyForDefault::CheckUpdate(const Account* const oldRecord, const Account* const newRecord)
{
	return AccountEqualForDefault()(oldRecord, newRecord);
}
const Account* AccountPrimaryKeyForDefault::Select(const CBrokerIDType& BrokerID, const CAccountIDType& AccountID, const CAccountClassType& AccountClass)
{
	m_SelectAccount.BrokerID = BrokerID;
	strcpy(m_SelectAccount.AccountID, AccountID);
	m_SelectAccount.AccountClass = AccountClass;

	auto it = m_Index.find(&m_SelectAccount);
	if (it == m_Index.end())
	{
		return nullptr;
	}
	return *it;
}


