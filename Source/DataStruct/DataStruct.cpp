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

const char* Order::GetString() const
{
	sprintf(t_DataStringBuffer, "%s, %lld, %s, %s, %c, %c, %s, %s, %c, %c, %c, %s, %lld, %s, %c, %c, %d, %d, %s, %s",
		OrgID, BrokerID, AccountID, PrimaryAccountID, AccountClass, AccountType, ExchangeID, InstrumentID, Direction, OffsetFlag, HedgeFlag, OrderSysID, OrderLocalID, BrokerOrderID, OrderStatus, OrderType, Volume, VolumeTraded, InsertDate, InsertTime);
	return t_DataStringBuffer;
}
const char* Order::GetDebugString() const
{
	sprintf(t_DataStringBuffer, "OrgID:[%s], BrokerID:[%lld], AccountID:[%s], PrimaryAccountID:[%s], AccountClass:[%c], AccountType:[%c], ExchangeID:[%s], InstrumentID:[%s], Direction:[%c], OffsetFlag:[%c], HedgeFlag:[%c], OrderSysID:[%s], OrderLocalID:[%lld], BrokerOrderID:[%s], OrderStatus:[%c], OrderType:[%c], Volume:[%d], VolumeTraded:[%d], InsertDate:[%s], InsertTime:[%s]",
		OrgID, BrokerID, AccountID, PrimaryAccountID, AccountClass, AccountType, ExchangeID, InstrumentID, Direction, OffsetFlag, HedgeFlag, OrderSysID, OrderLocalID, BrokerOrderID, OrderStatus, OrderType, Volume, VolumeTraded, InsertDate, InsertTime);
	return t_DataStringBuffer;
}


