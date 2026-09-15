// 本文件由 ../Templates/Cpp/Mdb/MdbTableRegistry.h.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#pragma once
#include "TableList.h"
#include <DBAdapters/DBInterface/SchemaRegistry.h>
#include <vector>

using dbadapters::TableSchema;

namespace mdb
{
	class MdbTableRegistry : public dbadapters::SchemaRegistry
	{
	public:
		explicit MdbTableRegistry(const TableList& tableList);

		const TableSchema* GetSchema(unsigned int tableID) const override;
		const TableSchema* const* GetAllSchemas() const override;
		int GetTableCount() const override;

	private:
		static const TableSchema* GetSchemaByID(unsigned int tableID);

		std::vector<unsigned int> tableIDs;
		std::vector<const TableSchema*> schemas;
	};
}
