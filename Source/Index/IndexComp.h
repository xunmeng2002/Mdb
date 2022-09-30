#pragma once

#include "DataStruct.h"


struct AccountEqualForPrimaryAccount
{
	bool operator()(const Account* const left, const Account* const right) const;
};
struct AccountLessForPrimaryAccount
{
	bool operator()(const Account* const left, const Account* const right) const;
};

