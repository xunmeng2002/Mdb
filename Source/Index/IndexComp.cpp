#include "IndexComp.h"
#include <string.h>



bool AccountEqualForPrimaryAccountIndex::operator()(const Account* const left, const Account* const right) const
{
	return left->BrokerID == right->BrokerID && strcmp(left->PrimaryAccountID, right->PrimaryAccountID) == 0&& strcmp(left->CurrencyID, right->CurrencyID) == 0;
}
bool AccountLessForPrimaryAccountIndex::operator()(const Account* const left, const Account* const right) const
{
	if (left->BrokerID < right->BrokerID)
		return true;
	else if (left->BrokerID > right->BrokerID)
		return false;
	if (strcmp(left->PrimaryAccountID, right->PrimaryAccountID) < 0)
		return true;
	else if (strcmp(left->PrimaryAccountID, right->PrimaryAccountID) > 0)
		return false;
	if (strcmp(left->CurrencyID, right->CurrencyID) < 0)
		return true;
	else if (strcmp(left->CurrencyID, right->CurrencyID) > 0)
		return false;
	return false;
}

bool AccountEqualForBrokerIndex::operator()(const Account* const left, const Account* const right) const
{
	return left->BrokerID == right->BrokerID && left->AccountType == right->AccountType ;
}
bool AccountLessForBrokerIndex::operator()(const Account* const left, const Account* const right) const
{
	if (left->BrokerID < right->BrokerID)
		return true;
	else if (left->BrokerID > right->BrokerID)
		return false;
	if (left->AccountType < right->AccountType)
		return true;
	else if (left->AccountType > right->AccountType)
		return false;
	return false;
}

bool OrderEqualForPrimaryAccountIndex::operator()(const Order* const left, const Order* const right) const
{
	return left->BrokerID == right->BrokerID && strcmp(left->PrimaryAccountID, right->PrimaryAccountID) == 0&& left->AccountClass == right->AccountClass && left->AccountType == right->AccountType && strcmp(left->OrderSysID, right->OrderSysID) == 0;
}
bool OrderLessForPrimaryAccountIndex::operator()(const Order* const left, const Order* const right) const
{
	if (left->BrokerID < right->BrokerID)
		return true;
	else if (left->BrokerID > right->BrokerID)
		return false;
	if (strcmp(left->PrimaryAccountID, right->PrimaryAccountID) < 0)
		return true;
	else if (strcmp(left->PrimaryAccountID, right->PrimaryAccountID) > 0)
		return false;
	if (left->AccountClass < right->AccountClass)
		return true;
	else if (left->AccountClass > right->AccountClass)
		return false;
	if (left->AccountType < right->AccountType)
		return true;
	else if (left->AccountType > right->AccountType)
		return false;
	if (strcmp(left->OrderSysID, right->OrderSysID) < 0)
		return true;
	else if (strcmp(left->OrderSysID, right->OrderSysID) > 0)
		return false;
	return false;
}

bool OrderEqualForInstrumentIndex::operator()(const Order* const left, const Order* const right) const
{
	return strcmp(left->ExchangeID, right->ExchangeID) == 0&& strcmp(left->InstrumentID, right->InstrumentID) == 0;
}
bool OrderLessForInstrumentIndex::operator()(const Order* const left, const Order* const right) const
{
	if (strcmp(left->ExchangeID, right->ExchangeID) < 0)
		return true;
	else if (strcmp(left->ExchangeID, right->ExchangeID) > 0)
		return false;
	if (strcmp(left->InstrumentID, right->InstrumentID) < 0)
		return true;
	else if (strcmp(left->InstrumentID, right->InstrumentID) > 0)
		return false;
	return false;
}

