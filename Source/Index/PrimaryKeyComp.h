#pragma once
#include "DataStruct.h"


struct AccountEqualForDefault
{
	bool operator()(const Account* const left, const Account* const right) const;
};
struct AccountHashForDefault
{
	size_t operator()(const Account* const record) const;
};



