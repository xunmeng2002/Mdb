#include "MdbTableRegistry.h"
#include "MdbStructs.h"


namespace mdb
{
	MdbTableRegistry::MdbTableRegistry(const TableList& tableList)
	{
		for (int i = 0; i < tableList.Count; ++i)
		{
			unsigned int tableID = tableList.TableIDs[i];
			m_TableIDs.push_back(tableID);
			m_Schemas.push_back(GetSchemaByID(tableID));
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
		for (size_t i = 0; i < m_TableIDs.size(); ++i)
		{
			if (m_TableIDs[i] == tableID)
			{
				return m_Schemas[i];
			}
		}
		return nullptr;
	}
	const TableSchema* const* MdbTableRegistry::GetAllSchemas() const
	{
		return m_Schemas.data();
	}
	int MdbTableRegistry::GetTableCount() const
	{
		return (int)m_TableIDs.size();
	}
}
