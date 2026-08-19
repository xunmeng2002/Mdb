#pragma once
#include "TableList.h"
#include <DBAdapters/DBInterface/SchemaRegistry.h>
#include <vector>


namespace mdb
{
	class MdbTableRegistry : public SchemaRegistry
	{
	public:
		explicit MdbTableRegistry(const TableList& tableList);

		const TableSchema* GetSchema(unsigned int tableID) const override;
		const TableSchema* const* GetAllSchemas() const override;
		int GetTableCount() const override;

	private:
		static const TableSchema* GetSchemaByID(unsigned int tableID);

		std::vector<unsigned int> m_TableIDs;
		std::vector<const TableSchema*> m_Schemas;
	};
}
