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

