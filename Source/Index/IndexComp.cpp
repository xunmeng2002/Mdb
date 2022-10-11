#include "IndexComp.h"
#include <string>

using std::string;



bool AccountEqualForPrimaryAccountIndex::operator()(const Account* const left, const Account* const right) const
{
	return left->BrokerID == right->BrokerID && strcmp(left->PrimaryAccountID, right->PrimaryAccountID) == 0&& strcmp(left->CurrencyID, right->CurrencyID) == 0;
}
bool AccountLessForPrimaryAccountIndex::operator()(const Account* const left, const Account* const right) const
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
	if (strcmp(left->CurrencyID, right->CurrencyID) < 0)
	{
		return true;
	}
	else if (strcmp(left->CurrencyID, right->CurrencyID) > 0)
	{
		return false;
	}
	return false;
};

bool AccountEqualForBrokerIndex::operator()(const Account* const left, const Account* const right) const
{
	return left->BrokerID == right->BrokerID && left->AccountType == right->AccountType ;
}
bool AccountLessForBrokerIndex::operator()(const Account* const left, const Account* const right) const
{
	if (left->BrokerID < right->BrokerID)
	{
		return true;
	}
	else if (left->BrokerID > right->BrokerID)
	{
		return false;
	}
	if (left->AccountType < right->AccountType)
	{
		return true;
	}
	else if (left->AccountType > right->AccountType)
	{
		return false;
	}
	return false;
};

