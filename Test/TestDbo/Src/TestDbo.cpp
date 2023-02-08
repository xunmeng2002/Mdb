#include <iostream>
#include <string.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_error.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/resultset.h>
#include "DboStructs.h"
#include "DboDB.h"

using namespace std;
using namespace sql;
using namespace sql::mysql;

DboAuthorizedSoftware* PrepareDboAuthorizedSoftware(int index)
{
	DboAuthorizedSoftware* record = new DboAuthorizedSoftware();
	strcpy(record->Software, std::to_string(index).c_str());
	strcpy(record->AuthCode, std::to_string(index).c_str());
	record->Account = index;
	return record;
}
void Print(DboAuthorizedSoftware* record)
{
	printf("Software:%s, AuthCode:%s, Account:%d\n", record->Software, record->AuthCode, record->Account);
}

int main()
{
	string host = "tcp://192.168.137.128:3306/Test";
	string user = "test";
	string passwd = "test";
	DboDB* dbo = new DboDB(host, user, passwd);

	dbo->TruncateDboAuthorizedSoftware();
	for (auto i = 0; i < 10; i++)
	{
		auto record = PrepareDboAuthorizedSoftware(i);
		dbo->InsertDboAuthorizedSoftware(record);
		delete record;
	}
	auto otherRecord = PrepareDboAuthorizedSoftware(2);
	strcpy(otherRecord->Software, "1");
	dbo->InsertDboAuthorizedSoftware(otherRecord);

	SoftwareType software("2");
	AuthCodeType authCode("2");
	AccountType account(2);

	auto record = dbo->QueryDboAuthorizedSoftwareFromPrimaryKey(account, software);
	cout << "QueryDboAuthorizedSoftwareFromPrimaryKey" << account << ", " << software << endl;
	if (record)
	{
		Print(record);
	}
	record = dbo->QueryDboAuthorizedSoftwareFromUniqueKeyAuthCode(software, authCode);
	cout << "QueryDboAuthorizedSoftwareFromUniqueKeyAuthCode:" << software <<", " << authCode << endl;
	if (record)
	{
		Print(record);
	}
	vector< DboAuthorizedSoftware*> records;
	dbo->QueryDboAuthorizedSoftwareFromIndexAccount(records, authCode, account);
	cout << "QueryDboAuthorizedSoftwareFromIndexAccount" << authCode << ", " << account << endl;
	if (records.size() > 0)
	{
		for (auto record : records)
		{
			Print(record);
		}
	}

	dbo->DeleteDboAuthorizedSoftware(otherRecord);

	records.clear();
	dbo->QueryDboAuthorizedSoftwareFromIndexAccount(records, authCode, account);
	cout << "QueryDboAuthorizedSoftwareFromIndexAccount After Delete" << authCode << ", " << account << endl;
	if (records.size() > 0)
	{
		for (auto record : records)
		{
			Print(record);
		}
	}

	dbo->TruncateDboAuthorizedSoftware();

	return 0;
}

