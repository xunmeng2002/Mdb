#include "InitMdbFromDB.h"
#include <list>

using namespace std;

namespace mdb
{
	void InitMdbFromDB::LoadTablesWithTradingDay(Mdb* mdb, DB* db, const DateType& tradingDay)
	{
	}
	void InitMdbFromDB::LoadTablesWithoutTradingDay(Mdb* mdb, DB* db)
	{
		LoadTradingDayTable(mdb, db);
		LoadExchangeTable(mdb, db);
		LoadProductTable(mdb, db);
		LoadInstrumentTable(mdb, db);
		LoadPrimaryAccountTable(mdb, db);
		LoadAccountTable(mdb, db);
		LoadCapitalTable(mdb, db);
		LoadPositionTable(mdb, db);
		LoadPositionDetailTable(mdb, db);
		LoadOrderTable(mdb, db);
		LoadTradeTable(mdb, db);
	}

	void InitMdbFromDB::LoadTradingDayTable(Mdb* mdb, DB* db)
	{
		list<TradingDay*> records;
		db->SelectTradingDay(records);
		for (auto record : records)
		{
			mdb->t_TradingDay->Insert(record);
		}
	}
	void InitMdbFromDB::LoadExchangeTable(Mdb* mdb, DB* db)
	{
		list<Exchange*> records;
		db->SelectExchange(records);
		for (auto record : records)
		{
			mdb->t_Exchange->Insert(record);
		}
	}
	void InitMdbFromDB::LoadProductTable(Mdb* mdb, DB* db)
	{
		list<Product*> records;
		db->SelectProduct(records);
		for (auto record : records)
		{
			mdb->t_Product->Insert(record);
		}
	}
	void InitMdbFromDB::LoadInstrumentTable(Mdb* mdb, DB* db)
	{
		list<Instrument*> records;
		db->SelectInstrument(records);
		for (auto record : records)
		{
			mdb->t_Instrument->Insert(record);
		}
	}
	void InitMdbFromDB::LoadPrimaryAccountTable(Mdb* mdb, DB* db)
	{
		list<PrimaryAccount*> records;
		db->SelectPrimaryAccount(records);
		for (auto record : records)
		{
			mdb->t_PrimaryAccount->Insert(record);
		}
	}
	void InitMdbFromDB::LoadAccountTable(Mdb* mdb, DB* db)
	{
		list<Account*> records;
		db->SelectAccount(records);
		for (auto record : records)
		{
			mdb->t_Account->Insert(record);
		}
	}
	void InitMdbFromDB::LoadCapitalTable(Mdb* mdb, DB* db)
	{
		list<Capital*> records;
		db->SelectCapital(records);
		for (auto record : records)
		{
			mdb->t_Capital->Insert(record);
		}
	}
	void InitMdbFromDB::LoadPositionTable(Mdb* mdb, DB* db)
	{
		list<Position*> records;
		db->SelectPosition(records);
		for (auto record : records)
		{
			mdb->t_Position->Insert(record);
		}
	}
	void InitMdbFromDB::LoadPositionDetailTable(Mdb* mdb, DB* db)
	{
		list<PositionDetail*> records;
		db->SelectPositionDetail(records);
		for (auto record : records)
		{
			mdb->t_PositionDetail->Insert(record);
		}
	}
	void InitMdbFromDB::LoadOrderTable(Mdb* mdb, DB* db)
	{
		list<Order*> records;
		db->SelectOrder(records);
		for (auto record : records)
		{
			mdb->t_Order->Insert(record);
		}
	}
	void InitMdbFromDB::LoadTradeTable(Mdb* mdb, DB* db)
	{
		list<Trade*> records;
		db->SelectTrade(records);
		for (auto record : records)
		{
			mdb->t_Trade->Insert(record);
		}
	}
}
