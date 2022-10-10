#include "MdbTables.h"
#include <string>

using std::string;


AccountTable::AccountTable()
{
}
Account* AccountTable::Alloc()
{
	return m_MemCache.Allocate();
}
void AccountTable::Free(Account* record)
{
	m_MemCache.Free(record);
}
bool AccountTable::Insert(Account* record)
{
	if (!m_DefaultPrimaryKey.Insert(record))
	{
		printf("AccountTable Insert Failed for Account:[%s]\n", record->GetString());
		return false;
	}

	m_PrimaryAccountIndex.Insert(record);
	m_BrokerIndex.Insert(record);

	return true;
}
bool AccountTable::Erase(Account* record)
{
	if (!m_DefaultPrimaryKey.Erase(record))
	{
		printf("AccountTable Erase Failed for Account:[%s]\n", record->GetString());
		return false;
	}

	m_PrimaryAccountIndex.Erase(record);
	m_BrokerIndex.Erase(record);

	return true;
}
bool AccountTable::Update(const Account* oldRecord, const Account* newRecord)
{
	if (!m_DefaultPrimaryKey.CheckUpdate(oldRecord, newRecord))
	{
		printf("AccountTable Update Failed for Account:[%s], [%s]\n", oldRecord->GetString(), newRecord->GetString());
		return false;
	}

	bool PrimaryAccountIndexUpdate = m_PrimaryAccountIndex.NeedUpdate(oldRecord, newRecord);
	if (PrimaryAccountIndexUpdate)
	{
		m_PrimaryAccountIndex.Erase((Account*)oldRecord);
	}
	bool BrokerIndexUpdate = m_BrokerIndex.NeedUpdate(oldRecord, newRecord);
	if (BrokerIndexUpdate)
	{
		m_BrokerIndex.Erase((Account*)oldRecord);
	}

	::memcpy((void*)oldRecord, newRecord, sizeof(Account));
	
	if (PrimaryAccountIndexUpdate)
	{
		m_PrimaryAccountIndex.Insert((Account*)oldRecord);
	}
	if (BrokerIndexUpdate)
	{
		m_BrokerIndex.Insert((Account*)oldRecord);
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

	fprintf(dumpFile, "OrgID, BrokerID, AccountID, AccountName, AccountClass, AccountType, PrimaryAccountID, AccountStatus, CurrencyID, DeleteFlag\n");
	char buff[4096] = { 0 };
	for (auto it = m_DefaultPrimaryKey.m_Index.begin(); it != m_DefaultPrimaryKey.m_Index.end(); ++it)
	{
		fprintf(dumpFile, "%s\n",
			(*it)->GetString());
	}
	fclose(dumpFile);
}


