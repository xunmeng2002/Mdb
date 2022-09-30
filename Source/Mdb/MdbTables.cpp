#include "MdbTables.h"
#include <string>

using std::string;

AccountTable::AccountTable()
	:m_PrimaryAccountIndexUpdate(false)
{

}
Account* AccountTable::Alloc()
{
	return m_MemCache.Allocate();
}
void AccountTable::Free(Account* account)
{
	m_MemCache.Free(account);
}
bool AccountTable::Insert(Account* account)
{
	if (!m_PrimaryKey.Insert(account))
	{
		printf("AccountTable Insert Failed for Account: BrokerID[%d], AccountID:[%s].\n", account->BrokerID, account->AccountID);
		return false;
	}
	m_PrimaryAccountIndex.Insert(account);
	return true;
}
bool AccountTable::Erase(Account* account)
{
	if (!m_PrimaryKey.Erase(account))
	{
		printf("AccountTable Erase Failed for Account: BrokerID[%d], AccountID:[%s].\n", account->BrokerID, account->AccountID);
		return false;
	}
	m_PrimaryAccountIndex.Erase(account);

	return true;
}
bool AccountTable::Update(const Account* oldAccount, const Account* newAccount)
{
	if (!m_PrimaryKey.CheckUpdate(oldAccount, newAccount))
	{
		return false;
	}
	m_PrimaryAccountIndexUpdate = m_PrimaryAccountIndex.NeedUpdate(oldAccount, newAccount);
	if (m_PrimaryAccountIndexUpdate)
	{
		m_PrimaryAccountIndex.Erase((Account*)oldAccount);
	}
	::memcpy((void*)oldAccount, newAccount, sizeof(Account));
	
	if (m_PrimaryAccountIndexUpdate)
	{
		m_PrimaryAccountIndex.Insert((Account*)oldAccount);
	}

	return true;
}
void AccountTable::Dump(const char* dir)
{
	string fileName = string(dir) + "//t_Account.csv";
	FILE* dumpFile = fopen(fileName.c_str(), "w");
	if (dumpFile == nullptr)
	{
		return;
	}
	fprintf(dumpFile, "BrokerID, AccountID, TradingDay, OrgID, AccountName, AccountClass, AccountType, PrimaryAccountID, AccountStatus, CurrencyID, DeleteFlag\n");
	char buff[4096] = { 0 };
	for (auto it = m_PrimaryKey.m_Index.begin(); it != m_PrimaryKey.m_Index.end(); ++it)
	{
		fprintf(dumpFile, "%d, %s, %s, %s, %s, %c, %c, %s, %c, %s, %d\n",
			(*it)->BrokerID, (*it)->AccountID, (*it)->TradingDay, (*it)->OrgID, (*it)->AccountName, (*it)->AccountClass, (*it)->AccountType, (*it)->PrimaryAccountID, (*it)->AccountStatus, (*it)->CurrencyID, (*it)->DeleteFlag);
	}
	fclose(dumpFile);
}
