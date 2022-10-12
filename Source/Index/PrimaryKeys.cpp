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
bool AccountPrimaryKeyDefault::CheckInsert(Account* const record)
{
	return m_Index.find(record) == m_Index.end();
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
bool AccountPrimaryKeyPrimaryAccount::CheckInsert(Account* const record)
{
	return m_Index.find(record) == m_Index.end();
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


OrderPrimaryKeyDefault::OrderPrimaryKeyDefault(size_t buckets)
	:m_Index(buckets), m_SelectOrder()
{
}
bool OrderPrimaryKeyDefault::Insert(Order* const record)
{
	return m_Index.insert(record).second;
}
bool OrderPrimaryKeyDefault::Erase(Order* const  record)
{
	return  m_Index.erase(record) > 0;
}
bool OrderPrimaryKeyDefault::CheckInsert(Order* const record)
{
	return m_Index.find(record) == m_Index.end();
}
bool OrderPrimaryKeyDefault::CheckUpdate(const Order* const oldRecord, const Order* const newRecord)
{
	return OrderEqualForDefaultPrimaryKey()(oldRecord, newRecord);
}
const Order* OrderPrimaryKeyDefault::Select(const CBrokerIDType& BrokerID, const CAccountIDType& AccountID, const CAccountClassType& AccountClass, const CDateType& InsertDate, const COrderLocalIDType& OrderLocalID)
{
	m_SelectOrder.BrokerID = BrokerID;
	strcpy(m_SelectOrder.AccountID, AccountID);
	m_SelectOrder.AccountClass = AccountClass;
	strcpy(m_SelectOrder.InsertDate, InsertDate);
	m_SelectOrder.OrderLocalID = OrderLocalID;

	auto it = m_Index.find(&m_SelectOrder);
	if (it == m_Index.end())
	{
		return nullptr;
	}
	return *it;
}


