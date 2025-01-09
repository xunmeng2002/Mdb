#include "Mdb.h"


namespace mdb
{
	Mdb::Mdb()
	{
		t_TradingDay = new TradingDayTable(this);
		t_Exchange = new ExchangeTable(this);
		t_Product = new ProductTable(this);
		t_Instrument = new InstrumentTable(this);
		t_Account = new AccountTable(this);
		t_Position = new PositionTable(this);
		t_Order = new OrderTable(this);
		t_Trade = new TradeTable(this);
		t_MdTick = new MdTickTable(this);
	}
	void Mdb::Subscribe(MdbSubscriber* mdbSubscriber)
	{
		t_TradingDay->Subscribe(mdbSubscriber);
		t_Exchange->Subscribe(mdbSubscriber);
		t_Product->Subscribe(mdbSubscriber);
		t_Instrument->Subscribe(mdbSubscriber);
		t_Account->Subscribe(mdbSubscriber);
		t_Position->Subscribe(mdbSubscriber);
		t_Order->Subscribe(mdbSubscriber);
		t_Trade->Subscribe(mdbSubscriber);
		t_MdTick->Subscribe(mdbSubscriber);
	}
	void Mdb::UnSubscribe()
	{
		t_TradingDay->UnSubscribe();
		t_Exchange->UnSubscribe();
		t_Product->UnSubscribe();
		t_Instrument->UnSubscribe();
		t_Account->UnSubscribe();
		t_Position->UnSubscribe();
		t_Order->UnSubscribe();
		t_Trade->UnSubscribe();
		t_MdTick->UnSubscribe();
	}
	void Mdb::InitDB()
	{
		t_TradingDay->InitDB();
		t_Exchange->InitDB();
		t_Product->InitDB();
		t_Instrument->InitDB();
		t_Account->InitDB();
		t_Position->InitDB();
		t_Order->InitDB();
		t_Trade->InitDB();
		t_MdTick->InitDB();
	}
	void Mdb::SetInitStatus(bool initStatus)
	{
		t_TradingDay->m_DBInited = initStatus;
		t_Exchange->m_DBInited = initStatus;
		t_Product->m_DBInited = initStatus;
		t_Instrument->m_DBInited = initStatus;
		t_Account->m_DBInited = initStatus;
		t_Position->m_DBInited = initStatus;
		t_Order->m_DBInited = initStatus;
		t_Trade->m_DBInited = initStatus;
		t_MdTick->m_DBInited = initStatus;
	}
	void Mdb::Dump(const char* dir)
	{
		t_TradingDay->Dump(dir);
		t_Exchange->Dump(dir);
		t_Product->Dump(dir);
		t_Instrument->Dump(dir);
		t_Account->Dump(dir);
		t_Position->Dump(dir);
		t_Order->Dump(dir);
		t_Trade->Dump(dir);
		t_MdTick->Dump(dir);
	}
	void Mdb::TruncateTables()
	{
		t_TradingDay->TruncateTable();
		t_Exchange->TruncateTable();
		t_Product->TruncateTable();
		t_Instrument->TruncateTable();
		t_Account->TruncateTable();
		t_Position->TruncateTable();
		t_Order->TruncateTable();
		t_Trade->TruncateTable();
		t_MdTick->TruncateTable();
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
		t_Account->m_DBInited = false;
		t_Position->m_DBInited = false;
		t_Order->m_DBInited = false;
		t_Trade->m_DBInited = false;
		t_MdTick->m_DBInited = false;
	}
}
