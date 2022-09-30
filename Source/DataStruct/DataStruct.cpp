#include "DataStruct.h"
#include <string>

thread_local char t_DataStringBuffer[1024];



const char* Account::GetString() const
{
	sprintf(t_DataStringBuffer, "%d, %s, %s, %s, %s, %c, %c, %s, %c, %s, %d\n",
		BrokerID, AccountID, TradingDay, OrgID, AccountName, AccountClass, AccountType, PrimaryAccountID, AccountStatus, CurrencyID, DeleteFlag);
	return t_DataStringBuffer;
}

