#pragma once

#include "DataStruct.h"


struct AccountEqualForPrimaryAccountIndex
{
	bool operator()(const Account* const left, const Account* const right) const;
};
struct AccountLessForPrimaryAccountIndex
{
	bool operator()(const Account* const left, const Account* const right) const;
};

struct AccountEqualForBrokerIndex
{
	bool operator()(const Account* const left, const Account* const right) const;
};
struct AccountLessForBrokerIndex
{
	bool operator()(const Account* const left, const Account* const right) const;
};

struct OrderEqualForPrimaryAccountIndex
{
	bool operator()(const Order* const left, const Order* const right) const;
};
struct OrderLessForPrimaryAccountIndex
{
	bool operator()(const Order* const left, const Order* const right) const;
};

struct OrderEqualForInstrumentIndex
{
	bool operator()(const Order* const left, const Order* const right) const;
};
struct OrderLessForInstrumentIndex
{
	bool operator()(const Order* const left, const Order* const right) const;
};

