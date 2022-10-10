#include "DataStruct.h"
#include <string>

thread_local char t_DataStringBuffer[1024];


const char* Account::GetString() const
{
	sprintf(t_DataStringBuffer, "%s, %lld, %s, %s, %c, %c, %s, %c, %s, %d",
		OrgID, BrokerID, AccountID, AccountName, AccountClass, AccountType, PrimaryAccountID, AccountStatus, CurrencyID, DeleteFlag);
	return t_DataStringBuffer;
}
const char* Account::GetDebugString() const
{
	sprintf(t_DataStringBuffer, "OrgID:[%s], BrokerID:[%lld], AccountID:[%s], AccountName:[%s], AccountClass:[%c], AccountType:[%c], PrimaryAccountID:[%s], AccountStatus:[%c], CurrencyID:[%s], DeleteFlag:[%d]",
		OrgID, BrokerID, AccountID, AccountName, AccountClass, AccountType, PrimaryAccountID, AccountStatus, CurrencyID, DeleteFlag);
	return t_DataStringBuffer;
}


