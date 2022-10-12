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

struct OrderEqualForDefaultPrimaryKey
{
	bool operator()(const Order* const left, const Order* const right) const;
};
struct OrderHashForDefaultPrimaryKey
{
	size_t operator()(const Order* const record) const;
};



