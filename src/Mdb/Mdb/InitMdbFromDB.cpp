#include <Mdb/Mdb/InitMdbFromDB.h>
#include <list>

using namespace std;

namespace mdb
{
	static void* AllocTradingDay()        { return TradingDay::Allocate(); }
	static void  PushTradingDay(void* records, void* record)
		{ ((list<TradingDay*>*)records)->push_back((TradingDay*)record); }
	static void* AllocExchange()          { return Exchange::Allocate(); }
	static void  PushExchange(void* records, void* record)
		{ ((list<Exchange*>*)records)->push_back((Exchange*)record); }
	static void* AllocProduct()           { return Product::Allocate(); }
	static void  PushProduct(void* records, void* record)
		{ ((list<Product*>*)records)->push_back((Product*)record); }
	static void* AllocInstrument()        { return Instrument::Allocate(); }
	static void  PushInstrument(void* records, void* record)
		{ ((list<Instrument*>*)records)->push_back((Instrument*)record); }
	static void* AllocPrimaryAccount()    { return PrimaryAccount::Allocate(); }
	static void  PushPrimaryAccount(void* records, void* record)
		{ ((list<PrimaryAccount*>*)records)->push_back((PrimaryAccount*)record); }
	static void* AllocAccount()           { return Account::Allocate(); }
	static void  PushAccount(void* records, void* record)
		{ ((list<Account*>*)records)->push_back((Account*)record); }
	static void* AllocCapital()           { return Capital::Allocate(); }
	static void  PushCapital(void* records, void* record)
		{ ((list<Capital*>*)records)->push_back((Capital*)record); }
	static void* AllocPosition()          { return Position::Allocate(); }
	static void  PushPosition(void* records, void* record)
		{ ((list<Position*>*)records)->push_back((Position*)record); }
	static void* AllocPositionDetail()    { return PositionDetail::Allocate(); }
	static void  PushPositionDetail(void* records, void* record)
		{ ((list<PositionDetail*>*)records)->push_back((PositionDetail*)record); }
	static void* AllocOrder()             { return Order::Allocate(); }
	static void  PushOrder(void* records, void* record)
		{ ((list<Order*>*)records)->push_back((Order*)record); }
	static void* AllocTrade()             { return Trade::Allocate(); }
	static void  PushTrade(void* records, void* record)
		{ ((list<Trade*>*)records)->push_back((Trade*)record); }

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
		std::list<TradingDay*> records;
		RecordFactory factory = { AllocTradingDay, PushTradingDay };
		db->SelectAll(&TradingDay::GetSchema(), &records, factory);
		for (auto record : records)
		{
			mdb->t_TradingDay->Insert(record);
		}
	}
	void InitMdbFromDB::LoadExchangeTable(Mdb* mdb, DB* db)
	{
		std::list<Exchange*> records;
		RecordFactory factory = { AllocExchange, PushExchange };
		db->SelectAll(&Exchange::GetSchema(), &records, factory);
		for (auto record : records)
		{
			mdb->t_Exchange->Insert(record);
		}
	}
	void InitMdbFromDB::LoadProductTable(Mdb* mdb, DB* db)
	{
		std::list<Product*> records;
		RecordFactory factory = { AllocProduct, PushProduct };
		db->SelectAll(&Product::GetSchema(), &records, factory);
		for (auto record : records)
		{
			mdb->t_Product->Insert(record);
		}
	}
	void InitMdbFromDB::LoadInstrumentTable(Mdb* mdb, DB* db)
	{
		std::list<Instrument*> records;
		RecordFactory factory = { AllocInstrument, PushInstrument };
		db->SelectAll(&Instrument::GetSchema(), &records, factory);
		for (auto record : records)
		{
			mdb->t_Instrument->Insert(record);
		}
	}
	void InitMdbFromDB::LoadPrimaryAccountTable(Mdb* mdb, DB* db)
	{
		std::list<PrimaryAccount*> records;
		RecordFactory factory = { AllocPrimaryAccount, PushPrimaryAccount };
		db->SelectAll(&PrimaryAccount::GetSchema(), &records, factory);
		for (auto record : records)
		{
			mdb->t_PrimaryAccount->Insert(record);
		}
	}
	void InitMdbFromDB::LoadAccountTable(Mdb* mdb, DB* db)
	{
		std::list<Account*> records;
		RecordFactory factory = { AllocAccount, PushAccount };
		db->SelectAll(&Account::GetSchema(), &records, factory);
		for (auto record : records)
		{
			mdb->t_Account->Insert(record);
		}
	}
	void InitMdbFromDB::LoadCapitalTable(Mdb* mdb, DB* db)
	{
		std::list<Capital*> records;
		RecordFactory factory = { AllocCapital, PushCapital };
		db->SelectAll(&Capital::GetSchema(), &records, factory);
		for (auto record : records)
		{
			mdb->t_Capital->Insert(record);
		}
	}
	void InitMdbFromDB::LoadPositionTable(Mdb* mdb, DB* db)
	{
		std::list<Position*> records;
		RecordFactory factory = { AllocPosition, PushPosition };
		db->SelectAll(&Position::GetSchema(), &records, factory);
		for (auto record : records)
		{
			mdb->t_Position->Insert(record);
		}
	}
	void InitMdbFromDB::LoadPositionDetailTable(Mdb* mdb, DB* db)
	{
		std::list<PositionDetail*> records;
		RecordFactory factory = { AllocPositionDetail, PushPositionDetail };
		db->SelectAll(&PositionDetail::GetSchema(), &records, factory);
		for (auto record : records)
		{
			mdb->t_PositionDetail->Insert(record);
		}
	}
	void InitMdbFromDB::LoadOrderTable(Mdb* mdb, DB* db)
	{
		std::list<Order*> records;
		RecordFactory factory = { AllocOrder, PushOrder };
		db->SelectAll(&Order::GetSchema(), &records, factory);
		for (auto record : records)
		{
			mdb->t_Order->Insert(record);
		}
	}
	void InitMdbFromDB::LoadTradeTable(Mdb* mdb, DB* db)
	{
		std::list<Trade*> records;
		RecordFactory factory = { AllocTrade, PushTrade };
		db->SelectAll(&Trade::GetSchema(), &records, factory);
		for (auto record : records)
		{
			mdb->t_Trade->Insert(record);
		}
	}
}
