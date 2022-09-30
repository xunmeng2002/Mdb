#include "Mdb.h"


Mdb::Mdb()
{
	t_Account = new AccountTable();
}
void Mdb::Dump(const char* dir)
{
	t_Account->Dump(dir);
}
void Mdb::Clear()
{

}

