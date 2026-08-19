#pragma once
#include <DBAdapters/DBInterface/MdbSubscriber.h>
#include <atomic>


namespace mdb
{
	class MdbTableBase
	{
	public:
		virtual ~MdbTableBase() = default;

		virtual void Subscribe(MdbSubscriber* mdbSubscriber) = 0;
		virtual void UnSubscribe() = 0;
		virtual void InitDB() = 0;
		virtual void TruncateTables() = 0;
		virtual void Dump(const char* dir) = 0;

	public:
		std::atomic<bool> m_DBInited{ false };
	};
}
