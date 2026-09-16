// 本文件由 ../Templates/Cpp/Mdb/MdbTableRegistry.h.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#pragma once
#include "TableList.h"
#include <DBAdapters/DbInterface/SchemaRegistry.h>
#include <vector>

using DbAdapters::TableSchema;

namespace Mdb
{
	class MdbTableRegistry : public DbAdapters::SchemaRegistry
	{
	public:
		explicit MdbTableRegistry(const TableList& tableList);

		const TableSchema* GetSchema(unsigned int tableId) const override;
		const TableSchema* const* GetAllSchemas() const override;
		int GetTableCount() const override;

	private:
		static const TableSchema* GetSchemaByID(unsigned int tableId);

		std::vector<unsigned int> tableIds_;
		std::vector<const TableSchema*> schemas_;
	};
}
