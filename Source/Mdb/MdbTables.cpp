#include "MdbTables.h"
#include <string>
#include <string.h>

using std::string;


AccountTable::AccountTable()
{
}
Account* AccountTable::Alloc()
{
	Account* record = m_MemCache.Allocate();
	::memset(record, 0, sizeof(Account));
	return record;
}
void AccountTable::Free(Account* record)
{
	m_MemCache.Free(record);
}
bool AccountTable::Insert(Account* record)
{
	if (!m_PrimaryKey.CheckInsert(record) && !m_PrimaryAccountUniqueKey.CheckInsert(record))
	{
		printf("Insert Failed for Account:[%s]\n", record->GetString());
		return false;
	}

	m_PrimaryKey.Insert(record);
	m_PrimaryAccountUniqueKey.Insert(record);

	m_PrimaryAccountIndex.Insert(record);
	m_BrokerIndex.Insert(record);

	return true;
}
bool AccountTable::Erase(Account* record)
{
	if (!m_PrimaryKey.Erase(record) && !m_PrimaryAccountUniqueKey.Erase(record))
	{
		printf("Erase Failed for Account:[%s]\n", record->GetString());
		return false;
	}

	m_PrimaryAccountIndex.Erase(record);
	m_BrokerIndex.Erase(record);

	return true;
}
bool AccountTable::Update(const Account* oldRecord, const Account* newRecord)
{
	if (!m_PrimaryKey.CheckUpdate(oldRecord, newRecord) && !m_PrimaryAccountUniqueKey.CheckUpdate(oldRecord, newRecord))
	{
		printf("Update Failed for Account:[%s], [%s]\n", oldRecord->GetString(), newRecord->GetString());
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
	
	m_MemCache.Free((Account*)newRecord);
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
	for (auto it = m_PrimaryKey.m_Index.begin(); it != m_PrimaryKey.m_Index.end(); ++it)
	{
		fprintf(dumpFile, "%s\n", (*it)->GetString());
	}
	fclose(dumpFile);
}

OrderTable::OrderTable()
{
}
Order* OrderTable::Alloc()
{
	Order* record = m_MemCache.Allocate();
	::memset(record, 0, sizeof(Order));
	return record;
}
void OrderTable::Free(Order* record)
{
	m_MemCache.Free(record);
}
bool OrderTable::Insert(Order* record)
{
	if (!m_PrimaryKey.CheckInsert(record))
	{
		printf("Insert Failed for Order:[%s]\n", record->GetString());
		return false;
	}

	m_PrimaryKey.Insert(record);

	m_PrimaryAccountIndex.Insert(record);
	m_InstrumentIndex.Insert(record);

	return true;
}
bool OrderTable::Erase(Order* record)
{
	if (!m_PrimaryKey.Erase(record))
	{
		printf("Erase Failed for Order:[%s]\n", record->GetString());
		return false;
	}

	m_PrimaryAccountIndex.Erase(record);
	m_InstrumentIndex.Erase(record);

	return true;
}
bool OrderTable::Update(const Order* oldRecord, const Order* newRecord)
{
	if (!m_PrimaryKey.CheckUpdate(oldRecord, newRecord))
	{
		printf("Update Failed for Order:[%s], [%s]\n", oldRecord->GetString(), newRecord->GetString());
		return false;
	}

	bool PrimaryAccountIndexUpdate = m_PrimaryAccountIndex.NeedUpdate(oldRecord, newRecord);
	if (PrimaryAccountIndexUpdate)
	{
		m_PrimaryAccountIndex.Erase((Order*)oldRecord);
	}
	bool InstrumentIndexUpdate = m_InstrumentIndex.NeedUpdate(oldRecord, newRecord);
	if (InstrumentIndexUpdate)
	{
		m_InstrumentIndex.Erase((Order*)oldRecord);
	}

	::memcpy((void*)oldRecord, newRecord, sizeof(Order));
	
	if (PrimaryAccountIndexUpdate)
	{
		m_PrimaryAccountIndex.Insert((Order*)oldRecord);
	}
	if (InstrumentIndexUpdate)
	{
		m_InstrumentIndex.Insert((Order*)oldRecord);
	}
	
	m_MemCache.Free((Order*)newRecord);
	return true;
}
void OrderTable::Dump(const char* dir)
{
	string fileName = string(dir) + "//t_Order.csv";
	FILE* dumpFile = fopen(fileName.c_str(), "w");
	if (dumpFile == nullptr)
	{
		return;
	}

	fprintf(dumpFile, "OrgID, BrokerID, AccountID, PrimaryAccountID, AccountClass, AccountType, ExchangeID, InstrumentID, Direction, OffsetFlag, HedgeFlag, OrderSysID, OrderLocalID, BrokerOrderID, OrderStatus, OrderType, Volume, VolumeTraded, InsertDate, InsertTime\n");
	char buff[4096] = { 0 };
	for (auto it = m_PrimaryKey.m_Index.begin(); it != m_PrimaryKey.m_Index.end(); ++it)
	{
		fprintf(dumpFile, "%s\n", (*it)->GetString());
	}
	fclose(dumpFile);
}


