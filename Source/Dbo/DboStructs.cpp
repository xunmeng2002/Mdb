#include "DboStructs.h"
#include <string>

thread_local char t_DboDataStringBuffer[10240];


const char* DboAuthorizedSoftware::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%s, %s, %d",
		Software, AuthCode, Account);
	return t_DboDataStringBuffer;
}
const char* DboAuthorizedSoftware::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboAuthorizedSoftware:Software:[%s], AuthCode:[%s], Account:[%d]",
		Software, AuthCode, Account);
	return t_DboDataStringBuffer;
}
int DboAuthorizedSoftware::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%s', '%s', '%d'",
		Software, AuthCode, Account);
}
int DboAuthorizedSoftware::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%s|%s|%d|",
		Software, AuthCode, Account);
}

const char* DboClose::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%d, %d, %s, %d, %d, %s, %d, %s, %s, %c, %c, %f, %f, %f, %d, %d, %d, %s, %s, %f, %f, %s, %s",
		OpenDate, OpenOrderID, OpenTradeID, CloseDate, CloseOrderID, CloseTradeID, Account, ExchangeID, ContractID, (char)BS, (char)Hedge, OpenPrice, LastSettle, ClosePrice, Volume, RealOpenDate, RealCloseDate, OpenTime, CloseTime, CloseProfitFloat, CloseProfit, CurrencyGroup, CurrencyID);
	return t_DboDataStringBuffer;
}
const char* DboClose::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboClose:OpenDate:[%d], OpenOrderID:[%d], OpenTradeID:[%s], CloseDate:[%d], CloseOrderID:[%d], CloseTradeID:[%s], Account:[%d], ExchangeID:[%s], ContractID:[%s], BS:[%c], Hedge:[%c], OpenPrice:[%f], LastSettle:[%f], ClosePrice:[%f], Volume:[%d], RealOpenDate:[%d], RealCloseDate:[%d], OpenTime:[%s], CloseTime:[%s], CloseProfitFloat:[%f], CloseProfit:[%f], CurrencyGroup:[%s], CurrencyID:[%s]",
		OpenDate, OpenOrderID, OpenTradeID, CloseDate, CloseOrderID, CloseTradeID, Account, ExchangeID, ContractID, (char)BS, (char)Hedge, OpenPrice, LastSettle, ClosePrice, Volume, RealOpenDate, RealCloseDate, OpenTime, CloseTime, CloseProfitFloat, CloseProfit, CurrencyGroup, CurrencyID);
	return t_DboDataStringBuffer;
}
int DboClose::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%d', '%d', '%s', '%d', '%d', '%s', '%d', '%s', '%s', '%c', '%c', '%f', '%f', '%f', '%d', '%d', '%d', '%s', '%s', '%f', '%f', '%s', '%s'",
		OpenDate, OpenOrderID, OpenTradeID, CloseDate, CloseOrderID, CloseTradeID, Account, ExchangeID, ContractID, (char)BS, (char)Hedge, OpenPrice, LastSettle, ClosePrice, Volume, RealOpenDate, RealCloseDate, OpenTime, CloseTime, CloseProfitFloat, CloseProfit, CurrencyGroup, CurrencyID);
}
int DboClose::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%d|%d|%s|%d|%d|%s|%d|%s|%s|%c|%c|%f|%f|%f|%d|%d|%d|%s|%s|%f|%f|%s|%s|",
		OpenDate, OpenOrderID, OpenTradeID, CloseDate, CloseOrderID, CloseTradeID, Account, ExchangeID, ContractID, (char)BS, (char)Hedge, OpenPrice, LastSettle, ClosePrice, Volume, RealOpenDate, RealCloseDate, OpenTime, CloseTime, CloseProfitFloat, CloseProfit, CurrencyGroup, CurrencyID);
}

const char* DboCommission::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%d, %d, %s, %s, %s, %f, %f, %f, %f",
		GroupID, Account, ExchangeID, ProductID, ContractID, OpenByVolume, CloseByVolume, OpenByMoney, CloseByMoney);
	return t_DboDataStringBuffer;
}
const char* DboCommission::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboCommission:GroupID:[%d], Account:[%d], ExchangeID:[%s], ProductID:[%s], ContractID:[%s], OpenByVolume:[%f], CloseByVolume:[%f], OpenByMoney:[%f], CloseByMoney:[%f]",
		GroupID, Account, ExchangeID, ProductID, ContractID, OpenByVolume, CloseByVolume, OpenByMoney, CloseByMoney);
	return t_DboDataStringBuffer;
}
int DboCommission::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%d', '%d', '%s', '%s', '%s', '%f', '%f', '%f', '%f'",
		GroupID, Account, ExchangeID, ProductID, ContractID, OpenByVolume, CloseByVolume, OpenByMoney, CloseByMoney);
}
int DboCommission::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%d|%d|%s|%s|%s|%f|%f|%f|%f|",
		GroupID, Account, ExchangeID, ProductID, ContractID, OpenByVolume, CloseByVolume, OpenByMoney, CloseByMoney);
}

const char* DboContract::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%s, %s, %s, %s, %d, %d, %d, %d, %d, %d, %d, %c, %f, %f",
		ExchangeID, ProductID, ContractID, ContractName, DeliveryYear, DeliveryMonth, OpenDate, ExpireDate, FirstNoticeDate, StartDelivDate, EndDelivDate, (char)TradeStatus, Margin, MinMargin);
	return t_DboDataStringBuffer;
}
const char* DboContract::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboContract:ExchangeID:[%s], ProductID:[%s], ContractID:[%s], ContractName:[%s], DeliveryYear:[%d], DeliveryMonth:[%d], OpenDate:[%d], ExpireDate:[%d], FirstNoticeDate:[%d], StartDelivDate:[%d], EndDelivDate:[%d], TradeStatus:[%c], Margin:[%f], MinMargin:[%f]",
		ExchangeID, ProductID, ContractID, ContractName, DeliveryYear, DeliveryMonth, OpenDate, ExpireDate, FirstNoticeDate, StartDelivDate, EndDelivDate, (char)TradeStatus, Margin, MinMargin);
	return t_DboDataStringBuffer;
}
int DboContract::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%s', '%s', '%s', '%s', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%c', '%f', '%f'",
		ExchangeID, ProductID, ContractID, ContractName, DeliveryYear, DeliveryMonth, OpenDate, ExpireDate, FirstNoticeDate, StartDelivDate, EndDelivDate, (char)TradeStatus, Margin, MinMargin);
}
int DboContract::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%s|%s|%s|%s|%d|%d|%d|%d|%d|%d|%d|%c|%f|%f|",
		ExchangeID, ProductID, ContractID, ContractName, DeliveryYear, DeliveryMonth, OpenDate, ExpireDate, FirstNoticeDate, StartDelivDate, EndDelivDate, (char)TradeStatus, Margin, MinMargin);
}

const char* DboCreditAmount::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%d, %s, %s, %s, %d, %f, %s, %c, %d, %s, %d, %s, %s, %d",
		ID, AuditTime, OperationTime, Remark, Status, Amount, CurrencyGroup, (char)Direction, TradingDay, OperatorID, Account, CurrencyID, AuditorID, Flag);
	return t_DboDataStringBuffer;
}
const char* DboCreditAmount::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboCreditAmount:ID:[%d], AuditTime:[%s], OperationTime:[%s], Remark:[%s], Status:[%d], Amount:[%f], CurrencyGroup:[%s], Direction:[%c], TradingDay:[%d], OperatorID:[%s], Account:[%d], CurrencyID:[%s], AuditorID:[%s], Flag:[%d]",
		ID, AuditTime, OperationTime, Remark, Status, Amount, CurrencyGroup, (char)Direction, TradingDay, OperatorID, Account, CurrencyID, AuditorID, Flag);
	return t_DboDataStringBuffer;
}
int DboCreditAmount::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%d', '%s', '%s', '%s', '%d', '%f', '%s', '%c', '%d', '%s', '%d', '%s', '%s', '%d'",
		ID, AuditTime, OperationTime, Remark, Status, Amount, CurrencyGroup, (char)Direction, TradingDay, OperatorID, Account, CurrencyID, AuditorID, Flag);
}
int DboCreditAmount::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%d|%s|%s|%s|%d|%f|%s|%c|%d|%s|%d|%s|%s|%d|",
		ID, AuditTime, OperationTime, Remark, Status, Amount, CurrencyGroup, (char)Direction, TradingDay, OperatorID, Account, CurrencyID, AuditorID, Flag);
}

const char* DboCurrencyRate::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%s, %s, %f, %f, %d",
		CurrencyGroup, CurrencyID, TradeRate, SettlementRate, IsBaseCurrency);
	return t_DboDataStringBuffer;
}
const char* DboCurrencyRate::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboCurrencyRate:CurrencyGroup:[%s], CurrencyID:[%s], TradeRate:[%f], SettlementRate:[%f], IsBaseCurrency:[%d]",
		CurrencyGroup, CurrencyID, TradeRate, SettlementRate, IsBaseCurrency);
	return t_DboDataStringBuffer;
}
int DboCurrencyRate::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%s', '%s', '%f', '%f', '%d'",
		CurrencyGroup, CurrencyID, TradeRate, SettlementRate, IsBaseCurrency);
}
int DboCurrencyRate::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%s|%s|%f|%f|%d|",
		CurrencyGroup, CurrencyID, TradeRate, SettlementRate, IsBaseCurrency);
}

const char* DboDepositWithdrawal::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%d, %s, %s, %s, %d, %f, %s, %s, %s, %c, %d, %d, %s, %d, %s, %d",
		ID, AuditTime, OperationTime, Remark, Status, Amount, BankCardID, CurrencyID, CurrencyGroup, (char)Direction, Method, TradingDay, OperatorID, Account, AuditorID, Flag);
	return t_DboDataStringBuffer;
}
const char* DboDepositWithdrawal::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboDepositWithdrawal:ID:[%d], AuditTime:[%s], OperationTime:[%s], Remark:[%s], Status:[%d], Amount:[%f], BankCardID:[%s], CurrencyID:[%s], CurrencyGroup:[%s], Direction:[%c], Method:[%d], TradingDay:[%d], OperatorID:[%s], Account:[%d], AuditorID:[%s], Flag:[%d]",
		ID, AuditTime, OperationTime, Remark, Status, Amount, BankCardID, CurrencyID, CurrencyGroup, (char)Direction, Method, TradingDay, OperatorID, Account, AuditorID, Flag);
	return t_DboDataStringBuffer;
}
int DboDepositWithdrawal::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%d', '%s', '%s', '%s', '%d', '%f', '%s', '%s', '%s', '%c', '%d', '%d', '%s', '%d', '%s', '%d'",
		ID, AuditTime, OperationTime, Remark, Status, Amount, BankCardID, CurrencyID, CurrencyGroup, (char)Direction, Method, TradingDay, OperatorID, Account, AuditorID, Flag);
}
int DboDepositWithdrawal::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%d|%s|%s|%s|%d|%f|%s|%s|%s|%c|%d|%d|%s|%d|%s|%d|",
		ID, AuditTime, OperationTime, Remark, Status, Amount, BankCardID, CurrencyID, CurrencyGroup, (char)Direction, Method, TradingDay, OperatorID, Account, AuditorID, Flag);
}

const char* DboExchange::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%s, %s, %c",
		ExchangeID, ExchangeName, (char)TradeStatus);
	return t_DboDataStringBuffer;
}
const char* DboExchange::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboExchange:ExchangeID:[%s], ExchangeName:[%s], TradeStatus:[%c]",
		ExchangeID, ExchangeName, (char)TradeStatus);
	return t_DboDataStringBuffer;
}
int DboExchange::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%s', '%s', '%c'",
		ExchangeID, ExchangeName, (char)TradeStatus);
}
int DboExchange::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%s|%s|%c|",
		ExchangeID, ExchangeName, (char)TradeStatus);
}

const char* DboInvertor::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%d, %c, %c, %c, %d, %c, %c, %d, %c, %d, %s, %d, %s, %d, %d, %d, %d, %c, %s, %d, %d, %d, %d",
		Account, (char)AccountStatus, (char)AccountType, (char)AgentType, CloseDate, (char)OrderType, (char)InvestorType, IsEmployee, (char)LoginStatus, maxOnLine, Name, OpenDate, Password, PasswordDate, PasswordExpired, PasswordValidDays, RiskLevel, (char)TradeStatus, TwoStepCode, TwoStepCodeTime, TwoStepLogin, CommissionGroupID, MarginGroupID);
	return t_DboDataStringBuffer;
}
const char* DboInvertor::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboInvertor:Account:[%d], AccountStatus:[%c], AccountType:[%c], AgentType:[%c], CloseDate:[%d], OrderType:[%c], InvestorType:[%c], IsEmployee:[%d], LoginStatus:[%c], maxOnLine:[%d], Name:[%s], OpenDate:[%d], Password:[%s], PasswordDate:[%d], PasswordExpired:[%d], PasswordValidDays:[%d], RiskLevel:[%d], TradeStatus:[%c], TwoStepCode:[%s], TwoStepCodeTime:[%d], TwoStepLogin:[%d], CommissionGroupID:[%d], MarginGroupID:[%d]",
		Account, (char)AccountStatus, (char)AccountType, (char)AgentType, CloseDate, (char)OrderType, (char)InvestorType, IsEmployee, (char)LoginStatus, maxOnLine, Name, OpenDate, Password, PasswordDate, PasswordExpired, PasswordValidDays, RiskLevel, (char)TradeStatus, TwoStepCode, TwoStepCodeTime, TwoStepLogin, CommissionGroupID, MarginGroupID);
	return t_DboDataStringBuffer;
}
int DboInvertor::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%d', '%c', '%c', '%c', '%d', '%c', '%c', '%d', '%c', '%d', '%s', '%d', '%s', '%d', '%d', '%d', '%d', '%c', '%s', '%d', '%d', '%d', '%d'",
		Account, (char)AccountStatus, (char)AccountType, (char)AgentType, CloseDate, (char)OrderType, (char)InvestorType, IsEmployee, (char)LoginStatus, maxOnLine, Name, OpenDate, Password, PasswordDate, PasswordExpired, PasswordValidDays, RiskLevel, (char)TradeStatus, TwoStepCode, TwoStepCodeTime, TwoStepLogin, CommissionGroupID, MarginGroupID);
}
int DboInvertor::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%d|%c|%c|%c|%d|%c|%c|%d|%c|%d|%s|%d|%s|%d|%d|%d|%d|%c|%s|%d|%d|%d|%d|",
		Account, (char)AccountStatus, (char)AccountType, (char)AgentType, CloseDate, (char)OrderType, (char)InvestorType, IsEmployee, (char)LoginStatus, maxOnLine, Name, OpenDate, Password, PasswordDate, PasswordExpired, PasswordValidDays, RiskLevel, (char)TradeStatus, TwoStepCode, TwoStepCodeTime, TwoStepLogin, CommissionGroupID, MarginGroupID);
}

const char* DboInvestorMobile::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%d, %s, %d, %d, %s",
		ID, Mobile, MobileType, Account, AreaCode);
	return t_DboDataStringBuffer;
}
const char* DboInvestorMobile::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboInvestorMobile:ID:[%d], Mobile:[%s], MobileType:[%d], Account:[%d], AreaCode:[%s]",
		ID, Mobile, MobileType, Account, AreaCode);
	return t_DboDataStringBuffer;
}
int DboInvestorMobile::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%d', '%s', '%d', '%d', '%s'",
		ID, Mobile, MobileType, Account, AreaCode);
}
int DboInvestorMobile::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%d|%s|%d|%d|%s|",
		ID, Mobile, MobileType, Account, AreaCode);
}

const char* DboInvestorParam::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%d, %s, %s, %s, %c, %d, %d, %d, %d",
		Account, ExchangeID, ProductID, ContractID, (char)TradeStatus, MaxPosition, MaxOrderVolume, CloseAdvanceDays, ChannelID);
	return t_DboDataStringBuffer;
}
const char* DboInvestorParam::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboInvestorParam:Account:[%d], ExchangeID:[%s], ProductID:[%s], ContractID:[%s], TradeStatus:[%c], MaxPosition:[%d], MaxOrderVolume:[%d], CloseAdvanceDays:[%d], ChannelID:[%d]",
		Account, ExchangeID, ProductID, ContractID, (char)TradeStatus, MaxPosition, MaxOrderVolume, CloseAdvanceDays, ChannelID);
	return t_DboDataStringBuffer;
}
int DboInvestorParam::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%d', '%s', '%s', '%s', '%c', '%d', '%d', '%d', '%d'",
		Account, ExchangeID, ProductID, ContractID, (char)TradeStatus, MaxPosition, MaxOrderVolume, CloseAdvanceDays, ChannelID);
}
int DboInvestorParam::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%d|%s|%s|%s|%c|%d|%d|%d|%d|",
		Account, ExchangeID, ProductID, ContractID, (char)TradeStatus, MaxPosition, MaxOrderVolume, CloseAdvanceDays, ChannelID);
}

const char* DboLogin::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%d, %d, %d, %d, %d, %s, %d, %s, %s, %s, %s, %s, %s, %d, %s",
		TradeDate, FrontID, SessionID, RealDate, Account, Admin, TwoStepLogin, Time, IP, MAC, UserProduct, Hardware, OS, LogoutDate, LogoutTime);
	return t_DboDataStringBuffer;
}
const char* DboLogin::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboLogin:TradeDate:[%d], FrontID:[%d], SessionID:[%d], RealDate:[%d], Account:[%d], Admin:[%s], TwoStepLogin:[%d], Time:[%s], IP:[%s], MAC:[%s], UserProduct:[%s], Hardware:[%s], OS:[%s], LogoutDate:[%d], LogoutTime:[%s]",
		TradeDate, FrontID, SessionID, RealDate, Account, Admin, TwoStepLogin, Time, IP, MAC, UserProduct, Hardware, OS, LogoutDate, LogoutTime);
	return t_DboDataStringBuffer;
}
int DboLogin::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%d', '%d', '%d', '%d', '%d', '%s', '%d', '%s', '%s', '%s', '%s', '%s', '%s', '%d', '%s'",
		TradeDate, FrontID, SessionID, RealDate, Account, Admin, TwoStepLogin, Time, IP, MAC, UserProduct, Hardware, OS, LogoutDate, LogoutTime);
}
int DboLogin::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%d|%d|%d|%d|%d|%s|%d|%s|%s|%s|%s|%s|%s|%d|%s|",
		TradeDate, FrontID, SessionID, RealDate, Account, Admin, TwoStepLogin, Time, IP, MAC, UserProduct, Hardware, OS, LogoutDate, LogoutTime);
}

const char* DboMargin::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%d, %d, %s, %s, %s, %c, %f, %f, %f",
		GroupID, Account, ExchangeID, ProductID, ContractID, (char)MarginType, Margin, MinMargin, SettleMargin);
	return t_DboDataStringBuffer;
}
const char* DboMargin::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboMargin:GroupID:[%d], Account:[%d], ExchangeID:[%s], ProductID:[%s], ContractID:[%s], MarginType:[%c], Margin:[%f], MinMargin:[%f], SettleMargin:[%f]",
		GroupID, Account, ExchangeID, ProductID, ContractID, (char)MarginType, Margin, MinMargin, SettleMargin);
	return t_DboDataStringBuffer;
}
int DboMargin::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%d', '%d', '%s', '%s', '%s', '%c', '%f', '%f', '%f'",
		GroupID, Account, ExchangeID, ProductID, ContractID, (char)MarginType, Margin, MinMargin, SettleMargin);
}
int DboMargin::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%d|%d|%s|%s|%s|%c|%f|%f|%f|",
		GroupID, Account, ExchangeID, ProductID, ContractID, (char)MarginType, Margin, MinMargin, SettleMargin);
}

const char* DboMaxBalance::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%d, %f, %f",
		Account, MaxBalance, EverMaxBalance);
	return t_DboDataStringBuffer;
}
const char* DboMaxBalance::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboMaxBalance:Account:[%d], MaxBalance:[%f], EverMaxBalance:[%f]",
		Account, MaxBalance, EverMaxBalance);
	return t_DboDataStringBuffer;
}
int DboMaxBalance::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%d', '%f', '%f'",
		Account, MaxBalance, EverMaxBalance);
}
int DboMaxBalance::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%d|%f|%f|",
		Account, MaxBalance, EverMaxBalance);
}

const char* DboOrder::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%s, %d, %d, %s, %s, %d, %s, %s, %c, %c, %c, %c, %f, %d, %d, %d, %d, %s, %s, %c, %s, %s, %s, %d, %d, %s, %c, %d, %c, %d, %c, %f, %d, %d, %s, %d, %d, %d, %d, %s, %s, %d, %d, %s, %s, %d, %d, %d, %d, %s",
		TradeDate, OrderID, Account, InsertAccount, CancelAccount, RequestID, ExchangeID, ContractID, (char)BS, (char)OC, (char)Hedge, (char)OrderPriceType, Price, Volume, TradeVolume, Status, RealDate, InsertTime, CancelTime, (char)ForceClose, ForceReason, OrderSysID, Remark, FrontID, SessionID, OrderRef, (char)TimeCondition, GTDDate, (char)VolumeCondition, MinVolume, (char)ContingentCondition, StopPrice, SwapOrder, ChannelID, Tag50, CancelFrontID, CancelSessionID, ChannelFrontID, ChannelSessionID, ChannelOrderRef, ChannelOrderID, T1Valid, Deleted, DeleteAccount, DeleteTime, InsertDate, InsertID, PatchOrder, CancelDate, UserOrderID);
	return t_DboDataStringBuffer;
}
const char* DboOrder::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboOrder:TradeDate:[%s], OrderID:[%d], Account:[%d], InsertAccount:[%s], CancelAccount:[%s], RequestID:[%d], ExchangeID:[%s], ContractID:[%s], BS:[%c], OC:[%c], Hedge:[%c], OrderPriceType:[%c], Price:[%f], Volume:[%d], TradeVolume:[%d], Status:[%d], RealDate:[%d], InsertTime:[%s], CancelTime:[%s], ForceClose:[%c], ForceReason:[%s], OrderSysID:[%s], Remark:[%s], FrontID:[%d], SessionID:[%d], OrderRef:[%s], TimeCondition:[%c], GTDDate:[%d], VolumeCondition:[%c], MinVolume:[%d], ContingentCondition:[%c], StopPrice:[%f], SwapOrder:[%d], ChannelID:[%d], Tag50:[%s], CancelFrontID:[%d], CancelSessionID:[%d], ChannelFrontID:[%d], ChannelSessionID:[%d], ChannelOrderRef:[%s], ChannelOrderID:[%s], T1Valid:[%d], Deleted:[%d], DeleteAccount:[%s], DeleteTime:[%s], InsertDate:[%d], InsertID:[%d], PatchOrder:[%d], CancelDate:[%d], UserOrderID:[%s]",
		TradeDate, OrderID, Account, InsertAccount, CancelAccount, RequestID, ExchangeID, ContractID, (char)BS, (char)OC, (char)Hedge, (char)OrderPriceType, Price, Volume, TradeVolume, Status, RealDate, InsertTime, CancelTime, (char)ForceClose, ForceReason, OrderSysID, Remark, FrontID, SessionID, OrderRef, (char)TimeCondition, GTDDate, (char)VolumeCondition, MinVolume, (char)ContingentCondition, StopPrice, SwapOrder, ChannelID, Tag50, CancelFrontID, CancelSessionID, ChannelFrontID, ChannelSessionID, ChannelOrderRef, ChannelOrderID, T1Valid, Deleted, DeleteAccount, DeleteTime, InsertDate, InsertID, PatchOrder, CancelDate, UserOrderID);
	return t_DboDataStringBuffer;
}
int DboOrder::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%s', '%d', '%d', '%s', '%s', '%d', '%s', '%s', '%c', '%c', '%c', '%c', '%f', '%d', '%d', '%d', '%d', '%s', '%s', '%c', '%s', '%s', '%s', '%d', '%d', '%s', '%c', '%d', '%c', '%d', '%c', '%f', '%d', '%d', '%s', '%d', '%d', '%d', '%d', '%s', '%s', '%d', '%d', '%s', '%s', '%d', '%d', '%d', '%d', '%s'",
		TradeDate, OrderID, Account, InsertAccount, CancelAccount, RequestID, ExchangeID, ContractID, (char)BS, (char)OC, (char)Hedge, (char)OrderPriceType, Price, Volume, TradeVolume, Status, RealDate, InsertTime, CancelTime, (char)ForceClose, ForceReason, OrderSysID, Remark, FrontID, SessionID, OrderRef, (char)TimeCondition, GTDDate, (char)VolumeCondition, MinVolume, (char)ContingentCondition, StopPrice, SwapOrder, ChannelID, Tag50, CancelFrontID, CancelSessionID, ChannelFrontID, ChannelSessionID, ChannelOrderRef, ChannelOrderID, T1Valid, Deleted, DeleteAccount, DeleteTime, InsertDate, InsertID, PatchOrder, CancelDate, UserOrderID);
}
int DboOrder::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%s|%d|%d|%s|%s|%d|%s|%s|%c|%c|%c|%c|%f|%d|%d|%d|%d|%s|%s|%c|%s|%s|%s|%d|%d|%s|%c|%d|%c|%d|%c|%f|%d|%d|%s|%d|%d|%d|%d|%s|%s|%d|%d|%s|%s|%d|%d|%d|%d|%s|",
		TradeDate, OrderID, Account, InsertAccount, CancelAccount, RequestID, ExchangeID, ContractID, (char)BS, (char)OC, (char)Hedge, (char)OrderPriceType, Price, Volume, TradeVolume, Status, RealDate, InsertTime, CancelTime, (char)ForceClose, ForceReason, OrderSysID, Remark, FrontID, SessionID, OrderRef, (char)TimeCondition, GTDDate, (char)VolumeCondition, MinVolume, (char)ContingentCondition, StopPrice, SwapOrder, ChannelID, Tag50, CancelFrontID, CancelSessionID, ChannelFrontID, ChannelSessionID, ChannelOrderRef, ChannelOrderID, T1Valid, Deleted, DeleteAccount, DeleteTime, InsertDate, InsertID, PatchOrder, CancelDate, UserOrderID);
}

const char* DboOrderCheck::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%d, %d, %d, %d, %f, %s, %d, %s",
		ID, Account, GroupID, Item, Param1, ProductID, Result, ExchangeID);
	return t_DboDataStringBuffer;
}
const char* DboOrderCheck::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboOrderCheck:ID:[%d], Account:[%d], GroupID:[%d], Item:[%d], Param1:[%f], ProductID:[%s], Result:[%d], ExchangeID:[%s]",
		ID, Account, GroupID, Item, Param1, ProductID, Result, ExchangeID);
	return t_DboDataStringBuffer;
}
int DboOrderCheck::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%d', '%d', '%d', '%d', '%f', '%s', '%d', '%s'",
		ID, Account, GroupID, Item, Param1, ProductID, Result, ExchangeID);
}
int DboOrderCheck::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%d|%d|%d|%d|%f|%s|%d|%s|",
		ID, Account, GroupID, Item, Param1, ProductID, Result, ExchangeID);
}

const char* DboPosition::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%d, %d, %s, %s, %c, %c, %d, %s, %d, %d, %d, %s, %d, %f, %f, %f, %f, %f, %f, %f, %d, %f, %f, %f, %c, %s, %s, %s, %s, %f",
		Account, TradeDate, ExchangeID, ContractID, (char)BS, (char)Hedge, OpenDate, TradeID, OrderID, LocalTradeID, RealOpenDate, TradeTime, Volume, OpenPrice, SettlePrice, LastSettle, VolumeMultiple, Margin, PositionProfitFloat, PositionProfit, CloseVolume, CloseProfitFloat, CloseProfit, CloseAmount, (char)TradeType, CombineContractID, CombineTradeID, CurrencyGroup, CurrencyID, MinMargin);
	return t_DboDataStringBuffer;
}
const char* DboPosition::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboPosition:Account:[%d], TradeDate:[%d], ExchangeID:[%s], ContractID:[%s], BS:[%c], Hedge:[%c], OpenDate:[%d], TradeID:[%s], OrderID:[%d], LocalTradeID:[%d], RealOpenDate:[%d], TradeTime:[%s], Volume:[%d], OpenPrice:[%f], SettlePrice:[%f], LastSettle:[%f], VolumeMultiple:[%f], Margin:[%f], PositionProfitFloat:[%f], PositionProfit:[%f], CloseVolume:[%d], CloseProfitFloat:[%f], CloseProfit:[%f], CloseAmount:[%f], TradeType:[%c], CombineContractID:[%s], CombineTradeID:[%s], CurrencyGroup:[%s], CurrencyID:[%s], MinMargin:[%f]",
		Account, TradeDate, ExchangeID, ContractID, (char)BS, (char)Hedge, OpenDate, TradeID, OrderID, LocalTradeID, RealOpenDate, TradeTime, Volume, OpenPrice, SettlePrice, LastSettle, VolumeMultiple, Margin, PositionProfitFloat, PositionProfit, CloseVolume, CloseProfitFloat, CloseProfit, CloseAmount, (char)TradeType, CombineContractID, CombineTradeID, CurrencyGroup, CurrencyID, MinMargin);
	return t_DboDataStringBuffer;
}
int DboPosition::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%d', '%d', '%s', '%s', '%c', '%c', '%d', '%s', '%d', '%d', '%d', '%s', '%d', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%d', '%f', '%f', '%f', '%c', '%s', '%s', '%s', '%s', '%f'",
		Account, TradeDate, ExchangeID, ContractID, (char)BS, (char)Hedge, OpenDate, TradeID, OrderID, LocalTradeID, RealOpenDate, TradeTime, Volume, OpenPrice, SettlePrice, LastSettle, VolumeMultiple, Margin, PositionProfitFloat, PositionProfit, CloseVolume, CloseProfitFloat, CloseProfit, CloseAmount, (char)TradeType, CombineContractID, CombineTradeID, CurrencyGroup, CurrencyID, MinMargin);
}
int DboPosition::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%d|%d|%s|%s|%c|%c|%d|%s|%d|%d|%d|%s|%d|%f|%f|%f|%f|%f|%f|%f|%d|%f|%f|%f|%c|%s|%s|%s|%s|%f|",
		Account, TradeDate, ExchangeID, ContractID, (char)BS, (char)Hedge, OpenDate, TradeID, OrderID, LocalTradeID, RealOpenDate, TradeTime, Volume, OpenPrice, SettlePrice, LastSettle, VolumeMultiple, Margin, PositionProfitFloat, PositionProfit, CloseVolume, CloseProfitFloat, CloseProfit, CloseAmount, (char)TradeType, CombineContractID, CombineTradeID, CurrencyGroup, CurrencyID, MinMargin);
}

const char* DboProduct::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%s, %s, %c, %s, %s, %s, %f, %f, %d, %d, %d, %d, %d, %d, %d, %c",
		ExchangeID, ProductID, (char)ProductClass, ProductName, CurrencyGroup, CurrencyID, VolumeMultiple, PriceTick, DecimalDigits, T1SplitTime, MaxPosition, MaxOrderVolume, CloseAdvanceDays, DeliveryMethod, TradeChannel, (char)TradeStatus);
	return t_DboDataStringBuffer;
}
const char* DboProduct::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboProduct:ExchangeID:[%s], ProductID:[%s], ProductClass:[%c], ProductName:[%s], CurrencyGroup:[%s], CurrencyID:[%s], VolumeMultiple:[%f], PriceTick:[%f], DecimalDigits:[%d], T1SplitTime:[%d], MaxPosition:[%d], MaxOrderVolume:[%d], CloseAdvanceDays:[%d], DeliveryMethod:[%d], TradeChannel:[%d], TradeStatus:[%c]",
		ExchangeID, ProductID, (char)ProductClass, ProductName, CurrencyGroup, CurrencyID, VolumeMultiple, PriceTick, DecimalDigits, T1SplitTime, MaxPosition, MaxOrderVolume, CloseAdvanceDays, DeliveryMethod, TradeChannel, (char)TradeStatus);
	return t_DboDataStringBuffer;
}
int DboProduct::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%s', '%s', '%c', '%s', '%s', '%s', '%f', '%f', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%c'",
		ExchangeID, ProductID, (char)ProductClass, ProductName, CurrencyGroup, CurrencyID, VolumeMultiple, PriceTick, DecimalDigits, T1SplitTime, MaxPosition, MaxOrderVolume, CloseAdvanceDays, DeliveryMethod, TradeChannel, (char)TradeStatus);
}
int DboProduct::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%s|%s|%c|%s|%s|%s|%f|%f|%d|%d|%d|%d|%d|%d|%d|%c|",
		ExchangeID, ProductID, (char)ProductClass, ProductName, CurrencyGroup, CurrencyID, VolumeMultiple, PriceTick, DecimalDigits, T1SplitTime, MaxPosition, MaxOrderVolume, CloseAdvanceDays, DeliveryMethod, TradeChannel, (char)TradeStatus);
}

const char* DboRiskControl::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%d, %d, %d",
		RiskID, Account, GroupID);
	return t_DboDataStringBuffer;
}
const char* DboRiskControl::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboRiskControl:RiskID:[%d], Account:[%d], GroupID:[%d]",
		RiskID, Account, GroupID);
	return t_DboDataStringBuffer;
}
int DboRiskControl::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%d', '%d', '%d'",
		RiskID, Account, GroupID);
}
int DboRiskControl::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%d|%d|%d|",
		RiskID, Account, GroupID);
}

const char* DboRiskControlTemplate::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%d, %d, %d, %s, %d, %d, %d, %d, %s, %s, %s, %f, %f, %f, %f, %f, %f, %s, %s",
		ID, RiskClass, RiskItem, Product, OpenDenied, CloseDenied, Notice, Position, Contents, TimeFrom, TimeTo, Param1, Param2, Param3, Param4, Param5, Param6, CreateTime, UpdateTime);
	return t_DboDataStringBuffer;
}
const char* DboRiskControlTemplate::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboRiskControlTemplate:ID:[%d], RiskClass:[%d], RiskItem:[%d], Product:[%s], OpenDenied:[%d], CloseDenied:[%d], Notice:[%d], Position:[%d], Contents:[%s], TimeFrom:[%s], TimeTo:[%s], Param1:[%f], Param2:[%f], Param3:[%f], Param4:[%f], Param5:[%f], Param6:[%f], CreateTime:[%s], UpdateTime:[%s]",
		ID, RiskClass, RiskItem, Product, OpenDenied, CloseDenied, Notice, Position, Contents, TimeFrom, TimeTo, Param1, Param2, Param3, Param4, Param5, Param6, CreateTime, UpdateTime);
	return t_DboDataStringBuffer;
}
int DboRiskControlTemplate::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%d', '%d', '%d', '%s', '%d', '%d', '%d', '%d', '%s', '%s', '%s', '%f', '%f', '%f', '%f', '%f', '%f', '%s', '%s'",
		ID, RiskClass, RiskItem, Product, OpenDenied, CloseDenied, Notice, Position, Contents, TimeFrom, TimeTo, Param1, Param2, Param3, Param4, Param5, Param6, CreateTime, UpdateTime);
}
int DboRiskControlTemplate::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%d|%d|%d|%s|%d|%d|%d|%d|%s|%s|%s|%f|%f|%f|%f|%f|%f|%s|%s|",
		ID, RiskClass, RiskItem, Product, OpenDenied, CloseDenied, Notice, Position, Contents, TimeFrom, TimeTo, Param1, Param2, Param3, Param4, Param5, Param6, CreateTime, UpdateTime);
}

const char* DboSession::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%d, %d, %d, %d",
		TradeDate, Account, FrontID, SessionID);
	return t_DboDataStringBuffer;
}
const char* DboSession::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboSession:TradeDate:[%d], Account:[%d], FrontID:[%d], SessionID:[%d]",
		TradeDate, Account, FrontID, SessionID);
	return t_DboDataStringBuffer;
}
int DboSession::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%d', '%d', '%d', '%d'",
		TradeDate, Account, FrontID, SessionID);
}
int DboSession::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%d|%d|%d|%d|",
		TradeDate, Account, FrontID, SessionID);
}

const char* DboSettlePrice::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%s, %s, %d, %f, %f, %f, %f",
		ExchangeID, ContractID, TradingDay, SettlePrice, PrevSettlePrice, Margin, MinMargin);
	return t_DboDataStringBuffer;
}
const char* DboSettlePrice::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboSettlePrice:ExchangeID:[%s], ContractID:[%s], TradingDay:[%d], SettlePrice:[%f], PrevSettlePrice:[%f], Margin:[%f], MinMargin:[%f]",
		ExchangeID, ContractID, TradingDay, SettlePrice, PrevSettlePrice, Margin, MinMargin);
	return t_DboDataStringBuffer;
}
int DboSettlePrice::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%s', '%s', '%d', '%f', '%f', '%f', '%f'",
		ExchangeID, ContractID, TradingDay, SettlePrice, PrevSettlePrice, Margin, MinMargin);
}
int DboSettlePrice::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%s|%s|%d|%f|%f|%f|%f|",
		ExchangeID, ContractID, TradingDay, SettlePrice, PrevSettlePrice, Margin, MinMargin);
}

const char* DboSummary::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%d, %d, %s, %s, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %d",
		Account, TradeDate, CurrencyGroup, CurrencyID, CurrencyRate, Balance, LastBalance, Available, Margin, Commission, PositionProfitFloat, PositionProfit, CloseProfitFloat, CloseProfit, Deposit, Withdraw, Risk, CloseBalance, CashInOutTotal, BalanceFloat, LastBalanceFloat, Credit, MinMargin, Stage);
	return t_DboDataStringBuffer;
}
const char* DboSummary::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboSummary:Account:[%d], TradeDate:[%d], CurrencyGroup:[%s], CurrencyID:[%s], CurrencyRate:[%f], Balance:[%f], LastBalance:[%f], Available:[%f], Margin:[%f], Commission:[%f], PositionProfitFloat:[%f], PositionProfit:[%f], CloseProfitFloat:[%f], CloseProfit:[%f], Deposit:[%f], Withdraw:[%f], Risk:[%f], CloseBalance:[%f], CashInOutTotal:[%f], BalanceFloat:[%f], LastBalanceFloat:[%f], Credit:[%f], MinMargin:[%f], Stage:[%d]",
		Account, TradeDate, CurrencyGroup, CurrencyID, CurrencyRate, Balance, LastBalance, Available, Margin, Commission, PositionProfitFloat, PositionProfit, CloseProfitFloat, CloseProfit, Deposit, Withdraw, Risk, CloseBalance, CashInOutTotal, BalanceFloat, LastBalanceFloat, Credit, MinMargin, Stage);
	return t_DboDataStringBuffer;
}
int DboSummary::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%d', '%d', '%s', '%s', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%f', '%d'",
		Account, TradeDate, CurrencyGroup, CurrencyID, CurrencyRate, Balance, LastBalance, Available, Margin, Commission, PositionProfitFloat, PositionProfit, CloseProfitFloat, CloseProfit, Deposit, Withdraw, Risk, CloseBalance, CashInOutTotal, BalanceFloat, LastBalanceFloat, Credit, MinMargin, Stage);
}
int DboSummary::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%d|%d|%s|%s|%f|%f|%f|%f|%f|%f|%f|%f|%f|%f|%f|%f|%f|%f|%f|%f|%f|%f|%f|%d|",
		Account, TradeDate, CurrencyGroup, CurrencyID, CurrencyRate, Balance, LastBalance, Available, Margin, Commission, PositionProfitFloat, PositionProfit, CloseProfitFloat, CloseProfit, Deposit, Withdraw, Risk, CloseBalance, CashInOutTotal, BalanceFloat, LastBalanceFloat, Credit, MinMargin, Stage);
}

const char* DboSysParam::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%s, %s",
		Param, Value);
	return t_DboDataStringBuffer;
}
const char* DboSysParam::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboSysParam:Param:[%s], Value:[%s]",
		Param, Value);
	return t_DboDataStringBuffer;
}
int DboSysParam::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%s', '%s'",
		Param, Value);
}
int DboSysParam::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%s|%s|",
		Param, Value);
}

const char* DboTrade::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%d, %d, %s, %d, %d, %s, %s, %c, %c, %f, %d, %f, %d, %s, %s, %f, %f, %f, %s, %c, %s, %d, %s, %s, %s, %d, %d, %d, %s, %s, %f, %f, %s",
		TradeDate, OrderID, TradeID, Account, RequestID, ExchangeID, ContractID, (char)BS, (char)Hedge, Price, Volume, Turnover, RealDate, TradeTime, OrderSysID, Commission, CloseProfitFloat, CloseProfit, OrderRef, (char)TradeType, CombineContractID, LocalTradeID, CurrencyGroup, CurrencyID, ExchangeTradeID, OpenVolume, CloseVolume, Deleted, DeleteAccount, DeleteTime, ExchangeFee, Levy, PatchAccount);
	return t_DboDataStringBuffer;
}
const char* DboTrade::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboTrade:TradeDate:[%d], OrderID:[%d], TradeID:[%s], Account:[%d], RequestID:[%d], ExchangeID:[%s], ContractID:[%s], BS:[%c], Hedge:[%c], Price:[%f], Volume:[%d], Turnover:[%f], RealDate:[%d], TradeTime:[%s], OrderSysID:[%s], Commission:[%f], CloseProfitFloat:[%f], CloseProfit:[%f], OrderRef:[%s], TradeType:[%c], CombineContractID:[%s], LocalTradeID:[%d], CurrencyGroup:[%s], CurrencyID:[%s], ExchangeTradeID:[%s], OpenVolume:[%d], CloseVolume:[%d], Deleted:[%d], DeleteAccount:[%s], DeleteTime:[%s], ExchangeFee:[%f], Levy:[%f], PatchAccount:[%s]",
		TradeDate, OrderID, TradeID, Account, RequestID, ExchangeID, ContractID, (char)BS, (char)Hedge, Price, Volume, Turnover, RealDate, TradeTime, OrderSysID, Commission, CloseProfitFloat, CloseProfit, OrderRef, (char)TradeType, CombineContractID, LocalTradeID, CurrencyGroup, CurrencyID, ExchangeTradeID, OpenVolume, CloseVolume, Deleted, DeleteAccount, DeleteTime, ExchangeFee, Levy, PatchAccount);
	return t_DboDataStringBuffer;
}
int DboTrade::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%d', '%d', '%s', '%d', '%d', '%s', '%s', '%c', '%c', '%f', '%d', '%f', '%d', '%s', '%s', '%f', '%f', '%f', '%s', '%c', '%s', '%d', '%s', '%s', '%s', '%d', '%d', '%d', '%s', '%s', '%f', '%f', '%s'",
		TradeDate, OrderID, TradeID, Account, RequestID, ExchangeID, ContractID, (char)BS, (char)Hedge, Price, Volume, Turnover, RealDate, TradeTime, OrderSysID, Commission, CloseProfitFloat, CloseProfit, OrderRef, (char)TradeType, CombineContractID, LocalTradeID, CurrencyGroup, CurrencyID, ExchangeTradeID, OpenVolume, CloseVolume, Deleted, DeleteAccount, DeleteTime, ExchangeFee, Levy, PatchAccount);
}
int DboTrade::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%d|%d|%s|%d|%d|%s|%s|%c|%c|%f|%d|%f|%d|%s|%s|%f|%f|%f|%s|%c|%s|%d|%s|%s|%s|%d|%d|%d|%s|%s|%f|%f|%s|",
		TradeDate, OrderID, TradeID, Account, RequestID, ExchangeID, ContractID, (char)BS, (char)Hedge, Price, Volume, Turnover, RealDate, TradeTime, OrderSysID, Commission, CloseProfitFloat, CloseProfit, OrderRef, (char)TradeType, CombineContractID, LocalTradeID, CurrencyGroup, CurrencyID, ExchangeTradeID, OpenVolume, CloseVolume, Deleted, DeleteAccount, DeleteTime, ExchangeFee, Levy, PatchAccount);
}

const char* DboTradeChannel::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%d, %s, %s, %s, %s, %d",
		ID, Name, UserID, Password, IP, Port);
	return t_DboDataStringBuffer;
}
const char* DboTradeChannel::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboTradeChannel:ID:[%d], Name:[%s], UserID:[%s], Password:[%s], IP:[%s], Port:[%d]",
		ID, Name, UserID, Password, IP, Port);
	return t_DboDataStringBuffer;
}
int DboTradeChannel::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%d', '%s', '%s', '%s', '%s', '%d'",
		ID, Name, UserID, Password, IP, Port);
}
int DboTradeChannel::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%d|%s|%s|%s|%s|%d|",
		ID, Name, UserID, Password, IP, Port);
}

const char* DboTrustedNetAddress::GetString() const
{
	sprintf(t_DboDataStringBuffer, "%d, %s, %s",
		Account, IP, MAC);
	return t_DboDataStringBuffer;
}
const char* DboTrustedNetAddress::GetDebugString() const
{
	sprintf(t_DboDataStringBuffer, "DboTrustedNetAddress:Account:[%d], IP:[%s], MAC:[%s]",
		Account, IP, MAC);
	return t_DboDataStringBuffer;
}
int DboTrustedNetAddress::ToStream(char* buff, int size) const
{
	return snprintf(buff, size, "'%d', '%s', '%s'",
		Account, IP, MAC);
}
int DboTrustedNetAddress::ToProtocolStream(char* buff, int size) const
{
	return snprintf(buff, size, "%d|%s|%s|",
		Account, IP, MAC);
}

