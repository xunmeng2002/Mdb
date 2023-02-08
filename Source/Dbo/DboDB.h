#pragma once
#include "DboStructs.h"
#include <jdbc/mysql_connection.h>
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_error.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset.h>
#include <string>
#include <vector>


class DboDB
{
public:
	DboDB(const std::string& host, const std::string& user, const std::string& passwd);
	~DboDB();
	bool InsertDboAuthorizedSoftware(DboAuthorizedSoftware* record);
	bool TruncateDboAuthorizedSoftware();
	bool DeleteDboAuthorizedSoftware(DboAuthorizedSoftware* record);
	bool UpdateDboAuthorizedSoftware(DboAuthorizedSoftware* record);
	DboAuthorizedSoftware* QueryDboAuthorizedSoftwareFromPrimaryKey(const AccountType& Account, const SoftwareType& Software);
	DboAuthorizedSoftware* QueryDboAuthorizedSoftwareFromUniqueKeyAuthCode(const SoftwareType& Software, const AuthCodeType& AuthCode);
	void QueryDboAuthorizedSoftwareFromIndexAccount(std::vector<DboAuthorizedSoftware*>& records, const AuthCodeType& AuthCode, const AccountType& Account);
	void SetStatementForDboAuthorizedSoftwareRecord(sql::PreparedStatement* statement, DboAuthorizedSoftware* record);
	void SetStatementForDboAuthorizedSoftwarePrimaryKey(sql::PreparedStatement* statement, const AccountType& Account, const SoftwareType& Software);
	void SetStatementForDboAuthorizedSoftwareUniqueKeyAuthCode(sql::PreparedStatement* statement, const SoftwareType& Software, const AuthCodeType& AuthCode);
	void SetStatementForDboAuthorizedSoftwareIndexAccount(sql::PreparedStatement* statement, const AuthCodeType& AuthCode, const AccountType& Account);
	DboAuthorizedSoftware* GetDboAuthorizedSoftwareRecord(sql::ResultSet* result);

	bool InsertDboClose(DboClose* record);
	bool TruncateDboClose();
	bool DeleteDboClose(DboClose* record);
	bool UpdateDboClose(DboClose* record);
	DboClose* QueryDboCloseFromPrimaryKey(const DateType& OpenDate, const OrderIDType& OpenOrderID, const TradeIDType& OpenTradeID, const DateType& CloseDate, const OrderIDType& CloseOrderID, const TradeIDType& CloseTradeID);
	void SetStatementForDboCloseRecord(sql::PreparedStatement* statement, DboClose* record);
	void SetStatementForDboClosePrimaryKey(sql::PreparedStatement* statement, const DateType& OpenDate, const OrderIDType& OpenOrderID, const TradeIDType& OpenTradeID, const DateType& CloseDate, const OrderIDType& CloseOrderID, const TradeIDType& CloseTradeID);
	DboClose* GetDboCloseRecord(sql::ResultSet* result);

	bool InsertDboCommission(DboCommission* record);
	bool TruncateDboCommission();
	bool DeleteDboCommission(DboCommission* record);
	bool UpdateDboCommission(DboCommission* record);
	DboCommission* QueryDboCommissionFromPrimaryKey(const GroupIDType& GroupID, const AccountType& Account, const ExchangeIDType& ExchangeID, const ProductIDType& ProductID, const ContractIDType& ContractID);
	void SetStatementForDboCommissionRecord(sql::PreparedStatement* statement, DboCommission* record);
	void SetStatementForDboCommissionPrimaryKey(sql::PreparedStatement* statement, const GroupIDType& GroupID, const AccountType& Account, const ExchangeIDType& ExchangeID, const ProductIDType& ProductID, const ContractIDType& ContractID);
	DboCommission* GetDboCommissionRecord(sql::ResultSet* result);

	bool InsertDboContract(DboContract* record);
	bool TruncateDboContract();
	bool DeleteDboContract(DboContract* record);
	bool UpdateDboContract(DboContract* record);
	DboContract* QueryDboContractFromPrimaryKey(const ExchangeIDType& ExchangeID, const ContractIDType& ContractID);
	void SetStatementForDboContractRecord(sql::PreparedStatement* statement, DboContract* record);
	void SetStatementForDboContractPrimaryKey(sql::PreparedStatement* statement, const ExchangeIDType& ExchangeID, const ContractIDType& ContractID);
	DboContract* GetDboContractRecord(sql::ResultSet* result);

	bool InsertDboCreditAmount(DboCreditAmount* record);
	bool TruncateDboCreditAmount();
	bool DeleteDboCreditAmount(DboCreditAmount* record);
	bool UpdateDboCreditAmount(DboCreditAmount* record);
	DboCreditAmount* QueryDboCreditAmountFromPrimaryKey(const IDType& ID);
	void SetStatementForDboCreditAmountRecord(sql::PreparedStatement* statement, DboCreditAmount* record);
	void SetStatementForDboCreditAmountPrimaryKey(sql::PreparedStatement* statement, const IDType& ID);
	DboCreditAmount* GetDboCreditAmountRecord(sql::ResultSet* result);

	bool InsertDboCurrencyRate(DboCurrencyRate* record);
	bool TruncateDboCurrencyRate();
	bool DeleteDboCurrencyRate(DboCurrencyRate* record);
	bool UpdateDboCurrencyRate(DboCurrencyRate* record);
	DboCurrencyRate* QueryDboCurrencyRateFromPrimaryKey(const CurrencyGroupType& CurrencyGroup, const CurrencyIDType& CurrencyID);
	void SetStatementForDboCurrencyRateRecord(sql::PreparedStatement* statement, DboCurrencyRate* record);
	void SetStatementForDboCurrencyRatePrimaryKey(sql::PreparedStatement* statement, const CurrencyGroupType& CurrencyGroup, const CurrencyIDType& CurrencyID);
	DboCurrencyRate* GetDboCurrencyRateRecord(sql::ResultSet* result);

	bool InsertDboDepositWithdrawal(DboDepositWithdrawal* record);
	bool TruncateDboDepositWithdrawal();
	bool DeleteDboDepositWithdrawal(DboDepositWithdrawal* record);
	bool UpdateDboDepositWithdrawal(DboDepositWithdrawal* record);
	DboDepositWithdrawal* QueryDboDepositWithdrawalFromPrimaryKey(const IDType& ID);
	void SetStatementForDboDepositWithdrawalRecord(sql::PreparedStatement* statement, DboDepositWithdrawal* record);
	void SetStatementForDboDepositWithdrawalPrimaryKey(sql::PreparedStatement* statement, const IDType& ID);
	DboDepositWithdrawal* GetDboDepositWithdrawalRecord(sql::ResultSet* result);

	bool InsertDboExchange(DboExchange* record);
	bool TruncateDboExchange();
	bool DeleteDboExchange(DboExchange* record);
	bool UpdateDboExchange(DboExchange* record);
	DboExchange* QueryDboExchangeFromPrimaryKey(const ExchangeIDType& ExchangeID);
	void SetStatementForDboExchangeRecord(sql::PreparedStatement* statement, DboExchange* record);
	void SetStatementForDboExchangePrimaryKey(sql::PreparedStatement* statement, const ExchangeIDType& ExchangeID);
	DboExchange* GetDboExchangeRecord(sql::ResultSet* result);

	bool InsertDboInvertor(DboInvertor* record);
	bool TruncateDboInvertor();
	bool DeleteDboInvertor(DboInvertor* record);
	bool UpdateDboInvertor(DboInvertor* record);
	DboInvertor* QueryDboInvertorFromPrimaryKey(const AccountType& Account);
	void SetStatementForDboInvertorRecord(sql::PreparedStatement* statement, DboInvertor* record);
	void SetStatementForDboInvertorPrimaryKey(sql::PreparedStatement* statement, const AccountType& Account);
	DboInvertor* GetDboInvertorRecord(sql::ResultSet* result);

	bool InsertDboInvestorMobile(DboInvestorMobile* record);
	bool TruncateDboInvestorMobile();
	bool DeleteDboInvestorMobile(DboInvestorMobile* record);
	bool UpdateDboInvestorMobile(DboInvestorMobile* record);
	DboInvestorMobile* QueryDboInvestorMobileFromPrimaryKey(const IDType& ID);
	void SetStatementForDboInvestorMobileRecord(sql::PreparedStatement* statement, DboInvestorMobile* record);
	void SetStatementForDboInvestorMobilePrimaryKey(sql::PreparedStatement* statement, const IDType& ID);
	DboInvestorMobile* GetDboInvestorMobileRecord(sql::ResultSet* result);

	bool InsertDboInvestorParam(DboInvestorParam* record);
	bool TruncateDboInvestorParam();
	bool DeleteDboInvestorParam(DboInvestorParam* record);
	bool UpdateDboInvestorParam(DboInvestorParam* record);
	DboInvestorParam* QueryDboInvestorParamFromPrimaryKey(const AccountType& Account, const ExchangeIDType& ExchangeID, const ProductIDType& ProductID, const ContractIDType& ContractID);
	void SetStatementForDboInvestorParamRecord(sql::PreparedStatement* statement, DboInvestorParam* record);
	void SetStatementForDboInvestorParamPrimaryKey(sql::PreparedStatement* statement, const AccountType& Account, const ExchangeIDType& ExchangeID, const ProductIDType& ProductID, const ContractIDType& ContractID);
	DboInvestorParam* GetDboInvestorParamRecord(sql::ResultSet* result);

	bool InsertDboLogin(DboLogin* record);
	bool TruncateDboLogin();
	bool DeleteDboLogin(DboLogin* record);
	bool UpdateDboLogin(DboLogin* record);
	DboLogin* QueryDboLoginFromPrimaryKey(const DateType& TradeDate, const FrontIDType& FrontID, const SessionIDType& SessionID);
	void SetStatementForDboLoginRecord(sql::PreparedStatement* statement, DboLogin* record);
	void SetStatementForDboLoginPrimaryKey(sql::PreparedStatement* statement, const DateType& TradeDate, const FrontIDType& FrontID, const SessionIDType& SessionID);
	DboLogin* GetDboLoginRecord(sql::ResultSet* result);

	bool InsertDboMargin(DboMargin* record);
	bool TruncateDboMargin();
	bool DeleteDboMargin(DboMargin* record);
	bool UpdateDboMargin(DboMargin* record);
	DboMargin* QueryDboMarginFromPrimaryKey(const GroupIDType& GroupID, const AccountType& Account, const ExchangeIDType& ExchangeID, const ProductIDType& ProductID, const ContractIDType& ContractID);
	void SetStatementForDboMarginRecord(sql::PreparedStatement* statement, DboMargin* record);
	void SetStatementForDboMarginPrimaryKey(sql::PreparedStatement* statement, const GroupIDType& GroupID, const AccountType& Account, const ExchangeIDType& ExchangeID, const ProductIDType& ProductID, const ContractIDType& ContractID);
	DboMargin* GetDboMarginRecord(sql::ResultSet* result);

	bool InsertDboMaxBalance(DboMaxBalance* record);
	bool TruncateDboMaxBalance();
	bool DeleteDboMaxBalance(DboMaxBalance* record);
	bool UpdateDboMaxBalance(DboMaxBalance* record);
	DboMaxBalance* QueryDboMaxBalanceFromPrimaryKey(const AccountType& Account);
	void SetStatementForDboMaxBalanceRecord(sql::PreparedStatement* statement, DboMaxBalance* record);
	void SetStatementForDboMaxBalancePrimaryKey(sql::PreparedStatement* statement, const AccountType& Account);
	DboMaxBalance* GetDboMaxBalanceRecord(sql::ResultSet* result);

	bool InsertDboOrder(DboOrder* record);
	bool TruncateDboOrder();
	bool DeleteDboOrder(DboOrder* record);
	bool UpdateDboOrder(DboOrder* record);
	DboOrder* QueryDboOrderFromPrimaryKey(const DateTimeType& TradeDate, const OrderIDType& OrderID);
	void SetStatementForDboOrderRecord(sql::PreparedStatement* statement, DboOrder* record);
	void SetStatementForDboOrderPrimaryKey(sql::PreparedStatement* statement, const DateTimeType& TradeDate, const OrderIDType& OrderID);
	DboOrder* GetDboOrderRecord(sql::ResultSet* result);

	bool InsertDboOrderCheck(DboOrderCheck* record);
	bool TruncateDboOrderCheck();
	bool DeleteDboOrderCheck(DboOrderCheck* record);
	bool UpdateDboOrderCheck(DboOrderCheck* record);
	DboOrderCheck* QueryDboOrderCheckFromPrimaryKey(const IDType& ID);
	void SetStatementForDboOrderCheckRecord(sql::PreparedStatement* statement, DboOrderCheck* record);
	void SetStatementForDboOrderCheckPrimaryKey(sql::PreparedStatement* statement, const IDType& ID);
	DboOrderCheck* GetDboOrderCheckRecord(sql::ResultSet* result);

	bool InsertDboPosition(DboPosition* record);
	bool TruncateDboPosition();
	bool DeleteDboPosition(DboPosition* record);
	bool UpdateDboPosition(DboPosition* record);
	DboPosition* QueryDboPositionFromPrimaryKey(const AccountType& Account, const DateType& TradeDate, const DateType& OpenDate, const TradeIDType& TradeID, const OrderIDType& OrderID, const TradeIDType& CombineTradeID);
	void SetStatementForDboPositionRecord(sql::PreparedStatement* statement, DboPosition* record);
	void SetStatementForDboPositionPrimaryKey(sql::PreparedStatement* statement, const AccountType& Account, const DateType& TradeDate, const DateType& OpenDate, const TradeIDType& TradeID, const OrderIDType& OrderID, const TradeIDType& CombineTradeID);
	DboPosition* GetDboPositionRecord(sql::ResultSet* result);

	bool InsertDboProduct(DboProduct* record);
	bool TruncateDboProduct();
	bool DeleteDboProduct(DboProduct* record);
	bool UpdateDboProduct(DboProduct* record);
	DboProduct* QueryDboProductFromPrimaryKey(const ExchangeIDType& ExchangeID, const ProductIDType& ProductID);
	void SetStatementForDboProductRecord(sql::PreparedStatement* statement, DboProduct* record);
	void SetStatementForDboProductPrimaryKey(sql::PreparedStatement* statement, const ExchangeIDType& ExchangeID, const ProductIDType& ProductID);
	DboProduct* GetDboProductRecord(sql::ResultSet* result);

	bool InsertDboRiskControl(DboRiskControl* record);
	bool TruncateDboRiskControl();
	bool DeleteDboRiskControl(DboRiskControl* record);
	bool UpdateDboRiskControl(DboRiskControl* record);
	DboRiskControl* QueryDboRiskControlFromPrimaryKey(const RiskIDType& RiskID, const AccountType& Account, const GroupIDType& GroupID);
	void SetStatementForDboRiskControlRecord(sql::PreparedStatement* statement, DboRiskControl* record);
	void SetStatementForDboRiskControlPrimaryKey(sql::PreparedStatement* statement, const RiskIDType& RiskID, const AccountType& Account, const GroupIDType& GroupID);
	DboRiskControl* GetDboRiskControlRecord(sql::ResultSet* result);

	bool InsertDboRiskControlTemplate(DboRiskControlTemplate* record);
	bool TruncateDboRiskControlTemplate();
	bool DeleteDboRiskControlTemplate(DboRiskControlTemplate* record);
	bool UpdateDboRiskControlTemplate(DboRiskControlTemplate* record);
	DboRiskControlTemplate* QueryDboRiskControlTemplateFromPrimaryKey(const IDType& ID);
	void SetStatementForDboRiskControlTemplateRecord(sql::PreparedStatement* statement, DboRiskControlTemplate* record);
	void SetStatementForDboRiskControlTemplatePrimaryKey(sql::PreparedStatement* statement, const IDType& ID);
	DboRiskControlTemplate* GetDboRiskControlTemplateRecord(sql::ResultSet* result);

	bool InsertDboSession(DboSession* record);
	bool TruncateDboSession();
	bool DeleteDboSession(DboSession* record);
	bool UpdateDboSession(DboSession* record);
	DboSession* QueryDboSessionFromPrimaryKey(const DateType& TradeDate, const AccountType& Account, const FrontIDType& FrontID, const SessionIDType& SessionID);
	void SetStatementForDboSessionRecord(sql::PreparedStatement* statement, DboSession* record);
	void SetStatementForDboSessionPrimaryKey(sql::PreparedStatement* statement, const DateType& TradeDate, const AccountType& Account, const FrontIDType& FrontID, const SessionIDType& SessionID);
	DboSession* GetDboSessionRecord(sql::ResultSet* result);

	bool InsertDboSettlePrice(DboSettlePrice* record);
	bool TruncateDboSettlePrice();
	bool DeleteDboSettlePrice(DboSettlePrice* record);
	bool UpdateDboSettlePrice(DboSettlePrice* record);
	DboSettlePrice* QueryDboSettlePriceFromPrimaryKey(const ExchangeIDType& ExchangeID, const ContractIDType& ContractID, const DateType& TradingDay);
	void SetStatementForDboSettlePriceRecord(sql::PreparedStatement* statement, DboSettlePrice* record);
	void SetStatementForDboSettlePricePrimaryKey(sql::PreparedStatement* statement, const ExchangeIDType& ExchangeID, const ContractIDType& ContractID, const DateType& TradingDay);
	DboSettlePrice* GetDboSettlePriceRecord(sql::ResultSet* result);

	bool InsertDboSummary(DboSummary* record);
	bool TruncateDboSummary();
	bool DeleteDboSummary(DboSummary* record);
	bool UpdateDboSummary(DboSummary* record);
	DboSummary* QueryDboSummaryFromPrimaryKey(const AccountType& Account, const DateType& TradeDate, const CurrencyGroupType& CurrencyGroup, const CurrencyIDType& CurrencyID);
	void SetStatementForDboSummaryRecord(sql::PreparedStatement* statement, DboSummary* record);
	void SetStatementForDboSummaryPrimaryKey(sql::PreparedStatement* statement, const AccountType& Account, const DateType& TradeDate, const CurrencyGroupType& CurrencyGroup, const CurrencyIDType& CurrencyID);
	DboSummary* GetDboSummaryRecord(sql::ResultSet* result);

	bool InsertDboSysParam(DboSysParam* record);
	bool TruncateDboSysParam();
	bool DeleteDboSysParam(DboSysParam* record);
	bool UpdateDboSysParam(DboSysParam* record);
	DboSysParam* QueryDboSysParamFromPrimaryKey(const SysParamType& Param);
	void SetStatementForDboSysParamRecord(sql::PreparedStatement* statement, DboSysParam* record);
	void SetStatementForDboSysParamPrimaryKey(sql::PreparedStatement* statement, const SysParamType& Param);
	DboSysParam* GetDboSysParamRecord(sql::ResultSet* result);

	bool InsertDboTrade(DboTrade* record);
	bool TruncateDboTrade();
	bool DeleteDboTrade(DboTrade* record);
	bool UpdateDboTrade(DboTrade* record);
	DboTrade* QueryDboTradeFromPrimaryKey(const DateType& TradeDate, const OrderIDType& OrderID, const TradeIDType& TradeID);
	void SetStatementForDboTradeRecord(sql::PreparedStatement* statement, DboTrade* record);
	void SetStatementForDboTradePrimaryKey(sql::PreparedStatement* statement, const DateType& TradeDate, const OrderIDType& OrderID, const TradeIDType& TradeID);
	DboTrade* GetDboTradeRecord(sql::ResultSet* result);

	bool InsertDboTradeChannel(DboTradeChannel* record);
	bool TruncateDboTradeChannel();
	bool DeleteDboTradeChannel(DboTradeChannel* record);
	bool UpdateDboTradeChannel(DboTradeChannel* record);
	DboTradeChannel* QueryDboTradeChannelFromPrimaryKey(const IDType& ID);
	void SetStatementForDboTradeChannelRecord(sql::PreparedStatement* statement, DboTradeChannel* record);
	void SetStatementForDboTradeChannelPrimaryKey(sql::PreparedStatement* statement, const IDType& ID);
	DboTradeChannel* GetDboTradeChannelRecord(sql::ResultSet* result);

	bool InsertDboTrustedNetAddress(DboTrustedNetAddress* record);
	bool TruncateDboTrustedNetAddress();
	bool DeleteDboTrustedNetAddress(DboTrustedNetAddress* record);
	bool UpdateDboTrustedNetAddress(DboTrustedNetAddress* record);
	DboTrustedNetAddress* QueryDboTrustedNetAddressFromPrimaryKey(const AccountType& Account, const IPType& IP, const MACType& MAC);
	void SetStatementForDboTrustedNetAddressRecord(sql::PreparedStatement* statement, DboTrustedNetAddress* record);
	void SetStatementForDboTrustedNetAddressPrimaryKey(sql::PreparedStatement* statement, const AccountType& Account, const IPType& IP, const MACType& MAC);
	DboTrustedNetAddress* GetDboTrustedNetAddressRecord(sql::ResultSet* result);


private:
	sql::Driver* m_Driver;
	sql::Connection* m_DBConnection;

	std::string m_Host;
	std::string m_User;
	std::string m_Passwd;
	char* m_Sql;

	sql::PreparedStatement* m_DboAuthorizedSoftwareInsertStatement;
	sql::PreparedStatement* m_DboAuthorizedSoftwareTruncateStatement;
	sql::PreparedStatement* m_DboAuthorizedSoftwareDeleteStatement;
	sql::PreparedStatement* m_DboAuthorizedSoftwareUpdateStatement;
	sql::PreparedStatement* m_DboAuthorizedSoftwarePrimaryKeyStatement;
	sql::PreparedStatement* m_DboAuthorizedSoftwareUniqueKeyStatementAuthCode;
	sql::PreparedStatement* m_DboAuthorizedSoftwareIndexStatementAccount;
	sql::PreparedStatement* m_DboCloseInsertStatement;
	sql::PreparedStatement* m_DboCloseTruncateStatement;
	sql::PreparedStatement* m_DboCloseDeleteStatement;
	sql::PreparedStatement* m_DboCloseUpdateStatement;
	sql::PreparedStatement* m_DboClosePrimaryKeyStatement;
	sql::PreparedStatement* m_DboCommissionInsertStatement;
	sql::PreparedStatement* m_DboCommissionTruncateStatement;
	sql::PreparedStatement* m_DboCommissionDeleteStatement;
	sql::PreparedStatement* m_DboCommissionUpdateStatement;
	sql::PreparedStatement* m_DboCommissionPrimaryKeyStatement;
	sql::PreparedStatement* m_DboContractInsertStatement;
	sql::PreparedStatement* m_DboContractTruncateStatement;
	sql::PreparedStatement* m_DboContractDeleteStatement;
	sql::PreparedStatement* m_DboContractUpdateStatement;
	sql::PreparedStatement* m_DboContractPrimaryKeyStatement;
	sql::PreparedStatement* m_DboCreditAmountInsertStatement;
	sql::PreparedStatement* m_DboCreditAmountTruncateStatement;
	sql::PreparedStatement* m_DboCreditAmountDeleteStatement;
	sql::PreparedStatement* m_DboCreditAmountUpdateStatement;
	sql::PreparedStatement* m_DboCreditAmountPrimaryKeyStatement;
	sql::PreparedStatement* m_DboCurrencyRateInsertStatement;
	sql::PreparedStatement* m_DboCurrencyRateTruncateStatement;
	sql::PreparedStatement* m_DboCurrencyRateDeleteStatement;
	sql::PreparedStatement* m_DboCurrencyRateUpdateStatement;
	sql::PreparedStatement* m_DboCurrencyRatePrimaryKeyStatement;
	sql::PreparedStatement* m_DboDepositWithdrawalInsertStatement;
	sql::PreparedStatement* m_DboDepositWithdrawalTruncateStatement;
	sql::PreparedStatement* m_DboDepositWithdrawalDeleteStatement;
	sql::PreparedStatement* m_DboDepositWithdrawalUpdateStatement;
	sql::PreparedStatement* m_DboDepositWithdrawalPrimaryKeyStatement;
	sql::PreparedStatement* m_DboExchangeInsertStatement;
	sql::PreparedStatement* m_DboExchangeTruncateStatement;
	sql::PreparedStatement* m_DboExchangeDeleteStatement;
	sql::PreparedStatement* m_DboExchangeUpdateStatement;
	sql::PreparedStatement* m_DboExchangePrimaryKeyStatement;
	sql::PreparedStatement* m_DboInvertorInsertStatement;
	sql::PreparedStatement* m_DboInvertorTruncateStatement;
	sql::PreparedStatement* m_DboInvertorDeleteStatement;
	sql::PreparedStatement* m_DboInvertorUpdateStatement;
	sql::PreparedStatement* m_DboInvertorPrimaryKeyStatement;
	sql::PreparedStatement* m_DboInvestorMobileInsertStatement;
	sql::PreparedStatement* m_DboInvestorMobileTruncateStatement;
	sql::PreparedStatement* m_DboInvestorMobileDeleteStatement;
	sql::PreparedStatement* m_DboInvestorMobileUpdateStatement;
	sql::PreparedStatement* m_DboInvestorMobilePrimaryKeyStatement;
	sql::PreparedStatement* m_DboInvestorParamInsertStatement;
	sql::PreparedStatement* m_DboInvestorParamTruncateStatement;
	sql::PreparedStatement* m_DboInvestorParamDeleteStatement;
	sql::PreparedStatement* m_DboInvestorParamUpdateStatement;
	sql::PreparedStatement* m_DboInvestorParamPrimaryKeyStatement;
	sql::PreparedStatement* m_DboLoginInsertStatement;
	sql::PreparedStatement* m_DboLoginTruncateStatement;
	sql::PreparedStatement* m_DboLoginDeleteStatement;
	sql::PreparedStatement* m_DboLoginUpdateStatement;
	sql::PreparedStatement* m_DboLoginPrimaryKeyStatement;
	sql::PreparedStatement* m_DboMarginInsertStatement;
	sql::PreparedStatement* m_DboMarginTruncateStatement;
	sql::PreparedStatement* m_DboMarginDeleteStatement;
	sql::PreparedStatement* m_DboMarginUpdateStatement;
	sql::PreparedStatement* m_DboMarginPrimaryKeyStatement;
	sql::PreparedStatement* m_DboMaxBalanceInsertStatement;
	sql::PreparedStatement* m_DboMaxBalanceTruncateStatement;
	sql::PreparedStatement* m_DboMaxBalanceDeleteStatement;
	sql::PreparedStatement* m_DboMaxBalanceUpdateStatement;
	sql::PreparedStatement* m_DboMaxBalancePrimaryKeyStatement;
	sql::PreparedStatement* m_DboOrderInsertStatement;
	sql::PreparedStatement* m_DboOrderTruncateStatement;
	sql::PreparedStatement* m_DboOrderDeleteStatement;
	sql::PreparedStatement* m_DboOrderUpdateStatement;
	sql::PreparedStatement* m_DboOrderPrimaryKeyStatement;
	sql::PreparedStatement* m_DboOrderCheckInsertStatement;
	sql::PreparedStatement* m_DboOrderCheckTruncateStatement;
	sql::PreparedStatement* m_DboOrderCheckDeleteStatement;
	sql::PreparedStatement* m_DboOrderCheckUpdateStatement;
	sql::PreparedStatement* m_DboOrderCheckPrimaryKeyStatement;
	sql::PreparedStatement* m_DboPositionInsertStatement;
	sql::PreparedStatement* m_DboPositionTruncateStatement;
	sql::PreparedStatement* m_DboPositionDeleteStatement;
	sql::PreparedStatement* m_DboPositionUpdateStatement;
	sql::PreparedStatement* m_DboPositionPrimaryKeyStatement;
	sql::PreparedStatement* m_DboProductInsertStatement;
	sql::PreparedStatement* m_DboProductTruncateStatement;
	sql::PreparedStatement* m_DboProductDeleteStatement;
	sql::PreparedStatement* m_DboProductUpdateStatement;
	sql::PreparedStatement* m_DboProductPrimaryKeyStatement;
	sql::PreparedStatement* m_DboRiskControlInsertStatement;
	sql::PreparedStatement* m_DboRiskControlTruncateStatement;
	sql::PreparedStatement* m_DboRiskControlDeleteStatement;
	sql::PreparedStatement* m_DboRiskControlUpdateStatement;
	sql::PreparedStatement* m_DboRiskControlPrimaryKeyStatement;
	sql::PreparedStatement* m_DboRiskControlTemplateInsertStatement;
	sql::PreparedStatement* m_DboRiskControlTemplateTruncateStatement;
	sql::PreparedStatement* m_DboRiskControlTemplateDeleteStatement;
	sql::PreparedStatement* m_DboRiskControlTemplateUpdateStatement;
	sql::PreparedStatement* m_DboRiskControlTemplatePrimaryKeyStatement;
	sql::PreparedStatement* m_DboSessionInsertStatement;
	sql::PreparedStatement* m_DboSessionTruncateStatement;
	sql::PreparedStatement* m_DboSessionDeleteStatement;
	sql::PreparedStatement* m_DboSessionUpdateStatement;
	sql::PreparedStatement* m_DboSessionPrimaryKeyStatement;
	sql::PreparedStatement* m_DboSettlePriceInsertStatement;
	sql::PreparedStatement* m_DboSettlePriceTruncateStatement;
	sql::PreparedStatement* m_DboSettlePriceDeleteStatement;
	sql::PreparedStatement* m_DboSettlePriceUpdateStatement;
	sql::PreparedStatement* m_DboSettlePricePrimaryKeyStatement;
	sql::PreparedStatement* m_DboSummaryInsertStatement;
	sql::PreparedStatement* m_DboSummaryTruncateStatement;
	sql::PreparedStatement* m_DboSummaryDeleteStatement;
	sql::PreparedStatement* m_DboSummaryUpdateStatement;
	sql::PreparedStatement* m_DboSummaryPrimaryKeyStatement;
	sql::PreparedStatement* m_DboSysParamInsertStatement;
	sql::PreparedStatement* m_DboSysParamTruncateStatement;
	sql::PreparedStatement* m_DboSysParamDeleteStatement;
	sql::PreparedStatement* m_DboSysParamUpdateStatement;
	sql::PreparedStatement* m_DboSysParamPrimaryKeyStatement;
	sql::PreparedStatement* m_DboTradeInsertStatement;
	sql::PreparedStatement* m_DboTradeTruncateStatement;
	sql::PreparedStatement* m_DboTradeDeleteStatement;
	sql::PreparedStatement* m_DboTradeUpdateStatement;
	sql::PreparedStatement* m_DboTradePrimaryKeyStatement;
	sql::PreparedStatement* m_DboTradeChannelInsertStatement;
	sql::PreparedStatement* m_DboTradeChannelTruncateStatement;
	sql::PreparedStatement* m_DboTradeChannelDeleteStatement;
	sql::PreparedStatement* m_DboTradeChannelUpdateStatement;
	sql::PreparedStatement* m_DboTradeChannelPrimaryKeyStatement;
	sql::PreparedStatement* m_DboTrustedNetAddressInsertStatement;
	sql::PreparedStatement* m_DboTrustedNetAddressTruncateStatement;
	sql::PreparedStatement* m_DboTrustedNetAddressDeleteStatement;
	sql::PreparedStatement* m_DboTrustedNetAddressUpdateStatement;
	sql::PreparedStatement* m_DboTrustedNetAddressPrimaryKeyStatement;
};
