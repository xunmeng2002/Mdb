#include "DuckDBCommon.h"
#include <string.h>

void CpyDuckdbString(char* data, duckdb_string_t& str)
{
	if (duckdb_string_is_inlined(str))
	{
		memcpy(data, str.value.inlined.inlined, str.value.inlined.length);
	}
	else
	{
		memcpy(data, str.value.pointer.ptr, str.value.pointer.length);
	}
}
