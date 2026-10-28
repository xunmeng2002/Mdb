// 本文件由 ../Templates/Cpp/Mdb/Rcu/PointerSwap/Mdb.cpp.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#include "Mdb.h"


namespace Mdb::Rcu::PointerSwap
{
	Mdb::Mdb(const TableList& tableList): replicaSubscriber_(nullptr)
	{
		for (int i = 0; i < tableList.Count; ++i)
		{
			switch (tableList.TableIds[i])
			{
			case TradingDay::TableId:  TradingDays = new TradingDayTable(); tables_.push_back(TradingDays); break;
			case Exchange::TableId:  Exchanges = new ExchangeTable(); tables_.push_back(Exchanges); break;
			case Product::TableId:  Products = new ProductTable(); tables_.push_back(Products); break;
			case Instrument::TableId:  Instruments = new InstrumentTable(); tables_.push_back(Instruments); break;
			case PrimaryAccount::TableId:  PrimaryAccounts = new PrimaryAccountTable(); tables_.push_back(PrimaryAccounts); break;
			case Account::TableId:  Accounts = new AccountTable(); tables_.push_back(Accounts); break;
			case Capital::TableId:  Capitals = new CapitalTable(); tables_.push_back(Capitals); break;
			case Position::TableId:  Positions = new PositionTable(); tables_.push_back(Positions); break;
			case PositionDetail::TableId:  PositionDetails = new PositionDetailTable(); tables_.push_back(PositionDetails); break;
			case Order::TableId:  Orders = new OrderTable(); tables_.push_back(Orders); break;
			case Trade::TableId:  Trades = new TradeTable(); tables_.push_back(Trades); break;
			default: break;
			}
		}
	}
	Mdb::~Mdb()
	{
		for (auto table : tables_)
		{
			delete table;
		}
	}
	void Mdb::Subscribe(MdbSubscriber* subscriber)
	{
		for (auto table : tables_)
		{
			table->Subscribe(subscriber);
		}
	}
	void Mdb::Subscribe(MdbSubscriber* subscriber, const std::vector<unsigned int>& tableIds)
	{
		for (unsigned int tableId : tableIds)
		{
			MdbTableBase* table = FindTable(tableId);
			if (table != nullptr)
			{
				table->Subscribe(subscriber);
			}
		}
	}
	void Mdb::SubscribeReplica(MdbReplicaSubscriber* replica)
	{
		replicaSubscriber_ = replica;
		for (auto table : tables_)
		{
			table->SubscribeReplica(replica);
		}
	}
	void Mdb::UnSubscribe()
	{
		for (auto table : tables_)
		{
			table->UnSubscribe();
		}
		replicaSubscriber_ = nullptr;
	}
	void Mdb::InitDb()
	{
		CreateTables();
		for (auto table : tables_)
		{
			table->InitDb();
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
		if (replicaSubscriber_ != nullptr)
		{
			replicaSubscriber_->OnTableOp(DbAdapters::DbOperateType::CreateTables);
		}
	}
	void Mdb::DropTables()
	{
		if (replicaSubscriber_ != nullptr)
		{
			replicaSubscriber_->OnTableOp(DbAdapters::DbOperateType::DropTables);
		}
	}
	void Mdb::TruncateTables()
	{
		if (replicaSubscriber_ != nullptr)
		{
			replicaSubscriber_->OnTableOp(DbAdapters::DbOperateType::TruncateTables);
		}
		for (auto table : tables_)
		{
			table->TruncateTables();
		}
	}
	MdbTableBase* Mdb::FindTable(unsigned int tableId)
	{
		for (auto table : tables_)
		{
			if (table->GetTableId() == tableId)
			{
				return table;
			}
		}
		return nullptr;
	}

	void Mdb::OnDbConnected()
	{
		InitDb();
	}
	void Mdb::OnDbDisConnected()
	{
		// 就绪位与待写队列由写库端在同一个临界区内清理，内存表不参与连接状态
	}
}
