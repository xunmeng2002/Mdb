#pragma once
#include <cstddef>

enum class FieldType : unsigned char
{
    Int,
    Int64,
    Double,
    Char,
    Bool,
};

struct FieldDescriptor
{
    const char*     name;
    FieldType       type;
    std::size_t     offset;
    std::size_t     arraySize;
};

struct RecordFactory
{
    void* (*Allocate)();
    void  (*PushBack)(void* records, void* record);
};

struct IndexDefinition
{
    unsigned int    indexID;
    const int*      fieldIndices;
    int             fieldCount;
};

struct TableSchema
{
    const char*             tableName;
    const FieldDescriptor*  fields;
    int                     fieldCount;
    const int*              primaryKeyIndices;
    int                     primaryKeyCount;
    void                    (*DeallocateRecord)(void*);
    const IndexDefinition*  secondaryIndices;
    int                     secondaryIndexCount;
};
