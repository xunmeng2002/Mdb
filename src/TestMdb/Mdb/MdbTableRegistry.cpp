#include "MdbTableRegistry.h"
#include "MdbStructs.h"


namespace mdb
{
	MdbTableRegistry& MdbTableRegistry::Instance()
	{
		static MdbTableRegistry instance;
		return instance;
	}
	const TableSchema* MdbTableRegistry::GetSchema(unsigned int tableID) const
	{
		switch (tableID)
		{
		case TradingDay::TableID:       return &TradingDay::GetSchema();
		case Exchange::TableID:       return &Exchange::GetSchema();
		case Product::TableID:       return &Product::GetSchema();
		case Instrument::TableID:       return &Instrument::GetSchema();
		case PrimaryAccount::TableID:       return &PrimaryAccount::GetSchema();
		case Account::TableID:       return &Account::GetSchema();
		case Capital::TableID:       return &Capital::GetSchema();
		case Position::TableID:       return &Position::GetSchema();
		case PositionDetail::TableID:       return &PositionDetail::GetSchema();
		case Order::TableID:       return &Order::GetSchema();
		case Trade::TableID:       return &Trade::GetSchema();
		default:                        return nullptr;
		}
	}
	const TableSchema* const* MdbTableRegistry::GetAllSchemas() const
	{
		static const TableSchema* const kAllSchemas[] =
		{
			&TradingDay::GetSchema(),
			&Exchange::GetSchema(),
			&Product::GetSchema(),
			&Instrument::GetSchema(),
			&PrimaryAccount::GetSchema(),
			&Account::GetSchema(),
			&Capital::GetSchema(),
			&Position::GetSchema(),
			&PositionDetail::GetSchema(),
			&Order::GetSchema(),
			&Trade::GetSchema(),
		};
		return kAllSchemas;
	}
	int MdbTableRegistry::GetTableCount() const
	{
		return 11;
	}
}
