#pragma once

typedef bool CBoolType;

typedef int CBrokerIDType;


typedef double CPriceType;

typedef char CAccountIDType[32];
typedef char CDateType[9];
typedef char COrgIDType[16];
typedef char CAccountNameType[128];
typedef char CCurrencyIDType[8];

enum class CAccountTypeType : char
{
	Primary = '0',
	Sub = '1',
};
enum class CAccountClassType : char
{
	Future = '0',
	Security = '1',
};
enum class CAccountStatusType : char
{
	Normal = '0',
	Forbbiden = '1',
};

