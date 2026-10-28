// 本文件由 ../Templates/Cpp/Common/Types.h.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#pragma once
#include <stdint.h>

namespace Mdb
{
//布尔类型
using BoolType = bool;







//32位整型类型
using Int32Type = int32_t;

//客户端委托编号类型
using ClientOrderIdType = int32_t;

//组代码类型
using GroupIdType = int32_t;

//报盘代码类型
using OfferIdType = int32_t;

//委托编号类型
using OrderIdType = int32_t;

//请求代码类型
using RequestIdType = int32_t;

//合约乘数类型
using VolumeMultipleType = int32_t;



	//会话代码类型
using SessionIdType = long long;

	//数量类型
using VolumeType = long long;


//金额类型
using MoneyType = double;

//价格类型
using PriceType = double;


//账号类型
using AccountIdType = char[32];

//账号名称类型
using AccountNameType = char[64];

//日期类型
using DateType = char[16];

//交易所代码类型
using ExchangeIdType = char[8];

//交易所名称类型
using ExchangeNameType = char[64];

//合约代码类型
using InstrumentIdType = char[32];

//合约名称类型
using InstrumentNameType = char[64];

//系统委托编号类型
using OrderSysIdType = char[64];

//密码类型
using PasswordType = char[64];

//品种代码类型
using ProductIdType = char[32];

//品种名称类型
using ProductNameType = char[32];

//交易节名称
using SessionNameType = char[32];

//时间类型
using TimeType = char[16];

//成交编号类型
using TradeIdType = char[64];


//账户类别
enum class AccountClassType : int32_t
{
	//期货
	Future = 0,
	//股票
	Stock = 1,
	//期权
	Option = 2,
};

//账户类型
enum class AccountTypeType : int32_t
{
	//主账户
	Primary = 0,
	//子账户
	Sub = 1,
};

//账户状态
enum class AccountStatusType : int32_t
{
	//启用
	Normal = 0,
	//禁用
	Forbidden = 1,
};

//买卖方向
enum class DirectionType : int32_t
{
	//买
	Buy = 0,
	//卖
	Sell = 1,
};

//行情初始化状态
enum class InitStatusType : int32_t
{
	//未初始化
	UnInitialized = 0,
	//正在初始化
	Initializing = 1,
	//已初始化
	Initialized = 2,
};

//合约类别
enum class InstrumentClassType : int32_t
{
	//普通合约
	Normal = 0,
	//主力合约
	Main = 1,
};

//登录状态
enum class LoginStatusType : int32_t
{
	//未登录
	NotLog = 0,
	//登录中
	Logging = 1,
	//已登录
	Logged = 2,
};

//持仓方向
enum class PosiDirectionType : int32_t
{
	//多头
	Long = 0,
	//空头
	Short = 1,
	//净持仓
	Net = 2,
};

//开平标志
enum class OffsetFlagType : int32_t
{
	//开仓
	Open = 0,
	//平仓
	Close = 1,
	//平今
	CloseToday = 2,
};

//委托价格类型
enum class OrderPriceTypeType : int32_t
{
	//限价
	LimitPrice = 0,
	//市价立即成交剩余撤销
	AnyPriceFAK = 1,
	//市价立即成全部交否则撤销
	AnyPriceFOK = 2,
	//限价立即成交剩余撤销
	LimitPriceFAK = 3,
	//限价立即成全部交否则撤销
	LimitPriceFOK = 4,
	//本方最优价
	BestOwnPrice = 5,
	//对方最优价
	BestOppoPrice = 6,
};

//委托状态
enum class OrderStatusType : int32_t
{
	//正在申报
	Inserting = 0,
	//已报
	Inserted = 1,
	//部成
	PartTraded = 2,
	//已成
	AllTraded = 3,
	//已撤
	Canceled = 4,
	//部成部撤
	PartTradedCanceled = 5,
	//审核中
	Auditing = 6,
	//审核驳回
	AuditReject = 7,
	//风控检查中
	RiskChecking = 8,
	//风控检查失败
	RiskCheckFailed = 9,
	//废单
	Error = 10,
	//未触发
	NotTouched = 11,
	//已触发
	Touched = 12,
	//未知
	Unknown = 13,
};

//产品类型
enum class ProductClassType : int32_t
{
	//期货
	Future = 0,
	//期货期权
	FutureOption = 1,
	//组合
	Combination = 2,
	//即期
	Spot = 3,
	//期转现
	EFP = 4,
	//指数
	Index = 5,
	//股票
	Stock = 6,
	//股票期权
	StockOption = 7,
	//ETF
	ETF = 8,
};



//线协议是按字段宽度直接 memcpy 上线的：宽度一变线上格式就静默错位，所以在这里钉死
//64 位保留 long long 而非 int64_t：Linux 上 int64_t 是 long，会让全仓 %lld 变成格式不匹配
static_assert(sizeof(long long) == 8, "Int64 别名用 long long，线上必须占 8 字节");
//无符号侧同理：保留 unsigned long long 而非 uint64_t，否则 Linux 上 %llu 同样不匹配
static_assert(sizeof(unsigned long long) == 8, "UInt64 别名用 unsigned long long，线上必须占 8 字节");
static_assert(sizeof(double) == 8, "Double 别名用 double，线上必须占 8 字节");
static_assert(sizeof(bool) == 1, "Bool 别名用 bool，线上必须占 1 字节");
}
