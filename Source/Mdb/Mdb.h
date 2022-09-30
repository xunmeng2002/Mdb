#pragma once
#include "MdbTables.h"


class Mdb
{
public:
	Mdb();
	void Dump(const char* dir);
	void Clear();

public:
	AccountTable* t_Account;
};



