// 本文件由 ../Templates/Cpp/Mdb/Common/MdbTableBase.h.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#pragma once
#include <DbAdapters/DbInterface/MdbSubscriber.h>


namespace Mdb
{
	class MdbTableBase
	{
	public:
		virtual ~MdbTableBase() = default;

		virtual unsigned int GetTableId() const = 0;
		virtual void Subscribe(DbAdapters::MdbSubscriber* subscriber) = 0;
		virtual void SubscribeReplica(DbAdapters::MdbReplicaSubscriber* replica) = 0;
		virtual void UnSubscribe() = 0;
		virtual void InitDb() = 0;
		virtual void TruncateTables() = 0;
		virtual void Dump(const char* dir) = 0;
	};
}
