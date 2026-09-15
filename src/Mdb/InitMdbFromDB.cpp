// 本文件由 ../Templates/Cpp/Mdb/InitMdbFromDB.cpp.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#include "InitMdbFromDB.h"
#include <DBAdapters/DBInterface/TypedTable.h>
#include <vector>

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
		std::vector<TradingDay*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->tradingDay->Insert(record);
		}
	}
	void InitMdbFromDB::LoadExchangeTable(Mdb* mdb, DB* db)
	{
		TypedTable<Exchange> table(db);
		std::vector<Exchange*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->exchange->Insert(record);
		}
	}
	void InitMdbFromDB::LoadProductTable(Mdb* mdb, DB* db)
	{
		TypedTable<Product> table(db);
		std::vector<Product*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->product->Insert(record);
		}
	}
	void InitMdbFromDB::LoadInstrumentTable(Mdb* mdb, DB* db)
	{
		TypedTable<Instrument> table(db);
		std::vector<Instrument*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->instrument->Insert(record);
		}
	}
	void InitMdbFromDB::LoadPrimaryAccountTable(Mdb* mdb, DB* db)
	{
		TypedTable<PrimaryAccount> table(db);
		std::vector<PrimaryAccount*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->primaryAccount->Insert(record);
		}
	}
	void InitMdbFromDB::LoadAccountTable(Mdb* mdb, DB* db)
	{
		TypedTable<Account> table(db);
		std::vector<Account*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->account->Insert(record);
		}
	}
	void InitMdbFromDB::LoadCapitalTable(Mdb* mdb, DB* db)
	{
		TypedTable<Capital> table(db);
		std::vector<Capital*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->capital->Insert(record);
		}
	}
	void InitMdbFromDB::LoadPositionTable(Mdb* mdb, DB* db)
	{
		TypedTable<Position> table(db);
		std::vector<Position*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->position->Insert(record);
		}
	}
	void InitMdbFromDB::LoadPositionDetailTable(Mdb* mdb, DB* db)
	{
		TypedTable<PositionDetail> table(db);
		std::vector<PositionDetail*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->positionDetail->Insert(record);
		}
	}
	void InitMdbFromDB::LoadOrderTable(Mdb* mdb, DB* db)
	{
		TypedTable<Order> table(db);
		std::vector<Order*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->order->Insert(record);
		}
	}
	void InitMdbFromDB::LoadTradeTable(Mdb* mdb, DB* db)
	{
		TypedTable<Trade> table(db);
		std::vector<Trade*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->trade->Insert(record);
		}
	}
}
