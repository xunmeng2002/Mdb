#pragma once
#include "Types.h"

class DboAuthorizedSoftware
{
public:
	SoftwareType Software;
	AuthCodeType AuthCode;
	AccountType Account;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};
class DboClose
{
public:
	DateType OpenDate;
	OrderIDType OpenOrderID;
	TradeIDType OpenTradeID;
	DateType CloseDate;
	OrderIDType CloseOrderID;
	TradeIDType CloseTradeID;
	AccountType Account;
	ExchangeIDType ExchangeID;
	ContractIDType ContractID;
	DirectionType BS;
	HedgeFlagType Hedge;
	PriceType OpenPrice;
	PriceType LastSettle;
	PriceType ClosePrice;
	VolumeType Volume;
	DateType RealOpenDate;
	DateType RealCloseDate;
	TimeType OpenTime;
	TimeType CloseTime;
	MoneyType CloseProfitFloat;
	MoneyType CloseProfit;
	CurrencyGroupType CurrencyGroup;
	CurrencyIDType CurrencyID;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};
class DboCommission
{
public:
	GroupIDType GroupID;
	AccountType Account;
	ExchangeIDType ExchangeID;
	ProductIDType ProductID;
	ContractIDType ContractID;
	RateType OpenByVolume;
	RateType CloseByVolume;
	RateType OpenByMoney;
	RateType CloseByMoney;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};
class DboContract
{
public:
	ExchangeIDType ExchangeID;
	ProductIDType ProductID;
	ContractIDType ContractID;
	ContractNameType ContractName;
	YearType DeliveryYear;
	MonthType DeliveryMonth;
	DateType OpenDate;
	DateType ExpireDate;
	DateType FirstNoticeDate;
	DateType StartDelivDate;
	DateType EndDelivDate;
	TradeStatusType TradeStatus;
	MoneyType Margin;
	MoneyType MinMargin;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};
class DboCreditAmount
{
public:
	IDType ID;
	DateTimeType AuditTime;
	DateTimeType OperationTime;
	RemarkType Remark;
	StatusType Status;
	MoneyType Amount;
	CurrencyGroupType CurrencyGroup;
	DirectionType Direction;
	DateType TradingDay;
	OperatorIDType OperatorID;
	AccountType Account;
	CurrencyIDType CurrencyID;
	AuditorIDType AuditorID;
	FlagType Flag;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};
class DboCurrencyRate
{
public:
	CurrencyGroupType CurrencyGroup;
	CurrencyIDType CurrencyID;
	RateType TradeRate;
	RateType SettlementRate;
	BoolType IsBaseCurrency;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};
class DboDepositWithdrawal
{
public:
	IDType ID;
	TimeType AuditTime;
	TimeType OperationTime;
	RemarkType Remark;
	StatusType Status;
	MoneyType Amount;
	RemarkType BankCardID;
	CurrencyIDType CurrencyID;
	CurrencyGroupType CurrencyGroup;
	DirectionType Direction;
	MethodType Method;
	DateType TradingDay;
	OperatorIDType OperatorID;
	AccountType Account;
	AuditorIDType AuditorID;
	FlagType Flag;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};
class DboExchange
{
public:
	ExchangeIDType ExchangeID;
	ExchangeNameType ExchangeName;
	TradeStatusType TradeStatus;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};
class DboInvertor
{
public:
	AccountType Account;
	AccountStatusType AccountStatus;
	AccountTypeType AccountType;
	AgentTypeType AgentType;
	DateType CloseDate;
	OrderTypeType OrderType;
	InvestorTypeType InvestorType;
	BoolType IsEmployee;
	LoginStatusType LoginStatus;
	NumType maxOnLine;
	InvestorNameType Name;
	DateType OpenDate;
	PasswordType Password;
	DateType PasswordDate;
	BoolType PasswordExpired;
	DateType PasswordValidDays;
	NumType RiskLevel;
	TradeStatusType TradeStatus;
	TwoStepCodeType TwoStepCode;
	TwoStepCodeTimeType TwoStepCodeTime;
	BoolType TwoStepLogin;
	GroupIDType CommissionGroupID;
	GroupIDType MarginGroupID;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};
class DboInvestorMobile
{
public:
	IDType ID;
	MobileType Mobile;
	MobileTypeType MobileType;
	AccountType Account;
	AreaCodeType AreaCode;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};
class DboInvestorParam
{
public:
	AccountType Account;
	ExchangeIDType ExchangeID;
	ProductIDType ProductID;
	ContractIDType ContractID;
	TradeStatusType TradeStatus;
	VolumeType MaxPosition;
	VolumeType MaxOrderVolume;
	NumType CloseAdvanceDays;
	ChannelIDType ChannelID;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};
class DboLogin
{
public:
	DateType TradeDate;
	FrontIDType FrontID;
	SessionIDType SessionID;
	DateType RealDate;
	AccountType Account;
	AdminType Admin;
	BoolType TwoStepLogin;
	TimeType Time;
	IPType IP;
	MACType MAC;
	UserProductType UserProduct;
	HardwareType Hardware;
	OSType OS;
	DateType LogoutDate;
	TimeType LogoutTime;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};
class DboMargin
{
public:
	GroupIDType GroupID;
	AccountType Account;
	ExchangeIDType ExchangeID;
	ProductIDType ProductID;
	ContractIDType ContractID;
	MarginTypeType MarginType;
	MoneyType Margin;
	MoneyType MinMargin;
	MoneyType SettleMargin;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};
class DboMaxBalance
{
public:
	AccountType Account;
	MoneyType MaxBalance;
	MoneyType EverMaxBalance;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};
class DboOrder
{
public:
	DateTimeType TradeDate;
	OrderIDType OrderID;
	AccountType Account;
	OperatorIDType InsertAccount;
	OperatorIDType CancelAccount;
	RequestIDType RequestID;
	ExchangeIDType ExchangeID;
	ContractIDType ContractID;
	DirectionType BS;
	OffsetFlagType OC;
	HedgeFlagType Hedge;
	OrderPriceTypeType OrderPriceType;
	PriceType Price;
	VolumeType Volume;
	VolumeType TradeVolume;
	StatusType Status;
	DateType RealDate;
	TimeType InsertTime;
	TimeType CancelTime;
	ForceCloseReasonType ForceClose;
	ForceReasonType ForceReason;
	OrderSysIDType OrderSysID;
	OrderRemarkType Remark;
	FrontIDType FrontID;
	SessionIDType SessionID;
	OrderRefType OrderRef;
	TimeConditionType TimeCondition;
	DateType GTDDate;
	VolumeConditionType VolumeCondition;
	VolumeType MinVolume;
	ContingentConditionType ContingentCondition;
	PriceType StopPrice;
	SwapOrderType SwapOrder;
	ChannelIDType ChannelID;
	Tag50Type Tag50;
	FrontIDType CancelFrontID;
	SessionIDType CancelSessionID;
	FrontIDType ChannelFrontID;
	SessionIDType ChannelSessionID;
	OrderRefType ChannelOrderRef;
	ChannelOrderIDType ChannelOrderID;
	T1ValidType T1Valid;
	BoolType Deleted;
	OperatorIDType DeleteAccount;
	TimeType DeleteTime;
	DateType InsertDate;
	InsertIDType InsertID;
	PatchOrderType PatchOrder;
	DateType CancelDate;
	UserOrderIDType UserOrderID;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};
class DboOrderCheck
{
public:
	IDType ID;
	AccountType Account;
	GroupIDType GroupID;
	ItemType Item;
	ParamType Param1;
	ProductIDType ProductID;
	ResultType Result;
	ExchangeIDType ExchangeID;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};
class DboPosition
{
public:
	AccountType Account;
	DateType TradeDate;
	ExchangeIDType ExchangeID;
	ContractIDType ContractID;
	DirectionType BS;
	HedgeFlagType Hedge;
	DateType OpenDate;
	TradeIDType TradeID;
	OrderIDType OrderID;
	LocalTradeIDType LocalTradeID;
	DateType RealOpenDate;
	TimeType TradeTime;
	VolumeType Volume;
	PriceType OpenPrice;
	PriceType SettlePrice;
	PriceType LastSettle;
	VolumeMultipleType VolumeMultiple;
	MoneyType Margin;
	MoneyType PositionProfitFloat;
	MoneyType PositionProfit;
	VolumeType CloseVolume;
	MoneyType CloseProfitFloat;
	MoneyType CloseProfit;
	MoneyType CloseAmount;
	TradeTypeType TradeType;
	ContractIDType CombineContractID;
	TradeIDType CombineTradeID;
	CurrencyGroupType CurrencyGroup;
	CurrencyIDType CurrencyID;
	MoneyType MinMargin;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};
class DboProduct
{
public:
	ExchangeIDType ExchangeID;
	ProductIDType ProductID;
	ProductClassType ProductClass;
	ProductNameType ProductName;
	CurrencyGroupType CurrencyGroup;
	CurrencyIDType CurrencyID;
	VolumeMultipleType VolumeMultiple;
	PriceTickType PriceTick;
	DecimalDigitsType DecimalDigits;
	T1SplitTimeType T1SplitTime;
	VolumeType MaxPosition;
	VolumeType MaxOrderVolume;
	NumType CloseAdvanceDays;
	DeliveryMethodType DeliveryMethod;
	ChannelIDType TradeChannel;
	TradeStatusType TradeStatus;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};
class DboRiskControl
{
public:
	RiskIDType RiskID;
	AccountType Account;
	GroupIDType GroupID;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};
class DboRiskControlTemplate
{
public:
	IDType ID;
	RiskClassType RiskClass;
	RiskItemType RiskItem;
	ProductCombinationType Product;
	BoolType OpenDenied;
	BoolType CloseDenied;
	NoticeType Notice;
	VolumeType Position;
	ContentsType Contents;
	TimeType TimeFrom;
	TimeType TimeTo;
	ParamType Param1;
	ParamType Param2;
	ParamType Param3;
	ParamType Param4;
	ParamType Param5;
	ParamType Param6;
	TimeType CreateTime;
	TimeType UpdateTime;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};
class DboSession
{
public:
	DateType TradeDate;
	AccountType Account;
	FrontIDType FrontID;
	SessionIDType SessionID;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};
class DboSettlePrice
{
public:
	ExchangeIDType ExchangeID;
	ContractIDType ContractID;
	DateType TradingDay;
	PriceType SettlePrice;
	PriceType PrevSettlePrice;
	MoneyType Margin;
	MoneyType MinMargin;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};
class DboSummary
{
public:
	AccountType Account;
	DateType TradeDate;
	CurrencyGroupType CurrencyGroup;
	CurrencyIDType CurrencyID;
	RateType CurrencyRate;
	MoneyType Balance;
	MoneyType LastBalance;
	MoneyType Available;
	MoneyType Margin;
	MoneyType Commission;
	MoneyType PositionProfitFloat;
	MoneyType PositionProfit;
	MoneyType CloseProfitFloat;
	MoneyType CloseProfit;
	MoneyType Deposit;
	MoneyType Withdraw;
	RateType Risk;
	MoneyType CloseBalance;
	MoneyType CashInOutTotal;
	MoneyType BalanceFloat;
	MoneyType LastBalanceFloat;
	MoneyType Credit;
	MoneyType MinMargin;
	StageType Stage;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};
class DboSysParam
{
public:
	SysParamType Param;
	SysParamValueType Value;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};
class DboTrade
{
public:
	DateType TradeDate;
	OrderIDType OrderID;
	TradeIDType TradeID;
	AccountType Account;
	RequestIDType RequestID;
	ExchangeIDType ExchangeID;
	ContractIDType ContractID;
	DirectionType BS;
	HedgeFlagType Hedge;
	PriceType Price;
	VolumeType Volume;
	MoneyType Turnover;
	DateType RealDate;
	TimeType TradeTime;
	OrderSysIDType OrderSysID;
	MoneyType Commission;
	MoneyType CloseProfitFloat;
	MoneyType CloseProfit;
	OrderRefType OrderRef;
	TradeTypeType TradeType;
	ContractIDType CombineContractID;
	LocalTradeIDType LocalTradeID;
	CurrencyGroupType CurrencyGroup;
	CurrencyIDType CurrencyID;
	ExchangeTradeIDType ExchangeTradeID;
	VolumeType OpenVolume;
	VolumeType CloseVolume;
	BoolType Deleted;
	OperatorIDType DeleteAccount;
	TimeType DeleteTime;
	MoneyType ExchangeFee;
	MoneyType Levy;
	OperatorIDType PatchAccount;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};
class DboTradeChannel
{
public:
	IDType ID;
	ChannelNameType Name;
	UserIDType UserID;
	PasswordType Password;
	IPType IP;
	PortType Port;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};
class DboTrustedNetAddress
{
public:
	AccountType Account;
	IPType IP;
	MACType MAC;
	
	const char* GetString() const;
	const char* GetDebugString() const;
	int ToStream(char* buff, int size) const;
	int ToProtocolStream(char* buff, int size) const;
};

