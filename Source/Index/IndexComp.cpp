#include "IndexComp.h"
#include <string>

using std::string;



bool AccountEqualForPrimaryAccount::operator()(const Account* const left, const Account* const right) const
{
	return left->BrokerID == right->BrokerID && strcmp(left->PrimaryAccountID, right->PrimaryAccountID) == 0;
}
bool AccountLessForPrimaryAccount::operator()(const Account* const left, const Account* const right) const
{
	if (left->BrokerID < right->BrokerID)
	{
		return true;
	}
	else if (left->BrokerID > right->BrokerID)
	{
		return false;
	}
	if (strcmp(left->PrimaryAccountID, right->PrimaryAccountID) < 0)
	{
		return true;
	}
	else if (strcmp(left->PrimaryAccountID, right->PrimaryAccountID) > 0)
	{
		return false;
	}
	return false;
};

bool AccountEqualForBroker::operator()(const Account* const left, const Account* const right) const
{
	return left->BrokerID == right->BrokerID ;
}
bool AccountLessForBroker::operator()(const Account* const left, const Account* const right) const
{
	if (left->BrokerID < right->BrokerID)
	{
		return true;
	}
	else if (left->BrokerID > right->BrokerID)
	{
		return false;
	}
	return false;
};

