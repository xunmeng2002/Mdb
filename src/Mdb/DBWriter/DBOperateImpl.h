#pragma once
#include <DBInterface/SchemaRegistry.h>
#include <DBInterface/DBOperate.h>
#include <vector>


class DBOperateImpl : public DBOperate
{
public:
    void SetSchemaRegistry(SchemaRegistry* registry) { schema_registry_ = registry; }

    virtual void Deallocate() override;
    virtual void DeallocateRecord() override;

    std::vector<const void*>& GetBatchData() { return batch_data_; }

private:
    std::vector<const void*> batch_data_;
    SchemaRegistry* schema_registry_ = nullptr;
};
