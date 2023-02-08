
CREATE TABLE IF NOT EXISTS `t_AdjustedSettlementDay` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `TradingDay` int,
  `AuditTime` datetime,
  `OperationTime` datetime,
  `Remark` char(64),
  `Status` int,
  `SettlementDay` int,
  `ExchangeID` char(8),
  `OperatorID` char(16),
  `AuditorID` char(16),

  PRIMARY KEY(ID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_AuthorizedSoftware` (
  `Software` char(12),
  `AuthCode` char(16),
  `Account` int,

  INDEX Account(AuthCode, Account),
  UNIQUE AuthCode(Software, AuthCode),
  PRIMARY KEY(Account, Software)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_Bank` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `Code` char(10),
  `Nation` int,
  `Name` char(32),

  PRIMARY KEY(ID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_Close` (
  `OpenDate` int,
  `OpenOrderID` int,
  `OpenTradeID` char(32),
  `CloseDate` int,
  `CloseOrderID` int,
  `CloseTradeID` char(32),
  `Account` int,
  `ExchangeID` char(8),
  `ContractID` char(32),
  `BS` char(1),
  `Hedge` char(1),
  `OpenPrice` double,
  `LastSettle` double,
  `ClosePrice` double,
  `Volume` int,
  `RealOpenDate` int,
  `RealCloseDate` int,
  `OpenTime` char(9),
  `CloseTime` char(9),
  `CloseProfitFloat` double,
  `CloseProfit` double,
  `CurrencyGroup` char(5),
  `CurrencyID` char(3),

  PRIMARY KEY(OpenDate, OpenOrderID, OpenTradeID, CloseDate, CloseOrderID, CloseTradeID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_Commission` (
  `GroupID` int,
  `Account` int,
  `ExchangeID` char(8),
  `ProductID` char(16),
  `ContractID` char(32),
  `OpenByVolume` double,
  `CloseByVolume` double,
  `OpenByMoney` double,
  `CloseByMoney` double,

  PRIMARY KEY(GroupID, Account, ExchangeID, ProductID, ContractID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_Contract` (
  `ExchangeID` char(8),
  `ProductID` char(16),
  `ContractID` char(32),
  `ContractName` char(32),
  `DeliveryYear` int,
  `DeliveryMonth` int,
  `OpenDate` int,
  `ExpireDate` int,
  `FirstNoticeDate` int,
  `StartDelivDate` int,
  `EndDelivDate` int,
  `TradeStatus` char(1),
  `Margin` double,
  `MinMargin` double,

  PRIMARY KEY(ExchangeID, ContractID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_CreditAmount` (
  `ID` int,
  `AuditTime` datetime,
  `OperationTime` datetime,
  `Remark` char(64),
  `Status` int,
  `Amount` double,
  `CurrencyGroup` char(5),
  `Direction` char(1),
  `TradingDay` int,
  `OperatorID` char(16),
  `Account` int,
  `CurrencyID` char(3),
  `AuditorID` char(16),
  `Flag` int,

  PRIMARY KEY(ID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_CurrencyRate` (
  `CurrencyGroup` char(5),
  `CurrencyID` char(3),
  `TradeRate` double,
  `SettlementRate` double,
  `IsBaseCurrency` int,

  PRIMARY KEY(CurrencyGroup, CurrencyID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_CurrencyRateRecord` (
  `ID` int,
  `AuditTime` datetime,
  `OperationTime` datetime,
  `Remark` char(64),
  `Status` int,
  `CurrencyGroup` char(5),
  `IsBaseCurrency` int,
  `SettlementRate` double,
  `TradeRate` double,
  `OperatorID` char(16),
  `CurrencyID` char(3),
  `AuditorID` char(16),

  PRIMARY KEY(ID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_DepositWithdrawal` (
  `ID` int,
  `AuditTime` char(9),
  `OperationTime` char(9),
  `Remark` char(64),
  `Status` int,
  `Amount` double,
  `BankCardID` char(64),
  `CurrencyID` char(3),
  `CurrencyGroup` char(5),
  `Direction` char(1),
  `Method` int,
  `TradingDay` int,
  `OperatorID` char(16),
  `Account` int,
  `AuditorID` char(16),
  `Flag` int,

  PRIMARY KEY(ID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_Exchange` (
  `ExchangeID` char(8),
  `ExchangeName` char(8),
  `TradeStatus` char(1),

  PRIMARY KEY(ExchangeID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_Group` (
  `ID` int,
  `Name` char(64),
  `Type` int,

  PRIMARY KEY(ID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_Invertor` (
  `Account` int,
  `AccountStatus` char(1),
  `AccountType` char(1),
  `AgentType` char(1),
  `CloseDate` int,
  `OrderType` char(1),
  `InvestorType` char(1),
  `IsEmployee` int,
  `LoginStatus` char(1),
  `maxOnLine` int,
  `Name` char(64),
  `OpenDate` int,
  `Password` char(36),
  `PasswordDate` int,
  `PasswordExpired` int,
  `PasswordValidDays` int,
  `RiskLevel` int,
  `TradeStatus` char(1),
  `TwoStepCode` char(8),
  `TwoStepCodeTime` int,
  `TwoStepLogin` int,
  `CommissionGroupID` int,
  `MarginGroupID` int,

  PRIMARY KEY(Account)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_InvestorAddress` (
  `ID` int,
  `Address` varchar(256),
  `Nation` int,
  `PostCode` char(16),
  `AddressType` int,
  `Account` int,

  PRIMARY KEY(ID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_InvestorBankCard` (
  `ID` int,
  `Branch` char(64),
  `CardNo` char(24),
  `CurrencyID` char(3),
  `DomesticAccountType` int,
  `DomesticCurrencyType` int,
  `OpeningID` int,
  `Owner` char(20),
  `BankID` int,
  `Account` int,
  `IdentityID` int,

  PRIMARY KEY(ID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_InvestorEmail` (
  `ID` int,
  `Email` char(128),
  `EmailType` int,
  `Account` int,

  PRIMARY KEY(ID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_InvestorExt` (
  `ID` int,
  `BillLanguage` int,
  `BillType` int,
  `BirthDate` int,
  `BirthNation` int,
  `Career` char(64),
  `EmployeeName` char(20),
  `EmployeeNo` char(64),
  `EnName` char(64),
  `ExchangeMethod` int,
  `FatcaStatus` int,
  `GinTaxNumber` char(64),
  `Nation` int,
  `RecoveryCriteria` int,
  `Remark` char(64),
  `Sex` int,
  `TaxTableDate` int,
  `TaxTableType` int,
  `TinTaxNumber` char(64),
  `ZhName` char(64),
  `Account` int,

  PRIMARY KEY(ID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_InvertorFatca` (
  `FatcaStatus` int,
  `FatcaType` int,
  `Account` int,

  PRIMARY KEY(Account)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_InvestorFax` (
  `ID` int,
  `Fax` char(64),
  `FaxType` int,
  `Account` int,
  `AreaCode` char(5),

  PRIMARY KEY(ID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_InvestorGroup` (
  `Account` int,
  `GroupID` int,
  `GroupType` int,

  PRIMARY KEY(GroupID, Account)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_InvertorIdentity` (
  `ID` int,
  `IDExpireDate` int,
  `IDIssueDate` int,
  `IDIssuePlace` char(255),
  `IDNumber` char(32),
  `IdRemark` char(64),
  `IDType` int,
  `Account` int,

  PRIMARY KEY(ID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_InvestorMobile` (
  `ID` int,
  `Mobile` char(16),
  `MobileType` int,
  `Account` int,
  `AreaCode` char(5),

  PRIMARY KEY(ID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_InvestorOrganization` (
  `ID` int,
  `BusinessLicenseNo` char(64),
  `LegalPerson` char(20),
  `LicenseExpireDate` int,
  `OrganizationalNature` int,
  `Remark` char(64),
  `RegisteredCapital` double,
  `TaxNumber` char(64),
  `Account` int,

  PRIMARY KEY(ID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_InvestorParam` (
  `Account` int,
  `ExchangeID` char(8),
  `ProductID` char(16),
  `ContractID` char(32),
  `TradeStatus` char(1),
  `MaxPosition` int,
  `MaxOrderVolume` int,
  `CloseAdvanceDays` int,
  `ChannelID` int,

  PRIMARY KEY(Account, ExchangeID, ProductID, ContractID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_InvestorTag50` (
  `ID` int,
  `Account` int,
  `Name` char(32),
  `IDType` int,
  `IDNumber` char(32),
  `Remark` char(64),

  PRIMARY KEY(ID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_InvestorTel` (
  `ID` int,
  `Tel` char(64),
  `TelType` int,
  `Account` int,
  `AreaCode` char(5),

  PRIMARY KEY(ID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_JournalSwap` (
  `ID` int,
  `Account` int,
  `OutCurrencyID` char(3),
  `OutCurrencyGroup` char(5),
  `OutAmount` double,
  `InCurrencyID` char(3),
  `InCurrencyGroup` char(5),
  `InAmount` double,
  `Status` int,
  `TradingDay` int,
  `OperationTime` datetime,
  `AuditTime` datetime,
  `Remark` char(64),
  `OperatorID` char(16),
  `AuditorID` char(16),
  `Rate` double,

  PRIMARY KEY(ID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_Login` (
  `TradeDate` int,
  `FrontID` int,
  `SessionID` int,
  `RealDate` int,
  `Account` int,
  `Admin` char(16),
  `TwoStepLogin` int,
  `Time` char(9),
  `IP` char(40),
  `MAC` char(24),
  `UserProduct` char(20),
  `Hardware` char(64),
  `OS` char(32),
  `LogoutDate` int,
  `LogoutTime` char(9),

  PRIMARY KEY(TradeDate, FrontID, SessionID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_Margin` (
  `GroupID` int,
  `Account` int,
  `ExchangeID` char(8),
  `ProductID` char(16),
  `ContractID` char(32),
  `MarginType` char(1),
  `Margin` double,
  `MinMargin` double,
  `SettleMargin` double,

  PRIMARY KEY(GroupID, Account, ExchangeID, ProductID, ContractID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_MaxBalance` (
  `Account` int,
  `MaxBalance` double,
  `EverMaxBalance` double,

  PRIMARY KEY(Account)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_Order` (
  `TradeDate` datetime,
  `OrderID` int,
  `Account` int,
  `InsertAccount` char(16),
  `CancelAccount` char(16),
  `RequestID` int,
  `ExchangeID` char(8),
  `ContractID` char(32),
  `BS` char(1),
  `OC` char(1),
  `Hedge` char(1),
  `PriceType` char(1),
  `Price` double,
  `Volume` int,
  `TradeVolume` int,
  `Status` int,
  `RealDate` int,
  `InsertTime` char(9),
  `CancelTime` char(9),
  `ForceClose` char(1),
  `ForceReason` char(64),
  `OrderSysID` char(40),
  `Remark` varchar(256),
  `FrontID` int,
  `SessionID` int,
  `OrderRef` char(16),
  `TimeCondition` char(1),
  `GTDDate` int,
  `VolumeCondition` char(1),
  `MinVolume` int,
  `ContingentCondition` char(1),
  `StopPrice` double,
  `SwapOrder` int,
  `ChannelID` int,
  `Tag50` char(16),
  `CancelFrontID` int,
  `CancelSessionID` int,
  `ChannelFrontID` int,
  `ChannelSessionID` int,
  `ChannelOrderRef` char(16),
  `ChannelOrderID` char(24),
  `T1Valid` int,
  `Deleted` int,
  `DeleteAccount` char(16),
  `DeleteTime` char(9),
  `InsertDate` int,
  `InsertID` int,
  `PatchOrder` int,
  `CancelDate` int,
  `UserOrderID` char(32),

  PRIMARY KEY(TradeDate, OrderID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_OrderCheck` (
  `ID` int,
  `Account` int,
  `GroupID` int,
  `Item` int,
  `Param1` double,
  `ProductID` char(16),
  `Result` int,
  `ExchangeID` char(8),

  PRIMARY KEY(ID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_Position` (
  `Account` int,
  `TradeDate` int,
  `ExchangeID` char(8),
  `ContractID` char(32),
  `BS` char(1),
  `Hedge` char(1),
  `OpenDate` int,
  `TradeID` char(32),
  `OrderID` int,
  `LocalTradeID` int,
  `RealOpenDate` int,
  `TradeTime` char(9),
  `Volume` int,
  `OpenPrice` double,
  `SettlePrice` double,
  `LastSettle` double,
  `VolumeMultiple` double,
  `Margin` double,
  `PositionProfitFloat` double,
  `PositionProfit` double,
  `CloseVolume` int,
  `CloseProfitFloat` double,
  `CloseProfit` double,
  `CloseAmount` double,
  `TradeType` char(1),
  `CombineContractID` char(32),
  `CombineTradeID` char(32),
  `CurrencyGroup` char(5),
  `CurrencyID` char(3),
  `MinMargin` double,

  PRIMARY KEY(Account, TradeDate, OpenDate, TradeID, OrderID, CombineTradeID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_Product` (
  `ExchangeID` char(8),
  `ProductID` char(16),
  `ProductClass` char(1),
  `ProductName` char(32),
  `CurrencyGroup` char(5),
  `CurrencyID` char(3),
  `VolumeMultiple` double,
  `PriceTick` double,
  `DecimalDigits` int,
  `T1SplitTime` int,
  `MaxPosition` int,
  `MaxOrderVolume` int,
  `CloseAdvanceDays` int,
  `DeliveryMethod` int,
  `TradeChannel` int,
  `TradeStatus` char(1),

  PRIMARY KEY(ExchangeID, ProductID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_RiskControl` (
  `RiskID` int,
  `Account` int,
  `GroupID` int,

  PRIMARY KEY(RiskID, Account, GroupID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_RiskControlTemplate` (
  `ID` int,
  `RiskClass` int,
  `RiskItem` int,
  `Product` varchar(256),
  `OpenDenied` int,
  `CloseDenied` int,
  `Notice` int,
  `Position` int,
  `Contents` varchar(256),
  `TimeFrom` char(9),
  `TimeTo` char(9),
  `Param1` double,
  `Param2` double,
  `Param3` double,
  `Param4` double,
  `Param5` double,
  `Param6` double,
  `CreateTime` char(9),
  `UpdateTime` char(9),

  PRIMARY KEY(ID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_Session` (
  `TradeDate` int,
  `Account` int,
  `FrontID` int,
  `SessionID` int,

  PRIMARY KEY(TradeDate, Account, FrontID, SessionID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_SettlePrice` (
  `ExchangeID` char(8),
  `ContractID` char(32),
  `TradingDay` int,
  `SettlePrice` double,
  `PrevSettlePrice` double,
  `Margin` double,
  `MinMargin` double,

  PRIMARY KEY(ExchangeID, ContractID, TradingDay)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_Summary` (
  `Account` int,
  `TradeDate` int,
  `CurrencyGroup` char(5),
  `CurrencyID` char(3),
  `CurrencyRate` double,
  `Balance` double,
  `LastBalance` double,
  `Available` double,
  `Margin` double,
  `Commission` double,
  `PositionProfitFloat` double,
  `PositionProfit` double,
  `CloseProfitFloat` double,
  `CloseProfit` double,
  `Deposit` double,
  `Withdraw` double,
  `Risk` double,
  `CloseBalance` double,
  `CashInOutTotal` double,
  `BalanceFloat` double,
  `LastBalanceFloat` double,
  `Credit` double,
  `MinMargin` double,
  `Stage` int,

  PRIMARY KEY(Account, TradeDate, CurrencyGroup, CurrencyID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_SysLog` (
  `ID` int,
  `OperatorID` char(16),
  `Title` char(64),
  `Comment` varchar(4096),
  `CreateTime` datetime,
  `IP` char(40),
  `MAC` char(24),
  `Action` int,
  `Account` int,

  PRIMARY KEY(ID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_SysMenu` (
  `ID` int,
  `Icon` int,
  `Name` char(32),
  `Permission` char(64),
  `Remark` char(64),
  `TargetUrl` varchar(1024),
  `ParentID` int,

  PRIMARY KEY(ID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_SysOperator` (
  `OperatorID` char(16),
  `Avatar` char(64),
  `Email` char(128),
  `LoginStatus` char(1),
  `Mobile` char(16),
  `Name` char(32),
  `Password` char(36),
  `PasswordDate` int,
  `PasswordExpired` int,
  `PasswordValidDays` int,
  `RoleID` int,
  `LastLoginTime` datetime,
  `LoginTime` datetime,
  `LastLoginIp` char(40),
  `LastLoginMac` char(24),
  `LoginIp` char(40),
  `LoginMac` char(24),

  PRIMARY KEY(OperatorID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_SysOperatorGroup` (
  `OperatorID` char(16),
  `GroupID` int,

  PRIMARY KEY(OperatorID, GroupID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_SysParam` (
  `Param` char(32),
  `Value` varchar(8192),

  PRIMARY KEY(Param)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_SysPermission` (
  `ID` int,
  `Name` char(32),
  `Permission` char(64),
  `Remark` char(64),
  `MenuID` int,

  PRIMARY KEY(ID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_SysRole` (
  `ID` int,
  `Name` char(32),
  `Remark` char(64),
  `Status` int,
  `IsAdmin` int,

  PRIMARY KEY(ID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_SysRoleMenu` (
  `RoleID` int,
  `MenuID` int,

  PRIMARY KEY(RoleID, MenuID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_SysRolePermission` (
  `RoleID` int,
  `PermissionID` int,

  PRIMARY KEY(RoleID, PermissionID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_Trade` (
  `TradeDate` int,
  `OrderID` int,
  `TradeID` char(32),
  `Account` int,
  `RequestID` int,
  `ExchangeID` char(8),
  `ContractID` char(32),
  `BS` char(1),
  `Hedge` char(1),
  `Price` double,
  `Volume` int,
  `Turnover` double,
  `RealDate` int,
  `TradeTime` char(9),
  `OrderSysID` char(40),
  `Commission` double,
  `CloseProfitFloat` double,
  `CloseProfit` double,
  `OrderRef` char(16),
  `TradeType` char(1),
  `CombineContractID` char(32),
  `LocalTradeID` int,
  `CurrencyGroup` char(5),
  `CurrencyID` char(3),
  `ExchangeTradeID` char(72),
  `OpenVolume` int,
  `CloseVolume` int,
  `Deleted` int,
  `DeleteAccount` char(16),
  `DeleteTime` char(9),
  `ExchangeFee` double,
  `Levy` double,
  `PatchAccount` char(16),

  PRIMARY KEY(TradeDate, OrderID, TradeID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';

CREATE TABLE IF NOT EXISTS `t_TradeChannel` (
  `ID` int,
  `Name` char(64),
  `UserID` char(32),
  `Password` char(36),
  `IP` char(40),
  `Port` int,

  PRIMARY KEY(ID)
) ENGINE=InnoDB DEFAULT COLLATE='utf8_bin' COMMENT='';


