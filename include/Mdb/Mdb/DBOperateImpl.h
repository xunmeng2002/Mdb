#pragma once
#include "PersonalLib/TemplateLib/DB/DBOperate.h"

class DBOperateImpl : public DBOperate
{
public:
	virtual void Deallocate() override;
	virtual void DeallocateRecord() override;
	virtual const char* GetDebugString() const override;
};
