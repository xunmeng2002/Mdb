#include "DBOperateImpl.h"
#include <PersonalLib/TemplateLib/TemplateLib.h>
#include <cstring>


DBOperate* DBOperate::Allocate()
{
	return ObjectPool<DBOperateImpl>::GetInstance().Allocate();
}
void DBOperateImpl::Deallocate()
{
	batch_data_.clear();
	ObjectPool<DBOperateImpl>::GetInstance().Deallocate(this);
}
void DBOperateImpl::DeallocateRecord()
{
	if (Operate == DBOperateType::Insert || Operate == DBOperateType::BatchInsert || Operate == DBOperateType::Truncate)
	{
		return;
	}
	if (schema_registry_)
	{
		const TableSchema* schema = schema_registry_->GetSchema(TableID);
		if (schema && schema->DeallocateRecord && Record)
		{
			schema->DeallocateRecord(Record);
		}
	}
	Record = nullptr;
}
