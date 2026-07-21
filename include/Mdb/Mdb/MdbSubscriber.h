#pragma once
#include <DBInterface/DBOperate.h>
#include <atomic>
#include <vector>


class MdbSubscriber
{
public:
	MdbSubscriber()
		:m_Connected(false)
	{
	}
	virtual ~MdbSubscriber() = default;

	virtual void OnTableOp(DBOperateType op) {}
	virtual void OnRecordInsert(unsigned int tableID, void* record) {}
	virtual void OnRecordBatchInsert(unsigned int tableID, std::vector<const void*>* records) {}
	virtual void OnRecordErase(unsigned int tableID, void* record) {}
	virtual void OnRecordEraseByIndex(unsigned int tableID, unsigned int indexID, void* record) {}
	virtual void OnRecordUpdate(unsigned int tableID, void* record) {}
	virtual void OnRecordTruncate(unsigned int tableID) {}

public:
	std::atomic<bool> m_Connected;
};
