// 本文件由 ../Templates/Cpp/Mdb/MdbTableRegistry.cpp.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#include "MdbTableRegistry.h"
#include "MdbStructs.h"


namespace Mdb
{
	MdbTableRegistry::MdbTableRegistry(const TableList& tableList)
	{
		for (int i = 0; i < tableList.Count; ++i)
		{
			unsigned int tableId = tableList.TableIds[i];
			tableIds_.push_back(tableId);
			schemas_.push_back(GetSchemaByID(tableId));
		}
	}
	const TableSchema* MdbTableRegistry::GetSchemaByID(unsigned int tableId)
	{
		switch (tableId)
		{
		case TradingDay::TableId: return &TradingDay::GetSchema();
		case Exchange::TableId: return &Exchange::GetSchema();
		case Product::TableId: return &Product::GetSchema();
		case Instrument::TableId: return &Instrument::GetSchema();
		case PrimaryAccount::TableId: return &PrimaryAccount::GetSchema();
		case Account::TableId: return &Account::GetSchema();
		case Capital::TableId: return &Capital::GetSchema();
		case Position::TableId: return &Position::GetSchema();
		case PositionDetail::TableId: return &PositionDetail::GetSchema();
		case Order::TableId: return &Order::GetSchema();
		case Trade::TableId: return &Trade::GetSchema();
		default: return nullptr;
		}
	}
	const TableSchema* MdbTableRegistry::GetSchema(unsigned int tableId) const
	{
		for (size_t i = 0; i < tableIds_.size(); ++i)
		{
			if (tableIds_[i] == tableId)
			{
				return schemas_[i];
			}
		}
		return nullptr;
	}
	const TableSchema* const* MdbTableRegistry::GetAllSchemas() const
	{
		return schemas_.data();
	}
	int MdbTableRegistry::GetTableCount() const
	{
		return static_cast<int>(tableIds_.size());
	}
}
