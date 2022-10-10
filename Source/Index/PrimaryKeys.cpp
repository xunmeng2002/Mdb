#include "PrimaryKeys.h"

using std::unordered_set;



AccountPrimaryKeyDefault::AccountPrimaryKeyDefault(size_t buckets)
	:m_Index(buckets), m_SelectAccount()
{
}
bool AccountPrimaryKeyDefault::Insert(Account* const record)
{
	return m_Index.insert(record).second;
}
bool AccountPrimaryKeyDefault::Erase(Account* const  record)
{
	return  m_Index.erase(record) > 0;
}
bool AccountPrimaryKeyDefault::CheckUpdate(const Account* const oldRecord, const Account* const newRecord)
{
	return AccountEqualForDefaultPrimaryKey()(oldRecord, newRecord);
}
const Account* AccountPrimaryKeyDefault::Select(const CBrokerIDType& BrokerID, const CAccountIDType& AccountID, const CAccountClassType& AccountClass)
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


AccountPrimaryKeyPrimaryAccount::AccountPrimaryKeyPrimaryAccount(size_t buckets)
	:m_Index(buckets), m_SelectAccount()
{
}
bool AccountPrimaryKeyPrimaryAccount::Insert(Account* const record)
{
	return m_Index.insert(record).second;
}
bool AccountPrimaryKeyPrimaryAccount::Erase(Account* const  record)
{
	return  m_Index.erase(record) > 0;
}
bool AccountPrimaryKeyPrimaryAccount::CheckUpdate(const Account* const oldRecord, const Account* const newRecord)
{
	return AccountEqualForPrimaryAccountPrimaryKey()(oldRecord, newRecord);
}
const Account* AccountPrimaryKeyPrimaryAccount::Select(const CBrokerIDType& BrokerID, const CAccountIDType& PrimaryAccountID, const CAccountClassType& AccountClass)
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


