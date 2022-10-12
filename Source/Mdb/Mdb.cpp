#include "Mdb.h"



Mdb::Mdb()
{
	t_Account = new AccountTable();
	t_Order = new OrderTable();
}
void Mdb::Dump(const char* dir)
{
	t_Account->Dump(dir);
	t_Order->Dump(dir);
}
void Mdb::Clear()
{

}

