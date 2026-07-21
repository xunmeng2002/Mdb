#include "DBOperateImpl.h"
#include <Mdb/Mdb/MdbTableRegistry.h>
#include <Mdb/Mdb/MdbStructs.h>
#include <PersonalLib/TemplateLib/TemplateLib.h>
#include <cstring>

using namespace mdb;

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
	const TableSchema* schema = MdbTableRegistry::Instance().GetSchema(TableID);
	if (schema && schema->DeallocateRecord && Record)
	{
		schema->DeallocateRecord(Record);
	}
	Record = nullptr;
}
