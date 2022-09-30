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
	account->BrokerID = index;
	strcpy(account->AccountID, to_string(index).c_str());
	account->AccountClass = CAccountClassType::AC_Future;
	account->AccountType = CAccountTypeType::AT_Primary;
	strcpy(account->AccountName, to_string(index).c_str());
	account->AccountStatus = CAccountStatusType::AS_Normal;
}


int main()
{
	Mdb mdb;

	for (auto i = 0; i < 10; i++)
	{
		auto account = mdb.t_Account->Alloc();
		PrepareAccount(account, i);
		mdb.t_Account->Insert(account);
	}

	auto dateTime = GetLocalDate();
	char path[128] = { 0 };
	sprintf(path, "%s", dateTime.c_str());

	auto ret = mkdir(path);
	if (ret != 0)
	{
		printf("_mkdir Failed for: %s, ret:%d\n", path, ret);
	}

	mdb.Dump(dateTime.c_str());

	return 0;
}


