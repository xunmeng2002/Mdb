// 本文件由 ../Templates/Cpp/Mdb/Rcu/StableRow/InitMdbFromDb.cpp.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#include "InitMdbFromDb.h"
#include <DbAdapters/DbInterface/TypedTable.h>
#include <vector>

using namespace DbAdapters;

namespace Mdb::Rcu::StableRow
{

	void InitMdbFromDb::LoadTables(Mdb* mdb, Db* db, const TableList& tableList)
	{
		for (int i = 0; i < tableList.Count; ++i)
		{
			switch (tableList.TableIds[i])
			{
			case TradingDay::TableId:  LoadTradingDay(mdb, db); break;
			case Exchange::TableId:  LoadExchange(mdb, db); break;
			case Product::TableId:  LoadProduct(mdb, db); break;
			case Instrument::TableId:  LoadInstrument(mdb, db); break;
			case PrimaryAccount::TableId:  LoadPrimaryAccount(mdb, db); break;
			case Account::TableId:  LoadAccount(mdb, db); break;
			case Capital::TableId:  LoadCapital(mdb, db); break;
			case Position::TableId:  LoadPosition(mdb, db); break;
			case PositionDetail::TableId:  LoadPositionDetail(mdb, db); break;
			case Order::TableId:  LoadOrder(mdb, db); break;
			case Trade::TableId:  LoadTrade(mdb, db); break;
			default: break;
			}
		}
	}

	void InitMdbFromDb::LoadTradingDay(Mdb* mdb, Db* db)
	{
		TypedTable<TradingDay> table(db);
		std::vector<TradingDay*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->TradingDay->Insert(record);
		}
	}
	void InitMdbFromDb::LoadExchange(Mdb* mdb, Db* db)
	{
		TypedTable<Exchange> table(db);
		std::vector<Exchange*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->Exchange->Insert(record);
		}
	}
	void InitMdbFromDb::LoadProduct(Mdb* mdb, Db* db)
	{
		TypedTable<Product> table(db);
		std::vector<Product*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->Product->Insert(record);
		}
	}
	void InitMdbFromDb::LoadInstrument(Mdb* mdb, Db* db)
	{
		TypedTable<Instrument> table(db);
		std::vector<Instrument*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->Instrument->Insert(record);
		}
	}
	void InitMdbFromDb::LoadPrimaryAccount(Mdb* mdb, Db* db)
	{
		TypedTable<PrimaryAccount> table(db);
		std::vector<PrimaryAccount*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->PrimaryAccount->Insert(record);
		}
	}
	void InitMdbFromDb::LoadAccount(Mdb* mdb, Db* db)
	{
		TypedTable<Account> table(db);
		std::vector<Account*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->Account->Insert(record);
		}
	}
	void InitMdbFromDb::LoadCapital(Mdb* mdb, Db* db)
	{
		TypedTable<Capital> table(db);
		std::vector<Capital*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->Capital->Insert(record);
		}
	}
	void InitMdbFromDb::LoadPosition(Mdb* mdb, Db* db)
	{
		TypedTable<Position> table(db);
		std::vector<Position*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->Position->Insert(record);
		}
	}
	void InitMdbFromDb::LoadPositionDetail(Mdb* mdb, Db* db)
	{
		TypedTable<PositionDetail> table(db);
		std::vector<PositionDetail*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->PositionDetail->Insert(record);
		}
	}
	void InitMdbFromDb::LoadOrder(Mdb* mdb, Db* db)
	{
		TypedTable<Order> table(db);
		std::vector<Order*> records;
		table.SelectAll(records);
		for (auto record : records)
		{
			mdb->Order->Insert(record);
		}
	}
	void InitMdbFromDb::LoadTrade(Mdb* mdb, Db* db)
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
