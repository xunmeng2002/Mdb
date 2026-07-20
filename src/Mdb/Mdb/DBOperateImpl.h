#pragma once
#include <PersonalLib/Core/DB/DBOperate.h>
#include <vector>


class DBOperateImpl : public DBOperate
{
public:
    virtual void Deallocate() override;
    virtual void DeallocateRecord() override;
    virtual const char* GetDebugString() const override;

    std::vector<const void*>& GetBatchData() { return batch_data_; }

private:
    std::vector<const void*> batch_data_;
};
