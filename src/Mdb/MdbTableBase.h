// 本文件由 ../Templates/Cpp/Mdb/MdbTableBase.h.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#pragma once
#include <DBAdapters/DBInterface/MdbSubscriber.h>
#include <atomic>


namespace mdb
{
	class MdbTableBase
	{
	public:
		virtual ~MdbTableBase() = default;

		virtual void Subscribe(dbadapters::MdbSubscriber* subscriber) = 0;
		virtual void UnSubscribe() = 0;
		virtual void InitDB() = 0;
		virtual void TruncateTables() = 0;
		virtual void Dump(const char* dir) = 0;

	public:
		std::atomic<bool> dbInited{ false };
	};
}
