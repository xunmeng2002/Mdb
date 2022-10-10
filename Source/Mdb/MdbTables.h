#pragma once
#include "DataStruct.h"
#include "PrimaryKeys.h"
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
	AccountPrimaryKeyDefault m_DefaultPrimaryKey;
	AccountPrimaryKeyPrimaryAccount m_PrimaryAccountPrimaryKey;

	AccountIndexPrimaryAccount m_PrimaryAccountIndex;
	AccountIndexBroker m_BrokerIndex;

private:
	MemCacheTemplate<Account> m_MemCache;
};


