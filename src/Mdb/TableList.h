#pragma once
#include "MdbStructs.h"


namespace mdb
{
	struct TableList
	{
		const char* Name;
		const unsigned int* TableIDs;
		int Count;
	};
}
