#pragma warning(disable: 4311)
#include "DboDB.h"
#include <string.h>


DboDB::DboDB(const std::string& host, const std::string& user, const std::string& passwd)
{
	m_Sql = new char[4096];
	m_Host = host;
	m_User = user;
	m_Passwd = passwd;

	m_Driver = sql::mysql::get_driver_instance();
	m_DBConnection = m_Driver->connect(host, user, passwd);

	m_DboAuthorizedSoftwareInsertStatement = nullptr;
	m_DboAuthorizedSoftwareTruncateStatement = nullptr;
	m_DboAuthorizedSoftwareDeleteStatement = nullptr;
	m_DboAuthorizedSoftwareUpdateStatement = nullptr;
	m_DboAuthorizedSoftwarePrimaryKeyStatement = nullptr;
	m_DboAuthorizedSoftwareUniqueKeyStatementAuthCode = nullptr;
	m_DboAuthorizedSoftwareIndexStatementAccount = nullptr;
	m_DboCloseInsertStatement = nullptr;
	m_DboCloseTruncateStatement = nullptr;
	m_DboCloseDeleteStatement = nullptr;
	m_DboCloseUpdateStatement = nullptr;
	m_DboClosePrimaryKeyStatement = nullptr;
	m_DboCommissionInsertStatement = nullptr;
	m_DboCommissionTruncateStatement = nullptr;
	m_DboCommissionDeleteStatement = nullptr;
	m_DboCommissionUpdateStatement = nullptr;
	m_DboCommissionPrimaryKeyStatement = nullptr;
	m_DboContractInsertStatement = nullptr;
	m_DboContractTruncateStatement = nullptr;
	m_DboContractDeleteStatement = nullptr;
	m_DboContractUpdateStatement = nullptr;
	m_DboContractPrimaryKeyStatement = nullptr;
	m_DboCreditAmountInsertStatement = nullptr;
	m_DboCreditAmountTruncateStatement = nullptr;
	m_DboCreditAmountDeleteStatement = nullptr;
	m_DboCreditAmountUpdateStatement = nullptr;
	m_DboCreditAmountPrimaryKeyStatement = nullptr;
	m_DboCurrencyRateInsertStatement = nullptr;
	m_DboCurrencyRateTruncateStatement = nullptr;
	m_DboCurrencyRateDeleteStatement = nullptr;
	m_DboCurrencyRateUpdateStatement = nullptr;
	m_DboCurrencyRatePrimaryKeyStatement = nullptr;
	m_DboDepositWithdrawalInsertStatement = nullptr;
	m_DboDepositWithdrawalTruncateStatement = nullptr;
	m_DboDepositWithdrawalDeleteStatement = nullptr;
	m_DboDepositWithdrawalUpdateStatement = nullptr;
	m_DboDepositWithdrawalPrimaryKeyStatement = nullptr;
	m_DboExchangeInsertStatement = nullptr;
	m_DboExchangeTruncateStatement = nullptr;
	m_DboExchangeDeleteStatement = nullptr;
	m_DboExchangeUpdateStatement = nullptr;
	m_DboExchangePrimaryKeyStatement = nullptr;
	m_DboInvertorInsertStatement = nullptr;
	m_DboInvertorTruncateStatement = nullptr;
	m_DboInvertorDeleteStatement = nullptr;
	m_DboInvertorUpdateStatement = nullptr;
	m_DboInvertorPrimaryKeyStatement = nullptr;
	m_DboInvestorMobileInsertStatement = nullptr;
	m_DboInvestorMobileTruncateStatement = nullptr;
	m_DboInvestorMobileDeleteStatement = nullptr;
	m_DboInvestorMobileUpdateStatement = nullptr;
	m_DboInvestorMobilePrimaryKeyStatement = nullptr;
	m_DboInvestorParamInsertStatement = nullptr;
	m_DboInvestorParamTruncateStatement = nullptr;
	m_DboInvestorParamDeleteStatement = nullptr;
	m_DboInvestorParamUpdateStatement = nullptr;
	m_DboInvestorParamPrimaryKeyStatement = nullptr;
	m_DboLoginInsertStatement = nullptr;
	m_DboLoginTruncateStatement = nullptr;
	m_DboLoginDeleteStatement = nullptr;
	m_DboLoginUpdateStatement = nullptr;
	m_DboLoginPrimaryKeyStatement = nullptr;
	m_DboMarginInsertStatement = nullptr;
	m_DboMarginTruncateStatement = nullptr;
	m_DboMarginDeleteStatement = nullptr;
	m_DboMarginUpdateStatement = nullptr;
	m_DboMarginPrimaryKeyStatement = nullptr;
	m_DboMaxBalanceInsertStatement = nullptr;
	m_DboMaxBalanceTruncateStatement = nullptr;
	m_DboMaxBalanceDeleteStatement = nullptr;
	m_DboMaxBalanceUpdateStatement = nullptr;
	m_DboMaxBalancePrimaryKeyStatement = nullptr;
	m_DboOrderInsertStatement = nullptr;
	m_DboOrderTruncateStatement = nullptr;
	m_DboOrderDeleteStatement = nullptr;
	m_DboOrderUpdateStatement = nullptr;
	m_DboOrderPrimaryKeyStatement = nullptr;
	m_DboOrderCheckInsertStatement = nullptr;
	m_DboOrderCheckTruncateStatement = nullptr;
	m_DboOrderCheckDeleteStatement = nullptr;
	m_DboOrderCheckUpdateStatement = nullptr;
	m_DboOrderCheckPrimaryKeyStatement = nullptr;
	m_DboPositionInsertStatement = nullptr;
	m_DboPositionTruncateStatement = nullptr;
	m_DboPositionDeleteStatement = nullptr;
	m_DboPositionUpdateStatement = nullptr;
	m_DboPositionPrimaryKeyStatement = nullptr;
	m_DboProductInsertStatement = nullptr;
	m_DboProductTruncateStatement = nullptr;
	m_DboProductDeleteStatement = nullptr;
	m_DboProductUpdateStatement = nullptr;
	m_DboProductPrimaryKeyStatement = nullptr;
	m_DboRiskControlInsertStatement = nullptr;
	m_DboRiskControlTruncateStatement = nullptr;
	m_DboRiskControlDeleteStatement = nullptr;
	m_DboRiskControlUpdateStatement = nullptr;
	m_DboRiskControlPrimaryKeyStatement = nullptr;
	m_DboRiskControlTemplateInsertStatement = nullptr;
	m_DboRiskControlTemplateTruncateStatement = nullptr;
	m_DboRiskControlTemplateDeleteStatement = nullptr;
	m_DboRiskControlTemplateUpdateStatement = nullptr;
	m_DboRiskControlTemplatePrimaryKeyStatement = nullptr;
	m_DboSessionInsertStatement = nullptr;
	m_DboSessionTruncateStatement = nullptr;
	m_DboSessionDeleteStatement = nullptr;
	m_DboSessionUpdateStatement = nullptr;
	m_DboSessionPrimaryKeyStatement = nullptr;
	m_DboSettlePriceInsertStatement = nullptr;
	m_DboSettlePriceTruncateStatement = nullptr;
	m_DboSettlePriceDeleteStatement = nullptr;
	m_DboSettlePriceUpdateStatement = nullptr;
	m_DboSettlePricePrimaryKeyStatement = nullptr;
	m_DboSummaryInsertStatement = nullptr;
	m_DboSummaryTruncateStatement = nullptr;
	m_DboSummaryDeleteStatement = nullptr;
	m_DboSummaryUpdateStatement = nullptr;
	m_DboSummaryPrimaryKeyStatement = nullptr;
	m_DboSysParamInsertStatement = nullptr;
	m_DboSysParamTruncateStatement = nullptr;
	m_DboSysParamDeleteStatement = nullptr;
	m_DboSysParamUpdateStatement = nullptr;
	m_DboSysParamPrimaryKeyStatement = nullptr;
	m_DboTradeInsertStatement = nullptr;
	m_DboTradeTruncateStatement = nullptr;
	m_DboTradeDeleteStatement = nullptr;
	m_DboTradeUpdateStatement = nullptr;
	m_DboTradePrimaryKeyStatement = nullptr;
	m_DboTradeChannelInsertStatement = nullptr;
	m_DboTradeChannelTruncateStatement = nullptr;
	m_DboTradeChannelDeleteStatement = nullptr;
	m_DboTradeChannelUpdateStatement = nullptr;
	m_DboTradeChannelPrimaryKeyStatement = nullptr;
	m_DboTrustedNetAddressInsertStatement = nullptr;
	m_DboTrustedNetAddressTruncateStatement = nullptr;
	m_DboTrustedNetAddressDeleteStatement = nullptr;
	m_DboTrustedNetAddressUpdateStatement = nullptr;
	m_DboTrustedNetAddressPrimaryKeyStatement = nullptr;
}
DboDB::~DboDB()
{
	delete[] m_Sql;
}


bool DboDB::InsertDboAuthorizedSoftware(DboAuthorizedSoftware* record)
{
	if (m_DboAuthorizedSoftwareInsertStatement == nullptr)
	{
		m_DboAuthorizedSoftwareInsertStatement = m_DBConnection->prepareStatement("insert into t_AuthorizedSoftware Values(?, ?, ?);");
	}
	SetStatementForDboAuthorizedSoftwareRecord(m_DboAuthorizedSoftwareInsertStatement, record);
	return m_DboAuthorizedSoftwareInsertStatement->execute();
}
bool DboDB::TruncateDboAuthorizedSoftware()
{
	if(m_DboAuthorizedSoftwareTruncateStatement == nullptr)
	{
		m_DboAuthorizedSoftwareTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_AuthorizedSoftware ;");
	}
	return m_DboAuthorizedSoftwareTruncateStatement->execute();
}
bool DboDB::DeleteDboAuthorizedSoftware(DboAuthorizedSoftware* record)
{
	if (m_DboAuthorizedSoftwareDeleteStatement == nullptr)
	{
		m_DboAuthorizedSoftwareDeleteStatement = m_DBConnection->prepareStatement("delete from t_AuthorizedSoftware where Account = ? and Software = ?;");
	}
	SetStatementForDboAuthorizedSoftwarePrimaryKey(m_DboAuthorizedSoftwareDeleteStatement, record->Account, record->Software);
	return m_DboAuthorizedSoftwareDeleteStatement->execute();
}
bool DboDB::UpdateDboAuthorizedSoftware(DboAuthorizedSoftware* record)
{
	if (m_DboAuthorizedSoftwareUpdateStatement == nullptr)
	{
		m_DboAuthorizedSoftwareUpdateStatement = m_DBConnection->prepareStatement("replace into t_AuthorizedSoftware Values(?, ?, ?);");
	}
	SetStatementForDboAuthorizedSoftwareRecord(m_DboAuthorizedSoftwareUpdateStatement, record);
	return m_DboAuthorizedSoftwareUpdateStatement->execute();
}
DboAuthorizedSoftware* DboDB::QueryDboAuthorizedSoftwareFromPrimaryKey(const AccountType& Account, const SoftwareType& Software)
{
	if (m_DboAuthorizedSoftwarePrimaryKeyStatement == nullptr)
	{
		m_DboAuthorizedSoftwarePrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_AuthorizedSoftware where Account = ? and Software = ?;");
	}
	SetStatementForDboAuthorizedSoftwarePrimaryKey(m_DboAuthorizedSoftwarePrimaryKeyStatement, Account, Software);
	auto result = m_DboAuthorizedSoftwarePrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboAuthorizedSoftwareRecord(result);
	}
	return nullptr;
}
DboAuthorizedSoftware* DboDB::QueryDboAuthorizedSoftwareFromUniqueKeyAuthCode(const SoftwareType& Software, const AuthCodeType& AuthCode)
{
	if (m_DboAuthorizedSoftwareUniqueKeyStatementAuthCode == nullptr)
	{
		m_DboAuthorizedSoftwareUniqueKeyStatementAuthCode = m_DBConnection->prepareStatement("select * from t_AuthorizedSoftware where Software = ? and AuthCode = ?;");
	}
	SetStatementForDboAuthorizedSoftwareUniqueKeyAuthCode(m_DboAuthorizedSoftwareUniqueKeyStatementAuthCode, Software, AuthCode);
	auto result = m_DboAuthorizedSoftwareUniqueKeyStatementAuthCode->executeQuery();
	if (result->next())
	{
		return GetDboAuthorizedSoftwareRecord(result);
	}
	return nullptr;
}
void DboDB::QueryDboAuthorizedSoftwareFromIndexAccount(std::vector<DboAuthorizedSoftware*>& records, const AuthCodeType& AuthCode, const AccountType& Account)
{
	if (m_DboAuthorizedSoftwareIndexStatementAccount == nullptr)
	{
		m_DboAuthorizedSoftwareIndexStatementAccount = m_DBConnection->prepareStatement("select * from t_AuthorizedSoftware where AuthCode = ? and Account = ?;");
	}
	SetStatementForDboAuthorizedSoftwareIndexAccount(m_DboAuthorizedSoftwareIndexStatementAccount, AuthCode, Account);
	auto result = m_DboAuthorizedSoftwareIndexStatementAccount->executeQuery();
	while (result->next())
	{
		auto record = GetDboAuthorizedSoftwareRecord(result);
		records.push_back(record);
	}
}
void DboDB::SetStatementForDboAuthorizedSoftwareRecord(sql::PreparedStatement* statement, DboAuthorizedSoftware* record)
{
	statement->setString(1, record->Software);
	statement->setString(2, record->AuthCode);
	statement->setInt(3, record->Account);
}
void DboDB::SetStatementForDboAuthorizedSoftwarePrimaryKey(sql::PreparedStatement* statement, const AccountType& Account, const SoftwareType& Software)
{
	statement->setInt(1, Account);
	statement->setString(2, Software);
}
void DboDB::SetStatementForDboAuthorizedSoftwareUniqueKeyAuthCode(sql::PreparedStatement* statement, const SoftwareType& Software, const AuthCodeType& AuthCode)
{
	statement->setString(1, Software);
	statement->setString(2, AuthCode);
}
void DboDB::SetStatementForDboAuthorizedSoftwareIndexAccount(sql::PreparedStatement* statement, const AuthCodeType& AuthCode, const AccountType& Account)
{
	statement->setString(1, AuthCode);
	statement->setInt(2, Account);
}
DboAuthorizedSoftware* DboDB::GetDboAuthorizedSoftwareRecord(sql::ResultSet* result)
{
	DboAuthorizedSoftware* record = new DboAuthorizedSoftware();
	strcpy(record->Software, result->getString(1).c_str());
	strcpy(record->AuthCode, result->getString(2).c_str());
	record->Account = result->getInt(3);
	return record;
}


bool DboDB::InsertDboClose(DboClose* record)
{
	if (m_DboCloseInsertStatement == nullptr)
	{
		m_DboCloseInsertStatement = m_DBConnection->prepareStatement("insert into t_Close Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboCloseRecord(m_DboCloseInsertStatement, record);
	return m_DboCloseInsertStatement->execute();
}
bool DboDB::TruncateDboClose()
{
	if(m_DboCloseTruncateStatement == nullptr)
	{
		m_DboCloseTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_Close ;");
	}
	return m_DboCloseTruncateStatement->execute();
}
bool DboDB::DeleteDboClose(DboClose* record)
{
	if (m_DboCloseDeleteStatement == nullptr)
	{
		m_DboCloseDeleteStatement = m_DBConnection->prepareStatement("delete from t_Close where OpenDate = ? and OpenOrderID = ? and OpenTradeID = ? and CloseDate = ? and CloseOrderID = ? and CloseTradeID = ?;");
	}
	SetStatementForDboClosePrimaryKey(m_DboCloseDeleteStatement, record->OpenDate, record->OpenOrderID, record->OpenTradeID, record->CloseDate, record->CloseOrderID, record->CloseTradeID);
	return m_DboCloseDeleteStatement->execute();
}
bool DboDB::UpdateDboClose(DboClose* record)
{
	if (m_DboCloseUpdateStatement == nullptr)
	{
		m_DboCloseUpdateStatement = m_DBConnection->prepareStatement("replace into t_Close Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboCloseRecord(m_DboCloseUpdateStatement, record);
	return m_DboCloseUpdateStatement->execute();
}
DboClose* DboDB::QueryDboCloseFromPrimaryKey(const DateType& OpenDate, const OrderIDType& OpenOrderID, const TradeIDType& OpenTradeID, const DateType& CloseDate, const OrderIDType& CloseOrderID, const TradeIDType& CloseTradeID)
{
	if (m_DboClosePrimaryKeyStatement == nullptr)
	{
		m_DboClosePrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_Close where OpenDate = ? and OpenOrderID = ? and OpenTradeID = ? and CloseDate = ? and CloseOrderID = ? and CloseTradeID = ?;");
	}
	SetStatementForDboClosePrimaryKey(m_DboClosePrimaryKeyStatement, OpenDate, OpenOrderID, OpenTradeID, CloseDate, CloseOrderID, CloseTradeID);
	auto result = m_DboClosePrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboCloseRecord(result);
	}
	return nullptr;
}
void DboDB::SetStatementForDboCloseRecord(sql::PreparedStatement* statement, DboClose* record)
{
	statement->setInt(1, record->OpenDate);
	statement->setInt(2, record->OpenOrderID);
	statement->setString(3, record->OpenTradeID);
	statement->setInt(4, record->CloseDate);
	statement->setInt(5, record->CloseOrderID);
	statement->setString(6, record->CloseTradeID);
	statement->setInt(7, record->Account);
	statement->setString(8, record->ExchangeID);
	statement->setString(9, record->ContractID);
	statement->setInt(10, int(record->BS));
	statement->setInt(11, int(record->Hedge));
	statement->setDouble(12, record->OpenPrice);
	statement->setDouble(13, record->LastSettle);
	statement->setDouble(14, record->ClosePrice);
	statement->setInt(15, record->Volume);
	statement->setInt(16, record->RealOpenDate);
	statement->setInt(17, record->RealCloseDate);
	statement->setString(18, record->OpenTime);
	statement->setString(19, record->CloseTime);
	statement->setDouble(20, record->CloseProfitFloat);
	statement->setDouble(21, record->CloseProfit);
	statement->setString(22, record->CurrencyGroup);
	statement->setString(23, record->CurrencyID);
}
void DboDB::SetStatementForDboClosePrimaryKey(sql::PreparedStatement* statement, const DateType& OpenDate, const OrderIDType& OpenOrderID, const TradeIDType& OpenTradeID, const DateType& CloseDate, const OrderIDType& CloseOrderID, const TradeIDType& CloseTradeID)
{
	statement->setInt(1, OpenDate);
	statement->setInt(2, OpenOrderID);
	statement->setString(3, OpenTradeID);
	statement->setInt(4, CloseDate);
	statement->setInt(5, CloseOrderID);
	statement->setString(6, CloseTradeID);
}
DboClose* DboDB::GetDboCloseRecord(sql::ResultSet* result)
{
	DboClose* record = new DboClose();
	record->OpenDate = result->getInt(1);
	record->OpenOrderID = result->getInt(2);
	strcpy(record->OpenTradeID, result->getString(3).c_str());
	record->CloseDate = result->getInt(4);
	record->CloseOrderID = result->getInt(5);
	strcpy(record->CloseTradeID, result->getString(6).c_str());
	record->Account = result->getInt(7);
	strcpy(record->ExchangeID, result->getString(8).c_str());
	strcpy(record->ContractID, result->getString(9).c_str());
	record->BS = DirectionType(result->getInt(10));
	record->Hedge = HedgeFlagType(result->getInt(11));
	record->OpenPrice = result->getDouble(12);
	record->LastSettle = result->getDouble(13);
	record->ClosePrice = result->getDouble(14);
	record->Volume = result->getInt(15);
	record->RealOpenDate = result->getInt(16);
	record->RealCloseDate = result->getInt(17);
	strcpy(record->OpenTime, result->getString(18).c_str());
	strcpy(record->CloseTime, result->getString(19).c_str());
	record->CloseProfitFloat = result->getDouble(20);
	record->CloseProfit = result->getDouble(21);
	strcpy(record->CurrencyGroup, result->getString(22).c_str());
	strcpy(record->CurrencyID, result->getString(23).c_str());
	return record;
}


bool DboDB::InsertDboCommission(DboCommission* record)
{
	if (m_DboCommissionInsertStatement == nullptr)
	{
		m_DboCommissionInsertStatement = m_DBConnection->prepareStatement("insert into t_Commission Values(?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboCommissionRecord(m_DboCommissionInsertStatement, record);
	return m_DboCommissionInsertStatement->execute();
}
bool DboDB::TruncateDboCommission()
{
	if(m_DboCommissionTruncateStatement == nullptr)
	{
		m_DboCommissionTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_Commission ;");
	}
	return m_DboCommissionTruncateStatement->execute();
}
bool DboDB::DeleteDboCommission(DboCommission* record)
{
	if (m_DboCommissionDeleteStatement == nullptr)
	{
		m_DboCommissionDeleteStatement = m_DBConnection->prepareStatement("delete from t_Commission where GroupID = ? and Account = ? and ExchangeID = ? and ProductID = ? and ContractID = ?;");
	}
	SetStatementForDboCommissionPrimaryKey(m_DboCommissionDeleteStatement, record->GroupID, record->Account, record->ExchangeID, record->ProductID, record->ContractID);
	return m_DboCommissionDeleteStatement->execute();
}
bool DboDB::UpdateDboCommission(DboCommission* record)
{
	if (m_DboCommissionUpdateStatement == nullptr)
	{
		m_DboCommissionUpdateStatement = m_DBConnection->prepareStatement("replace into t_Commission Values(?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboCommissionRecord(m_DboCommissionUpdateStatement, record);
	return m_DboCommissionUpdateStatement->execute();
}
DboCommission* DboDB::QueryDboCommissionFromPrimaryKey(const GroupIDType& GroupID, const AccountType& Account, const ExchangeIDType& ExchangeID, const ProductIDType& ProductID, const ContractIDType& ContractID)
{
	if (m_DboCommissionPrimaryKeyStatement == nullptr)
	{
		m_DboCommissionPrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_Commission where GroupID = ? and Account = ? and ExchangeID = ? and ProductID = ? and ContractID = ?;");
	}
	SetStatementForDboCommissionPrimaryKey(m_DboCommissionPrimaryKeyStatement, GroupID, Account, ExchangeID, ProductID, ContractID);
	auto result = m_DboCommissionPrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboCommissionRecord(result);
	}
	return nullptr;
}
void DboDB::SetStatementForDboCommissionRecord(sql::PreparedStatement* statement, DboCommission* record)
{
	statement->setInt(1, record->GroupID);
	statement->setInt(2, record->Account);
	statement->setString(3, record->ExchangeID);
	statement->setString(4, record->ProductID);
	statement->setString(5, record->ContractID);
	statement->setDouble(6, record->OpenByVolume);
	statement->setDouble(7, record->CloseByVolume);
	statement->setDouble(8, record->OpenByMoney);
	statement->setDouble(9, record->CloseByMoney);
}
void DboDB::SetStatementForDboCommissionPrimaryKey(sql::PreparedStatement* statement, const GroupIDType& GroupID, const AccountType& Account, const ExchangeIDType& ExchangeID, const ProductIDType& ProductID, const ContractIDType& ContractID)
{
	statement->setInt(1, GroupID);
	statement->setInt(2, Account);
	statement->setString(3, ExchangeID);
	statement->setString(4, ProductID);
	statement->setString(5, ContractID);
}
DboCommission* DboDB::GetDboCommissionRecord(sql::ResultSet* result)
{
	DboCommission* record = new DboCommission();
	record->GroupID = result->getInt(1);
	record->Account = result->getInt(2);
	strcpy(record->ExchangeID, result->getString(3).c_str());
	strcpy(record->ProductID, result->getString(4).c_str());
	strcpy(record->ContractID, result->getString(5).c_str());
	record->OpenByVolume = result->getDouble(6);
	record->CloseByVolume = result->getDouble(7);
	record->OpenByMoney = result->getDouble(8);
	record->CloseByMoney = result->getDouble(9);
	return record;
}


bool DboDB::InsertDboContract(DboContract* record)
{
	if (m_DboContractInsertStatement == nullptr)
	{
		m_DboContractInsertStatement = m_DBConnection->prepareStatement("insert into t_Contract Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboContractRecord(m_DboContractInsertStatement, record);
	return m_DboContractInsertStatement->execute();
}
bool DboDB::TruncateDboContract()
{
	if(m_DboContractTruncateStatement == nullptr)
	{
		m_DboContractTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_Contract ;");
	}
	return m_DboContractTruncateStatement->execute();
}
bool DboDB::DeleteDboContract(DboContract* record)
{
	if (m_DboContractDeleteStatement == nullptr)
	{
		m_DboContractDeleteStatement = m_DBConnection->prepareStatement("delete from t_Contract where ExchangeID = ? and ContractID = ?;");
	}
	SetStatementForDboContractPrimaryKey(m_DboContractDeleteStatement, record->ExchangeID, record->ContractID);
	return m_DboContractDeleteStatement->execute();
}
bool DboDB::UpdateDboContract(DboContract* record)
{
	if (m_DboContractUpdateStatement == nullptr)
	{
		m_DboContractUpdateStatement = m_DBConnection->prepareStatement("replace into t_Contract Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboContractRecord(m_DboContractUpdateStatement, record);
	return m_DboContractUpdateStatement->execute();
}
DboContract* DboDB::QueryDboContractFromPrimaryKey(const ExchangeIDType& ExchangeID, const ContractIDType& ContractID)
{
	if (m_DboContractPrimaryKeyStatement == nullptr)
	{
		m_DboContractPrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_Contract where ExchangeID = ? and ContractID = ?;");
	}
	SetStatementForDboContractPrimaryKey(m_DboContractPrimaryKeyStatement, ExchangeID, ContractID);
	auto result = m_DboContractPrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboContractRecord(result);
	}
	return nullptr;
}
void DboDB::SetStatementForDboContractRecord(sql::PreparedStatement* statement, DboContract* record)
{
	statement->setString(1, record->ExchangeID);
	statement->setString(2, record->ProductID);
	statement->setString(3, record->ContractID);
	statement->setString(4, record->ContractName);
	statement->setInt(5, record->DeliveryYear);
	statement->setInt(6, record->DeliveryMonth);
	statement->setInt(7, record->OpenDate);
	statement->setInt(8, record->ExpireDate);
	statement->setInt(9, record->FirstNoticeDate);
	statement->setInt(10, record->StartDelivDate);
	statement->setInt(11, record->EndDelivDate);
	statement->setInt(12, int(record->TradeStatus));
	statement->setDouble(13, record->Margin);
	statement->setDouble(14, record->MinMargin);
}
void DboDB::SetStatementForDboContractPrimaryKey(sql::PreparedStatement* statement, const ExchangeIDType& ExchangeID, const ContractIDType& ContractID)
{
	statement->setString(1, ExchangeID);
	statement->setString(2, ContractID);
}
DboContract* DboDB::GetDboContractRecord(sql::ResultSet* result)
{
	DboContract* record = new DboContract();
	strcpy(record->ExchangeID, result->getString(1).c_str());
	strcpy(record->ProductID, result->getString(2).c_str());
	strcpy(record->ContractID, result->getString(3).c_str());
	strcpy(record->ContractName, result->getString(4).c_str());
	record->DeliveryYear = result->getInt(5);
	record->DeliveryMonth = result->getInt(6);
	record->OpenDate = result->getInt(7);
	record->ExpireDate = result->getInt(8);
	record->FirstNoticeDate = result->getInt(9);
	record->StartDelivDate = result->getInt(10);
	record->EndDelivDate = result->getInt(11);
	record->TradeStatus = TradeStatusType(result->getInt(12));
	record->Margin = result->getDouble(13);
	record->MinMargin = result->getDouble(14);
	return record;
}


bool DboDB::InsertDboCreditAmount(DboCreditAmount* record)
{
	if (m_DboCreditAmountInsertStatement == nullptr)
	{
		m_DboCreditAmountInsertStatement = m_DBConnection->prepareStatement("insert into t_CreditAmount Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboCreditAmountRecord(m_DboCreditAmountInsertStatement, record);
	return m_DboCreditAmountInsertStatement->execute();
}
bool DboDB::TruncateDboCreditAmount()
{
	if(m_DboCreditAmountTruncateStatement == nullptr)
	{
		m_DboCreditAmountTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_CreditAmount ;");
	}
	return m_DboCreditAmountTruncateStatement->execute();
}
bool DboDB::DeleteDboCreditAmount(DboCreditAmount* record)
{
	if (m_DboCreditAmountDeleteStatement == nullptr)
	{
		m_DboCreditAmountDeleteStatement = m_DBConnection->prepareStatement("delete from t_CreditAmount where ID = ?;");
	}
	SetStatementForDboCreditAmountPrimaryKey(m_DboCreditAmountDeleteStatement, record->ID);
	return m_DboCreditAmountDeleteStatement->execute();
}
bool DboDB::UpdateDboCreditAmount(DboCreditAmount* record)
{
	if (m_DboCreditAmountUpdateStatement == nullptr)
	{
		m_DboCreditAmountUpdateStatement = m_DBConnection->prepareStatement("replace into t_CreditAmount Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboCreditAmountRecord(m_DboCreditAmountUpdateStatement, record);
	return m_DboCreditAmountUpdateStatement->execute();
}
DboCreditAmount* DboDB::QueryDboCreditAmountFromPrimaryKey(const IDType& ID)
{
	if (m_DboCreditAmountPrimaryKeyStatement == nullptr)
	{
		m_DboCreditAmountPrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_CreditAmount where ID = ?;");
	}
	SetStatementForDboCreditAmountPrimaryKey(m_DboCreditAmountPrimaryKeyStatement, ID);
	auto result = m_DboCreditAmountPrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboCreditAmountRecord(result);
	}
	return nullptr;
}
void DboDB::SetStatementForDboCreditAmountRecord(sql::PreparedStatement* statement, DboCreditAmount* record)
{
	statement->setInt(1, record->ID);
	statement->setString(2, record->AuditTime);
	statement->setString(3, record->OperationTime);
	statement->setString(4, record->Remark);
	statement->setInt(5, record->Status);
	statement->setDouble(6, record->Amount);
	statement->setString(7, record->CurrencyGroup);
	statement->setInt(8, int(record->Direction));
	statement->setInt(9, record->TradingDay);
	statement->setString(10, record->OperatorID);
	statement->setInt(11, record->Account);
	statement->setString(12, record->CurrencyID);
	statement->setString(13, record->AuditorID);
	statement->setInt(14, record->Flag);
}
void DboDB::SetStatementForDboCreditAmountPrimaryKey(sql::PreparedStatement* statement, const IDType& ID)
{
	statement->setInt(1, ID);
}
DboCreditAmount* DboDB::GetDboCreditAmountRecord(sql::ResultSet* result)
{
	DboCreditAmount* record = new DboCreditAmount();
	record->ID = result->getInt(1);
	strcpy(record->AuditTime, result->getString(2).c_str());
	strcpy(record->OperationTime, result->getString(3).c_str());
	strcpy(record->Remark, result->getString(4).c_str());
	record->Status = result->getInt(5);
	record->Amount = result->getDouble(6);
	strcpy(record->CurrencyGroup, result->getString(7).c_str());
	record->Direction = DirectionType(result->getInt(8));
	record->TradingDay = result->getInt(9);
	strcpy(record->OperatorID, result->getString(10).c_str());
	record->Account = result->getInt(11);
	strcpy(record->CurrencyID, result->getString(12).c_str());
	strcpy(record->AuditorID, result->getString(13).c_str());
	record->Flag = result->getInt(14);
	return record;
}


bool DboDB::InsertDboCurrencyRate(DboCurrencyRate* record)
{
	if (m_DboCurrencyRateInsertStatement == nullptr)
	{
		m_DboCurrencyRateInsertStatement = m_DBConnection->prepareStatement("insert into t_CurrencyRate Values(?, ?, ?, ?, ?);");
	}
	SetStatementForDboCurrencyRateRecord(m_DboCurrencyRateInsertStatement, record);
	return m_DboCurrencyRateInsertStatement->execute();
}
bool DboDB::TruncateDboCurrencyRate()
{
	if(m_DboCurrencyRateTruncateStatement == nullptr)
	{
		m_DboCurrencyRateTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_CurrencyRate ;");
	}
	return m_DboCurrencyRateTruncateStatement->execute();
}
bool DboDB::DeleteDboCurrencyRate(DboCurrencyRate* record)
{
	if (m_DboCurrencyRateDeleteStatement == nullptr)
	{
		m_DboCurrencyRateDeleteStatement = m_DBConnection->prepareStatement("delete from t_CurrencyRate where CurrencyGroup = ? and CurrencyID = ?;");
	}
	SetStatementForDboCurrencyRatePrimaryKey(m_DboCurrencyRateDeleteStatement, record->CurrencyGroup, record->CurrencyID);
	return m_DboCurrencyRateDeleteStatement->execute();
}
bool DboDB::UpdateDboCurrencyRate(DboCurrencyRate* record)
{
	if (m_DboCurrencyRateUpdateStatement == nullptr)
	{
		m_DboCurrencyRateUpdateStatement = m_DBConnection->prepareStatement("replace into t_CurrencyRate Values(?, ?, ?, ?, ?);");
	}
	SetStatementForDboCurrencyRateRecord(m_DboCurrencyRateUpdateStatement, record);
	return m_DboCurrencyRateUpdateStatement->execute();
}
DboCurrencyRate* DboDB::QueryDboCurrencyRateFromPrimaryKey(const CurrencyGroupType& CurrencyGroup, const CurrencyIDType& CurrencyID)
{
	if (m_DboCurrencyRatePrimaryKeyStatement == nullptr)
	{
		m_DboCurrencyRatePrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_CurrencyRate where CurrencyGroup = ? and CurrencyID = ?;");
	}
	SetStatementForDboCurrencyRatePrimaryKey(m_DboCurrencyRatePrimaryKeyStatement, CurrencyGroup, CurrencyID);
	auto result = m_DboCurrencyRatePrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboCurrencyRateRecord(result);
	}
	return nullptr;
}
void DboDB::SetStatementForDboCurrencyRateRecord(sql::PreparedStatement* statement, DboCurrencyRate* record)
{
	statement->setString(1, record->CurrencyGroup);
	statement->setString(2, record->CurrencyID);
	statement->setDouble(3, record->TradeRate);
	statement->setDouble(4, record->SettlementRate);
	statement->setInt(5, record->IsBaseCurrency);
}
void DboDB::SetStatementForDboCurrencyRatePrimaryKey(sql::PreparedStatement* statement, const CurrencyGroupType& CurrencyGroup, const CurrencyIDType& CurrencyID)
{
	statement->setString(1, CurrencyGroup);
	statement->setString(2, CurrencyID);
}
DboCurrencyRate* DboDB::GetDboCurrencyRateRecord(sql::ResultSet* result)
{
	DboCurrencyRate* record = new DboCurrencyRate();
	strcpy(record->CurrencyGroup, result->getString(1).c_str());
	strcpy(record->CurrencyID, result->getString(2).c_str());
	record->TradeRate = result->getDouble(3);
	record->SettlementRate = result->getDouble(4);
	record->IsBaseCurrency = result->getInt(5);
	return record;
}


bool DboDB::InsertDboDepositWithdrawal(DboDepositWithdrawal* record)
{
	if (m_DboDepositWithdrawalInsertStatement == nullptr)
	{
		m_DboDepositWithdrawalInsertStatement = m_DBConnection->prepareStatement("insert into t_DepositWithdrawal Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboDepositWithdrawalRecord(m_DboDepositWithdrawalInsertStatement, record);
	return m_DboDepositWithdrawalInsertStatement->execute();
}
bool DboDB::TruncateDboDepositWithdrawal()
{
	if(m_DboDepositWithdrawalTruncateStatement == nullptr)
	{
		m_DboDepositWithdrawalTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_DepositWithdrawal ;");
	}
	return m_DboDepositWithdrawalTruncateStatement->execute();
}
bool DboDB::DeleteDboDepositWithdrawal(DboDepositWithdrawal* record)
{
	if (m_DboDepositWithdrawalDeleteStatement == nullptr)
	{
		m_DboDepositWithdrawalDeleteStatement = m_DBConnection->prepareStatement("delete from t_DepositWithdrawal where ID = ?;");
	}
	SetStatementForDboDepositWithdrawalPrimaryKey(m_DboDepositWithdrawalDeleteStatement, record->ID);
	return m_DboDepositWithdrawalDeleteStatement->execute();
}
bool DboDB::UpdateDboDepositWithdrawal(DboDepositWithdrawal* record)
{
	if (m_DboDepositWithdrawalUpdateStatement == nullptr)
	{
		m_DboDepositWithdrawalUpdateStatement = m_DBConnection->prepareStatement("replace into t_DepositWithdrawal Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboDepositWithdrawalRecord(m_DboDepositWithdrawalUpdateStatement, record);
	return m_DboDepositWithdrawalUpdateStatement->execute();
}
DboDepositWithdrawal* DboDB::QueryDboDepositWithdrawalFromPrimaryKey(const IDType& ID)
{
	if (m_DboDepositWithdrawalPrimaryKeyStatement == nullptr)
	{
		m_DboDepositWithdrawalPrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_DepositWithdrawal where ID = ?;");
	}
	SetStatementForDboDepositWithdrawalPrimaryKey(m_DboDepositWithdrawalPrimaryKeyStatement, ID);
	auto result = m_DboDepositWithdrawalPrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboDepositWithdrawalRecord(result);
	}
	return nullptr;
}
void DboDB::SetStatementForDboDepositWithdrawalRecord(sql::PreparedStatement* statement, DboDepositWithdrawal* record)
{
	statement->setInt(1, record->ID);
	statement->setString(2, record->AuditTime);
	statement->setString(3, record->OperationTime);
	statement->setString(4, record->Remark);
	statement->setInt(5, record->Status);
	statement->setDouble(6, record->Amount);
	statement->setString(7, record->BankCardID);
	statement->setString(8, record->CurrencyID);
	statement->setString(9, record->CurrencyGroup);
	statement->setInt(10, int(record->Direction));
	statement->setInt(11, record->Method);
	statement->setInt(12, record->TradingDay);
	statement->setString(13, record->OperatorID);
	statement->setInt(14, record->Account);
	statement->setString(15, record->AuditorID);
	statement->setInt(16, record->Flag);
}
void DboDB::SetStatementForDboDepositWithdrawalPrimaryKey(sql::PreparedStatement* statement, const IDType& ID)
{
	statement->setInt(1, ID);
}
DboDepositWithdrawal* DboDB::GetDboDepositWithdrawalRecord(sql::ResultSet* result)
{
	DboDepositWithdrawal* record = new DboDepositWithdrawal();
	record->ID = result->getInt(1);
	strcpy(record->AuditTime, result->getString(2).c_str());
	strcpy(record->OperationTime, result->getString(3).c_str());
	strcpy(record->Remark, result->getString(4).c_str());
	record->Status = result->getInt(5);
	record->Amount = result->getDouble(6);
	strcpy(record->BankCardID, result->getString(7).c_str());
	strcpy(record->CurrencyID, result->getString(8).c_str());
	strcpy(record->CurrencyGroup, result->getString(9).c_str());
	record->Direction = DirectionType(result->getInt(10));
	record->Method = result->getInt(11);
	record->TradingDay = result->getInt(12);
	strcpy(record->OperatorID, result->getString(13).c_str());
	record->Account = result->getInt(14);
	strcpy(record->AuditorID, result->getString(15).c_str());
	record->Flag = result->getInt(16);
	return record;
}


bool DboDB::InsertDboExchange(DboExchange* record)
{
	if (m_DboExchangeInsertStatement == nullptr)
	{
		m_DboExchangeInsertStatement = m_DBConnection->prepareStatement("insert into t_Exchange Values(?, ?, ?);");
	}
	SetStatementForDboExchangeRecord(m_DboExchangeInsertStatement, record);
	return m_DboExchangeInsertStatement->execute();
}
bool DboDB::TruncateDboExchange()
{
	if(m_DboExchangeTruncateStatement == nullptr)
	{
		m_DboExchangeTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_Exchange ;");
	}
	return m_DboExchangeTruncateStatement->execute();
}
bool DboDB::DeleteDboExchange(DboExchange* record)
{
	if (m_DboExchangeDeleteStatement == nullptr)
	{
		m_DboExchangeDeleteStatement = m_DBConnection->prepareStatement("delete from t_Exchange where ExchangeID = ?;");
	}
	SetStatementForDboExchangePrimaryKey(m_DboExchangeDeleteStatement, record->ExchangeID);
	return m_DboExchangeDeleteStatement->execute();
}
bool DboDB::UpdateDboExchange(DboExchange* record)
{
	if (m_DboExchangeUpdateStatement == nullptr)
	{
		m_DboExchangeUpdateStatement = m_DBConnection->prepareStatement("replace into t_Exchange Values(?, ?, ?);");
	}
	SetStatementForDboExchangeRecord(m_DboExchangeUpdateStatement, record);
	return m_DboExchangeUpdateStatement->execute();
}
DboExchange* DboDB::QueryDboExchangeFromPrimaryKey(const ExchangeIDType& ExchangeID)
{
	if (m_DboExchangePrimaryKeyStatement == nullptr)
	{
		m_DboExchangePrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_Exchange where ExchangeID = ?;");
	}
	SetStatementForDboExchangePrimaryKey(m_DboExchangePrimaryKeyStatement, ExchangeID);
	auto result = m_DboExchangePrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboExchangeRecord(result);
	}
	return nullptr;
}
void DboDB::SetStatementForDboExchangeRecord(sql::PreparedStatement* statement, DboExchange* record)
{
	statement->setString(1, record->ExchangeID);
	statement->setString(2, record->ExchangeName);
	statement->setInt(3, int(record->TradeStatus));
}
void DboDB::SetStatementForDboExchangePrimaryKey(sql::PreparedStatement* statement, const ExchangeIDType& ExchangeID)
{
	statement->setString(1, ExchangeID);
}
DboExchange* DboDB::GetDboExchangeRecord(sql::ResultSet* result)
{
	DboExchange* record = new DboExchange();
	strcpy(record->ExchangeID, result->getString(1).c_str());
	strcpy(record->ExchangeName, result->getString(2).c_str());
	record->TradeStatus = TradeStatusType(result->getInt(3));
	return record;
}


bool DboDB::InsertDboInvertor(DboInvertor* record)
{
	if (m_DboInvertorInsertStatement == nullptr)
	{
		m_DboInvertorInsertStatement = m_DBConnection->prepareStatement("insert into t_Invertor Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboInvertorRecord(m_DboInvertorInsertStatement, record);
	return m_DboInvertorInsertStatement->execute();
}
bool DboDB::TruncateDboInvertor()
{
	if(m_DboInvertorTruncateStatement == nullptr)
	{
		m_DboInvertorTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_Invertor ;");
	}
	return m_DboInvertorTruncateStatement->execute();
}
bool DboDB::DeleteDboInvertor(DboInvertor* record)
{
	if (m_DboInvertorDeleteStatement == nullptr)
	{
		m_DboInvertorDeleteStatement = m_DBConnection->prepareStatement("delete from t_Invertor where Account = ?;");
	}
	SetStatementForDboInvertorPrimaryKey(m_DboInvertorDeleteStatement, record->Account);
	return m_DboInvertorDeleteStatement->execute();
}
bool DboDB::UpdateDboInvertor(DboInvertor* record)
{
	if (m_DboInvertorUpdateStatement == nullptr)
	{
		m_DboInvertorUpdateStatement = m_DBConnection->prepareStatement("replace into t_Invertor Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboInvertorRecord(m_DboInvertorUpdateStatement, record);
	return m_DboInvertorUpdateStatement->execute();
}
DboInvertor* DboDB::QueryDboInvertorFromPrimaryKey(const AccountType& Account)
{
	if (m_DboInvertorPrimaryKeyStatement == nullptr)
	{
		m_DboInvertorPrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_Invertor where Account = ?;");
	}
	SetStatementForDboInvertorPrimaryKey(m_DboInvertorPrimaryKeyStatement, Account);
	auto result = m_DboInvertorPrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboInvertorRecord(result);
	}
	return nullptr;
}
void DboDB::SetStatementForDboInvertorRecord(sql::PreparedStatement* statement, DboInvertor* record)
{
	statement->setInt(1, record->Account);
	statement->setInt(2, int(record->AccountStatus));
	statement->setInt(3, int(record->AccountType));
	statement->setInt(4, int(record->AgentType));
	statement->setInt(5, record->CloseDate);
	statement->setInt(6, int(record->OrderType));
	statement->setInt(7, int(record->InvestorType));
	statement->setInt(8, record->IsEmployee);
	statement->setInt(9, int(record->LoginStatus));
	statement->setInt(10, record->maxOnLine);
	statement->setString(11, record->Name);
	statement->setInt(12, record->OpenDate);
	statement->setString(13, record->Password);
	statement->setInt(14, record->PasswordDate);
	statement->setInt(15, record->PasswordExpired);
	statement->setInt(16, record->PasswordValidDays);
	statement->setInt(17, record->RiskLevel);
	statement->setInt(18, int(record->TradeStatus));
	statement->setString(19, record->TwoStepCode);
	statement->setInt(20, record->TwoStepCodeTime);
	statement->setInt(21, record->TwoStepLogin);
	statement->setInt(22, record->CommissionGroupID);
	statement->setInt(23, record->MarginGroupID);
}
void DboDB::SetStatementForDboInvertorPrimaryKey(sql::PreparedStatement* statement, const AccountType& Account)
{
	statement->setInt(1, Account);
}
DboInvertor* DboDB::GetDboInvertorRecord(sql::ResultSet* result)
{
	DboInvertor* record = new DboInvertor();
	record->Account = result->getInt(1);
	record->AccountStatus = AccountStatusType(result->getInt(2));
	record->AccountType = AccountTypeType(result->getInt(3));
	record->AgentType = AgentTypeType(result->getInt(4));
	record->CloseDate = result->getInt(5);
	record->OrderType = OrderTypeType(result->getInt(6));
	record->InvestorType = InvestorTypeType(result->getInt(7));
	record->IsEmployee = result->getInt(8);
	record->LoginStatus = LoginStatusType(result->getInt(9));
	record->maxOnLine = result->getInt(10);
	strcpy(record->Name, result->getString(11).c_str());
	record->OpenDate = result->getInt(12);
	strcpy(record->Password, result->getString(13).c_str());
	record->PasswordDate = result->getInt(14);
	record->PasswordExpired = result->getInt(15);
	record->PasswordValidDays = result->getInt(16);
	record->RiskLevel = result->getInt(17);
	record->TradeStatus = TradeStatusType(result->getInt(18));
	strcpy(record->TwoStepCode, result->getString(19).c_str());
	record->TwoStepCodeTime = result->getInt(20);
	record->TwoStepLogin = result->getInt(21);
	record->CommissionGroupID = result->getInt(22);
	record->MarginGroupID = result->getInt(23);
	return record;
}


bool DboDB::InsertDboInvestorMobile(DboInvestorMobile* record)
{
	if (m_DboInvestorMobileInsertStatement == nullptr)
	{
		m_DboInvestorMobileInsertStatement = m_DBConnection->prepareStatement("insert into t_InvestorMobile Values(?, ?, ?, ?, ?);");
	}
	SetStatementForDboInvestorMobileRecord(m_DboInvestorMobileInsertStatement, record);
	return m_DboInvestorMobileInsertStatement->execute();
}
bool DboDB::TruncateDboInvestorMobile()
{
	if(m_DboInvestorMobileTruncateStatement == nullptr)
	{
		m_DboInvestorMobileTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_InvestorMobile ;");
	}
	return m_DboInvestorMobileTruncateStatement->execute();
}
bool DboDB::DeleteDboInvestorMobile(DboInvestorMobile* record)
{
	if (m_DboInvestorMobileDeleteStatement == nullptr)
	{
		m_DboInvestorMobileDeleteStatement = m_DBConnection->prepareStatement("delete from t_InvestorMobile where ID = ?;");
	}
	SetStatementForDboInvestorMobilePrimaryKey(m_DboInvestorMobileDeleteStatement, record->ID);
	return m_DboInvestorMobileDeleteStatement->execute();
}
bool DboDB::UpdateDboInvestorMobile(DboInvestorMobile* record)
{
	if (m_DboInvestorMobileUpdateStatement == nullptr)
	{
		m_DboInvestorMobileUpdateStatement = m_DBConnection->prepareStatement("replace into t_InvestorMobile Values(?, ?, ?, ?, ?);");
	}
	SetStatementForDboInvestorMobileRecord(m_DboInvestorMobileUpdateStatement, record);
	return m_DboInvestorMobileUpdateStatement->execute();
}
DboInvestorMobile* DboDB::QueryDboInvestorMobileFromPrimaryKey(const IDType& ID)
{
	if (m_DboInvestorMobilePrimaryKeyStatement == nullptr)
	{
		m_DboInvestorMobilePrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_InvestorMobile where ID = ?;");
	}
	SetStatementForDboInvestorMobilePrimaryKey(m_DboInvestorMobilePrimaryKeyStatement, ID);
	auto result = m_DboInvestorMobilePrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboInvestorMobileRecord(result);
	}
	return nullptr;
}
void DboDB::SetStatementForDboInvestorMobileRecord(sql::PreparedStatement* statement, DboInvestorMobile* record)
{
	statement->setInt(1, record->ID);
	statement->setString(2, record->Mobile);
	statement->setInt(3, record->MobileType);
	statement->setInt(4, record->Account);
	statement->setString(5, record->AreaCode);
}
void DboDB::SetStatementForDboInvestorMobilePrimaryKey(sql::PreparedStatement* statement, const IDType& ID)
{
	statement->setInt(1, ID);
}
DboInvestorMobile* DboDB::GetDboInvestorMobileRecord(sql::ResultSet* result)
{
	DboInvestorMobile* record = new DboInvestorMobile();
	record->ID = result->getInt(1);
	strcpy(record->Mobile, result->getString(2).c_str());
	record->MobileType = result->getInt(3);
	record->Account = result->getInt(4);
	strcpy(record->AreaCode, result->getString(5).c_str());
	return record;
}


bool DboDB::InsertDboInvestorParam(DboInvestorParam* record)
{
	if (m_DboInvestorParamInsertStatement == nullptr)
	{
		m_DboInvestorParamInsertStatement = m_DBConnection->prepareStatement("insert into t_InvestorParam Values(?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboInvestorParamRecord(m_DboInvestorParamInsertStatement, record);
	return m_DboInvestorParamInsertStatement->execute();
}
bool DboDB::TruncateDboInvestorParam()
{
	if(m_DboInvestorParamTruncateStatement == nullptr)
	{
		m_DboInvestorParamTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_InvestorParam ;");
	}
	return m_DboInvestorParamTruncateStatement->execute();
}
bool DboDB::DeleteDboInvestorParam(DboInvestorParam* record)
{
	if (m_DboInvestorParamDeleteStatement == nullptr)
	{
		m_DboInvestorParamDeleteStatement = m_DBConnection->prepareStatement("delete from t_InvestorParam where Account = ? and ExchangeID = ? and ProductID = ? and ContractID = ?;");
	}
	SetStatementForDboInvestorParamPrimaryKey(m_DboInvestorParamDeleteStatement, record->Account, record->ExchangeID, record->ProductID, record->ContractID);
	return m_DboInvestorParamDeleteStatement->execute();
}
bool DboDB::UpdateDboInvestorParam(DboInvestorParam* record)
{
	if (m_DboInvestorParamUpdateStatement == nullptr)
	{
		m_DboInvestorParamUpdateStatement = m_DBConnection->prepareStatement("replace into t_InvestorParam Values(?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboInvestorParamRecord(m_DboInvestorParamUpdateStatement, record);
	return m_DboInvestorParamUpdateStatement->execute();
}
DboInvestorParam* DboDB::QueryDboInvestorParamFromPrimaryKey(const AccountType& Account, const ExchangeIDType& ExchangeID, const ProductIDType& ProductID, const ContractIDType& ContractID)
{
	if (m_DboInvestorParamPrimaryKeyStatement == nullptr)
	{
		m_DboInvestorParamPrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_InvestorParam where Account = ? and ExchangeID = ? and ProductID = ? and ContractID = ?;");
	}
	SetStatementForDboInvestorParamPrimaryKey(m_DboInvestorParamPrimaryKeyStatement, Account, ExchangeID, ProductID, ContractID);
	auto result = m_DboInvestorParamPrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboInvestorParamRecord(result);
	}
	return nullptr;
}
void DboDB::SetStatementForDboInvestorParamRecord(sql::PreparedStatement* statement, DboInvestorParam* record)
{
	statement->setInt(1, record->Account);
	statement->setString(2, record->ExchangeID);
	statement->setString(3, record->ProductID);
	statement->setString(4, record->ContractID);
	statement->setInt(5, int(record->TradeStatus));
	statement->setInt(6, record->MaxPosition);
	statement->setInt(7, record->MaxOrderVolume);
	statement->setInt(8, record->CloseAdvanceDays);
	statement->setInt(9, record->ChannelID);
}
void DboDB::SetStatementForDboInvestorParamPrimaryKey(sql::PreparedStatement* statement, const AccountType& Account, const ExchangeIDType& ExchangeID, const ProductIDType& ProductID, const ContractIDType& ContractID)
{
	statement->setInt(1, Account);
	statement->setString(2, ExchangeID);
	statement->setString(3, ProductID);
	statement->setString(4, ContractID);
}
DboInvestorParam* DboDB::GetDboInvestorParamRecord(sql::ResultSet* result)
{
	DboInvestorParam* record = new DboInvestorParam();
	record->Account = result->getInt(1);
	strcpy(record->ExchangeID, result->getString(2).c_str());
	strcpy(record->ProductID, result->getString(3).c_str());
	strcpy(record->ContractID, result->getString(4).c_str());
	record->TradeStatus = TradeStatusType(result->getInt(5));
	record->MaxPosition = result->getInt(6);
	record->MaxOrderVolume = result->getInt(7);
	record->CloseAdvanceDays = result->getInt(8);
	record->ChannelID = result->getInt(9);
	return record;
}


bool DboDB::InsertDboLogin(DboLogin* record)
{
	if (m_DboLoginInsertStatement == nullptr)
	{
		m_DboLoginInsertStatement = m_DBConnection->prepareStatement("insert into t_Login Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboLoginRecord(m_DboLoginInsertStatement, record);
	return m_DboLoginInsertStatement->execute();
}
bool DboDB::TruncateDboLogin()
{
	if(m_DboLoginTruncateStatement == nullptr)
	{
		m_DboLoginTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_Login ;");
	}
	return m_DboLoginTruncateStatement->execute();
}
bool DboDB::DeleteDboLogin(DboLogin* record)
{
	if (m_DboLoginDeleteStatement == nullptr)
	{
		m_DboLoginDeleteStatement = m_DBConnection->prepareStatement("delete from t_Login where TradeDate = ? and FrontID = ? and SessionID = ?;");
	}
	SetStatementForDboLoginPrimaryKey(m_DboLoginDeleteStatement, record->TradeDate, record->FrontID, record->SessionID);
	return m_DboLoginDeleteStatement->execute();
}
bool DboDB::UpdateDboLogin(DboLogin* record)
{
	if (m_DboLoginUpdateStatement == nullptr)
	{
		m_DboLoginUpdateStatement = m_DBConnection->prepareStatement("replace into t_Login Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboLoginRecord(m_DboLoginUpdateStatement, record);
	return m_DboLoginUpdateStatement->execute();
}
DboLogin* DboDB::QueryDboLoginFromPrimaryKey(const DateType& TradeDate, const FrontIDType& FrontID, const SessionIDType& SessionID)
{
	if (m_DboLoginPrimaryKeyStatement == nullptr)
	{
		m_DboLoginPrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_Login where TradeDate = ? and FrontID = ? and SessionID = ?;");
	}
	SetStatementForDboLoginPrimaryKey(m_DboLoginPrimaryKeyStatement, TradeDate, FrontID, SessionID);
	auto result = m_DboLoginPrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboLoginRecord(result);
	}
	return nullptr;
}
void DboDB::SetStatementForDboLoginRecord(sql::PreparedStatement* statement, DboLogin* record)
{
	statement->setInt(1, record->TradeDate);
	statement->setInt(2, record->FrontID);
	statement->setInt(3, record->SessionID);
	statement->setInt(4, record->RealDate);
	statement->setInt(5, record->Account);
	statement->setString(6, record->Admin);
	statement->setInt(7, record->TwoStepLogin);
	statement->setString(8, record->Time);
	statement->setString(9, record->IP);
	statement->setString(10, record->MAC);
	statement->setString(11, record->UserProduct);
	statement->setString(12, record->Hardware);
	statement->setString(13, record->OS);
	statement->setInt(14, record->LogoutDate);
	statement->setString(15, record->LogoutTime);
}
void DboDB::SetStatementForDboLoginPrimaryKey(sql::PreparedStatement* statement, const DateType& TradeDate, const FrontIDType& FrontID, const SessionIDType& SessionID)
{
	statement->setInt(1, TradeDate);
	statement->setInt(2, FrontID);
	statement->setInt(3, SessionID);
}
DboLogin* DboDB::GetDboLoginRecord(sql::ResultSet* result)
{
	DboLogin* record = new DboLogin();
	record->TradeDate = result->getInt(1);
	record->FrontID = result->getInt(2);
	record->SessionID = result->getInt(3);
	record->RealDate = result->getInt(4);
	record->Account = result->getInt(5);
	strcpy(record->Admin, result->getString(6).c_str());
	record->TwoStepLogin = result->getInt(7);
	strcpy(record->Time, result->getString(8).c_str());
	strcpy(record->IP, result->getString(9).c_str());
	strcpy(record->MAC, result->getString(10).c_str());
	strcpy(record->UserProduct, result->getString(11).c_str());
	strcpy(record->Hardware, result->getString(12).c_str());
	strcpy(record->OS, result->getString(13).c_str());
	record->LogoutDate = result->getInt(14);
	strcpy(record->LogoutTime, result->getString(15).c_str());
	return record;
}


bool DboDB::InsertDboMargin(DboMargin* record)
{
	if (m_DboMarginInsertStatement == nullptr)
	{
		m_DboMarginInsertStatement = m_DBConnection->prepareStatement("insert into t_Margin Values(?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboMarginRecord(m_DboMarginInsertStatement, record);
	return m_DboMarginInsertStatement->execute();
}
bool DboDB::TruncateDboMargin()
{
	if(m_DboMarginTruncateStatement == nullptr)
	{
		m_DboMarginTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_Margin ;");
	}
	return m_DboMarginTruncateStatement->execute();
}
bool DboDB::DeleteDboMargin(DboMargin* record)
{
	if (m_DboMarginDeleteStatement == nullptr)
	{
		m_DboMarginDeleteStatement = m_DBConnection->prepareStatement("delete from t_Margin where GroupID = ? and Account = ? and ExchangeID = ? and ProductID = ? and ContractID = ?;");
	}
	SetStatementForDboMarginPrimaryKey(m_DboMarginDeleteStatement, record->GroupID, record->Account, record->ExchangeID, record->ProductID, record->ContractID);
	return m_DboMarginDeleteStatement->execute();
}
bool DboDB::UpdateDboMargin(DboMargin* record)
{
	if (m_DboMarginUpdateStatement == nullptr)
	{
		m_DboMarginUpdateStatement = m_DBConnection->prepareStatement("replace into t_Margin Values(?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboMarginRecord(m_DboMarginUpdateStatement, record);
	return m_DboMarginUpdateStatement->execute();
}
DboMargin* DboDB::QueryDboMarginFromPrimaryKey(const GroupIDType& GroupID, const AccountType& Account, const ExchangeIDType& ExchangeID, const ProductIDType& ProductID, const ContractIDType& ContractID)
{
	if (m_DboMarginPrimaryKeyStatement == nullptr)
	{
		m_DboMarginPrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_Margin where GroupID = ? and Account = ? and ExchangeID = ? and ProductID = ? and ContractID = ?;");
	}
	SetStatementForDboMarginPrimaryKey(m_DboMarginPrimaryKeyStatement, GroupID, Account, ExchangeID, ProductID, ContractID);
	auto result = m_DboMarginPrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboMarginRecord(result);
	}
	return nullptr;
}
void DboDB::SetStatementForDboMarginRecord(sql::PreparedStatement* statement, DboMargin* record)
{
	statement->setInt(1, record->GroupID);
	statement->setInt(2, record->Account);
	statement->setString(3, record->ExchangeID);
	statement->setString(4, record->ProductID);
	statement->setString(5, record->ContractID);
	statement->setInt(6, int(record->MarginType));
	statement->setDouble(7, record->Margin);
	statement->setDouble(8, record->MinMargin);
	statement->setDouble(9, record->SettleMargin);
}
void DboDB::SetStatementForDboMarginPrimaryKey(sql::PreparedStatement* statement, const GroupIDType& GroupID, const AccountType& Account, const ExchangeIDType& ExchangeID, const ProductIDType& ProductID, const ContractIDType& ContractID)
{
	statement->setInt(1, GroupID);
	statement->setInt(2, Account);
	statement->setString(3, ExchangeID);
	statement->setString(4, ProductID);
	statement->setString(5, ContractID);
}
DboMargin* DboDB::GetDboMarginRecord(sql::ResultSet* result)
{
	DboMargin* record = new DboMargin();
	record->GroupID = result->getInt(1);
	record->Account = result->getInt(2);
	strcpy(record->ExchangeID, result->getString(3).c_str());
	strcpy(record->ProductID, result->getString(4).c_str());
	strcpy(record->ContractID, result->getString(5).c_str());
	record->MarginType = MarginTypeType(result->getInt(6));
	record->Margin = result->getDouble(7);
	record->MinMargin = result->getDouble(8);
	record->SettleMargin = result->getDouble(9);
	return record;
}


bool DboDB::InsertDboMaxBalance(DboMaxBalance* record)
{
	if (m_DboMaxBalanceInsertStatement == nullptr)
	{
		m_DboMaxBalanceInsertStatement = m_DBConnection->prepareStatement("insert into t_MaxBalance Values(?, ?, ?);");
	}
	SetStatementForDboMaxBalanceRecord(m_DboMaxBalanceInsertStatement, record);
	return m_DboMaxBalanceInsertStatement->execute();
}
bool DboDB::TruncateDboMaxBalance()
{
	if(m_DboMaxBalanceTruncateStatement == nullptr)
	{
		m_DboMaxBalanceTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_MaxBalance ;");
	}
	return m_DboMaxBalanceTruncateStatement->execute();
}
bool DboDB::DeleteDboMaxBalance(DboMaxBalance* record)
{
	if (m_DboMaxBalanceDeleteStatement == nullptr)
	{
		m_DboMaxBalanceDeleteStatement = m_DBConnection->prepareStatement("delete from t_MaxBalance where Account = ?;");
	}
	SetStatementForDboMaxBalancePrimaryKey(m_DboMaxBalanceDeleteStatement, record->Account);
	return m_DboMaxBalanceDeleteStatement->execute();
}
bool DboDB::UpdateDboMaxBalance(DboMaxBalance* record)
{
	if (m_DboMaxBalanceUpdateStatement == nullptr)
	{
		m_DboMaxBalanceUpdateStatement = m_DBConnection->prepareStatement("replace into t_MaxBalance Values(?, ?, ?);");
	}
	SetStatementForDboMaxBalanceRecord(m_DboMaxBalanceUpdateStatement, record);
	return m_DboMaxBalanceUpdateStatement->execute();
}
DboMaxBalance* DboDB::QueryDboMaxBalanceFromPrimaryKey(const AccountType& Account)
{
	if (m_DboMaxBalancePrimaryKeyStatement == nullptr)
	{
		m_DboMaxBalancePrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_MaxBalance where Account = ?;");
	}
	SetStatementForDboMaxBalancePrimaryKey(m_DboMaxBalancePrimaryKeyStatement, Account);
	auto result = m_DboMaxBalancePrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboMaxBalanceRecord(result);
	}
	return nullptr;
}
void DboDB::SetStatementForDboMaxBalanceRecord(sql::PreparedStatement* statement, DboMaxBalance* record)
{
	statement->setInt(1, record->Account);
	statement->setDouble(2, record->MaxBalance);
	statement->setDouble(3, record->EverMaxBalance);
}
void DboDB::SetStatementForDboMaxBalancePrimaryKey(sql::PreparedStatement* statement, const AccountType& Account)
{
	statement->setInt(1, Account);
}
DboMaxBalance* DboDB::GetDboMaxBalanceRecord(sql::ResultSet* result)
{
	DboMaxBalance* record = new DboMaxBalance();
	record->Account = result->getInt(1);
	record->MaxBalance = result->getDouble(2);
	record->EverMaxBalance = result->getDouble(3);
	return record;
}


bool DboDB::InsertDboOrder(DboOrder* record)
{
	if (m_DboOrderInsertStatement == nullptr)
	{
		m_DboOrderInsertStatement = m_DBConnection->prepareStatement("insert into t_Order Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboOrderRecord(m_DboOrderInsertStatement, record);
	return m_DboOrderInsertStatement->execute();
}
bool DboDB::TruncateDboOrder()
{
	if(m_DboOrderTruncateStatement == nullptr)
	{
		m_DboOrderTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_Order ;");
	}
	return m_DboOrderTruncateStatement->execute();
}
bool DboDB::DeleteDboOrder(DboOrder* record)
{
	if (m_DboOrderDeleteStatement == nullptr)
	{
		m_DboOrderDeleteStatement = m_DBConnection->prepareStatement("delete from t_Order where TradeDate = ? and OrderID = ?;");
	}
	SetStatementForDboOrderPrimaryKey(m_DboOrderDeleteStatement, record->TradeDate, record->OrderID);
	return m_DboOrderDeleteStatement->execute();
}
bool DboDB::UpdateDboOrder(DboOrder* record)
{
	if (m_DboOrderUpdateStatement == nullptr)
	{
		m_DboOrderUpdateStatement = m_DBConnection->prepareStatement("replace into t_Order Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboOrderRecord(m_DboOrderUpdateStatement, record);
	return m_DboOrderUpdateStatement->execute();
}
DboOrder* DboDB::QueryDboOrderFromPrimaryKey(const DateTimeType& TradeDate, const OrderIDType& OrderID)
{
	if (m_DboOrderPrimaryKeyStatement == nullptr)
	{
		m_DboOrderPrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_Order where TradeDate = ? and OrderID = ?;");
	}
	SetStatementForDboOrderPrimaryKey(m_DboOrderPrimaryKeyStatement, TradeDate, OrderID);
	auto result = m_DboOrderPrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboOrderRecord(result);
	}
	return nullptr;
}
void DboDB::SetStatementForDboOrderRecord(sql::PreparedStatement* statement, DboOrder* record)
{
	statement->setString(1, record->TradeDate);
	statement->setInt(2, record->OrderID);
	statement->setInt(3, record->Account);
	statement->setString(4, record->InsertAccount);
	statement->setString(5, record->CancelAccount);
	statement->setInt(6, record->RequestID);
	statement->setString(7, record->ExchangeID);
	statement->setString(8, record->ContractID);
	statement->setInt(9, int(record->BS));
	statement->setInt(10, int(record->OC));
	statement->setInt(11, int(record->Hedge));
	statement->setInt(12, int(record->OrderPriceType));
	statement->setDouble(13, record->Price);
	statement->setInt(14, record->Volume);
	statement->setInt(15, record->TradeVolume);
	statement->setInt(16, record->Status);
	statement->setInt(17, record->RealDate);
	statement->setString(18, record->InsertTime);
	statement->setString(19, record->CancelTime);
	statement->setInt(20, int(record->ForceClose));
	statement->setString(21, record->ForceReason);
	statement->setString(22, record->OrderSysID);
	statement->setString(23, record->Remark);
	statement->setInt(24, record->FrontID);
	statement->setInt(25, record->SessionID);
	statement->setString(26, record->OrderRef);
	statement->setInt(27, int(record->TimeCondition));
	statement->setInt(28, record->GTDDate);
	statement->setInt(29, int(record->VolumeCondition));
	statement->setInt(30, record->MinVolume);
	statement->setInt(31, int(record->ContingentCondition));
	statement->setDouble(32, record->StopPrice);
	statement->setInt(33, record->SwapOrder);
	statement->setInt(34, record->ChannelID);
	statement->setString(35, record->Tag50);
	statement->setInt(36, record->CancelFrontID);
	statement->setInt(37, record->CancelSessionID);
	statement->setInt(38, record->ChannelFrontID);
	statement->setInt(39, record->ChannelSessionID);
	statement->setString(40, record->ChannelOrderRef);
	statement->setString(41, record->ChannelOrderID);
	statement->setInt(42, record->T1Valid);
	statement->setInt(43, record->Deleted);
	statement->setString(44, record->DeleteAccount);
	statement->setString(45, record->DeleteTime);
	statement->setInt(46, record->InsertDate);
	statement->setInt(47, record->InsertID);
	statement->setInt(48, record->PatchOrder);
	statement->setInt(49, record->CancelDate);
	statement->setString(50, record->UserOrderID);
}
void DboDB::SetStatementForDboOrderPrimaryKey(sql::PreparedStatement* statement, const DateTimeType& TradeDate, const OrderIDType& OrderID)
{
	statement->setString(1, TradeDate);
	statement->setInt(2, OrderID);
}
DboOrder* DboDB::GetDboOrderRecord(sql::ResultSet* result)
{
	DboOrder* record = new DboOrder();
	strcpy(record->TradeDate, result->getString(1).c_str());
	record->OrderID = result->getInt(2);
	record->Account = result->getInt(3);
	strcpy(record->InsertAccount, result->getString(4).c_str());
	strcpy(record->CancelAccount, result->getString(5).c_str());
	record->RequestID = result->getInt(6);
	strcpy(record->ExchangeID, result->getString(7).c_str());
	strcpy(record->ContractID, result->getString(8).c_str());
	record->BS = DirectionType(result->getInt(9));
	record->OC = OffsetFlagType(result->getInt(10));
	record->Hedge = HedgeFlagType(result->getInt(11));
	record->OrderPriceType = OrderPriceTypeType(result->getInt(12));
	record->Price = result->getDouble(13);
	record->Volume = result->getInt(14);
	record->TradeVolume = result->getInt(15);
	record->Status = result->getInt(16);
	record->RealDate = result->getInt(17);
	strcpy(record->InsertTime, result->getString(18).c_str());
	strcpy(record->CancelTime, result->getString(19).c_str());
	record->ForceClose = ForceCloseReasonType(result->getInt(20));
	strcpy(record->ForceReason, result->getString(21).c_str());
	strcpy(record->OrderSysID, result->getString(22).c_str());
	strcpy(record->Remark, result->getString(23).c_str());
	record->FrontID = result->getInt(24);
	record->SessionID = result->getInt(25);
	strcpy(record->OrderRef, result->getString(26).c_str());
	record->TimeCondition = TimeConditionType(result->getInt(27));
	record->GTDDate = result->getInt(28);
	record->VolumeCondition = VolumeConditionType(result->getInt(29));
	record->MinVolume = result->getInt(30);
	record->ContingentCondition = ContingentConditionType(result->getInt(31));
	record->StopPrice = result->getDouble(32);
	record->SwapOrder = result->getInt(33);
	record->ChannelID = result->getInt(34);
	strcpy(record->Tag50, result->getString(35).c_str());
	record->CancelFrontID = result->getInt(36);
	record->CancelSessionID = result->getInt(37);
	record->ChannelFrontID = result->getInt(38);
	record->ChannelSessionID = result->getInt(39);
	strcpy(record->ChannelOrderRef, result->getString(40).c_str());
	strcpy(record->ChannelOrderID, result->getString(41).c_str());
	record->T1Valid = result->getInt(42);
	record->Deleted = result->getInt(43);
	strcpy(record->DeleteAccount, result->getString(44).c_str());
	strcpy(record->DeleteTime, result->getString(45).c_str());
	record->InsertDate = result->getInt(46);
	record->InsertID = result->getInt(47);
	record->PatchOrder = result->getInt(48);
	record->CancelDate = result->getInt(49);
	strcpy(record->UserOrderID, result->getString(50).c_str());
	return record;
}


bool DboDB::InsertDboOrderCheck(DboOrderCheck* record)
{
	if (m_DboOrderCheckInsertStatement == nullptr)
	{
		m_DboOrderCheckInsertStatement = m_DBConnection->prepareStatement("insert into t_OrderCheck Values(?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboOrderCheckRecord(m_DboOrderCheckInsertStatement, record);
	return m_DboOrderCheckInsertStatement->execute();
}
bool DboDB::TruncateDboOrderCheck()
{
	if(m_DboOrderCheckTruncateStatement == nullptr)
	{
		m_DboOrderCheckTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_OrderCheck ;");
	}
	return m_DboOrderCheckTruncateStatement->execute();
}
bool DboDB::DeleteDboOrderCheck(DboOrderCheck* record)
{
	if (m_DboOrderCheckDeleteStatement == nullptr)
	{
		m_DboOrderCheckDeleteStatement = m_DBConnection->prepareStatement("delete from t_OrderCheck where ID = ?;");
	}
	SetStatementForDboOrderCheckPrimaryKey(m_DboOrderCheckDeleteStatement, record->ID);
	return m_DboOrderCheckDeleteStatement->execute();
}
bool DboDB::UpdateDboOrderCheck(DboOrderCheck* record)
{
	if (m_DboOrderCheckUpdateStatement == nullptr)
	{
		m_DboOrderCheckUpdateStatement = m_DBConnection->prepareStatement("replace into t_OrderCheck Values(?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboOrderCheckRecord(m_DboOrderCheckUpdateStatement, record);
	return m_DboOrderCheckUpdateStatement->execute();
}
DboOrderCheck* DboDB::QueryDboOrderCheckFromPrimaryKey(const IDType& ID)
{
	if (m_DboOrderCheckPrimaryKeyStatement == nullptr)
	{
		m_DboOrderCheckPrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_OrderCheck where ID = ?;");
	}
	SetStatementForDboOrderCheckPrimaryKey(m_DboOrderCheckPrimaryKeyStatement, ID);
	auto result = m_DboOrderCheckPrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboOrderCheckRecord(result);
	}
	return nullptr;
}
void DboDB::SetStatementForDboOrderCheckRecord(sql::PreparedStatement* statement, DboOrderCheck* record)
{
	statement->setInt(1, record->ID);
	statement->setInt(2, record->Account);
	statement->setInt(3, record->GroupID);
	statement->setInt(4, record->Item);
	statement->setDouble(5, record->Param1);
	statement->setString(6, record->ProductID);
	statement->setInt(7, record->Result);
	statement->setString(8, record->ExchangeID);
}
void DboDB::SetStatementForDboOrderCheckPrimaryKey(sql::PreparedStatement* statement, const IDType& ID)
{
	statement->setInt(1, ID);
}
DboOrderCheck* DboDB::GetDboOrderCheckRecord(sql::ResultSet* result)
{
	DboOrderCheck* record = new DboOrderCheck();
	record->ID = result->getInt(1);
	record->Account = result->getInt(2);
	record->GroupID = result->getInt(3);
	record->Item = result->getInt(4);
	record->Param1 = result->getDouble(5);
	strcpy(record->ProductID, result->getString(6).c_str());
	record->Result = result->getInt(7);
	strcpy(record->ExchangeID, result->getString(8).c_str());
	return record;
}


bool DboDB::InsertDboPosition(DboPosition* record)
{
	if (m_DboPositionInsertStatement == nullptr)
	{
		m_DboPositionInsertStatement = m_DBConnection->prepareStatement("insert into t_Position Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboPositionRecord(m_DboPositionInsertStatement, record);
	return m_DboPositionInsertStatement->execute();
}
bool DboDB::TruncateDboPosition()
{
	if(m_DboPositionTruncateStatement == nullptr)
	{
		m_DboPositionTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_Position ;");
	}
	return m_DboPositionTruncateStatement->execute();
}
bool DboDB::DeleteDboPosition(DboPosition* record)
{
	if (m_DboPositionDeleteStatement == nullptr)
	{
		m_DboPositionDeleteStatement = m_DBConnection->prepareStatement("delete from t_Position where Account = ? and TradeDate = ? and OpenDate = ? and TradeID = ? and OrderID = ? and CombineTradeID = ?;");
	}
	SetStatementForDboPositionPrimaryKey(m_DboPositionDeleteStatement, record->Account, record->TradeDate, record->OpenDate, record->TradeID, record->OrderID, record->CombineTradeID);
	return m_DboPositionDeleteStatement->execute();
}
bool DboDB::UpdateDboPosition(DboPosition* record)
{
	if (m_DboPositionUpdateStatement == nullptr)
	{
		m_DboPositionUpdateStatement = m_DBConnection->prepareStatement("replace into t_Position Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboPositionRecord(m_DboPositionUpdateStatement, record);
	return m_DboPositionUpdateStatement->execute();
}
DboPosition* DboDB::QueryDboPositionFromPrimaryKey(const AccountType& Account, const DateType& TradeDate, const DateType& OpenDate, const TradeIDType& TradeID, const OrderIDType& OrderID, const TradeIDType& CombineTradeID)
{
	if (m_DboPositionPrimaryKeyStatement == nullptr)
	{
		m_DboPositionPrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_Position where Account = ? and TradeDate = ? and OpenDate = ? and TradeID = ? and OrderID = ? and CombineTradeID = ?;");
	}
	SetStatementForDboPositionPrimaryKey(m_DboPositionPrimaryKeyStatement, Account, TradeDate, OpenDate, TradeID, OrderID, CombineTradeID);
	auto result = m_DboPositionPrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboPositionRecord(result);
	}
	return nullptr;
}
void DboDB::SetStatementForDboPositionRecord(sql::PreparedStatement* statement, DboPosition* record)
{
	statement->setInt(1, record->Account);
	statement->setInt(2, record->TradeDate);
	statement->setString(3, record->ExchangeID);
	statement->setString(4, record->ContractID);
	statement->setInt(5, int(record->BS));
	statement->setInt(6, int(record->Hedge));
	statement->setInt(7, record->OpenDate);
	statement->setString(8, record->TradeID);
	statement->setInt(9, record->OrderID);
	statement->setInt(10, record->LocalTradeID);
	statement->setInt(11, record->RealOpenDate);
	statement->setString(12, record->TradeTime);
	statement->setInt(13, record->Volume);
	statement->setDouble(14, record->OpenPrice);
	statement->setDouble(15, record->SettlePrice);
	statement->setDouble(16, record->LastSettle);
	statement->setDouble(17, record->VolumeMultiple);
	statement->setDouble(18, record->Margin);
	statement->setDouble(19, record->PositionProfitFloat);
	statement->setDouble(20, record->PositionProfit);
	statement->setInt(21, record->CloseVolume);
	statement->setDouble(22, record->CloseProfitFloat);
	statement->setDouble(23, record->CloseProfit);
	statement->setDouble(24, record->CloseAmount);
	statement->setInt(25, int(record->TradeType));
	statement->setString(26, record->CombineContractID);
	statement->setString(27, record->CombineTradeID);
	statement->setString(28, record->CurrencyGroup);
	statement->setString(29, record->CurrencyID);
	statement->setDouble(30, record->MinMargin);
}
void DboDB::SetStatementForDboPositionPrimaryKey(sql::PreparedStatement* statement, const AccountType& Account, const DateType& TradeDate, const DateType& OpenDate, const TradeIDType& TradeID, const OrderIDType& OrderID, const TradeIDType& CombineTradeID)
{
	statement->setInt(1, Account);
	statement->setInt(2, TradeDate);
	statement->setInt(3, OpenDate);
	statement->setString(4, TradeID);
	statement->setInt(5, OrderID);
	statement->setString(6, CombineTradeID);
}
DboPosition* DboDB::GetDboPositionRecord(sql::ResultSet* result)
{
	DboPosition* record = new DboPosition();
	record->Account = result->getInt(1);
	record->TradeDate = result->getInt(2);
	strcpy(record->ExchangeID, result->getString(3).c_str());
	strcpy(record->ContractID, result->getString(4).c_str());
	record->BS = DirectionType(result->getInt(5));
	record->Hedge = HedgeFlagType(result->getInt(6));
	record->OpenDate = result->getInt(7);
	strcpy(record->TradeID, result->getString(8).c_str());
	record->OrderID = result->getInt(9);
	record->LocalTradeID = result->getInt(10);
	record->RealOpenDate = result->getInt(11);
	strcpy(record->TradeTime, result->getString(12).c_str());
	record->Volume = result->getInt(13);
	record->OpenPrice = result->getDouble(14);
	record->SettlePrice = result->getDouble(15);
	record->LastSettle = result->getDouble(16);
	record->VolumeMultiple = result->getDouble(17);
	record->Margin = result->getDouble(18);
	record->PositionProfitFloat = result->getDouble(19);
	record->PositionProfit = result->getDouble(20);
	record->CloseVolume = result->getInt(21);
	record->CloseProfitFloat = result->getDouble(22);
	record->CloseProfit = result->getDouble(23);
	record->CloseAmount = result->getDouble(24);
	record->TradeType = TradeTypeType(result->getInt(25));
	strcpy(record->CombineContractID, result->getString(26).c_str());
	strcpy(record->CombineTradeID, result->getString(27).c_str());
	strcpy(record->CurrencyGroup, result->getString(28).c_str());
	strcpy(record->CurrencyID, result->getString(29).c_str());
	record->MinMargin = result->getDouble(30);
	return record;
}


bool DboDB::InsertDboProduct(DboProduct* record)
{
	if (m_DboProductInsertStatement == nullptr)
	{
		m_DboProductInsertStatement = m_DBConnection->prepareStatement("insert into t_Product Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboProductRecord(m_DboProductInsertStatement, record);
	return m_DboProductInsertStatement->execute();
}
bool DboDB::TruncateDboProduct()
{
	if(m_DboProductTruncateStatement == nullptr)
	{
		m_DboProductTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_Product ;");
	}
	return m_DboProductTruncateStatement->execute();
}
bool DboDB::DeleteDboProduct(DboProduct* record)
{
	if (m_DboProductDeleteStatement == nullptr)
	{
		m_DboProductDeleteStatement = m_DBConnection->prepareStatement("delete from t_Product where ExchangeID = ? and ProductID = ?;");
	}
	SetStatementForDboProductPrimaryKey(m_DboProductDeleteStatement, record->ExchangeID, record->ProductID);
	return m_DboProductDeleteStatement->execute();
}
bool DboDB::UpdateDboProduct(DboProduct* record)
{
	if (m_DboProductUpdateStatement == nullptr)
	{
		m_DboProductUpdateStatement = m_DBConnection->prepareStatement("replace into t_Product Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboProductRecord(m_DboProductUpdateStatement, record);
	return m_DboProductUpdateStatement->execute();
}
DboProduct* DboDB::QueryDboProductFromPrimaryKey(const ExchangeIDType& ExchangeID, const ProductIDType& ProductID)
{
	if (m_DboProductPrimaryKeyStatement == nullptr)
	{
		m_DboProductPrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_Product where ExchangeID = ? and ProductID = ?;");
	}
	SetStatementForDboProductPrimaryKey(m_DboProductPrimaryKeyStatement, ExchangeID, ProductID);
	auto result = m_DboProductPrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboProductRecord(result);
	}
	return nullptr;
}
void DboDB::SetStatementForDboProductRecord(sql::PreparedStatement* statement, DboProduct* record)
{
	statement->setString(1, record->ExchangeID);
	statement->setString(2, record->ProductID);
	statement->setInt(3, int(record->ProductClass));
	statement->setString(4, record->ProductName);
	statement->setString(5, record->CurrencyGroup);
	statement->setString(6, record->CurrencyID);
	statement->setDouble(7, record->VolumeMultiple);
	statement->setDouble(8, record->PriceTick);
	statement->setInt(9, record->DecimalDigits);
	statement->setInt(10, record->T1SplitTime);
	statement->setInt(11, record->MaxPosition);
	statement->setInt(12, record->MaxOrderVolume);
	statement->setInt(13, record->CloseAdvanceDays);
	statement->setInt(14, record->DeliveryMethod);
	statement->setInt(15, record->TradeChannel);
	statement->setInt(16, int(record->TradeStatus));
}
void DboDB::SetStatementForDboProductPrimaryKey(sql::PreparedStatement* statement, const ExchangeIDType& ExchangeID, const ProductIDType& ProductID)
{
	statement->setString(1, ExchangeID);
	statement->setString(2, ProductID);
}
DboProduct* DboDB::GetDboProductRecord(sql::ResultSet* result)
{
	DboProduct* record = new DboProduct();
	strcpy(record->ExchangeID, result->getString(1).c_str());
	strcpy(record->ProductID, result->getString(2).c_str());
	record->ProductClass = ProductClassType(result->getInt(3));
	strcpy(record->ProductName, result->getString(4).c_str());
	strcpy(record->CurrencyGroup, result->getString(5).c_str());
	strcpy(record->CurrencyID, result->getString(6).c_str());
	record->VolumeMultiple = result->getDouble(7);
	record->PriceTick = result->getDouble(8);
	record->DecimalDigits = result->getInt(9);
	record->T1SplitTime = result->getInt(10);
	record->MaxPosition = result->getInt(11);
	record->MaxOrderVolume = result->getInt(12);
	record->CloseAdvanceDays = result->getInt(13);
	record->DeliveryMethod = result->getInt(14);
	record->TradeChannel = result->getInt(15);
	record->TradeStatus = TradeStatusType(result->getInt(16));
	return record;
}


bool DboDB::InsertDboRiskControl(DboRiskControl* record)
{
	if (m_DboRiskControlInsertStatement == nullptr)
	{
		m_DboRiskControlInsertStatement = m_DBConnection->prepareStatement("insert into t_RiskControl Values(?, ?, ?);");
	}
	SetStatementForDboRiskControlRecord(m_DboRiskControlInsertStatement, record);
	return m_DboRiskControlInsertStatement->execute();
}
bool DboDB::TruncateDboRiskControl()
{
	if(m_DboRiskControlTruncateStatement == nullptr)
	{
		m_DboRiskControlTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_RiskControl ;");
	}
	return m_DboRiskControlTruncateStatement->execute();
}
bool DboDB::DeleteDboRiskControl(DboRiskControl* record)
{
	if (m_DboRiskControlDeleteStatement == nullptr)
	{
		m_DboRiskControlDeleteStatement = m_DBConnection->prepareStatement("delete from t_RiskControl where RiskID = ? and Account = ? and GroupID = ?;");
	}
	SetStatementForDboRiskControlPrimaryKey(m_DboRiskControlDeleteStatement, record->RiskID, record->Account, record->GroupID);
	return m_DboRiskControlDeleteStatement->execute();
}
bool DboDB::UpdateDboRiskControl(DboRiskControl* record)
{
	if (m_DboRiskControlUpdateStatement == nullptr)
	{
		m_DboRiskControlUpdateStatement = m_DBConnection->prepareStatement("replace into t_RiskControl Values(?, ?, ?);");
	}
	SetStatementForDboRiskControlRecord(m_DboRiskControlUpdateStatement, record);
	return m_DboRiskControlUpdateStatement->execute();
}
DboRiskControl* DboDB::QueryDboRiskControlFromPrimaryKey(const RiskIDType& RiskID, const AccountType& Account, const GroupIDType& GroupID)
{
	if (m_DboRiskControlPrimaryKeyStatement == nullptr)
	{
		m_DboRiskControlPrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_RiskControl where RiskID = ? and Account = ? and GroupID = ?;");
	}
	SetStatementForDboRiskControlPrimaryKey(m_DboRiskControlPrimaryKeyStatement, RiskID, Account, GroupID);
	auto result = m_DboRiskControlPrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboRiskControlRecord(result);
	}
	return nullptr;
}
void DboDB::SetStatementForDboRiskControlRecord(sql::PreparedStatement* statement, DboRiskControl* record)
{
	statement->setInt(1, record->RiskID);
	statement->setInt(2, record->Account);
	statement->setInt(3, record->GroupID);
}
void DboDB::SetStatementForDboRiskControlPrimaryKey(sql::PreparedStatement* statement, const RiskIDType& RiskID, const AccountType& Account, const GroupIDType& GroupID)
{
	statement->setInt(1, RiskID);
	statement->setInt(2, Account);
	statement->setInt(3, GroupID);
}
DboRiskControl* DboDB::GetDboRiskControlRecord(sql::ResultSet* result)
{
	DboRiskControl* record = new DboRiskControl();
	record->RiskID = result->getInt(1);
	record->Account = result->getInt(2);
	record->GroupID = result->getInt(3);
	return record;
}


bool DboDB::InsertDboRiskControlTemplate(DboRiskControlTemplate* record)
{
	if (m_DboRiskControlTemplateInsertStatement == nullptr)
	{
		m_DboRiskControlTemplateInsertStatement = m_DBConnection->prepareStatement("insert into t_RiskControlTemplate Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboRiskControlTemplateRecord(m_DboRiskControlTemplateInsertStatement, record);
	return m_DboRiskControlTemplateInsertStatement->execute();
}
bool DboDB::TruncateDboRiskControlTemplate()
{
	if(m_DboRiskControlTemplateTruncateStatement == nullptr)
	{
		m_DboRiskControlTemplateTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_RiskControlTemplate ;");
	}
	return m_DboRiskControlTemplateTruncateStatement->execute();
}
bool DboDB::DeleteDboRiskControlTemplate(DboRiskControlTemplate* record)
{
	if (m_DboRiskControlTemplateDeleteStatement == nullptr)
	{
		m_DboRiskControlTemplateDeleteStatement = m_DBConnection->prepareStatement("delete from t_RiskControlTemplate where ID = ?;");
	}
	SetStatementForDboRiskControlTemplatePrimaryKey(m_DboRiskControlTemplateDeleteStatement, record->ID);
	return m_DboRiskControlTemplateDeleteStatement->execute();
}
bool DboDB::UpdateDboRiskControlTemplate(DboRiskControlTemplate* record)
{
	if (m_DboRiskControlTemplateUpdateStatement == nullptr)
	{
		m_DboRiskControlTemplateUpdateStatement = m_DBConnection->prepareStatement("replace into t_RiskControlTemplate Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboRiskControlTemplateRecord(m_DboRiskControlTemplateUpdateStatement, record);
	return m_DboRiskControlTemplateUpdateStatement->execute();
}
DboRiskControlTemplate* DboDB::QueryDboRiskControlTemplateFromPrimaryKey(const IDType& ID)
{
	if (m_DboRiskControlTemplatePrimaryKeyStatement == nullptr)
	{
		m_DboRiskControlTemplatePrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_RiskControlTemplate where ID = ?;");
	}
	SetStatementForDboRiskControlTemplatePrimaryKey(m_DboRiskControlTemplatePrimaryKeyStatement, ID);
	auto result = m_DboRiskControlTemplatePrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboRiskControlTemplateRecord(result);
	}
	return nullptr;
}
void DboDB::SetStatementForDboRiskControlTemplateRecord(sql::PreparedStatement* statement, DboRiskControlTemplate* record)
{
	statement->setInt(1, record->ID);
	statement->setInt(2, record->RiskClass);
	statement->setInt(3, record->RiskItem);
	statement->setString(4, record->Product);
	statement->setInt(5, record->OpenDenied);
	statement->setInt(6, record->CloseDenied);
	statement->setInt(7, record->Notice);
	statement->setInt(8, record->Position);
	statement->setString(9, record->Contents);
	statement->setString(10, record->TimeFrom);
	statement->setString(11, record->TimeTo);
	statement->setDouble(12, record->Param1);
	statement->setDouble(13, record->Param2);
	statement->setDouble(14, record->Param3);
	statement->setDouble(15, record->Param4);
	statement->setDouble(16, record->Param5);
	statement->setDouble(17, record->Param6);
	statement->setString(18, record->CreateTime);
	statement->setString(19, record->UpdateTime);
}
void DboDB::SetStatementForDboRiskControlTemplatePrimaryKey(sql::PreparedStatement* statement, const IDType& ID)
{
	statement->setInt(1, ID);
}
DboRiskControlTemplate* DboDB::GetDboRiskControlTemplateRecord(sql::ResultSet* result)
{
	DboRiskControlTemplate* record = new DboRiskControlTemplate();
	record->ID = result->getInt(1);
	record->RiskClass = result->getInt(2);
	record->RiskItem = result->getInt(3);
	strcpy(record->Product, result->getString(4).c_str());
	record->OpenDenied = result->getInt(5);
	record->CloseDenied = result->getInt(6);
	record->Notice = result->getInt(7);
	record->Position = result->getInt(8);
	strcpy(record->Contents, result->getString(9).c_str());
	strcpy(record->TimeFrom, result->getString(10).c_str());
	strcpy(record->TimeTo, result->getString(11).c_str());
	record->Param1 = result->getDouble(12);
	record->Param2 = result->getDouble(13);
	record->Param3 = result->getDouble(14);
	record->Param4 = result->getDouble(15);
	record->Param5 = result->getDouble(16);
	record->Param6 = result->getDouble(17);
	strcpy(record->CreateTime, result->getString(18).c_str());
	strcpy(record->UpdateTime, result->getString(19).c_str());
	return record;
}


bool DboDB::InsertDboSession(DboSession* record)
{
	if (m_DboSessionInsertStatement == nullptr)
	{
		m_DboSessionInsertStatement = m_DBConnection->prepareStatement("insert into t_Session Values(?, ?, ?, ?);");
	}
	SetStatementForDboSessionRecord(m_DboSessionInsertStatement, record);
	return m_DboSessionInsertStatement->execute();
}
bool DboDB::TruncateDboSession()
{
	if(m_DboSessionTruncateStatement == nullptr)
	{
		m_DboSessionTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_Session ;");
	}
	return m_DboSessionTruncateStatement->execute();
}
bool DboDB::DeleteDboSession(DboSession* record)
{
	if (m_DboSessionDeleteStatement == nullptr)
	{
		m_DboSessionDeleteStatement = m_DBConnection->prepareStatement("delete from t_Session where TradeDate = ? and Account = ? and FrontID = ? and SessionID = ?;");
	}
	SetStatementForDboSessionPrimaryKey(m_DboSessionDeleteStatement, record->TradeDate, record->Account, record->FrontID, record->SessionID);
	return m_DboSessionDeleteStatement->execute();
}
bool DboDB::UpdateDboSession(DboSession* record)
{
	if (m_DboSessionUpdateStatement == nullptr)
	{
		m_DboSessionUpdateStatement = m_DBConnection->prepareStatement("replace into t_Session Values(?, ?, ?, ?);");
	}
	SetStatementForDboSessionRecord(m_DboSessionUpdateStatement, record);
	return m_DboSessionUpdateStatement->execute();
}
DboSession* DboDB::QueryDboSessionFromPrimaryKey(const DateType& TradeDate, const AccountType& Account, const FrontIDType& FrontID, const SessionIDType& SessionID)
{
	if (m_DboSessionPrimaryKeyStatement == nullptr)
	{
		m_DboSessionPrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_Session where TradeDate = ? and Account = ? and FrontID = ? and SessionID = ?;");
	}
	SetStatementForDboSessionPrimaryKey(m_DboSessionPrimaryKeyStatement, TradeDate, Account, FrontID, SessionID);
	auto result = m_DboSessionPrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboSessionRecord(result);
	}
	return nullptr;
}
void DboDB::SetStatementForDboSessionRecord(sql::PreparedStatement* statement, DboSession* record)
{
	statement->setInt(1, record->TradeDate);
	statement->setInt(2, record->Account);
	statement->setInt(3, record->FrontID);
	statement->setInt(4, record->SessionID);
}
void DboDB::SetStatementForDboSessionPrimaryKey(sql::PreparedStatement* statement, const DateType& TradeDate, const AccountType& Account, const FrontIDType& FrontID, const SessionIDType& SessionID)
{
	statement->setInt(1, TradeDate);
	statement->setInt(2, Account);
	statement->setInt(3, FrontID);
	statement->setInt(4, SessionID);
}
DboSession* DboDB::GetDboSessionRecord(sql::ResultSet* result)
{
	DboSession* record = new DboSession();
	record->TradeDate = result->getInt(1);
	record->Account = result->getInt(2);
	record->FrontID = result->getInt(3);
	record->SessionID = result->getInt(4);
	return record;
}


bool DboDB::InsertDboSettlePrice(DboSettlePrice* record)
{
	if (m_DboSettlePriceInsertStatement == nullptr)
	{
		m_DboSettlePriceInsertStatement = m_DBConnection->prepareStatement("insert into t_SettlePrice Values(?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboSettlePriceRecord(m_DboSettlePriceInsertStatement, record);
	return m_DboSettlePriceInsertStatement->execute();
}
bool DboDB::TruncateDboSettlePrice()
{
	if(m_DboSettlePriceTruncateStatement == nullptr)
	{
		m_DboSettlePriceTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_SettlePrice ;");
	}
	return m_DboSettlePriceTruncateStatement->execute();
}
bool DboDB::DeleteDboSettlePrice(DboSettlePrice* record)
{
	if (m_DboSettlePriceDeleteStatement == nullptr)
	{
		m_DboSettlePriceDeleteStatement = m_DBConnection->prepareStatement("delete from t_SettlePrice where ExchangeID = ? and ContractID = ? and TradingDay = ?;");
	}
	SetStatementForDboSettlePricePrimaryKey(m_DboSettlePriceDeleteStatement, record->ExchangeID, record->ContractID, record->TradingDay);
	return m_DboSettlePriceDeleteStatement->execute();
}
bool DboDB::UpdateDboSettlePrice(DboSettlePrice* record)
{
	if (m_DboSettlePriceUpdateStatement == nullptr)
	{
		m_DboSettlePriceUpdateStatement = m_DBConnection->prepareStatement("replace into t_SettlePrice Values(?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboSettlePriceRecord(m_DboSettlePriceUpdateStatement, record);
	return m_DboSettlePriceUpdateStatement->execute();
}
DboSettlePrice* DboDB::QueryDboSettlePriceFromPrimaryKey(const ExchangeIDType& ExchangeID, const ContractIDType& ContractID, const DateType& TradingDay)
{
	if (m_DboSettlePricePrimaryKeyStatement == nullptr)
	{
		m_DboSettlePricePrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_SettlePrice where ExchangeID = ? and ContractID = ? and TradingDay = ?;");
	}
	SetStatementForDboSettlePricePrimaryKey(m_DboSettlePricePrimaryKeyStatement, ExchangeID, ContractID, TradingDay);
	auto result = m_DboSettlePricePrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboSettlePriceRecord(result);
	}
	return nullptr;
}
void DboDB::SetStatementForDboSettlePriceRecord(sql::PreparedStatement* statement, DboSettlePrice* record)
{
	statement->setString(1, record->ExchangeID);
	statement->setString(2, record->ContractID);
	statement->setInt(3, record->TradingDay);
	statement->setDouble(4, record->SettlePrice);
	statement->setDouble(5, record->PrevSettlePrice);
	statement->setDouble(6, record->Margin);
	statement->setDouble(7, record->MinMargin);
}
void DboDB::SetStatementForDboSettlePricePrimaryKey(sql::PreparedStatement* statement, const ExchangeIDType& ExchangeID, const ContractIDType& ContractID, const DateType& TradingDay)
{
	statement->setString(1, ExchangeID);
	statement->setString(2, ContractID);
	statement->setInt(3, TradingDay);
}
DboSettlePrice* DboDB::GetDboSettlePriceRecord(sql::ResultSet* result)
{
	DboSettlePrice* record = new DboSettlePrice();
	strcpy(record->ExchangeID, result->getString(1).c_str());
	strcpy(record->ContractID, result->getString(2).c_str());
	record->TradingDay = result->getInt(3);
	record->SettlePrice = result->getDouble(4);
	record->PrevSettlePrice = result->getDouble(5);
	record->Margin = result->getDouble(6);
	record->MinMargin = result->getDouble(7);
	return record;
}


bool DboDB::InsertDboSummary(DboSummary* record)
{
	if (m_DboSummaryInsertStatement == nullptr)
	{
		m_DboSummaryInsertStatement = m_DBConnection->prepareStatement("insert into t_Summary Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboSummaryRecord(m_DboSummaryInsertStatement, record);
	return m_DboSummaryInsertStatement->execute();
}
bool DboDB::TruncateDboSummary()
{
	if(m_DboSummaryTruncateStatement == nullptr)
	{
		m_DboSummaryTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_Summary ;");
	}
	return m_DboSummaryTruncateStatement->execute();
}
bool DboDB::DeleteDboSummary(DboSummary* record)
{
	if (m_DboSummaryDeleteStatement == nullptr)
	{
		m_DboSummaryDeleteStatement = m_DBConnection->prepareStatement("delete from t_Summary where Account = ? and TradeDate = ? and CurrencyGroup = ? and CurrencyID = ?;");
	}
	SetStatementForDboSummaryPrimaryKey(m_DboSummaryDeleteStatement, record->Account, record->TradeDate, record->CurrencyGroup, record->CurrencyID);
	return m_DboSummaryDeleteStatement->execute();
}
bool DboDB::UpdateDboSummary(DboSummary* record)
{
	if (m_DboSummaryUpdateStatement == nullptr)
	{
		m_DboSummaryUpdateStatement = m_DBConnection->prepareStatement("replace into t_Summary Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboSummaryRecord(m_DboSummaryUpdateStatement, record);
	return m_DboSummaryUpdateStatement->execute();
}
DboSummary* DboDB::QueryDboSummaryFromPrimaryKey(const AccountType& Account, const DateType& TradeDate, const CurrencyGroupType& CurrencyGroup, const CurrencyIDType& CurrencyID)
{
	if (m_DboSummaryPrimaryKeyStatement == nullptr)
	{
		m_DboSummaryPrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_Summary where Account = ? and TradeDate = ? and CurrencyGroup = ? and CurrencyID = ?;");
	}
	SetStatementForDboSummaryPrimaryKey(m_DboSummaryPrimaryKeyStatement, Account, TradeDate, CurrencyGroup, CurrencyID);
	auto result = m_DboSummaryPrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboSummaryRecord(result);
	}
	return nullptr;
}
void DboDB::SetStatementForDboSummaryRecord(sql::PreparedStatement* statement, DboSummary* record)
{
	statement->setInt(1, record->Account);
	statement->setInt(2, record->TradeDate);
	statement->setString(3, record->CurrencyGroup);
	statement->setString(4, record->CurrencyID);
	statement->setDouble(5, record->CurrencyRate);
	statement->setDouble(6, record->Balance);
	statement->setDouble(7, record->LastBalance);
	statement->setDouble(8, record->Available);
	statement->setDouble(9, record->Margin);
	statement->setDouble(10, record->Commission);
	statement->setDouble(11, record->PositionProfitFloat);
	statement->setDouble(12, record->PositionProfit);
	statement->setDouble(13, record->CloseProfitFloat);
	statement->setDouble(14, record->CloseProfit);
	statement->setDouble(15, record->Deposit);
	statement->setDouble(16, record->Withdraw);
	statement->setDouble(17, record->Risk);
	statement->setDouble(18, record->CloseBalance);
	statement->setDouble(19, record->CashInOutTotal);
	statement->setDouble(20, record->BalanceFloat);
	statement->setDouble(21, record->LastBalanceFloat);
	statement->setDouble(22, record->Credit);
	statement->setDouble(23, record->MinMargin);
	statement->setInt(24, record->Stage);
}
void DboDB::SetStatementForDboSummaryPrimaryKey(sql::PreparedStatement* statement, const AccountType& Account, const DateType& TradeDate, const CurrencyGroupType& CurrencyGroup, const CurrencyIDType& CurrencyID)
{
	statement->setInt(1, Account);
	statement->setInt(2, TradeDate);
	statement->setString(3, CurrencyGroup);
	statement->setString(4, CurrencyID);
}
DboSummary* DboDB::GetDboSummaryRecord(sql::ResultSet* result)
{
	DboSummary* record = new DboSummary();
	record->Account = result->getInt(1);
	record->TradeDate = result->getInt(2);
	strcpy(record->CurrencyGroup, result->getString(3).c_str());
	strcpy(record->CurrencyID, result->getString(4).c_str());
	record->CurrencyRate = result->getDouble(5);
	record->Balance = result->getDouble(6);
	record->LastBalance = result->getDouble(7);
	record->Available = result->getDouble(8);
	record->Margin = result->getDouble(9);
	record->Commission = result->getDouble(10);
	record->PositionProfitFloat = result->getDouble(11);
	record->PositionProfit = result->getDouble(12);
	record->CloseProfitFloat = result->getDouble(13);
	record->CloseProfit = result->getDouble(14);
	record->Deposit = result->getDouble(15);
	record->Withdraw = result->getDouble(16);
	record->Risk = result->getDouble(17);
	record->CloseBalance = result->getDouble(18);
	record->CashInOutTotal = result->getDouble(19);
	record->BalanceFloat = result->getDouble(20);
	record->LastBalanceFloat = result->getDouble(21);
	record->Credit = result->getDouble(22);
	record->MinMargin = result->getDouble(23);
	record->Stage = result->getInt(24);
	return record;
}


bool DboDB::InsertDboSysParam(DboSysParam* record)
{
	if (m_DboSysParamInsertStatement == nullptr)
	{
		m_DboSysParamInsertStatement = m_DBConnection->prepareStatement("insert into t_SysParam Values(?, ?);");
	}
	SetStatementForDboSysParamRecord(m_DboSysParamInsertStatement, record);
	return m_DboSysParamInsertStatement->execute();
}
bool DboDB::TruncateDboSysParam()
{
	if(m_DboSysParamTruncateStatement == nullptr)
	{
		m_DboSysParamTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_SysParam ;");
	}
	return m_DboSysParamTruncateStatement->execute();
}
bool DboDB::DeleteDboSysParam(DboSysParam* record)
{
	if (m_DboSysParamDeleteStatement == nullptr)
	{
		m_DboSysParamDeleteStatement = m_DBConnection->prepareStatement("delete from t_SysParam where Param = ?;");
	}
	SetStatementForDboSysParamPrimaryKey(m_DboSysParamDeleteStatement, record->Param);
	return m_DboSysParamDeleteStatement->execute();
}
bool DboDB::UpdateDboSysParam(DboSysParam* record)
{
	if (m_DboSysParamUpdateStatement == nullptr)
	{
		m_DboSysParamUpdateStatement = m_DBConnection->prepareStatement("replace into t_SysParam Values(?, ?);");
	}
	SetStatementForDboSysParamRecord(m_DboSysParamUpdateStatement, record);
	return m_DboSysParamUpdateStatement->execute();
}
DboSysParam* DboDB::QueryDboSysParamFromPrimaryKey(const SysParamType& Param)
{
	if (m_DboSysParamPrimaryKeyStatement == nullptr)
	{
		m_DboSysParamPrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_SysParam where Param = ?;");
	}
	SetStatementForDboSysParamPrimaryKey(m_DboSysParamPrimaryKeyStatement, Param);
	auto result = m_DboSysParamPrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboSysParamRecord(result);
	}
	return nullptr;
}
void DboDB::SetStatementForDboSysParamRecord(sql::PreparedStatement* statement, DboSysParam* record)
{
	statement->setString(1, record->Param);
	statement->setString(2, record->Value);
}
void DboDB::SetStatementForDboSysParamPrimaryKey(sql::PreparedStatement* statement, const SysParamType& Param)
{
	statement->setString(1, Param);
}
DboSysParam* DboDB::GetDboSysParamRecord(sql::ResultSet* result)
{
	DboSysParam* record = new DboSysParam();
	strcpy(record->Param, result->getString(1).c_str());
	strcpy(record->Value, result->getString(2).c_str());
	return record;
}


bool DboDB::InsertDboTrade(DboTrade* record)
{
	if (m_DboTradeInsertStatement == nullptr)
	{
		m_DboTradeInsertStatement = m_DBConnection->prepareStatement("insert into t_Trade Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboTradeRecord(m_DboTradeInsertStatement, record);
	return m_DboTradeInsertStatement->execute();
}
bool DboDB::TruncateDboTrade()
{
	if(m_DboTradeTruncateStatement == nullptr)
	{
		m_DboTradeTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_Trade ;");
	}
	return m_DboTradeTruncateStatement->execute();
}
bool DboDB::DeleteDboTrade(DboTrade* record)
{
	if (m_DboTradeDeleteStatement == nullptr)
	{
		m_DboTradeDeleteStatement = m_DBConnection->prepareStatement("delete from t_Trade where TradeDate = ? and OrderID = ? and TradeID = ?;");
	}
	SetStatementForDboTradePrimaryKey(m_DboTradeDeleteStatement, record->TradeDate, record->OrderID, record->TradeID);
	return m_DboTradeDeleteStatement->execute();
}
bool DboDB::UpdateDboTrade(DboTrade* record)
{
	if (m_DboTradeUpdateStatement == nullptr)
	{
		m_DboTradeUpdateStatement = m_DBConnection->prepareStatement("replace into t_Trade Values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboTradeRecord(m_DboTradeUpdateStatement, record);
	return m_DboTradeUpdateStatement->execute();
}
DboTrade* DboDB::QueryDboTradeFromPrimaryKey(const DateType& TradeDate, const OrderIDType& OrderID, const TradeIDType& TradeID)
{
	if (m_DboTradePrimaryKeyStatement == nullptr)
	{
		m_DboTradePrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_Trade where TradeDate = ? and OrderID = ? and TradeID = ?;");
	}
	SetStatementForDboTradePrimaryKey(m_DboTradePrimaryKeyStatement, TradeDate, OrderID, TradeID);
	auto result = m_DboTradePrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboTradeRecord(result);
	}
	return nullptr;
}
void DboDB::SetStatementForDboTradeRecord(sql::PreparedStatement* statement, DboTrade* record)
{
	statement->setInt(1, record->TradeDate);
	statement->setInt(2, record->OrderID);
	statement->setString(3, record->TradeID);
	statement->setInt(4, record->Account);
	statement->setInt(5, record->RequestID);
	statement->setString(6, record->ExchangeID);
	statement->setString(7, record->ContractID);
	statement->setInt(8, int(record->BS));
	statement->setInt(9, int(record->Hedge));
	statement->setDouble(10, record->Price);
	statement->setInt(11, record->Volume);
	statement->setDouble(12, record->Turnover);
	statement->setInt(13, record->RealDate);
	statement->setString(14, record->TradeTime);
	statement->setString(15, record->OrderSysID);
	statement->setDouble(16, record->Commission);
	statement->setDouble(17, record->CloseProfitFloat);
	statement->setDouble(18, record->CloseProfit);
	statement->setString(19, record->OrderRef);
	statement->setInt(20, int(record->TradeType));
	statement->setString(21, record->CombineContractID);
	statement->setInt(22, record->LocalTradeID);
	statement->setString(23, record->CurrencyGroup);
	statement->setString(24, record->CurrencyID);
	statement->setString(25, record->ExchangeTradeID);
	statement->setInt(26, record->OpenVolume);
	statement->setInt(27, record->CloseVolume);
	statement->setInt(28, record->Deleted);
	statement->setString(29, record->DeleteAccount);
	statement->setString(30, record->DeleteTime);
	statement->setDouble(31, record->ExchangeFee);
	statement->setDouble(32, record->Levy);
	statement->setString(33, record->PatchAccount);
}
void DboDB::SetStatementForDboTradePrimaryKey(sql::PreparedStatement* statement, const DateType& TradeDate, const OrderIDType& OrderID, const TradeIDType& TradeID)
{
	statement->setInt(1, TradeDate);
	statement->setInt(2, OrderID);
	statement->setString(3, TradeID);
}
DboTrade* DboDB::GetDboTradeRecord(sql::ResultSet* result)
{
	DboTrade* record = new DboTrade();
	record->TradeDate = result->getInt(1);
	record->OrderID = result->getInt(2);
	strcpy(record->TradeID, result->getString(3).c_str());
	record->Account = result->getInt(4);
	record->RequestID = result->getInt(5);
	strcpy(record->ExchangeID, result->getString(6).c_str());
	strcpy(record->ContractID, result->getString(7).c_str());
	record->BS = DirectionType(result->getInt(8));
	record->Hedge = HedgeFlagType(result->getInt(9));
	record->Price = result->getDouble(10);
	record->Volume = result->getInt(11);
	record->Turnover = result->getDouble(12);
	record->RealDate = result->getInt(13);
	strcpy(record->TradeTime, result->getString(14).c_str());
	strcpy(record->OrderSysID, result->getString(15).c_str());
	record->Commission = result->getDouble(16);
	record->CloseProfitFloat = result->getDouble(17);
	record->CloseProfit = result->getDouble(18);
	strcpy(record->OrderRef, result->getString(19).c_str());
	record->TradeType = TradeTypeType(result->getInt(20));
	strcpy(record->CombineContractID, result->getString(21).c_str());
	record->LocalTradeID = result->getInt(22);
	strcpy(record->CurrencyGroup, result->getString(23).c_str());
	strcpy(record->CurrencyID, result->getString(24).c_str());
	strcpy(record->ExchangeTradeID, result->getString(25).c_str());
	record->OpenVolume = result->getInt(26);
	record->CloseVolume = result->getInt(27);
	record->Deleted = result->getInt(28);
	strcpy(record->DeleteAccount, result->getString(29).c_str());
	strcpy(record->DeleteTime, result->getString(30).c_str());
	record->ExchangeFee = result->getDouble(31);
	record->Levy = result->getDouble(32);
	strcpy(record->PatchAccount, result->getString(33).c_str());
	return record;
}


bool DboDB::InsertDboTradeChannel(DboTradeChannel* record)
{
	if (m_DboTradeChannelInsertStatement == nullptr)
	{
		m_DboTradeChannelInsertStatement = m_DBConnection->prepareStatement("insert into t_TradeChannel Values(?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboTradeChannelRecord(m_DboTradeChannelInsertStatement, record);
	return m_DboTradeChannelInsertStatement->execute();
}
bool DboDB::TruncateDboTradeChannel()
{
	if(m_DboTradeChannelTruncateStatement == nullptr)
	{
		m_DboTradeChannelTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_TradeChannel ;");
	}
	return m_DboTradeChannelTruncateStatement->execute();
}
bool DboDB::DeleteDboTradeChannel(DboTradeChannel* record)
{
	if (m_DboTradeChannelDeleteStatement == nullptr)
	{
		m_DboTradeChannelDeleteStatement = m_DBConnection->prepareStatement("delete from t_TradeChannel where ID = ?;");
	}
	SetStatementForDboTradeChannelPrimaryKey(m_DboTradeChannelDeleteStatement, record->ID);
	return m_DboTradeChannelDeleteStatement->execute();
}
bool DboDB::UpdateDboTradeChannel(DboTradeChannel* record)
{
	if (m_DboTradeChannelUpdateStatement == nullptr)
	{
		m_DboTradeChannelUpdateStatement = m_DBConnection->prepareStatement("replace into t_TradeChannel Values(?, ?, ?, ?, ?, ?);");
	}
	SetStatementForDboTradeChannelRecord(m_DboTradeChannelUpdateStatement, record);
	return m_DboTradeChannelUpdateStatement->execute();
}
DboTradeChannel* DboDB::QueryDboTradeChannelFromPrimaryKey(const IDType& ID)
{
	if (m_DboTradeChannelPrimaryKeyStatement == nullptr)
	{
		m_DboTradeChannelPrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_TradeChannel where ID = ?;");
	}
	SetStatementForDboTradeChannelPrimaryKey(m_DboTradeChannelPrimaryKeyStatement, ID);
	auto result = m_DboTradeChannelPrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboTradeChannelRecord(result);
	}
	return nullptr;
}
void DboDB::SetStatementForDboTradeChannelRecord(sql::PreparedStatement* statement, DboTradeChannel* record)
{
	statement->setInt(1, record->ID);
	statement->setString(2, record->Name);
	statement->setString(3, record->UserID);
	statement->setString(4, record->Password);
	statement->setString(5, record->IP);
	statement->setInt(6, record->Port);
}
void DboDB::SetStatementForDboTradeChannelPrimaryKey(sql::PreparedStatement* statement, const IDType& ID)
{
	statement->setInt(1, ID);
}
DboTradeChannel* DboDB::GetDboTradeChannelRecord(sql::ResultSet* result)
{
	DboTradeChannel* record = new DboTradeChannel();
	record->ID = result->getInt(1);
	strcpy(record->Name, result->getString(2).c_str());
	strcpy(record->UserID, result->getString(3).c_str());
	strcpy(record->Password, result->getString(4).c_str());
	strcpy(record->IP, result->getString(5).c_str());
	record->Port = result->getInt(6);
	return record;
}


bool DboDB::InsertDboTrustedNetAddress(DboTrustedNetAddress* record)
{
	if (m_DboTrustedNetAddressInsertStatement == nullptr)
	{
		m_DboTrustedNetAddressInsertStatement = m_DBConnection->prepareStatement("insert into t_TrustedNetAddress Values(?, ?, ?);");
	}
	SetStatementForDboTrustedNetAddressRecord(m_DboTrustedNetAddressInsertStatement, record);
	return m_DboTrustedNetAddressInsertStatement->execute();
}
bool DboDB::TruncateDboTrustedNetAddress()
{
	if(m_DboTrustedNetAddressTruncateStatement == nullptr)
	{
		m_DboTrustedNetAddressTruncateStatement = m_DBConnection->prepareStatement("truncate  table t_TrustedNetAddress ;");
	}
	return m_DboTrustedNetAddressTruncateStatement->execute();
}
bool DboDB::DeleteDboTrustedNetAddress(DboTrustedNetAddress* record)
{
	if (m_DboTrustedNetAddressDeleteStatement == nullptr)
	{
		m_DboTrustedNetAddressDeleteStatement = m_DBConnection->prepareStatement("delete from t_TrustedNetAddress where Account = ? and IP = ? and MAC = ?;");
	}
	SetStatementForDboTrustedNetAddressPrimaryKey(m_DboTrustedNetAddressDeleteStatement, record->Account, record->IP, record->MAC);
	return m_DboTrustedNetAddressDeleteStatement->execute();
}
bool DboDB::UpdateDboTrustedNetAddress(DboTrustedNetAddress* record)
{
	if (m_DboTrustedNetAddressUpdateStatement == nullptr)
	{
		m_DboTrustedNetAddressUpdateStatement = m_DBConnection->prepareStatement("replace into t_TrustedNetAddress Values(?, ?, ?);");
	}
	SetStatementForDboTrustedNetAddressRecord(m_DboTrustedNetAddressUpdateStatement, record);
	return m_DboTrustedNetAddressUpdateStatement->execute();
}
DboTrustedNetAddress* DboDB::QueryDboTrustedNetAddressFromPrimaryKey(const AccountType& Account, const IPType& IP, const MACType& MAC)
{
	if (m_DboTrustedNetAddressPrimaryKeyStatement == nullptr)
	{
		m_DboTrustedNetAddressPrimaryKeyStatement = m_DBConnection->prepareStatement("select * from t_TrustedNetAddress where Account = ? and IP = ? and MAC = ?;");
	}
	SetStatementForDboTrustedNetAddressPrimaryKey(m_DboTrustedNetAddressPrimaryKeyStatement, Account, IP, MAC);
	auto result = m_DboTrustedNetAddressPrimaryKeyStatement->executeQuery();
	if (result->next())
	{
		return GetDboTrustedNetAddressRecord(result);
	}
	return nullptr;
}
void DboDB::SetStatementForDboTrustedNetAddressRecord(sql::PreparedStatement* statement, DboTrustedNetAddress* record)
{
	statement->setInt(1, record->Account);
	statement->setString(2, record->IP);
	statement->setString(3, record->MAC);
}
void DboDB::SetStatementForDboTrustedNetAddressPrimaryKey(sql::PreparedStatement* statement, const AccountType& Account, const IPType& IP, const MACType& MAC)
{
	statement->setInt(1, Account);
	statement->setString(2, IP);
	statement->setString(3, MAC);
}
DboTrustedNetAddress* DboDB::GetDboTrustedNetAddressRecord(sql::ResultSet* result)
{
	DboTrustedNetAddress* record = new DboTrustedNetAddress();
	record->Account = result->getInt(1);
	strcpy(record->IP, result->getString(2).c_str());
	strcpy(record->MAC, result->getString(3).c_str());
	return record;
}

