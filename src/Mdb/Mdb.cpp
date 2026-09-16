// 本文件由 ../Templates/Cpp/Mdb/Mdb.cpp.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#include "Mdb.h"


namespace Mdb
{
	Mdb::Mdb(const TableList& tableList): mdbSubscriber_(nullptr)
	{
		for (int i = 0; i < tableList.Count; ++i)
		{
			switch (tableList.TableIds[i])
			{
			case TradingDay::TableId:  TradingDay = new TradingDayTable(); tables_.push_back(TradingDay); break;
			case Exchange::TableId:  Exchange = new ExchangeTable(); tables_.push_back(Exchange); break;
			case Product::TableId:  Product = new ProductTable(); tables_.push_back(Product); break;
			case Instrument::TableId:  Instrument = new InstrumentTable(); tables_.push_back(Instrument); break;
			case PrimaryAccount::TableId:  PrimaryAccount = new PrimaryAccountTable(); tables_.push_back(PrimaryAccount); break;
			case Account::TableId:  Account = new AccountTable(); tables_.push_back(Account); break;
			case Capital::TableId:  Capital = new CapitalTable(); tables_.push_back(Capital); break;
			case Position::TableId:  Position = new PositionTable(); tables_.push_back(Position); break;
			case PositionDetail::TableId:  PositionDetail = new PositionDetailTable(); tables_.push_back(PositionDetail); break;
			case Order::TableId:  Order = new OrderTable(); tables_.push_back(Order); break;
			case Trade::TableId:  Trade = new TradeTable(); tables_.push_back(Trade); break;
			default: break;
			}
		}
	}
	void Mdb::Subscribe(MdbSubscriber* subscriber)
	{
		mdbSubscriber_ = subscriber;
		for (auto table : tables_)
		{
			table->Subscribe(subscriber);
		}
	}
	void Mdb::UnSubscribe()
	{
		for (auto table : tables_)
		{
			table->UnSubscribe();
		}
	}
	void Mdb::InitDb()
	{
		CreateTables();
		for (auto table : tables_)
		{
			table->InitDb();
		}
	}
	void Mdb::SetInitStatus(bool initStatus)
	{
		for (auto table : tables_)
		{
			table->DbInited = initStatus;
		}
	}
	void Mdb::Dump(const char* dir)
	{
		for (auto table : tables_)
		{
			table->Dump(dir);
		}
	}
	void Mdb::CreateTables()
	{
		if (mdbSubscriber_ != nullptr)
		{
			mdbSubscriber_->OnTableOp(DbOperateType::CreateTables);
		}
	}
	void Mdb::DropTables()
	{
		if (mdbSubscriber_ != nullptr)
		{
			mdbSubscriber_->OnTableOp(DbOperateType::DropTables);
		}
	}
	void Mdb::TruncateTables()
	{
		if (mdbSubscriber_ != nullptr)
		{
			mdbSubscriber_->OnTableOp(DbOperateType::TruncateTables);
		}
		for (auto table : tables_)
		{
			table->TruncateTables();
		}
	}

	void Mdb::OnDbConnected()
	{
		InitDb();
	}
	void Mdb::OnDbDisConnected()
	{
		for (auto table : tables_)
		{
			table->DbInited = false;
		}
	}
}
