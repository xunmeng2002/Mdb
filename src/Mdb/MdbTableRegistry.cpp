// 本文件由 ../Templates/Cpp/Mdb/MdbTableRegistry.cpp.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#include "MdbTableRegistry.h"
#include "MdbStructs.h"


namespace mdb
{
	MdbTableRegistry::MdbTableRegistry(const TableList& tableList)
	{
		for (int i = 0; i < tableList.Count; ++i)
		{
			unsigned int tableID = tableList.TableIDs[i];
			tableIDs.push_back(tableID);
			schemas.push_back(GetSchemaByID(tableID));
		}
	}
	const TableSchema* MdbTableRegistry::GetSchemaByID(unsigned int tableID)
	{
		switch (tableID)
		{
		case TradingDay::TableID: return &TradingDay::GetSchema();
		case Exchange::TableID: return &Exchange::GetSchema();
		case Product::TableID: return &Product::GetSchema();
		case Instrument::TableID: return &Instrument::GetSchema();
		case PrimaryAccount::TableID: return &PrimaryAccount::GetSchema();
		case Account::TableID: return &Account::GetSchema();
		case Capital::TableID: return &Capital::GetSchema();
		case Position::TableID: return &Position::GetSchema();
		case PositionDetail::TableID: return &PositionDetail::GetSchema();
		case Order::TableID: return &Order::GetSchema();
		case Trade::TableID: return &Trade::GetSchema();
		default: return nullptr;
		}
	}
	const TableSchema* MdbTableRegistry::GetSchema(unsigned int tableID) const
	{
		for (size_t i = 0; i < tableIDs.size(); ++i)
		{
			if (tableIDs[i] == tableID)
			{
				return schemas[i];
			}
		}
		return nullptr;
	}
	const TableSchema* const* MdbTableRegistry::GetAllSchemas() const
	{
		return schemas.data();
	}
	int MdbTableRegistry::GetTableCount() const
	{
		return static_cast<int>(tableIDs.size());
	}
}
