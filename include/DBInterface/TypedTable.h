#pragma once
#include <DBInterface/DB.h>
#include <vector>


template<typename T>
class TypedTable
{
public:
    explicit TypedTable(DB* db) : db_(db) {}

    void Insert(const T& record)
    {
        db_->Insert(&T::GetSchema(), &record);
    }

    void Update(const T& record)
    {
        db_->Update(&T::GetSchema(), &record);
    }

    void Delete(const T& record,
                const int* keyIndices = T::GetSchema().primaryKeyIndices,
                int keyCount = T::GetSchema().primaryKeyCount)
    {
        db_->Delete(&T::GetSchema(), &record, keyIndices, keyCount);
    }

    void SelectAll(std::vector<T*>& out)
    {
        db_->SelectAll(&T::GetSchema(), &out, GetFactory());
    }

    void SelectWithSql(const char* sql, std::vector<T*>& out)
    {
        db_->SelectWithSql(sql, &T::GetSchema(), &out, GetFactory());
    }

    void BatchInsert(const std::vector<const T*>& records)
    {
        std::vector<const void*> rawPtrs(records.size());
        for (std::size_t i = 0; i < records.size(); ++i)
            rawPtrs[i] = records[i];
        db_->BatchInsert(&T::GetSchema(), rawPtrs.data(),
                         static_cast<int>(rawPtrs.size()));
    }

private:
    static const RecordFactory& GetFactory()
    {
        static const RecordFactory factory = {
            []() -> void* { return T::Allocate(); },
            [](void* records, void* record) {
                static_cast<std::vector<T*>*>(records)->push_back(static_cast<T*>(record));
            }
        };
        return factory;
    }

    DB* db_;
};
