#pragma once
#include "DBOperate.h"

class DBOperateImpl : public DBOperate
{
public:
	virtual void Free() override;
	virtual void FreeRecord() override;
	virtual const char* GetDebugString() const override;
};
