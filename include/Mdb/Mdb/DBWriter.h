#pragma once
#include <Mdb/Mdb/MdbExport.h>
#include <Mdb/Mdb/MdbSubscriber.h>
#include <Mdb/Mdb/MdbStructs.h>
#include <DBInterface/DB.h>
#include <DBInterface/SchemaRegistry.h>
#include <PersonalLib/TemplateLib/TemplateLib.h>
#include <PersonalLib/Core/Core.h>
#include <list>
#include <atomic>
#include <mutex>
#include <condition_variable>


class MDB_EXPORTS DBWriter : public ThreadBase, public MdbSubscriber
{
public:
	DBWriter(DB* db, SchemaRegistry* schemaRegistry);
	~DBWriter();
	void Subscribe(DBSubscriber* dbSubscriber);
	DB* GetDB();
	bool Connect();
	void DisConnect();

	virtual void OnTableOp(DBOperateType op) override;
	virtual void OnRecordInsert(unsigned int tableID, void* record) override;
	virtual void OnRecordBatchInsert(unsigned int tableID, std::vector<const void*>* records) override;
	virtual void OnRecordErase(unsigned int tableID, void* record) override;
	virtual void OnRecordEraseByIndex(unsigned int tableID, unsigned int indexID, void* record) override;
	virtual void OnRecordUpdate(unsigned int tableID, void* record) override;
	virtual void OnRecordTruncate(unsigned int tableID) override;


protected:
	virtual void Run() override;
	void CheckConnect();
	void CheckDBOperate();
	void HandleDBOperate();
	DBOperate* GetDBOperate();

private:
	void AddDBOperate(DBOperate* dbOperate);

	void CreateTables(DBOperate* dbOperate);
	void DropTables(DBOperate* dbOperate);
	void TruncateTables(DBOperate* dbOperate);
	void InsertRecord(DBOperate* dbOperate);
	void DeleteRecord(DBOperate* dbOperate);
	void DeleteRecordByIndex(DBOperate* dbOperate);
	void UpdateRecord(DBOperate* dbOperate);
	void BatchInsertRecords(DBOperate* dbOperate);
	void TruncateTable(DBOperate* dbOperate);

private:
	DB* m_DB;
	SchemaRegistry* m_SchemaRegistry;
	DBSubscriber* m_DBSubscriber;
	std::list<DBOperate*> m_DBOperates;
	std::mutex m_Mutex;
	std::condition_variable m_ConditionVariable;
};
