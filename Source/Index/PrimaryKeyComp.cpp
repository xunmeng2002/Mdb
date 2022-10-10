#include "PrimaryKeyComp.h"
#include <string>
#include <functional>

using std::string;


bool AccountEqualForDefault::operator()(const Account* const left, const Account* const right) const
{
	return left->BrokerID == right->BrokerID && strcmp(left->AccountID, right->AccountID) == 0 && left->AccountClass == right->AccountClass;
}
size_t AccountHashForDefault::operator()(const Account* const record) const
{
	return std::hash<int>()(record->BrokerID) + std::hash<string>()(record->AccountID) + std::hash<char>()((char)record->AccountClass);
};

