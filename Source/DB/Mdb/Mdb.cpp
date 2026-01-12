#include "Mdb.h"


namespace mdb
{
	Mdb::Mdb(): m_MdbSubscriber(nullptr)
	{
		t_TradingDay = new TradingDayTable(this);
		t_Exchange = new ExchangeTable(this);
		t_Product = new ProductTable(this);
		t_Instrument = new InstrumentTable(this);
		t_PrimaryAccount = new PrimaryAccountTable(this);
		t_Account = new AccountTable(this);
		t_Capital = new CapitalTable(this);
		t_Position = new PositionTable(this);
		t_PositionDetail = new PositionDetailTable(this);
		t_Order = new OrderTable(this);
		t_Trade = new TradeTable(this);
	}
	void Mdb::Subscribe(MdbSubscriber* mdbSubscriber)
	{
		m_MdbSubscriber = mdbSubscriber;
		t_TradingDay->Subscribe(mdbSubscriber);
		t_Exchange->Subscribe(mdbSubscriber);
		t_Product->Subscribe(mdbSubscriber);
		t_Instrument->Subscribe(mdbSubscriber);
		t_PrimaryAccount->Subscribe(mdbSubscriber);
		t_Account->Subscribe(mdbSubscriber);
		t_Capital->Subscribe(mdbSubscriber);
		t_Position->Subscribe(mdbSubscriber);
		t_PositionDetail->Subscribe(mdbSubscriber);
		t_Order->Subscribe(mdbSubscriber);
		t_Trade->Subscribe(mdbSubscriber);
	}
	void Mdb::UnSubscribe()
	{
		t_TradingDay->UnSubscribe();
		t_Exchange->UnSubscribe();
		t_Product->UnSubscribe();
		t_Instrument->UnSubscribe();
		t_PrimaryAccount->UnSubscribe();
		t_Account->UnSubscribe();
		t_Capital->UnSubscribe();
		t_Position->UnSubscribe();
		t_PositionDetail->UnSubscribe();
		t_Order->UnSubscribe();
		t_Trade->UnSubscribe();
	}
	void Mdb::InitDB()
	{
		CreateTables();
		t_TradingDay->InitDB();
		t_Exchange->InitDB();
		t_Product->InitDB();
		t_Instrument->InitDB();
		t_PrimaryAccount->InitDB();
		t_Account->InitDB();
		t_Capital->InitDB();
		t_Position->InitDB();
		t_PositionDetail->InitDB();
		t_Order->InitDB();
		t_Trade->InitDB();
	}
	void Mdb::SetInitStatus(bool initStatus)
	{
		t_TradingDay->m_DBInited = initStatus;
		t_Exchange->m_DBInited = initStatus;
		t_Product->m_DBInited = initStatus;
		t_Instrument->m_DBInited = initStatus;
		t_PrimaryAccount->m_DBInited = initStatus;
		t_Account->m_DBInited = initStatus;
		t_Capital->m_DBInited = initStatus;
		t_Position->m_DBInited = initStatus;
		t_PositionDetail->m_DBInited = initStatus;
		t_Order->m_DBInited = initStatus;
		t_Trade->m_DBInited = initStatus;
	}
	void Mdb::Dump(const char* dir)
	{
		t_TradingDay->Dump(dir);
		t_Exchange->Dump(dir);
		t_Product->Dump(dir);
		t_Instrument->Dump(dir);
		t_PrimaryAccount->Dump(dir);
		t_Account->Dump(dir);
		t_Capital->Dump(dir);
		t_Position->Dump(dir);
		t_PositionDetail->Dump(dir);
		t_Order->Dump(dir);
		t_Trade->Dump(dir);
	}
	void Mdb::CreateTables()
	{
		if (m_MdbSubscriber != nullptr)
		{
			m_MdbSubscriber->OnCreateTables();	
		}
	}
	void Mdb::DropTables()
	{
		if (m_MdbSubscriber != nullptr)
		{
			m_MdbSubscriber->OnDropTables();
		}
	}
	void Mdb::TruncateTables()
	{
		if (m_MdbSubscriber != nullptr)
		{
			m_MdbSubscriber->OnTruncateTables();
		}
		t_TradingDay->TruncateTables();
		t_Exchange->TruncateTables();
		t_Product->TruncateTables();
		t_Instrument->TruncateTables();
		t_PrimaryAccount->TruncateTables();
		t_Account->TruncateTables();
		t_Capital->TruncateTables();
		t_Position->TruncateTables();
		t_PositionDetail->TruncateTables();
		t_Order->TruncateTables();
		t_Trade->TruncateTables();
	}
	
	void Mdb::OnDBConnected()
	{
		InitDB();
	}
	void Mdb::OnDBDisConnected()
	{
		t_TradingDay->m_DBInited = false;
		t_Exchange->m_DBInited = false;
		t_Product->m_DBInited = false;
		t_Instrument->m_DBInited = false;
		t_PrimaryAccount->m_DBInited = false;
		t_Account->m_DBInited = false;
		t_Capital->m_DBInited = false;
		t_Position->m_DBInited = false;
		t_PositionDetail->m_DBInited = false;
		t_Order->m_DBInited = false;
		t_Trade->m_DBInited = false;
	}
}
