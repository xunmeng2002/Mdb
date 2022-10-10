#pragma once
#include "DataStruct.h"


struct AccountEqualForDefaultPrimaryKey
{
	bool operator()(const Account* const left, const Account* const right) const;
};
struct AccountHashForDefaultPrimaryKey
{
	size_t operator()(const Account* const record) const;
};

struct AccountEqualForPrimaryAccountPrimaryKey
{
	bool operator()(const Account* const left, const Account* const right) const;
};
struct AccountHashForPrimaryAccountPrimaryKey
{
	size_t operator()(const Account* const record) const;
};



