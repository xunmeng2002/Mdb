#include "DBOperateImpl.h"
#include "MdbTableRegistry.h"
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
	ObjectPool<DBOperateImpl>::GetInstance().Deallocate(this);
}
void DBOperateImpl::DeallocateRecord()
{
	if (Operate == DBOperateType::Insert || Operate == DBOperateType::BatchInsert || Operate == DBOperateType::Truncate)
	{
		return;
	}
	const TableSchema* schema = mdb::GetSchemaByTableID(TableID);
	if (schema && schema->DeallocateRecord && Record)
	{
		schema->DeallocateRecord(Record);
	}
	Record = nullptr;
}
const char* DBOperateImpl::GetDebugString() const
{
	switch (TableID)
	{
	case TradingDay::TableID:
	{
		return ((TradingDay*)Record)->GetDebugString();
	}
	case Exchange::TableID:
	{
		return ((Exchange*)Record)->GetDebugString();
	}
	case Product::TableID:
	{
		return ((Product*)Record)->GetDebugString();
	}
	case Instrument::TableID:
	{
		return ((Instrument*)Record)->GetDebugString();
	}
	case PrimaryAccount::TableID:
	{
		return ((PrimaryAccount*)Record)->GetDebugString();
	}
	case Account::TableID:
	{
		return ((Account*)Record)->GetDebugString();
	}
	case Capital::TableID:
	{
		return ((Capital*)Record)->GetDebugString();
	}
	case Position::TableID:
	{
		return ((Position*)Record)->GetDebugString();
	}
	case PositionDetail::TableID:
	{
		return ((PositionDetail*)Record)->GetDebugString();
	}
	case Order::TableID:
	{
		return ((Order*)Record)->GetDebugString();
	}
	case Trade::TableID:
	{
		return ((Trade*)Record)->GetDebugString();
	}
	default:
		break;
	}
	return "";
}
