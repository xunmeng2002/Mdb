// 本文件由 ../Templates/Cpp/Mdb/Rcu/StableRow/InitMdbFromCsv.cpp.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#include "InitMdbFromCsv.h"
#include <Spark/Serialization/Csv/CsvRecord.h>
#include <Spark/Core/Utility/Utility.h>
#include <fstream>
#include <assert.h>

using namespace Spark::Core;
using namespace Spark::Serialization;


namespace Mdb::Rcu::StableRow
{
	static char HeaderBuffer[1024] = { 0 };
	static char ContentBuffer[64 * 1024] = { 0 };

	void InitMdbFromCsv::LoadTables(Mdb* mdb, const char* dir, const TableList& tableList)
	{
		for (int i = 0; i < tableList.Count; ++i)
		{
			switch (tableList.TableIds[i])
			{
			case TradingDay::TableId:  LoadTradingDays(mdb, dir); break;
			case Exchange::TableId:  LoadExchanges(mdb, dir); break;
			case Product::TableId:  LoadProducts(mdb, dir); break;
			case Instrument::TableId:  LoadInstruments(mdb, dir); break;
			case PrimaryAccount::TableId:  LoadPrimaryAccounts(mdb, dir); break;
			case Account::TableId:  LoadAccounts(mdb, dir); break;
			case Capital::TableId:  LoadCapitals(mdb, dir); break;
			case Position::TableId:  LoadPositions(mdb, dir); break;
			case PositionDetail::TableId:  LoadPositionDetails(mdb, dir); break;
			case Order::TableId:  LoadOrders(mdb, dir); break;
			case Trade::TableId:  LoadTrades(mdb, dir); break;
			default: break;
			}
		}
	}

	void InitMdbFromCsv::LoadTradingDays(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		snprintf(fullPath, sizeof(fullPath), "%s/TradingDays.csv", dir);
		std::fstream file(fullPath, std::fstream::in);
		if (!file)
		{
			throw std::string(fullPath) + " Open Failed.";
		}

		file.getline(HeaderBuffer, sizeof(HeaderBuffer), '\n');
		CsvRecord csvRecord;
		if (!csvRecord.AnalysisFieldName(HeaderBuffer))
		{
			throw std::string("AnalysisFieldName TradingDays.csv failed");
		}
		while (!file.eof())
		{
			::memset(ContentBuffer, 0, sizeof(ContentBuffer));
			file.getline(ContentBuffer, sizeof(ContentBuffer), '\n');
			if (ContentBuffer[0] == '\0')
				break;
			if (!csvRecord.AnalysisFieldContent(ContentBuffer))
			{
				throw std::string("AnalysisFieldContent TradingDays.csv failed");
			}

			auto record = TradingDay::Allocate();
			record->Pk = csvRecord.GetFieldAsInt("Pk");
			Utility::Strcpy(record->CurrTradingDay, csvRecord.GetFieldAsString("CurrTradingDay"));
			Utility::Strcpy(record->PreTradingDay, csvRecord.GetFieldAsString("PreTradingDay"));
			mdb->TradingDays->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadExchanges(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		snprintf(fullPath, sizeof(fullPath), "%s/Exchanges.csv", dir);
		std::fstream file(fullPath, std::fstream::in);
		if (!file)
		{
			throw std::string(fullPath) + " Open Failed.";
		}

		file.getline(HeaderBuffer, sizeof(HeaderBuffer), '\n');
		CsvRecord csvRecord;
		if (!csvRecord.AnalysisFieldName(HeaderBuffer))
		{
			throw std::string("AnalysisFieldName Exchanges.csv failed");
		}
		while (!file.eof())
		{
			::memset(ContentBuffer, 0, sizeof(ContentBuffer));
			file.getline(ContentBuffer, sizeof(ContentBuffer), '\n');
			if (ContentBuffer[0] == '\0')
				break;
			if (!csvRecord.AnalysisFieldContent(ContentBuffer))
			{
				throw std::string("AnalysisFieldContent Exchanges.csv failed");
			}

			auto record = Exchange::Allocate();
			Utility::Strcpy(record->ExchangeId, csvRecord.GetFieldAsString("ExchangeId"));
			Utility::Strcpy(record->ExchangeName, csvRecord.GetFieldAsString("ExchangeName"));
			mdb->Exchanges->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadProducts(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		snprintf(fullPath, sizeof(fullPath), "%s/Products.csv", dir);
		std::fstream file(fullPath, std::fstream::in);
		if (!file)
		{
			throw std::string(fullPath) + " Open Failed.";
		}

		file.getline(HeaderBuffer, sizeof(HeaderBuffer), '\n');
		CsvRecord csvRecord;
		if (!csvRecord.AnalysisFieldName(HeaderBuffer))
		{
			throw std::string("AnalysisFieldName Products.csv failed");
		}
		while (!file.eof())
		{
			::memset(ContentBuffer, 0, sizeof(ContentBuffer));
			file.getline(ContentBuffer, sizeof(ContentBuffer), '\n');
			if (ContentBuffer[0] == '\0')
				break;
			if (!csvRecord.AnalysisFieldContent(ContentBuffer))
			{
				throw std::string("AnalysisFieldContent Products.csv failed");
			}

			auto record = Product::Allocate();
			Utility::Strcpy(record->ExchangeId, csvRecord.GetFieldAsString("ExchangeId"));
			Utility::Strcpy(record->ProductId, csvRecord.GetFieldAsString("ProductId"));
			Utility::Strcpy(record->ProductName, csvRecord.GetFieldAsString("ProductName"));
			record->ProductClass = static_cast<ProductClassType>(csvRecord.GetFieldAsInt("ProductClass"));
			record->VolumeMultiple = csvRecord.GetFieldAsInt("VolumeMultiple");
			record->PriceTick = csvRecord.GetFieldAsDouble("PriceTick");
			record->MaxMarketOrderVolume = csvRecord.GetFieldAsInt64("MaxMarketOrderVolume");
			record->MinMarketOrderVolume = csvRecord.GetFieldAsInt64("MinMarketOrderVolume");
			record->MaxLimitOrderVolume = csvRecord.GetFieldAsInt64("MaxLimitOrderVolume");
			record->MinLimitOrderVolume = csvRecord.GetFieldAsInt64("MinLimitOrderVolume");
			Utility::Strcpy(record->SessionName, csvRecord.GetFieldAsString("SessionName"));
			mdb->Products->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadInstruments(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		snprintf(fullPath, sizeof(fullPath), "%s/Instruments.csv", dir);
		std::fstream file(fullPath, std::fstream::in);
		if (!file)
		{
			throw std::string(fullPath) + " Open Failed.";
		}

		file.getline(HeaderBuffer, sizeof(HeaderBuffer), '\n');
		CsvRecord csvRecord;
		if (!csvRecord.AnalysisFieldName(HeaderBuffer))
		{
			throw std::string("AnalysisFieldName Instruments.csv failed");
		}
		while (!file.eof())
		{
			::memset(ContentBuffer, 0, sizeof(ContentBuffer));
			file.getline(ContentBuffer, sizeof(ContentBuffer), '\n');
			if (ContentBuffer[0] == '\0')
				break;
			if (!csvRecord.AnalysisFieldContent(ContentBuffer))
			{
				throw std::string("AnalysisFieldContent Instruments.csv failed");
			}

			auto record = Instrument::Allocate();
			Utility::Strcpy(record->ExchangeId, csvRecord.GetFieldAsString("ExchangeId"));
			Utility::Strcpy(record->InstrumentId, csvRecord.GetFieldAsString("InstrumentId"));
			Utility::Strcpy(record->ExchangeInstId, csvRecord.GetFieldAsString("ExchangeInstId"));
			Utility::Strcpy(record->InstrumentName, csvRecord.GetFieldAsString("InstrumentName"));
			Utility::Strcpy(record->ProductId, csvRecord.GetFieldAsString("ProductId"));
			record->ProductClass = static_cast<ProductClassType>(csvRecord.GetFieldAsInt("ProductClass"));
			record->InstrumentClass = static_cast<InstrumentClassType>(csvRecord.GetFieldAsInt("InstrumentClass"));
			record->Rank = csvRecord.GetFieldAsInt("Rank");
			record->VolumeMultiple = csvRecord.GetFieldAsInt("VolumeMultiple");
			record->PriceTick = csvRecord.GetFieldAsDouble("PriceTick");
			record->MaxMarketOrderVolume = csvRecord.GetFieldAsInt64("MaxMarketOrderVolume");
			record->MinMarketOrderVolume = csvRecord.GetFieldAsInt64("MinMarketOrderVolume");
			record->MaxLimitOrderVolume = csvRecord.GetFieldAsInt64("MaxLimitOrderVolume");
			record->MinLimitOrderVolume = csvRecord.GetFieldAsInt64("MinLimitOrderVolume");
			Utility::Strcpy(record->SessionName, csvRecord.GetFieldAsString("SessionName"));
			mdb->Instruments->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadPrimaryAccounts(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		snprintf(fullPath, sizeof(fullPath), "%s/PrimaryAccounts.csv", dir);
		std::fstream file(fullPath, std::fstream::in);
		if (!file)
		{
			throw std::string(fullPath) + " Open Failed.";
		}

		file.getline(HeaderBuffer, sizeof(HeaderBuffer), '\n');
		CsvRecord csvRecord;
		if (!csvRecord.AnalysisFieldName(HeaderBuffer))
		{
			throw std::string("AnalysisFieldName PrimaryAccounts.csv failed");
		}
		while (!file.eof())
		{
			::memset(ContentBuffer, 0, sizeof(ContentBuffer));
			file.getline(ContentBuffer, sizeof(ContentBuffer), '\n');
			if (ContentBuffer[0] == '\0')
				break;
			if (!csvRecord.AnalysisFieldContent(ContentBuffer))
			{
				throw std::string("AnalysisFieldContent PrimaryAccounts.csv failed");
			}

			auto record = PrimaryAccount::Allocate();
			Utility::Strcpy(record->PrimaryAccountId, csvRecord.GetFieldAsString("PrimaryAccountId"));
			Utility::Strcpy(record->PrimaryAccountName, csvRecord.GetFieldAsString("PrimaryAccountName"));
			record->AccountClass = static_cast<AccountClassType>(csvRecord.GetFieldAsInt("AccountClass"));
			Utility::Strcpy(record->BrokerPassword, csvRecord.GetFieldAsString("BrokerPassword"));
			record->OfferId = csvRecord.GetFieldAsInt("OfferId");
			record->IsAllowLogin = static_cast<bool>(csvRecord.GetFieldAsInt("IsAllowLogin"));
			record->IsSimulateAccount = static_cast<bool>(csvRecord.GetFieldAsInt("IsSimulateAccount"));
			record->LoginStatus = static_cast<LoginStatusType>(csvRecord.GetFieldAsInt("LoginStatus"));
			record->InitStatus = static_cast<InitStatusType>(csvRecord.GetFieldAsInt("InitStatus"));
			mdb->PrimaryAccounts->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadAccounts(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		snprintf(fullPath, sizeof(fullPath), "%s/Accounts.csv", dir);
		std::fstream file(fullPath, std::fstream::in);
		if (!file)
		{
			throw std::string(fullPath) + " Open Failed.";
		}

		file.getline(HeaderBuffer, sizeof(HeaderBuffer), '\n');
		CsvRecord csvRecord;
		if (!csvRecord.AnalysisFieldName(HeaderBuffer))
		{
			throw std::string("AnalysisFieldName Accounts.csv failed");
		}
		while (!file.eof())
		{
			::memset(ContentBuffer, 0, sizeof(ContentBuffer));
			file.getline(ContentBuffer, sizeof(ContentBuffer), '\n');
			if (ContentBuffer[0] == '\0')
				break;
			if (!csvRecord.AnalysisFieldContent(ContentBuffer))
			{
				throw std::string("AnalysisFieldContent Accounts.csv failed");
			}

			auto record = Account::Allocate();
			Utility::Strcpy(record->AccountId, csvRecord.GetFieldAsString("AccountId"));
			Utility::Strcpy(record->AccountName, csvRecord.GetFieldAsString("AccountName"));
			record->AccountType = static_cast<AccountTypeType>(csvRecord.GetFieldAsInt("AccountType"));
			record->AccountStatus = static_cast<AccountStatusType>(csvRecord.GetFieldAsInt("AccountStatus"));
			Utility::Strcpy(record->Password, csvRecord.GetFieldAsString("Password"));
			record->TradeGroupId = csvRecord.GetFieldAsInt("TradeGroupId");
			record->RiskGroupId = csvRecord.GetFieldAsInt("RiskGroupId");
			record->CommissionGroupId = csvRecord.GetFieldAsInt("CommissionGroupId");
			mdb->Accounts->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadCapitals(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		snprintf(fullPath, sizeof(fullPath), "%s/Capitals.csv", dir);
		std::fstream file(fullPath, std::fstream::in);
		if (!file)
		{
			throw std::string(fullPath) + " Open Failed.";
		}

		file.getline(HeaderBuffer, sizeof(HeaderBuffer), '\n');
		CsvRecord csvRecord;
		if (!csvRecord.AnalysisFieldName(HeaderBuffer))
		{
			throw std::string("AnalysisFieldName Capitals.csv failed");
		}
		while (!file.eof())
		{
			::memset(ContentBuffer, 0, sizeof(ContentBuffer));
			file.getline(ContentBuffer, sizeof(ContentBuffer), '\n');
			if (ContentBuffer[0] == '\0')
				break;
			if (!csvRecord.AnalysisFieldContent(ContentBuffer))
			{
				throw std::string("AnalysisFieldContent Capitals.csv failed");
			}

			auto record = Capital::Allocate();
			Utility::Strcpy(record->TradingDay, csvRecord.GetFieldAsString("TradingDay"));
			Utility::Strcpy(record->AccountId, csvRecord.GetFieldAsString("AccountId"));
			record->AccountType = static_cast<AccountTypeType>(csvRecord.GetFieldAsInt("AccountType"));
			record->Balance = csvRecord.GetFieldAsDouble("Balance");
			record->PreBalance = csvRecord.GetFieldAsDouble("PreBalance");
			record->Available = csvRecord.GetFieldAsDouble("Available");
			record->MarketValue = csvRecord.GetFieldAsDouble("MarketValue");
			record->CashIn = csvRecord.GetFieldAsDouble("CashIn");
			record->CashOut = csvRecord.GetFieldAsDouble("CashOut");
			record->Margin = csvRecord.GetFieldAsDouble("Margin");
			record->Commission = csvRecord.GetFieldAsDouble("Commission");
			record->FrozenCash = csvRecord.GetFieldAsDouble("FrozenCash");
			record->FrozenMargin = csvRecord.GetFieldAsDouble("FrozenMargin");
			record->FrozenCommission = csvRecord.GetFieldAsDouble("FrozenCommission");
			record->CloseProfitByDate = csvRecord.GetFieldAsDouble("CloseProfitByDate");
			record->CloseProfitByTrade = csvRecord.GetFieldAsDouble("CloseProfitByTrade");
			record->PositionProfitByDate = csvRecord.GetFieldAsDouble("PositionProfitByDate");
			record->PositionProfitByTrade = csvRecord.GetFieldAsDouble("PositionProfitByTrade");
			record->Deposit = csvRecord.GetFieldAsDouble("Deposit");
			record->Withdraw = csvRecord.GetFieldAsDouble("Withdraw");
			mdb->Capitals->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadPositions(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		snprintf(fullPath, sizeof(fullPath), "%s/Positions.csv", dir);
		std::fstream file(fullPath, std::fstream::in);
		if (!file)
		{
			throw std::string(fullPath) + " Open Failed.";
		}

		file.getline(HeaderBuffer, sizeof(HeaderBuffer), '\n');
		CsvRecord csvRecord;
		if (!csvRecord.AnalysisFieldName(HeaderBuffer))
		{
			throw std::string("AnalysisFieldName Positions.csv failed");
		}
		while (!file.eof())
		{
			::memset(ContentBuffer, 0, sizeof(ContentBuffer));
			file.getline(ContentBuffer, sizeof(ContentBuffer), '\n');
			if (ContentBuffer[0] == '\0')
				break;
			if (!csvRecord.AnalysisFieldContent(ContentBuffer))
			{
				throw std::string("AnalysisFieldContent Positions.csv failed");
			}

			auto record = Position::Allocate();
			Utility::Strcpy(record->TradingDay, csvRecord.GetFieldAsString("TradingDay"));
			Utility::Strcpy(record->AccountId, csvRecord.GetFieldAsString("AccountId"));
			record->AccountType = static_cast<AccountTypeType>(csvRecord.GetFieldAsInt("AccountType"));
			Utility::Strcpy(record->ExchangeId, csvRecord.GetFieldAsString("ExchangeId"));
			Utility::Strcpy(record->InstrumentId, csvRecord.GetFieldAsString("InstrumentId"));
			record->ProductClass = static_cast<ProductClassType>(csvRecord.GetFieldAsInt("ProductClass"));
			record->PosiDirection = static_cast<PosiDirectionType>(csvRecord.GetFieldAsInt("PosiDirection"));
			record->TotalPosition = csvRecord.GetFieldAsInt64("TotalPosition");
			record->PositionFrozen = csvRecord.GetFieldAsInt64("PositionFrozen");
			record->TodayPosition = csvRecord.GetFieldAsInt64("TodayPosition");
			record->MarketValue = csvRecord.GetFieldAsDouble("MarketValue");
			record->CashIn = csvRecord.GetFieldAsDouble("CashIn");
			record->CashOut = csvRecord.GetFieldAsDouble("CashOut");
			record->Margin = csvRecord.GetFieldAsDouble("Margin");
			record->Commission = csvRecord.GetFieldAsDouble("Commission");
			record->FrozenCash = csvRecord.GetFieldAsDouble("FrozenCash");
			record->FrozenMargin = csvRecord.GetFieldAsDouble("FrozenMargin");
			record->FrozenCommission = csvRecord.GetFieldAsDouble("FrozenCommission");
			record->VolumeMultiple = csvRecord.GetFieldAsInt("VolumeMultiple");
			record->CloseProfitByDate = csvRecord.GetFieldAsDouble("CloseProfitByDate");
			record->CloseProfitByTrade = csvRecord.GetFieldAsDouble("CloseProfitByTrade");
			record->PositionProfitByDate = csvRecord.GetFieldAsDouble("PositionProfitByDate");
			record->PositionProfitByTrade = csvRecord.GetFieldAsDouble("PositionProfitByTrade");
			record->SettlementPrice = csvRecord.GetFieldAsDouble("SettlementPrice");
			record->PreSettlementPrice = csvRecord.GetFieldAsDouble("PreSettlementPrice");
			mdb->Positions->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadPositionDetails(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		snprintf(fullPath, sizeof(fullPath), "%s/PositionDetails.csv", dir);
		std::fstream file(fullPath, std::fstream::in);
		if (!file)
		{
			throw std::string(fullPath) + " Open Failed.";
		}

		file.getline(HeaderBuffer, sizeof(HeaderBuffer), '\n');
		CsvRecord csvRecord;
		if (!csvRecord.AnalysisFieldName(HeaderBuffer))
		{
			throw std::string("AnalysisFieldName PositionDetails.csv failed");
		}
		while (!file.eof())
		{
			::memset(ContentBuffer, 0, sizeof(ContentBuffer));
			file.getline(ContentBuffer, sizeof(ContentBuffer), '\n');
			if (ContentBuffer[0] == '\0')
				break;
			if (!csvRecord.AnalysisFieldContent(ContentBuffer))
			{
				throw std::string("AnalysisFieldContent PositionDetails.csv failed");
			}

			auto record = PositionDetail::Allocate();
			Utility::Strcpy(record->TradingDay, csvRecord.GetFieldAsString("TradingDay"));
			Utility::Strcpy(record->AccountId, csvRecord.GetFieldAsString("AccountId"));
			record->AccountType = static_cast<AccountTypeType>(csvRecord.GetFieldAsInt("AccountType"));
			Utility::Strcpy(record->ExchangeId, csvRecord.GetFieldAsString("ExchangeId"));
			Utility::Strcpy(record->InstrumentId, csvRecord.GetFieldAsString("InstrumentId"));
			record->ProductClass = static_cast<ProductClassType>(csvRecord.GetFieldAsInt("ProductClass"));
			record->PosiDirection = static_cast<PosiDirectionType>(csvRecord.GetFieldAsInt("PosiDirection"));
			Utility::Strcpy(record->OpenDate, csvRecord.GetFieldAsString("OpenDate"));
			Utility::Strcpy(record->TradeId, csvRecord.GetFieldAsString("TradeId"));
			record->Volume = csvRecord.GetFieldAsInt64("Volume");
			record->OpenPrice = csvRecord.GetFieldAsDouble("OpenPrice");
			record->MarketValue = csvRecord.GetFieldAsDouble("MarketValue");
			record->CashIn = csvRecord.GetFieldAsDouble("CashIn");
			record->CashOut = csvRecord.GetFieldAsDouble("CashOut");
			record->Margin = csvRecord.GetFieldAsDouble("Margin");
			record->Commission = csvRecord.GetFieldAsDouble("Commission");
			record->VolumeMultiple = csvRecord.GetFieldAsInt("VolumeMultiple");
			record->CloseProfitByDate = csvRecord.GetFieldAsDouble("CloseProfitByDate");
			record->CloseProfitByTrade = csvRecord.GetFieldAsDouble("CloseProfitByTrade");
			record->PositionProfitByDate = csvRecord.GetFieldAsDouble("PositionProfitByDate");
			record->PositionProfitByTrade = csvRecord.GetFieldAsDouble("PositionProfitByTrade");
			record->SettlementPrice = csvRecord.GetFieldAsDouble("SettlementPrice");
			record->PreSettlementPrice = csvRecord.GetFieldAsDouble("PreSettlementPrice");
			record->CloseVolume = csvRecord.GetFieldAsInt64("CloseVolume");
			record->CloseAmount = csvRecord.GetFieldAsDouble("CloseAmount");
			mdb->PositionDetails->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadOrders(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		snprintf(fullPath, sizeof(fullPath), "%s/Orders.csv", dir);
		std::fstream file(fullPath, std::fstream::in);
		if (!file)
		{
			throw std::string(fullPath) + " Open Failed.";
		}

		file.getline(HeaderBuffer, sizeof(HeaderBuffer), '\n');
		CsvRecord csvRecord;
		if (!csvRecord.AnalysisFieldName(HeaderBuffer))
		{
			throw std::string("AnalysisFieldName Orders.csv failed");
		}
		while (!file.eof())
		{
			::memset(ContentBuffer, 0, sizeof(ContentBuffer));
			file.getline(ContentBuffer, sizeof(ContentBuffer), '\n');
			if (ContentBuffer[0] == '\0')
				break;
			if (!csvRecord.AnalysisFieldContent(ContentBuffer))
			{
				throw std::string("AnalysisFieldContent Orders.csv failed");
			}

			auto record = Order::Allocate();
			Utility::Strcpy(record->TradingDay, csvRecord.GetFieldAsString("TradingDay"));
			Utility::Strcpy(record->AccountId, csvRecord.GetFieldAsString("AccountId"));
			record->AccountType = static_cast<AccountTypeType>(csvRecord.GetFieldAsInt("AccountType"));
			Utility::Strcpy(record->ExchangeId, csvRecord.GetFieldAsString("ExchangeId"));
			Utility::Strcpy(record->InstrumentId, csvRecord.GetFieldAsString("InstrumentId"));
			record->ProductClass = static_cast<ProductClassType>(csvRecord.GetFieldAsInt("ProductClass"));
			record->OrderId = csvRecord.GetFieldAsInt("OrderId");
			Utility::Strcpy(record->OrderSysId, csvRecord.GetFieldAsString("OrderSysId"));
			record->Direction = static_cast<DirectionType>(csvRecord.GetFieldAsInt("Direction"));
			record->OffsetFlag = static_cast<OffsetFlagType>(csvRecord.GetFieldAsInt("OffsetFlag"));
			record->OrderPriceType = static_cast<OrderPriceTypeType>(csvRecord.GetFieldAsInt("OrderPriceType"));
			record->Price = csvRecord.GetFieldAsDouble("Price");
			record->Volume = csvRecord.GetFieldAsInt64("Volume");
			record->VolumeTotal = csvRecord.GetFieldAsInt64("VolumeTotal");
			record->VolumeTraded = csvRecord.GetFieldAsInt64("VolumeTraded");
			record->VolumeMultiple = csvRecord.GetFieldAsInt("VolumeMultiple");
			record->OrderStatus = static_cast<OrderStatusType>(csvRecord.GetFieldAsInt("OrderStatus"));
			Utility::Strcpy(record->OrderDate, csvRecord.GetFieldAsString("OrderDate"));
			Utility::Strcpy(record->OrderTime, csvRecord.GetFieldAsString("OrderTime"));
			Utility::Strcpy(record->CancelDate, csvRecord.GetFieldAsString("CancelDate"));
			Utility::Strcpy(record->CancelTime, csvRecord.GetFieldAsString("CancelTime"));
			record->SessionId = csvRecord.GetFieldAsInt64("SessionId");
			record->ClientOrderId = csvRecord.GetFieldAsInt("ClientOrderId");
			record->RequestId = csvRecord.GetFieldAsInt("RequestId");
			record->OfferId = csvRecord.GetFieldAsInt("OfferId");
			record->TradeGroupId = csvRecord.GetFieldAsInt("TradeGroupId");
			record->RiskGroupId = csvRecord.GetFieldAsInt("RiskGroupId");
			record->CommissionGroupId = csvRecord.GetFieldAsInt("CommissionGroupId");
			record->FrozenCash = csvRecord.GetFieldAsDouble("FrozenCash");
			record->FrozenMargin = csvRecord.GetFieldAsDouble("FrozenMargin");
			record->FrozenCommission = csvRecord.GetFieldAsDouble("FrozenCommission");
			record->RebuildMark = static_cast<bool>(csvRecord.GetFieldAsInt("RebuildMark"));
			record->IsForceClose = static_cast<bool>(csvRecord.GetFieldAsInt("IsForceClose"));
			mdb->Orders->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadTrades(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		snprintf(fullPath, sizeof(fullPath), "%s/Trades.csv", dir);
		std::fstream file(fullPath, std::fstream::in);
		if (!file)
		{
			throw std::string(fullPath) + " Open Failed.";
		}

		file.getline(HeaderBuffer, sizeof(HeaderBuffer), '\n');
		CsvRecord csvRecord;
		if (!csvRecord.AnalysisFieldName(HeaderBuffer))
		{
			throw std::string("AnalysisFieldName Trades.csv failed");
		}
		while (!file.eof())
		{
			::memset(ContentBuffer, 0, sizeof(ContentBuffer));
			file.getline(ContentBuffer, sizeof(ContentBuffer), '\n');
			if (ContentBuffer[0] == '\0')
				break;
			if (!csvRecord.AnalysisFieldContent(ContentBuffer))
			{
				throw std::string("AnalysisFieldContent Trades.csv failed");
			}

			auto record = Trade::Allocate();
			Utility::Strcpy(record->TradingDay, csvRecord.GetFieldAsString("TradingDay"));
			Utility::Strcpy(record->AccountId, csvRecord.GetFieldAsString("AccountId"));
			record->AccountType = static_cast<AccountTypeType>(csvRecord.GetFieldAsInt("AccountType"));
			Utility::Strcpy(record->ExchangeId, csvRecord.GetFieldAsString("ExchangeId"));
			Utility::Strcpy(record->InstrumentId, csvRecord.GetFieldAsString("InstrumentId"));
			record->ProductClass = static_cast<ProductClassType>(csvRecord.GetFieldAsInt("ProductClass"));
			record->OrderId = csvRecord.GetFieldAsInt("OrderId");
			Utility::Strcpy(record->OrderSysId, csvRecord.GetFieldAsString("OrderSysId"));
			Utility::Strcpy(record->TradeId, csvRecord.GetFieldAsString("TradeId"));
			record->Direction = static_cast<DirectionType>(csvRecord.GetFieldAsInt("Direction"));
			record->OffsetFlag = static_cast<OffsetFlagType>(csvRecord.GetFieldAsInt("OffsetFlag"));
			record->Price = csvRecord.GetFieldAsDouble("Price");
			record->Volume = csvRecord.GetFieldAsInt64("Volume");
			record->VolumeMultiple = csvRecord.GetFieldAsInt("VolumeMultiple");
			record->TradeAmount = csvRecord.GetFieldAsDouble("TradeAmount");
			record->Commission = csvRecord.GetFieldAsDouble("Commission");
			Utility::Strcpy(record->TradeDate, csvRecord.GetFieldAsString("TradeDate"));
			Utility::Strcpy(record->TradeTime, csvRecord.GetFieldAsString("TradeTime"));
			mdb->Trades->Insert(record);
		}
		file.close();
	}
}
