// 本文件由 ../Templates/Cpp/Mdb/Mdb.cpp.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#include "Mdb.h"


namespace mdb
{
	Mdb::Mdb(const TableList& tableList): mdbSubscriber(nullptr)
	{
		for (int i = 0; i < tableList.Count; ++i)
		{
			switch (tableList.TableIDs[i])
			{
			case TradingDay::TableID:  tradingDay = new TradingDayTable(); tables.push_back(tradingDay); break;
			case Exchange::TableID:  exchange = new ExchangeTable(); tables.push_back(exchange); break;
			case Product::TableID:  product = new ProductTable(); tables.push_back(product); break;
			case Instrument::TableID:  instrument = new InstrumentTable(); tables.push_back(instrument); break;
			case PrimaryAccount::TableID:  primaryAccount = new PrimaryAccountTable(); tables.push_back(primaryAccount); break;
			case Account::TableID:  account = new AccountTable(); tables.push_back(account); break;
			case Capital::TableID:  capital = new CapitalTable(); tables.push_back(capital); break;
			case Position::TableID:  position = new PositionTable(); tables.push_back(position); break;
			case PositionDetail::TableID:  positionDetail = new PositionDetailTable(); tables.push_back(positionDetail); break;
			case Order::TableID:  order = new OrderTable(); tables.push_back(order); break;
			case Trade::TableID:  trade = new TradeTable(); tables.push_back(trade); break;
			default: break;
			}
		}
	}
	void Mdb::Subscribe(MdbSubscriber* subscriber)
	{
		mdbSubscriber = subscriber;
		for (auto table : tables)
		{
			table->Subscribe(subscriber);
		}
	}
	void Mdb::UnSubscribe()
	{
		for (auto table : tables)
		{
			table->UnSubscribe();
		}
	}
	void Mdb::InitDB()
	{
		CreateTables();
		for (auto table : tables)
		{
			table->InitDB();
		}
	}
	void Mdb::SetInitStatus(bool initStatus)
	{
		for (auto table : tables)
		{
			table->dbInited = initStatus;
		}
	}
	void Mdb::Dump(const char* dir)
	{
		for (auto table : tables)
		{
			table->Dump(dir);
		}
	}
	void Mdb::CreateTables()
	{
		if (mdbSubscriber != nullptr)
		{
			mdbSubscriber->OnTableOp(DBOperateType::CreateTables);
		}
	}
	void Mdb::DropTables()
	{
		if (mdbSubscriber != nullptr)
		{
			mdbSubscriber->OnTableOp(DBOperateType::DropTables);
		}
	}
	void Mdb::TruncateTables()
	{
		if (mdbSubscriber != nullptr)
		{
			mdbSubscriber->OnTableOp(DBOperateType::TruncateTables);
		}
		for (auto table : tables)
		{
			table->TruncateTables();
		}
	}

	void Mdb::OnDBConnected()
	{
		InitDB();
	}
	void Mdb::OnDBDisConnected()
	{
		for (auto table : tables)
		{
			table->dbInited = false;
		}
	}
}
