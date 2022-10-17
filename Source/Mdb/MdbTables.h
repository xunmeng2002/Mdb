#pragma once
#include "DataStruct.h"
#include "PrimaryKeys.h"
#include "UniqueKeys.h"
#include "Indexes.h"
#include "MemCacheTemplate.h"


class AccountTable
{
public:
	AccountTable();
	Account* Alloc();
	void Free(Account* record);
	bool Insert(Account* record);
	bool Erase(Account* record);
	bool Update(const Account* oldRecord, const Account* newRecord);
	void Dump(const char* dir);

public:
	AccountPrimaryKey m_PrimaryKey;

	AccountUniqueKeyPrimaryAccount m_PrimaryAccountUniqueKey;

	AccountIndexPrimaryAccount m_PrimaryAccountIndex;
	AccountIndexBroker m_BrokerIndex;

private:
	MemCacheTemplate<Account> m_MemCache;
};

class OrderTable
{
public:
	OrderTable();
	Order* Alloc();
	void Free(Order* record);
	bool Insert(Order* record);
	bool Erase(Order* record);
	bool Update(const Order* oldRecord, const Order* newRecord);
	void Dump(const char* dir);

public:
	OrderPrimaryKey m_PrimaryKey;


	OrderIndexPrimaryAccount m_PrimaryAccountIndex;
	OrderIndexInstrument m_InstrumentIndex;

private:
	MemCacheTemplate<Order> m_MemCache;
};


