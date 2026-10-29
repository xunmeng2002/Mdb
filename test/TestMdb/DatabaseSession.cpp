#include "DatabaseSession.h"

#include <thread>

DatabaseSession::DatabaseSession(std::unique_ptr<DbAdapters::Db> database)
	: schemaRegistry_(::Mdb::Full::FullTableList)
	, mdb_(::Mdb::Full::FullTableList)
	, dbWriter_(std::move(database), &schemaRegistry_)
	, started_(false)
{
}

DatabaseSession::~DatabaseSession()
{
	Stop();
}

bool DatabaseSession::Start()
{
	mdb_.SubscribeReplica(&dbWriter_);
	dbWriter_.Subscribe(&mdb_);
	// 先连接：OnDbConnected 会触发 InitDb（建表 + 全量快照入队），线程启动后直接排空，
	// 不必再靠 sleep 等"线程大概连上了"
	const bool connected = dbWriter_.Connect();
	dbWriter_.Start();
	started_ = true;
	return connected;
}

void DatabaseSession::Stop()
{
	if (!started_)
	{
		return;
	}
	dbWriter_.Stop();
	dbWriter_.Join();
	mdb_.UnSubscribe();
	started_ = false;
}

bool DatabaseSession::WaitForPendingZero(std::chrono::milliseconds timeout)
{
	const auto deadline = std::chrono::steady_clock::now() + timeout;
	for (;;)
	{
		if (dbWriter_.PendingOperateCount() == 0)
		{
			return true;
		}
		if (std::chrono::steady_clock::now() >= deadline)
		{
			return false;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

::Mdb::Mdb& DatabaseSession::Tables()
{
	return mdb_;
}

DbAdapters::AsyncDbWriter& DatabaseSession::Writer()
{
	return dbWriter_;
}
