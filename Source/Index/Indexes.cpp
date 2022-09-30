#include "Indexes.h"


void AccountIndexForPrimaryAccount::Insert(Account* const account)
{
	m_Index.insert(account);
}
void AccountIndexForPrimaryAccount::Erase(Account* const account)
{
	m_Index.erase(account);
}
bool AccountIndexForPrimaryAccount::NeedUpdate(const Account* const oldAccount, const Account* const newAccount)
{
	return !AccountEqualForPrimaryAccount()(oldAccount, newAccount);
}
