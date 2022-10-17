#include "PrimaryKeyComp.h"
#include <string>
#include <functional>

using std::string;


bool AccountEqualForAccountPrimaryKey::operator()(const Account* const left, const Account* const right) const
{
	return left->BrokerID == right->BrokerID && strcmp(left->AccountID, right->AccountID) == 0 && left->AccountClass == right->AccountClass;
}
size_t AccountHashForAccountPrimaryKey::operator()(const Account* const record) const
{
	return std::hash<long long>()(record->BrokerID) + std::hash<string>()(record->AccountID) + std::hash<char>()((char)record->AccountClass);
}

bool OrderEqualForOrderPrimaryKey::operator()(const Order* const left, const Order* const right) const
{
	return left->BrokerID == right->BrokerID && strcmp(left->AccountID, right->AccountID) == 0 && left->AccountClass == right->AccountClass && strcmp(left->InsertDate, right->InsertDate) == 0 && left->OrderLocalID == right->OrderLocalID;
}
size_t OrderHashForOrderPrimaryKey::operator()(const Order* const record) const
{
	return std::hash<long long>()(record->BrokerID) + std::hash<string>()(record->AccountID) + std::hash<char>()((char)record->AccountClass) + std::hash<string>()(record->InsertDate) + std::hash<long long>()(record->OrderLocalID);
}

