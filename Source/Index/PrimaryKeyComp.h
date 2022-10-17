#pragma once
#include "DataStruct.h"


struct AccountEqualForAccountPrimaryKey
{
	bool operator()(const Account* const left, const Account* const right) const;
};
struct AccountHashForAccountPrimaryKey
{
	size_t operator()(const Account* const record) const;
};

struct OrderEqualForOrderPrimaryKey
{
	bool operator()(const Order* const left, const Order* const right) const;
};
struct OrderHashForOrderPrimaryKey
{
	size_t operator()(const Order* const record) const;
};



