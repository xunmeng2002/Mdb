#include "DBOperateImpl.h"
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
	switch(TableID)
	{
	case TradingDay::TableID:
	{
		((TradingDay*)Record)->Deallocate();
		break;
	}
	case Exchange::TableID:
	{
		((Exchange*)Record)->Deallocate();
		break;
	}
	case Product::TableID:
	{
		((Product*)Record)->Deallocate();
		break;
	}
	case Instrument::TableID:
	{
		((Instrument*)Record)->Deallocate();
		break;
	}
	case PrimaryAccount::TableID:
	{
		((PrimaryAccount*)Record)->Deallocate();
		break;
	}
	case Account::TableID:
	{
		((Account*)Record)->Deallocate();
		break;
	}
	case Capital::TableID:
	{
		((Capital*)Record)->Deallocate();
		break;
	}
	case Position::TableID:
	{
		((Position*)Record)->Deallocate();
		break;
	}
	case PositionDetail::TableID:
	{
		((PositionDetail*)Record)->Deallocate();
		break;
	}
	case Order::TableID:
	{
		((Order*)Record)->Deallocate();
		break;
	}
	case Trade::TableID:
	{
		((Trade*)Record)->Deallocate();
		break;
	}
	default:
		break;
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
