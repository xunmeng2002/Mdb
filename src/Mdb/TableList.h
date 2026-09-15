// 本文件由 ../Templates/Cpp/Mdb/TableList.h.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#pragma once
#include "MdbStructs.h"


namespace mdb
{
	struct TableList
	{
		const char* Name;
		const unsigned int* TableIDs;
		int Count;
	};
}
