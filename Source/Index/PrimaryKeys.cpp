#include "PrimaryKeys.h"

using std::unordered_set;



AccountPrimaryKey::AccountPrimaryKey(size_t buckets)
	:m_Index(buckets), m_SelectAccount()
{
}
bool AccountPrimaryKey::Insert(Account* const account)
{
	return m_Index.insert(account).second;
}
bool AccountPrimaryKey::Erase(Account* const  account)
{
	return  m_Index.erase(account) > 0;
}
bool AccountPrimaryKey::CheckUpdate(const Account* const oldAccount, const Account* const newAccount)
{
	return AccountEqualForPrimaryKey()(oldAccount, newAccount);
}
const Account* AccountPrimaryKey::Select(const CBrokerIDType& brokerID, const CAccountIDType& accountID, const CAccountClassType& accountClass)
{
	m_SelectAccount.BrokerID = brokerID;
	strcpy(m_SelectAccount.AccountID, accountID);
	m_SelectAccount.AccountClass = accountClass;

	auto it = m_Index.find(&m_SelectAccount);
	if (it == m_Index.end())
	{
		return nullptr;
	}
	return *it;
}
