#include <iostream>
#include "DataStruct.h"
#include "Mdb.h"
#include "MdbTables.h"
#include "TimeUtility.h"
#include <string>
#include <direct.h>

using namespace std;

void PrepareAccount(Account* account, int index)
{
	strcpy(account->OrgID, "Pobo");
	account->BrokerID = index;
	strcpy(account->AccountID, to_string(index).c_str());
	account->AccountClass = CAccountClassType::Future;
	account->AccountType = CAccountTypeType::Primary;
	strcpy(account->AccountName, to_string(index).c_str());
	account->AccountStatus = CAccountStatusType::Normal;
	strcpy(account->PrimaryAccountID, to_string(index + 1).c_str());
	strcpy(account->CurrencyID, "CNY");
}
const Account* SelectAccount(Mdb* mdb, const CBrokerIDType& brokerID, const CAccountIDType& accountID, const CAccountClassType& accountClass)
{
	auto account = mdb->t_Account->m_DefaultPrimaryKey.Select(brokerID, accountID, accountClass);
	if (account)
	{
		printf("Select Account: %s\n", account->GetString());
		printf("Select Account: %s\n", account->GetDebugString());
	}
	else
	{
		printf("Not Find Account For BrokerID:[%lld], AccountID:[%s], AccountClass:[%c]\n", brokerID, accountID, accountClass);
	}
	return account;
}

void TestUpdateAccount(Mdb* mdb)
{
	auto newAccount = mdb->t_Account->Alloc();
	PrepareAccount(newAccount, 1);
	strcpy(newAccount->CurrencyID, "USD");
	newAccount->AccountType = CAccountTypeType::Sub;

	auto oldAccount = SelectAccount(mdb, newAccount->BrokerID, newAccount->AccountID, newAccount->AccountClass);
	if (!oldAccount)
	{
		return;
	}
	mdb->t_Account->Update(oldAccount, newAccount);


	auto account = SelectAccount(mdb, newAccount->BrokerID, newAccount->AccountID, newAccount->AccountClass);
	printf("Account After Update, Account:%s\n", account->GetDebugString());
}
void TestSelectFromIndex(Mdb* mdb)
{
	cout << "TestSelectFromIndex" << endl;

	auto lowAccount = mdb->t_Account->Alloc();
	PrepareAccount(lowAccount, 1);
	strcpy(lowAccount->CurrencyID, "USD");
	cout << "lowAccount: " << lowAccount->GetDebugString() << endl;
	auto upAccount = mdb->t_Account->Alloc();
	PrepareAccount(upAccount, 10);
	cout << "upAccount: " << upAccount->GetDebugString() << endl;


	auto startIt = mdb->t_Account->m_PrimaryAccountIndex.LowerBound(lowAccount);
	auto endIt = mdb->t_Account->m_PrimaryAccountIndex.UpperBound(upAccount);
	for (auto& it = startIt; it != endIt; it++)
	{
		cout << (*it)->GetDebugString() << endl;
	}
}
void TestSelectFromEqualRange(Mdb* mdb)
{
	cout << "TestSelectFromEqualRange" << endl;

	auto account = mdb->t_Account->Alloc();
	PrepareAccount(account, 1);
	strcpy(account->CurrencyID, "USD");

	auto p = mdb->t_Account->m_PrimaryAccountIndex.EqualRange(account);
	for (auto& it = p.first; it != p.second; it++)
	{
		cout << (*it)->GetDebugString() << endl;
	}
}


int main()
{
	Mdb* mdb = new Mdb();

	for (auto i = 0; i < 10; i++)
	{
		auto account = mdb->t_Account->Alloc();
		PrepareAccount(account, i);
		mdb->t_Account->Insert(account);
	}
	{
		auto account = mdb->t_Account->Alloc();
		PrepareAccount(account, 1);
		mdb->t_Account->Insert(account);
	}

	auto dateTime = GetLocalDate();
	char path[128] = { 0 };
	sprintf(path, "%s", dateTime.c_str());

	auto ret = mkdir(path);
	if (ret != 0)
	{
		printf("mkdir Failed for: %s, ret:%d\n", path, ret);
	}

	mdb->Dump(dateTime.c_str());

	SelectAccount(mdb, 1, CAccountIDType("1"), CAccountClassType::Future);
	SelectAccount(mdb, 2, CAccountIDType("3"), CAccountClassType::Future);
	SelectAccount(mdb, 3, CAccountIDType("3"), CAccountClassType::Future);

	TestUpdateAccount(mdb);

	TestSelectFromIndex(mdb);

	TestSelectFromEqualRange(mdb);

	return 0;
}


