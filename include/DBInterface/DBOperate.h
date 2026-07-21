#pragma once
#include <PersonalLib/Types.h>

class DBOperate
{
public:
	static DBOperate* Allocate();
	virtual void Deallocate() = 0;
	virtual void DeallocateRecord() = 0;


	DBOperateType Operate;
	unsigned int TableID;
	unsigned int IndexID;
	void* Record;
};
