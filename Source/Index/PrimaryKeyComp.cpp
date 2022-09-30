#include "PrimaryKeyComp.h"
#include <string>
#include <functional>

using std::string;

size_t AccountHashForPrimaryKey::operator()(const Account* item) const
{
	return std::hash<int>()(item->BrokerID) + std::hash<string>()(item->AccountID) + std::hash<char>()((char)item->AccountClass);
}
bool AccountEqualForPrimaryKey::operator()(const Account* left, const Account* right) const
{
	return left->BrokerID == right->BrokerID && strcmp(left->AccountID, right->AccountID) == 0 && left->AccountClass == right->AccountClass;
}


