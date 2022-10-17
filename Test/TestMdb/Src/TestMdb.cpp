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
	auto account = mdb->t_Account->m_PrimaryKey.Select(brokerID, accountID, accountClass);
	if (account)
	{
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
void TestSelectAccountFromIndex(Mdb* mdb)
{
	cout << "TestSelectAccountFromIndex" << endl;

	auto lowAccount = mdb->t_Account->Alloc();
	PrepareAccount(lowAccount, 1);
	//strcpy(lowAccount->CurrencyID, "USD");
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
void TestSelectAccountFromEqualRange(Mdb* mdb)
{
	cout << "TestSelectAccountFromEqualRange" << endl;

	auto account = mdb->t_Account->Alloc();
	PrepareAccount(account, 1);
	//strcpy(account->CurrencyID, "USD");

	auto p = mdb->t_Account->m_PrimaryAccountIndex.EqualRange(account);
	for (auto& it = p.first; it != p.second; it++)
	{
		cout << (*it)->GetDebugString() << endl;
	}
}
void TestAccountTable(Mdb* mdb)
{
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

	SelectAccount(mdb, 1, CAccountIDType("1"), CAccountClassType::Future);
	SelectAccount(mdb, 2, CAccountIDType("3"), CAccountClassType::Future);
	SelectAccount(mdb, 3, CAccountIDType("3"), CAccountClassType::Future);

	TestUpdateAccount(mdb);

	TestSelectAccountFromIndex(mdb);

	TestSelectAccountFromEqualRange(mdb);
}

void PrepareOrder(Order* order, int index)
{
	strcpy(order->OrgID, "Pobo");
	order->BrokerID = index;
	strcpy(order->AccountID, to_string(index).c_str());
	strcpy(order->PrimaryAccountID, to_string(index).c_str());
	order->AccountClass = CAccountClassType::Future;
	order->AccountType = CAccountTypeType::Primary;
	strcpy(order->ExchangeID, "CFFEX");
	strcpy(order->InstrumentID, "IF2212");
	order->Direction = CDirectionType::Buy;
	order->OffsetFlag = COffsetFlagType::Open;
	order->HedgeFlag = CHedgeFlagType::Speculation;
	strcpy(order->OrderSysID, to_string(index).c_str());
	order->OrderLocalID = index;
	strcpy(order->BrokerOrderID, to_string(index).c_str());
	order->OrderStatus = COrderStatusType::BrokerAccepted;
	order->OrderType = COrderTypeType::Normal;
	order->Volume = index;
	order->VolumeTraded = 0;
	strcpy(order->InsertDate, "20221012");
	strcpy(order->InsertTime, "10:11:12");
}
const Order* SelectOrder(Mdb* mdb, const CBrokerIDType& brokerID, const CAccountIDType& accountID, const CAccountClassType& accountClass, const CDateType& insertDate, const COrderLocalIDType& orderLocalID)
{
	auto order = mdb->t_Order->m_PrimaryKey.Select(brokerID, accountID, accountClass, insertDate, orderLocalID);
	if (order)
	{
		printf("Select Order: %s\n", order->GetDebugString());
	}
	else
	{
		printf("Not Find Order For BrokerID:[%lld], AccountID:[%s], AccountClass:[%c], InsertDate:[%s], OrderLocalID:[%lld]\n", brokerID, accountID, accountClass, insertDate, orderLocalID);
	}
	return order;
}
void TestUpdateOrder(Mdb* mdb)
{
	auto newOrder = mdb->t_Order->Alloc();
	PrepareOrder(newOrder, 1);
	auto oldOrder = SelectOrder(mdb, newOrder->BrokerID, newOrder->AccountID, newOrder->AccountClass, newOrder->InsertDate, newOrder->OrderLocalID);
	if (!oldOrder)
	{
		return;
	}
	newOrder->OrderLocalID = 10;
	mdb->t_Order->Update(oldOrder, newOrder);

	newOrder->OrderLocalID = 1;
	strcpy(newOrder->OrderSysID, "10");
	mdb->t_Order->Update(oldOrder, newOrder);

	auto order = SelectOrder(mdb, newOrder->BrokerID, newOrder->AccountID, newOrder->AccountClass, newOrder->InsertDate, newOrder->OrderLocalID);
	printf("Order After Update, Order:%s\n", order->GetDebugString());
}
void TestSelectOrderFromIndex(Mdb* mdb)
{
	cout << "TestSelectOrderFromIndex" << endl;

	auto lowOrder = mdb->t_Order->Alloc();
	PrepareOrder(lowOrder, 1);
	cout << "lowOrder: " << lowOrder->GetDebugString() << endl;
	auto upOrder = mdb->t_Order->Alloc();
	PrepareOrder(upOrder, 10);
	cout << "upOrder: " << upOrder->GetDebugString() << endl;


	auto startIt = mdb->t_Order->m_PrimaryAccountIndex.LowerBound(lowOrder);
	auto endIt = mdb->t_Order->m_PrimaryAccountIndex.UpperBound(upOrder);
	for (auto& it = startIt; it != endIt; it++)
	{
		cout << (*it)->GetDebugString() << endl;
	}
}
void TestSelectOrderFromEqualRange(Mdb* mdb)
{
	cout << "TestSelectOrderFromEqualRange" << endl;

	auto order = mdb->t_Order->Alloc();
	PrepareOrder(order, 2);

	cout << "EqualRange of InstrumentIndex" << endl;
	auto p = mdb->t_Order->m_InstrumentIndex.EqualRange(order);
	for (auto& it = p.first; it != p.second; it++)
	{
		cout << (*it)->GetDebugString() << endl;
	}

	cout << "EqualRange of PrimaryAccountIndex" << endl;
	auto p2 = mdb->t_Order->m_PrimaryAccountIndex.EqualRange(order);
	for (auto& it = p2.first; it != p2.second; it++)
	{
		cout << (*it)->GetDebugString() << endl;
	}
}
void TestOrderTable(Mdb* mdb)
{
	for (auto i = 0; i < 10; i++)
	{
		auto order = mdb->t_Order->Alloc();
		PrepareOrder(order, i);
		mdb->t_Order->Insert(order);
	}
	{
		auto order = mdb->t_Order->Alloc();
		PrepareOrder(order, 1);
		mdb->t_Order->Insert(order);
	}


	SelectOrder(mdb, 1, CAccountIDType("1"), CAccountClassType::Future, "20221012", 1);
	SelectOrder(mdb, 2, CAccountIDType("3"), CAccountClassType::Future, "20221012", 2);
	SelectOrder(mdb, 3, CAccountIDType("3"), CAccountClassType::Future, "20221012", 3);

	TestUpdateOrder(mdb);

	TestSelectOrderFromIndex(mdb);

	TestSelectOrderFromEqualRange(mdb);
}

int main()
{
	Mdb* mdb = new Mdb();
	TestAccountTable(mdb);
	TestOrderTable(mdb);


	auto dateTime = GetLocalDate();
	char path[128] = { 0 };
	sprintf(path, "%s", dateTime.c_str());

	auto ret = mkdir(path);
	if (ret != 0)
	{
		printf("mkdir Failed for: %s, ret:%d\n", path, ret);
	}
	mdb->Dump(dateTime.c_str());
	return 0;
}


