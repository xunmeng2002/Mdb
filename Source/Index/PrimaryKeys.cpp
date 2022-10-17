#include "PrimaryKeys.h"

using std::unordered_set;



AccountPrimaryKey::AccountPrimaryKey(size_t buckets)
	:m_Index(buckets), m_SelectAccount()
{
}
bool AccountPrimaryKey::Insert(Account* const record)
{
	return m_Index.insert(record).second;
}
bool AccountPrimaryKey::Erase(Account* const  record)
{
	return  m_Index.erase(record) > 0;
}
bool AccountPrimaryKey::CheckInsert(Account* const record)
{
	return m_Index.find(record) == m_Index.end();
}
bool AccountPrimaryKey::CheckUpdate(const Account* const oldRecord, const Account* const newRecord)
{
	return AccountEqualForAccountPrimaryKey()(oldRecord, newRecord);
}
const Account* AccountPrimaryKey::Select(const CBrokerIDType& BrokerID, const CAccountIDType& AccountID, const CAccountClassType& AccountClass)
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


OrderPrimaryKey::OrderPrimaryKey(size_t buckets)
	:m_Index(buckets), m_SelectOrder()
{
}
bool OrderPrimaryKey::Insert(Order* const record)
{
	return m_Index.insert(record).second;
}
bool OrderPrimaryKey::Erase(Order* const  record)
{
	return  m_Index.erase(record) > 0;
}
bool OrderPrimaryKey::CheckInsert(Order* const record)
{
	return m_Index.find(record) == m_Index.end();
}
bool OrderPrimaryKey::CheckUpdate(const Order* const oldRecord, const Order* const newRecord)
{
	return OrderEqualForOrderPrimaryKey()(oldRecord, newRecord);
}
const Order* OrderPrimaryKey::Select(const CBrokerIDType& BrokerID, const CAccountIDType& AccountID, const CAccountClassType& AccountClass, const CDateType& InsertDate, const COrderLocalIDType& OrderLocalID)
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


