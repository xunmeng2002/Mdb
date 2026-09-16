// 本文件由 ../Templates/Cpp/Mdb/InitMdbFromDb.cpp.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#include "InitMdbFromDb.h"
#include <DbAdapters/DbInterface/TypedTable.h>
#include <vector>

using namespace DbAdapters;

namespace Mdb
{

	void InitMdbFromDb::LoadTables(Mdb* mdb, Db* db, const TableList& tableList)
	{
		for (int i = 0; i < tableList.Count; ++i)
		{
			switch (tableList.TableIds[i])
			{
			case TradingDay::TableId:  LoadTradingDayTable(mdb, db); break;
			case Exchange::TableId:  LoadExchangeTable(mdb, db); break;
			case Product::TableId:  LoadProductTable(mdb, db); break;
			case Instrument::TableId:  LoadInstrumentTable(mdb, db); break;
			case PrimaryAccount::TableId:  LoadPrimaryAccountTable(mdb, db); break;
			case Account::TableId:  LoadAccountTable(mdb, db); break;
			case Capital::TableId:  LoadCapitalTable(mdb, db); break;
			case Position::TableId:  LoadPositionTable(mdb, db); break;
			case PositionDetail::TableId:  LoadPositionDetailTable(mdb, db); break;
			case Order::TableId:  LoadOrderTable(mdb, db); break;
			case Trade::TableId:  LoadTradeTable(mdb, db); break;
			default: break;
			}
		}
	}

	void InitMdbFromDb::LoadTradingDayTable(Mdb* mdb, Db* db)
	{
		TypedTable<TradingDay> table(db);
		std::vector<TradingDay*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->TradingDay->Insert(record);
		}
	}
	void InitMdbFromDb::LoadExchangeTable(Mdb* mdb, Db* db)
	{
		TypedTable<Exchange> table(db);
		std::vector<Exchange*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->Exchange->Insert(record);
		}
	}
	void InitMdbFromDb::LoadProductTable(Mdb* mdb, Db* db)
	{
		TypedTable<Product> table(db);
		std::vector<Product*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->Product->Insert(record);
		}
	}
	void InitMdbFromDb::LoadInstrumentTable(Mdb* mdb, Db* db)
	{
		TypedTable<Instrument> table(db);
		std::vector<Instrument*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->Instrument->Insert(record);
		}
	}
	void InitMdbFromDb::LoadPrimaryAccountTable(Mdb* mdb, Db* db)
	{
		TypedTable<PrimaryAccount> table(db);
		std::vector<PrimaryAccount*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->PrimaryAccount->Insert(record);
		}
	}
	void InitMdbFromDb::LoadAccountTable(Mdb* mdb, Db* db)
	{
		TypedTable<Account> table(db);
		std::vector<Account*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->Account->Insert(record);
		}
	}
	void InitMdbFromDb::LoadCapitalTable(Mdb* mdb, Db* db)
	{
		TypedTable<Capital> table(db);
		std::vector<Capital*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->Capital->Insert(record);
		}
	}
	void InitMdbFromDb::LoadPositionTable(Mdb* mdb, Db* db)
	{
		TypedTable<Position> table(db);
		std::vector<Position*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->Position->Insert(record);
		}
	}
	void InitMdbFromDb::LoadPositionDetailTable(Mdb* mdb, Db* db)
	{
		TypedTable<PositionDetail> table(db);
		std::vector<PositionDetail*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->PositionDetail->Insert(record);
		}
	}
	void InitMdbFromDb::LoadOrderTable(Mdb* mdb, Db* db)
	{
		TypedTable<Order> table(db);
		std::vector<Order*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->Order->Insert(record);
		}
	}
	void InitMdbFromDb::LoadTradeTable(Mdb* mdb, Db* db)
	{
		TypedTable<Trade> table(db);
		std::vector<Trade*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->Trade->Insert(record);
		}
	}
}
