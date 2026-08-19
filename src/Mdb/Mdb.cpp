#include "Mdb.h"


namespace mdb
{
	Mdb::Mdb(const TableList& tableList): m_MdbSubscriber(nullptr)
	{
		for (int i = 0; i < tableList.Count; ++i)
		{
			switch (tableList.TableIDs[i])
			{
			case TradingDay::TableID:  t_TradingDay = new TradingDayTable(); m_Tables.push_back(t_TradingDay); break;
			case Exchange::TableID:  t_Exchange = new ExchangeTable(); m_Tables.push_back(t_Exchange); break;
			case Product::TableID:  t_Product = new ProductTable(); m_Tables.push_back(t_Product); break;
			case Instrument::TableID:  t_Instrument = new InstrumentTable(); m_Tables.push_back(t_Instrument); break;
			case PrimaryAccount::TableID:  t_PrimaryAccount = new PrimaryAccountTable(); m_Tables.push_back(t_PrimaryAccount); break;
			case Account::TableID:  t_Account = new AccountTable(); m_Tables.push_back(t_Account); break;
			case Capital::TableID:  t_Capital = new CapitalTable(); m_Tables.push_back(t_Capital); break;
			case Position::TableID:  t_Position = new PositionTable(); m_Tables.push_back(t_Position); break;
			case PositionDetail::TableID:  t_PositionDetail = new PositionDetailTable(); m_Tables.push_back(t_PositionDetail); break;
			case Order::TableID:  t_Order = new OrderTable(); m_Tables.push_back(t_Order); break;
			case Trade::TableID:  t_Trade = new TradeTable(); m_Tables.push_back(t_Trade); break;
			default: break;
			}
		}
	}
	void Mdb::Subscribe(MdbSubscriber* mdbSubscriber)
	{
		m_MdbSubscriber = mdbSubscriber;
		for (auto table : m_Tables)
		{
			table->Subscribe(mdbSubscriber);
		}
	}
	void Mdb::UnSubscribe()
	{
		for (auto table : m_Tables)
		{
			table->UnSubscribe();
		}
	}
	void Mdb::InitDB()
	{
		CreateTables();
		for (auto table : m_Tables)
		{
			table->InitDB();
		}
	}
	void Mdb::SetInitStatus(bool initStatus)
	{
		for (auto table : m_Tables)
		{
			table->m_DBInited = initStatus;
		}
	}
	void Mdb::Dump(const char* dir)
	{
		for (auto table : m_Tables)
		{
			table->Dump(dir);
		}
	}
	void Mdb::CreateTables()
	{
		if (m_MdbSubscriber != nullptr)
		{
			m_MdbSubscriber->OnTableOp(DBOperateType::CreateTables);
		}
	}
	void Mdb::DropTables()
	{
		if (m_MdbSubscriber != nullptr)
		{
			m_MdbSubscriber->OnTableOp(DBOperateType::DropTables);
		}
	}
	void Mdb::TruncateTables()
	{
		if (m_MdbSubscriber != nullptr)
		{
			m_MdbSubscriber->OnTableOp(DBOperateType::TruncateTables);
		}
		for (auto table : m_Tables)
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
		for (auto table : m_Tables)
		{
			table->m_DBInited = false;
		}
	}
}
