#pragma once
#include <stdint.h>


//布尔类型
typedef int BoolType;
//整型
typedef int NumType;
typedef int IDType;
typedef int IDTypeType;
//8位数字表示的日期
typedef int DateType;
//年
typedef int YearType;
//月
typedef int MonthType;
typedef int StatusType;
typedef int AccountType;
typedef int NationType;
//报单编号
typedef int OrderIDType;
//数量
typedef int VolumeType;
typedef int GroupIDType;
typedef int GroupTypeType;
typedef int FlagType;
//银行卡号
typedef int BankCardIDType;
//出入金方式
typedef int MethodType;
typedef int TwoStepCodeTimeType;
typedef int AddressTypeType;
typedef int DomesticAccountTypeType;
typedef int DomesticCurrencyTypeType;
typedef int OpeningIDType;
typedef int BankIDType;
typedef int IdentityIDType;
typedef int EmailTypeType;
typedef int BillLanguageType;
typedef int BillTypeType;
typedef int ExchangeMethodType;
typedef int FatcaStatusType;
typedef int RecoveryCriteriaType;
typedef int SexType;
typedef int TaxTableTypeType;
typedef int FatcaStatusType;
typedef int FatcaTypeType;
typedef int FaxTypeType;
typedef int MobileTypeType;
typedef int OrganizationalNatureType;
typedef int ChannelIDType;
typedef int TelTypeType;
typedef int FrontIDType;
typedef int SessionIDType;
typedef int RequestIDType;
typedef int SwapOrderType;
typedef int T1ValidType;
typedef int InsertIDType;
typedef int PatchOrderType;
typedef int ItemType;
typedef int ResultType;
typedef int LocalTradeIDType;
typedef int DecimalDigitsType;
typedef int T1SplitTimeType;
typedef int DeliveryMethodType;
typedef int RiskIDType;
typedef int RiskClassType;
typedef int RiskItemType;
typedef int NoticeType;
typedef int StageType;
typedef int ActionType;
typedef int IconType;
typedef int ParentIDType;
typedef int RoleIDType;
typedef int MenuIDType;
typedef int PermissionIDType;
typedef int PortType;


//比率
typedef double RateType;
//价格
typedef double PriceType;
//金额
typedef double MoneyType;
typedef double ParamType;
typedef double VolumeMultipleType;
typedef double PriceTickType;
typedef double LevyType;

typedef char RemarkType[64];
//交易所编号
typedef char ExchangeIDType[8];
//交易所名称
typedef char ExchangeNameType[8];
//合约
typedef char ContractIDType[32];
//合约名称
typedef char ContractNameType[32];
//品种
typedef char ProductIDType[16];
typedef char OperatorIDType[16];
typedef char AuditorIDType[16];
typedef char SoftwareType[12];
typedef char AuthCodeType[16];
typedef char CodeType[10];
typedef char NameType[32];
//成交编号
typedef char TradeIDType[32];
typedef char TimeType[9];
typedef char CurrencyGroupType[5];
typedef char CurrencyIDType[3];
typedef char GroupNameType[64];
typedef char InvestorNameType[64];
typedef char PasswordType[36];
typedef char TwoStepCodeType[8];
typedef char AddressType[256];
typedef char PostCodeType[16];
typedef char BranchType[64];
typedef char CardNoType[24];
typedef char OwnerType[20];
typedef char EmailType[128];
typedef char EmployeeNameType[20];
typedef char CareerType[64];
typedef char EmployeeNoType[64];
typedef char EnNameType[64];
typedef char GinTaxNumberType[64];
typedef char TinTaxNumberType[64];
typedef char ZhNameType[64];
typedef char FaxType[64];
typedef char AreaCodeType[5];
typedef char IDIssuePlaceType[255];
typedef char IDNumberType[32];
typedef char MobileType[16];
typedef char BusinessLicenseNoType[64];
typedef char LegalPersonType[20];
typedef char TaxNumberType[64];
typedef char TelType[64];
typedef char AdminType[16];
typedef char IPType[40];
typedef char MACType[24];
typedef char UserProductType[20];
typedef char HardwareType[64];
typedef char OSType[32];
typedef char ForceReasonType[64];
typedef char OrderSysIDType[40];
typedef char OrderRemarkType[256];
typedef char OrderRefType[16];
typedef char Tag50Type[16];
typedef char ChannelOrderIDType[24];
typedef char UserOrderIDType[32];
typedef char ProductNameType[32];
typedef char ProductCombinationType[256];
typedef char ContentsType[256];
typedef char TitleType[64];
typedef char CommentType[4096];
typedef char PermissionType[64];
typedef char TargetUrlType[1024];
typedef char AvatarType[64];
typedef char SysParamType[32];
typedef char SysParamValueType[8192];
typedef char ExchangeTradeIDType[72];
typedef char PatchAccountType[16];
typedef char UserIDType[32];
typedef char ChannelNameType[64];
typedef char ExchangeTradeIDType[72];

//交易状态
enum class TradeStatusType : char
{
	//未知
	Unknown = '0',
	//集合竞价报单
	AuctionOrdering = '1',
	//连续交易
	Trading = '2',
	//暂停
	Pause = '3',
	//收市
	Closed = '4',
};
//产品类型
enum class ProductClassType : char
{
	//未知
	Futures = '1',
	//集合竞价报单
	Options = '2',
	//连续交易
	Combination = '3',
	//暂停
	Spot = '4',
	//收市
	EFP = '5',
	//收市
	SpotOption = '6',
};
//持仓类型
enum class PositionTypeType : char
{
	//净持仓
	Net = '1',
	//综合持仓
	Gross = '2',
};
//持仓类型
enum class PositionDateTypeType : char
{
	//使用历史持仓（区分今昨仓）
	UseHistory = '1',
	//不使用历史持仓（不区分今昨仓）
	NoUseHistory = '2',
};
//报单状态
enum class OrderStatusType : char
{
	//正在申报
	PTK_OST_Inserting = '0',
	//已报
	PTK_OST_Inserted = '1',
	//部成
	PTK_OST_PartTraded = '2',
	//已成
	PTK_OST_AllTraded = '3',
	//已撤
	PTK_OST_Canceled = '4',
	//部成部撤
	PTK_OST_PartTradedCanceled = '5',
	//废单
	PTK_OST_Error = 'e',
	//未触发
	PTK_OST_NotTouched = 'p',
	//已触发
	PTK_OST_Touched = 't',
	//未知
	PTK_OST_Unknown = 'x',
};
//买卖方向
enum class DirectionType : char
{
	//买入
	Buy = '0',
	//卖出
	Sell = '1',
};
//投保标志
enum class HedgeFlagType : char
{
	//投机
	Speculation = '0',
	//套利
	Arbitrage = '1',
	//保值
	Hedge = '2',
};
//报单价格条件
enum class OrderPriceTypeType : char
{
	//市价
	AnyPrice = '1',
	//限价
	LimitPrice = '2',
	//最优价
	BestPrice = '3',
	//最新价
	LastPrice = '4',
	//最新价浮动上浮1个ticks
	LastPricePlusOneTicks = '5',
	//最新价浮动上浮2个ticks
	LastPricePlusTwoTicks = '6',
	//最新价浮动上浮3个ticks
	LastPricePlusThreeTicks = '7',
	//卖一价
	AskPrice1 = '8',
	//卖一价浮动上浮1个ticks
	AskPrice1PlusOneTicks = '9',
	//卖一价浮动上浮2个ticks
	AskPrice1PlusTwoTicks = 'A',
	//卖一价浮动上浮3个ticks
	AskPrice1PlusThreeTicks = 'B',
	//买一价
	BidPrice1 = 'C',
	//买一价浮动上浮1个ticks
	BidPrice1PlusOneTicks = 'D',
	//买一价浮动上浮2个ticks
	BidPrice1PlusTwoTicks = 'E',
	//买一价浮动上浮3个ticks
	BidPrice1PlusThreeTicks = 'F',
};
//开平标志
enum class OffsetFlagType : char
{
	//开仓
	Open = '0',
	//平仓
	Close = '1',
	//平今
	CloseToday = '2',
};
//撤单改单类型
enum class CancelTypeType : char
{
	//撤单
	Delete = '0',
	//改单
	Modify = '3',
};
//触发条件类型
enum class ContingentConditionType : char
{
	//立即
	Immediately = '1',
	//止损
	Touch = '2',
	//止赢
	TouchProfit = '3',
	//预埋单
	ParkedOrder = '4',
	//最新价大于条件价
	LastPriceGreaterThanStopPrice = '5',
	//最新价大于等于条件价
	LastPriceGreaterEqualStopPrice = '6',
	//最新价小于条件价
	LastPriceLesserThanStopPrice = '7',
	//最新价小于等于条件价
	LastPriceLesserEqualStopPrice = '8',
	//卖一价大于条件价
	AskPriceGreaterThanStopPrice = '9',
	//卖一价大于等于条件价
	AskPriceGreaterEqualStopPrice = 'A',
	//卖一价小于条件价
	AskPriceLesserThanStopPrice = 'B',
	//卖一价小于等于条件价
	AskPriceLesserEqualStopPrice = 'C',
	//买一价大于条件价
	BidPriceGreaterThanStopPrice = 'D',
	//买一价大于等于条件价
	BidPriceGreaterEqualStopPrice = 'E',
	//买一价小于条件价
	BidPriceLesserThanStopPrice = 'F',
	//买一价小于等于条件价
	BidPriceLesserEqualStopPrice = 'H',
};
//有效期类型
enum class TimeConditionType : char
{
	//立即完成，否则撤销
	IOC = '1',
	//本节有效
	GFS = '2',
	//当日有效
	GFD = '3',
	//指定日期前有效
	GTD = '4',
	//撤销前有效
	GTC = '5',
	//集合竞价有效
	GFA = '6',
};
//成交量类型
enum class VolumeConditionType : char
{
	//任何数量
	AV = '1',
	//最小数量
	MV = '2',
	//全部数量
	CV = '3',
};
//强平原因
enum class ForceCloseReasonType : char
{
	//非强平
	NotForceClose = '0',
	//资金不足
	LackDeposit = '1',
	//客户超仓
	ClientOverPositionLimit = '2',
	//会员超仓
	MemberOverPositionLimit = '3',
	//持仓非整数倍
	NotMultiple = '4',
	//违规
	Violation = '5',
	//其它
	Other = '6',
	//自然人临近交割
	PersonDeliv = '7',
};
//成交类型
enum class TradeTypeType : char
{
	//普通成交
	Common = '0',
	//期权执行
	OptionsExecution = '1',
	//OTC成交
	OTC = '2',
	//期转现衍生成交
	EFPDerived = '3',
	//组合衍生成交
	CombinationDerived = '4',
};
//保证金价格类型
enum class MarginPriceType : char
{
	//昨结算价
	PreSettlementPrice = '1',
	//最新价
	SettlementPrice = '2',
	//成交均价
	AveragePrice = '3',
	//开仓价
	OpenPrice = '4',
};
//浮盈浮亏是否计入可用
enum class IncludePositionProfitType : char
{
	//浮盈浮亏都计算
	All = '1',
	//浮盈不计，浮亏计
	OnlyLost = '2',
	//浮盈计，浮亏不计
	OnlyGain = '3',
	//浮盈浮亏都不计算
	None = '4',
};
//平仓盈利类型是否可用
enum class IncludeCloseProfitType : char
{
	//浮盈浮亏都计算
	Include = '0',
	//浮盈不计，浮亏计
	NotInclude = '2',
};
//是否启用上期所大额单边保证金
enum class LargeSideMarginType : char
{
	//不使用大额单边保证金算法
	NO = '0',
	//使用大额单边保证金算法
	YES = '1',
};
//账户类型
enum class InvestorAccountType : char
{
	//投资者
	Investor = '0',
	//管理员
	Admin = '1',
};
//通知类型
enum class NotifyType : char
{
	//普通
	Message = '0',
	//警告
	Warning = '1',
	//已强平
	ForceClose = '2',
	//CTP通知（此消息只推给监控）
	CTP = '3',
};
//账号状态
enum class AccountStatusType : char
{
	//禁用
	Forbidden = '0',
	//启用
	Enable = '1',
};
//账号类型
enum class AccountTypeType : char
{
	//个人
	Individual = '1',
	//机构
	Organizational = '2',
	//做市商
	MarketMaker = '3',
};
//代理类型
enum class AgentTypeType : char
{
	Client = '1',
	House = '2',
	Account = '3',
};
//委托类型
enum class OrderTypeType : char
{
	NonPlenipotentiary = '1',
	Plenipotentiary = '2',
};
//投资者类型
enum class InvestorTypeType : char
{
	InvestorA = '1',
	InvestorB = '2',
};
//登录状态
enum class LoginStatusType : char
{
	//正常
	Normal = '1',
	//禁止登录
	Forbidden = '2',
};
//保证金类型
enum class MarginTypeType : char
{
	//比例
	Rate = '1',
	//固定金额
	Fixed = '2',
	//折扣
	Discount = '3',
};

//日期时间类型
typedef char DateTimeType[32];

