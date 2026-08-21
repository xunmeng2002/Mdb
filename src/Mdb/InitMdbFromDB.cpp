#include "InitMdbFromDB.h"
#include <DBAdapters/DBInterface/TypedTable.h>
#include <vector>

using namespace std;
using namespace dbadapters;

namespace mdb
{

	void InitMdbFromDB::LoadTables(Mdb* mdb, DB* db, const TableList& tableList)
	{
		for (int i = 0; i < tableList.Count; ++i)
		{
			switch (tableList.TableIDs[i])
			{
			case TradingDay::TableID:  LoadTradingDayTable(mdb, db); break;
			case Exchange::TableID:  LoadExchangeTable(mdb, db); break;
			case Product::TableID:  LoadProductTable(mdb, db); break;
			case Instrument::TableID:  LoadInstrumentTable(mdb, db); break;
			case PrimaryAccount::TableID:  LoadPrimaryAccountTable(mdb, db); break;
			case Account::TableID:  LoadAccountTable(mdb, db); break;
			case Capital::TableID:  LoadCapitalTable(mdb, db); break;
			case Position::TableID:  LoadPositionTable(mdb, db); break;
			case PositionDetail::TableID:  LoadPositionDetailTable(mdb, db); break;
			case Order::TableID:  LoadOrderTable(mdb, db); break;
			case Trade::TableID:  LoadTradeTable(mdb, db); break;
			default: break;
			}
		}
	}

	void InitMdbFromDB::LoadTradingDayTable(Mdb* mdb, DB* db)
	{
		TypedTable<TradingDay> table(db);
		vector<TradingDay*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->t_TradingDay->Insert(record);
		}
	}
	void InitMdbFromDB::LoadExchangeTable(Mdb* mdb, DB* db)
	{
		TypedTable<Exchange> table(db);
		vector<Exchange*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->t_Exchange->Insert(record);
		}
	}
	void InitMdbFromDB::LoadProductTable(Mdb* mdb, DB* db)
	{
		TypedTable<Product> table(db);
		vector<Product*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->t_Product->Insert(record);
		}
	}
	void InitMdbFromDB::LoadInstrumentTable(Mdb* mdb, DB* db)
	{
		TypedTable<Instrument> table(db);
		vector<Instrument*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->t_Instrument->Insert(record);
		}
	}
	void InitMdbFromDB::LoadPrimaryAccountTable(Mdb* mdb, DB* db)
	{
		TypedTable<PrimaryAccount> table(db);
		vector<PrimaryAccount*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->t_PrimaryAccount->Insert(record);
		}
	}
	void InitMdbFromDB::LoadAccountTable(Mdb* mdb, DB* db)
	{
		TypedTable<Account> table(db);
		vector<Account*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->t_Account->Insert(record);
		}
	}
	void InitMdbFromDB::LoadCapitalTable(Mdb* mdb, DB* db)
	{
		TypedTable<Capital> table(db);
		vector<Capital*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->t_Capital->Insert(record);
		}
	}
	void InitMdbFromDB::LoadPositionTable(Mdb* mdb, DB* db)
	{
		TypedTable<Position> table(db);
		vector<Position*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->t_Position->Insert(record);
		}
	}
	void InitMdbFromDB::LoadPositionDetailTable(Mdb* mdb, DB* db)
	{
		TypedTable<PositionDetail> table(db);
		vector<PositionDetail*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->t_PositionDetail->Insert(record);
		}
	}
	void InitMdbFromDB::LoadOrderTable(Mdb* mdb, DB* db)
	{
		TypedTable<Order> table(db);
		vector<Order*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->t_Order->Insert(record);
		}
	}
	void InitMdbFromDB::LoadTradeTable(Mdb* mdb, DB* db)
	{
		TypedTable<Trade> table(db);
		vector<Trade*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->t_Trade->Insert(record);
		}
	}
}
