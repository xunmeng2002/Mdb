#include "UniqueKeyComp.h"
#include <string>
#include <functional>

using std::string;


bool AccountEqualForPrimaryAccountUniqueKey::operator()(const Account* const left, const Account* const right) const
{
	return left->BrokerID == right->BrokerID && strcmp(left->PrimaryAccountID, right->PrimaryAccountID) == 0 && left->AccountClass == right->AccountClass;
}
size_t AccountHashForPrimaryAccountUniqueKey::operator()(const Account* const record) const
{
	return std::hash<long long>()(record->BrokerID) + std::hash<string>()(record->PrimaryAccountID) + std::hash<char>()((char)record->AccountClass);
}

