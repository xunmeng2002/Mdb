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
	void Free(Account* account);
	bool Insert(Account* account);
	bool Erase(Account* account);
	bool Update(const Account* oldAccount, const Account* newAccount);
	void Dump(const char* dir);

public:
	AccountPrimaryKey m_PrimaryKey;
	AccountIndexForPrimaryAccount m_PrimaryAccountIndex;


private:
	bool m_PrimaryAccountIndexUpdate;

	MemCacheTemplate<Account> m_MemCache;
};




