#pragma once
#include "DataStruct.h"


struct AccountHashForPrimaryKey
{
	size_t operator()(const Account* item) const;
};
struct AccountEqualForPrimaryKey
{
	bool operator()(const Account* left, const Account* right) const;
};




