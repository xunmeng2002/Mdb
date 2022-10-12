#include "PrimaryKeyComp.h"
#include <string>
#include <functional>

using std::string;


bool AccountEqualForDefaultPrimaryKey::operator()(const Account* const left, const Account* const right) const
{
	return left->BrokerID == right->BrokerID && strcmp(left->AccountID, right->AccountID) == 0 && left->AccountClass == right->AccountClass;
}
size_t AccountHashForDefaultPrimaryKey::operator()(const Account* const record) const
{
	return std::hash<long long>()(record->BrokerID) + std::hash<string>()(record->AccountID) + std::hash<char>()((char)record->AccountClass);
}

bool AccountEqualForPrimaryAccountPrimaryKey::operator()(const Account* const left, const Account* const right) const
{
	return left->BrokerID == right->BrokerID && strcmp(left->PrimaryAccountID, right->PrimaryAccountID) == 0 && left->AccountClass == right->AccountClass;
}
size_t AccountHashForPrimaryAccountPrimaryKey::operator()(const Account* const record) const
{
	return std::hash<long long>()(record->BrokerID) + std::hash<string>()(record->PrimaryAccountID) + std::hash<char>()((char)record->AccountClass);
}

bool OrderEqualForDefaultPrimaryKey::operator()(const Order* const left, const Order* const right) const
{
	return left->BrokerID == right->BrokerID && strcmp(left->AccountID, right->AccountID) == 0 && left->AccountClass == right->AccountClass && strcmp(left->InsertDate, right->InsertDate) == 0 && left->OrderLocalID == right->OrderLocalID;
}
size_t OrderHashForDefaultPrimaryKey::operator()(const Order* const record) const
{
	return std::hash<long long>()(record->BrokerID) + std::hash<string>()(record->AccountID) + std::hash<char>()((char)record->AccountClass) + std::hash<string>()(record->InsertDate) + std::hash<long long>()(record->OrderLocalID);
}

