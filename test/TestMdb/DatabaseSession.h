#pragma once

#include "FullTableList.h"

#include "Common/MdbTableRegistry.h"
#include "LockBased/Mdb.h"

#include <DbAdapters/AsyncDbWriter/AsyncDbWriter.h>
#include <DbAdapters/DbInterface/Db.h>

#include <chrono>
#include <memory>
#include <utility>

// 物理库 + 内存表 + 异步写库线程的组合。Db 所有权随构造转入并交给 AsyncDbWriter，本对象不另持指针。
// Start 先同步连接（连接成功即建表、全量快照入队）再起写库线程，析构先停线程再退订阅。
class DatabaseSession
{
public:
	explicit DatabaseSession(std::unique_ptr<DbAdapters::Db> database);
	~DatabaseSession();
	DatabaseSession(const DatabaseSession&) = delete;
	DatabaseSession& operator=(const DatabaseSession&) = delete;

	// false 表示后端连不上；此时写库线程照常启动，会按自己的退避周期重试
	bool Start();
	void Stop();
	// 待写队列排空（不含正在提交的那一条），超时返回 false
	bool WaitForPendingZero(std::chrono::milliseconds timeout);

	::Mdb::Mdb& Tables();
	DbAdapters::AsyncDbWriter& Writer();

private:
	::Mdb::MdbTableRegistry schemaRegistry_;
	::Mdb::Mdb mdb_;
	DbAdapters::AsyncDbWriter dbWriter_;
	bool started_;
};
