#include "DBOperateImpl.h"
#include "MemCacheTemplateSingleton.h"
#include "MdbStructs.h"
#include <cstring>

using namespace mdb;

DBOperate* DBOperate::Allocate()
{
	return ::Allocate<DBOperateImpl>();
}
void DBOperateImpl::Free()
{
	::Free<DBOperateImpl>(this);
}
void DBOperateImpl::FreeRecord()
{
	if (Operate == DBOperateType::Insert || Operate == DBOperateType::BatchInsert || Operate == DBOperateType::Truncate)
	{
		return;
	}
	switch(TableID)
	{
	case TradingDay::TableID:
	{
		((TradingDay*)Record)->Free();
		break;
	}
	case Exchange::TableID:
	{
		((Exchange*)Record)->Free();
		break;
	}
	case Product::TableID:
	{
		((Product*)Record)->Free();
		break;
	}
	case Instrument::TableID:
	{
		((Instrument*)Record)->Free();
		break;
	}
	case PrimaryAccount::TableID:
	{
		((PrimaryAccount*)Record)->Free();
		break;
	}
	case Account::TableID:
	{
		((Account*)Record)->Free();
		break;
	}
	case Capital::TableID:
	{
		((Capital*)Record)->Free();
		break;
	}
	case Position::TableID:
	{
		((Position*)Record)->Free();
		break;
	}
	case PositionDetail::TableID:
	{
		((PositionDetail*)Record)->Free();
		break;
	}
	case Order::TableID:
	{
		((Order*)Record)->Free();
		break;
	}
	case Trade::TableID:
	{
		((Trade*)Record)->Free();
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
