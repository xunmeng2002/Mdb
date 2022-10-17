#pragma once
#include "DataStruct.h"


struct AccountEqualForPrimaryAccountUniqueKey
{
	bool operator()(const Account* const left, const Account* const right) const;
};
struct AccountHashForPrimaryAccountUniqueKey
{
	size_t operator()(const Account* const record) const;
};



