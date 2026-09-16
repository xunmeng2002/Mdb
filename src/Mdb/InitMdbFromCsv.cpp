// 本文件由 ../Templates/Cpp/Mdb/InitMdbFromCsv.cpp.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
#include "InitMdbFromCsv.h"
#include <Spark/Serialization/Csv/CsvRecord.h>
#include <Spark/Core/Utility/Utility.h>
#include <fstream>
#include <assert.h>

using namespace Spark::Core;
using namespace Spark::Serialization;


namespace Mdb
{
	static char HeaderBuffer[1024] = { 0 };
	static char ContentBuffer[64 * 1024] = { 0 };

	void InitMdbFromCsv::LoadTables(Mdb* mdb, const char* dir, const TableList& tableList)
	{
		for (int i = 0; i < tableList.Count; ++i)
		{
			switch (tableList.TableIDs[i])
			{
			case TradingDay::TableID:  LoadTradingDayTable(mdb, dir); break;
			case Exchange::TableID:  LoadExchangeTable(mdb, dir); break;
			case Product::TableID:  LoadProductTable(mdb, dir); break;
			case Instrument::TableID:  LoadInstrumentTable(mdb, dir); break;
			case PrimaryAccount::TableID:  LoadPrimaryAccountTable(mdb, dir); break;
			case Account::TableID:  LoadAccountTable(mdb, dir); break;
			case Capital::TableID:  LoadCapitalTable(mdb, dir); break;
			case Position::TableID:  LoadPositionTable(mdb, dir); break;
			case PositionDetail::TableID:  LoadPositionDetailTable(mdb, dir); break;
			case Order::TableID:  LoadOrderTable(mdb, dir); break;
			case Trade::TableID:  LoadTradeTable(mdb, dir); break;
			default: break;
			}
		}
	}

	void InitMdbFromCsv::LoadTradingDayTable(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		snprintf(fullPath, sizeof(fullPath), "%s/t_TradingDay.csv", dir);
		std::fstream file(fullPath, std::fstream::in);
		if (!file)
		{
			throw std::string(fullPath) + " Open Failed.";
		}

		file.getline(HeaderBuffer, sizeof(HeaderBuffer), '\n');
		CSVRecord csv_record;
		if (!csv_record.AnalysisFieldName(HeaderBuffer))
		{
			throw std::string("AnalysisFieldName t_TradingDay.csv failed");
		}
		while (!file.eof())
		{
			::memset(ContentBuffer, 0, sizeof(ContentBuffer));
			file.getline(ContentBuffer, sizeof(ContentBuffer), '\n');
			if (ContentBuffer[0] == '\0')
				break;
			if (!csv_record.AnalysisFieldContent(ContentBuffer))
			{
				throw std::string("AnalysisFieldContent t_TradingDay.csv failed");
			}

			auto record = TradingDay::Allocate();
			record->PK = csv_record.GetFieldAsInt("PK");
			Utility::Strcpy(record->CurrTradingDay, csv_record.GetFieldAsString("CurrTradingDay"));
			Utility::Strcpy(record->PreTradingDay, csv_record.GetFieldAsString("PreTradingDay"));
			mdb->tradingDay->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadExchangeTable(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		snprintf(fullPath, sizeof(fullPath), "%s/t_Exchange.csv", dir);
		std::fstream file(fullPath, std::fstream::in);
		if (!file)
		{
			throw std::string(fullPath) + " Open Failed.";
		}

		file.getline(HeaderBuffer, sizeof(HeaderBuffer), '\n');
		CSVRecord csv_record;
		if (!csv_record.AnalysisFieldName(HeaderBuffer))
		{
			throw std::string("AnalysisFieldName t_Exchange.csv failed");
		}
		while (!file.eof())
		{
			::memset(ContentBuffer, 0, sizeof(ContentBuffer));
			file.getline(ContentBuffer, sizeof(ContentBuffer), '\n');
			if (ContentBuffer[0] == '\0')
				break;
			if (!csv_record.AnalysisFieldContent(ContentBuffer))
			{
				throw std::string("AnalysisFieldContent t_Exchange.csv failed");
			}

			auto record = Exchange::Allocate();
			Utility::Strcpy(record->ExchangeId, csv_record.GetFieldAsString("ExchangeId"));
			Utility::Strcpy(record->ExchangeName, csv_record.GetFieldAsString("ExchangeName"));
			mdb->exchange->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadProductTable(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		snprintf(fullPath, sizeof(fullPath), "%s/t_Product.csv", dir);
		std::fstream file(fullPath, std::fstream::in);
		if (!file)
		{
			throw std::string(fullPath) + " Open Failed.";
		}

		file.getline(HeaderBuffer, sizeof(HeaderBuffer), '\n');
		CSVRecord csv_record;
		if (!csv_record.AnalysisFieldName(HeaderBuffer))
		{
			throw std::string("AnalysisFieldName t_Product.csv failed");
		}
		while (!file.eof())
		{
			::memset(ContentBuffer, 0, sizeof(ContentBuffer));
			file.getline(ContentBuffer, sizeof(ContentBuffer), '\n');
			if (ContentBuffer[0] == '\0')
				break;
			if (!csv_record.AnalysisFieldContent(ContentBuffer))
			{
				throw std::string("AnalysisFieldContent t_Product.csv failed");
			}

			auto record = Product::Allocate();
			Utility::Strcpy(record->ExchangeId, csv_record.GetFieldAsString("ExchangeId"));
			Utility::Strcpy(record->ProductId, csv_record.GetFieldAsString("ProductId"));
			Utility::Strcpy(record->ProductName, csv_record.GetFieldAsString("ProductName"));
			record->ProductClass = static_cast<ProductClassType>(csv_record.GetFieldAsInt("ProductClass"));
			record->VolumeMultiple = csv_record.GetFieldAsInt("VolumeMultiple");
			record->PriceTick = csv_record.GetFieldAsDouble("PriceTick");
			record->MaxMarketOrderVolume = csv_record.GetFieldAsInt64("MaxMarketOrderVolume");
			record->MinMarketOrderVolume = csv_record.GetFieldAsInt64("MinMarketOrderVolume");
			record->MaxLimitOrderVolume = csv_record.GetFieldAsInt64("MaxLimitOrderVolume");
			record->MinLimitOrderVolume = csv_record.GetFieldAsInt64("MinLimitOrderVolume");
			Utility::Strcpy(record->SessionName, csv_record.GetFieldAsString("SessionName"));
			mdb->product->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadInstrumentTable(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		snprintf(fullPath, sizeof(fullPath), "%s/t_Instrument.csv", dir);
		std::fstream file(fullPath, std::fstream::in);
		if (!file)
		{
			throw std::string(fullPath) + " Open Failed.";
		}

		file.getline(HeaderBuffer, sizeof(HeaderBuffer), '\n');
		CSVRecord csv_record;
		if (!csv_record.AnalysisFieldName(HeaderBuffer))
		{
			throw std::string("AnalysisFieldName t_Instrument.csv failed");
		}
		while (!file.eof())
		{
			::memset(ContentBuffer, 0, sizeof(ContentBuffer));
			file.getline(ContentBuffer, sizeof(ContentBuffer), '\n');
			if (ContentBuffer[0] == '\0')
				break;
			if (!csv_record.AnalysisFieldContent(ContentBuffer))
			{
				throw std::string("AnalysisFieldContent t_Instrument.csv failed");
			}

			auto record = Instrument::Allocate();
			Utility::Strcpy(record->ExchangeId, csv_record.GetFieldAsString("ExchangeId"));
			Utility::Strcpy(record->InstrumentId, csv_record.GetFieldAsString("InstrumentId"));
			Utility::Strcpy(record->ExchangeInstId, csv_record.GetFieldAsString("ExchangeInstId"));
			Utility::Strcpy(record->InstrumentName, csv_record.GetFieldAsString("InstrumentName"));
			Utility::Strcpy(record->ProductId, csv_record.GetFieldAsString("ProductId"));
			record->ProductClass = static_cast<ProductClassType>(csv_record.GetFieldAsInt("ProductClass"));
			record->InstrumentClass = static_cast<InstrumentClassType>(csv_record.GetFieldAsInt("InstrumentClass"));
			record->Rank = csv_record.GetFieldAsInt("Rank");
			record->VolumeMultiple = csv_record.GetFieldAsInt("VolumeMultiple");
			record->PriceTick = csv_record.GetFieldAsDouble("PriceTick");
			record->MaxMarketOrderVolume = csv_record.GetFieldAsInt64("MaxMarketOrderVolume");
			record->MinMarketOrderVolume = csv_record.GetFieldAsInt64("MinMarketOrderVolume");
			record->MaxLimitOrderVolume = csv_record.GetFieldAsInt64("MaxLimitOrderVolume");
			record->MinLimitOrderVolume = csv_record.GetFieldAsInt64("MinLimitOrderVolume");
			Utility::Strcpy(record->SessionName, csv_record.GetFieldAsString("SessionName"));
			mdb->instrument->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadPrimaryAccountTable(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		snprintf(fullPath, sizeof(fullPath), "%s/t_PrimaryAccount.csv", dir);
		std::fstream file(fullPath, std::fstream::in);
		if (!file)
		{
			throw std::string(fullPath) + " Open Failed.";
		}

		file.getline(HeaderBuffer, sizeof(HeaderBuffer), '\n');
		CSVRecord csv_record;
		if (!csv_record.AnalysisFieldName(HeaderBuffer))
		{
			throw std::string("AnalysisFieldName t_PrimaryAccount.csv failed");
		}
		while (!file.eof())
		{
			::memset(ContentBuffer, 0, sizeof(ContentBuffer));
			file.getline(ContentBuffer, sizeof(ContentBuffer), '\n');
			if (ContentBuffer[0] == '\0')
				break;
			if (!csv_record.AnalysisFieldContent(ContentBuffer))
			{
				throw std::string("AnalysisFieldContent t_PrimaryAccount.csv failed");
			}

			auto record = PrimaryAccount::Allocate();
			Utility::Strcpy(record->PrimaryAccountId, csv_record.GetFieldAsString("PrimaryAccountId"));
			Utility::Strcpy(record->PrimaryAccountName, csv_record.GetFieldAsString("PrimaryAccountName"));
			record->AccountClass = static_cast<AccountClassType>(csv_record.GetFieldAsInt("AccountClass"));
			Utility::Strcpy(record->BrokerPassword, csv_record.GetFieldAsString("BrokerPassword"));
			record->OfferId = csv_record.GetFieldAsInt("OfferId");
			record->IsAllowLogin = static_cast<bool>(csv_record.GetFieldAsInt("IsAllowLogin"));
			record->IsSimulateAccount = static_cast<bool>(csv_record.GetFieldAsInt("IsSimulateAccount"));
			record->LoginStatus = static_cast<LoginStatusType>(csv_record.GetFieldAsInt("LoginStatus"));
			record->InitStatus = static_cast<InitStatusType>(csv_record.GetFieldAsInt("InitStatus"));
			mdb->primaryAccount->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadAccountTable(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		snprintf(fullPath, sizeof(fullPath), "%s/t_Account.csv", dir);
		std::fstream file(fullPath, std::fstream::in);
		if (!file)
		{
			throw std::string(fullPath) + " Open Failed.";
		}

		file.getline(HeaderBuffer, sizeof(HeaderBuffer), '\n');
		CSVRecord csv_record;
		if (!csv_record.AnalysisFieldName(HeaderBuffer))
		{
			throw std::string("AnalysisFieldName t_Account.csv failed");
		}
		while (!file.eof())
		{
			::memset(ContentBuffer, 0, sizeof(ContentBuffer));
			file.getline(ContentBuffer, sizeof(ContentBuffer), '\n');
			if (ContentBuffer[0] == '\0')
				break;
			if (!csv_record.AnalysisFieldContent(ContentBuffer))
			{
				throw std::string("AnalysisFieldContent t_Account.csv failed");
			}

			auto record = Account::Allocate();
			Utility::Strcpy(record->AccountId, csv_record.GetFieldAsString("AccountId"));
			Utility::Strcpy(record->AccountName, csv_record.GetFieldAsString("AccountName"));
			record->AccountType = static_cast<AccountTypeType>(csv_record.GetFieldAsInt("AccountType"));
			record->AccountStatus = static_cast<AccountStatusType>(csv_record.GetFieldAsInt("AccountStatus"));
			Utility::Strcpy(record->Password, csv_record.GetFieldAsString("Password"));
			record->TradeGroupId = csv_record.GetFieldAsInt("TradeGroupId");
			record->RiskGroupId = csv_record.GetFieldAsInt("RiskGroupId");
			record->CommissionGroupId = csv_record.GetFieldAsInt("CommissionGroupId");
			mdb->account->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadCapitalTable(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		snprintf(fullPath, sizeof(fullPath), "%s/t_Capital.csv", dir);
		std::fstream file(fullPath, std::fstream::in);
		if (!file)
		{
			throw std::string(fullPath) + " Open Failed.";
		}

		file.getline(HeaderBuffer, sizeof(HeaderBuffer), '\n');
		CSVRecord csv_record;
		if (!csv_record.AnalysisFieldName(HeaderBuffer))
		{
			throw std::string("AnalysisFieldName t_Capital.csv failed");
		}
		while (!file.eof())
		{
			::memset(ContentBuffer, 0, sizeof(ContentBuffer));
			file.getline(ContentBuffer, sizeof(ContentBuffer), '\n');
			if (ContentBuffer[0] == '\0')
				break;
			if (!csv_record.AnalysisFieldContent(ContentBuffer))
			{
				throw std::string("AnalysisFieldContent t_Capital.csv failed");
			}

			auto record = Capital::Allocate();
			Utility::Strcpy(record->TradingDay, csv_record.GetFieldAsString("TradingDay"));
			Utility::Strcpy(record->AccountId, csv_record.GetFieldAsString("AccountId"));
			record->AccountType = static_cast<AccountTypeType>(csv_record.GetFieldAsInt("AccountType"));
			record->Balance = csv_record.GetFieldAsDouble("Balance");
			record->PreBalance = csv_record.GetFieldAsDouble("PreBalance");
			record->Available = csv_record.GetFieldAsDouble("Available");
			record->MarketValue = csv_record.GetFieldAsDouble("MarketValue");
			record->CashIn = csv_record.GetFieldAsDouble("CashIn");
			record->CashOut = csv_record.GetFieldAsDouble("CashOut");
			record->Margin = csv_record.GetFieldAsDouble("Margin");
			record->Commission = csv_record.GetFieldAsDouble("Commission");
			record->FrozenCash = csv_record.GetFieldAsDouble("FrozenCash");
			record->FrozenMargin = csv_record.GetFieldAsDouble("FrozenMargin");
			record->FrozenCommission = csv_record.GetFieldAsDouble("FrozenCommission");
			record->CloseProfitByDate = csv_record.GetFieldAsDouble("CloseProfitByDate");
			record->CloseProfitByTrade = csv_record.GetFieldAsDouble("CloseProfitByTrade");
			record->PositionProfitByDate = csv_record.GetFieldAsDouble("PositionProfitByDate");
			record->PositionProfitByTrade = csv_record.GetFieldAsDouble("PositionProfitByTrade");
			record->Deposit = csv_record.GetFieldAsDouble("Deposit");
			record->Withdraw = csv_record.GetFieldAsDouble("Withdraw");
			mdb->capital->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadPositionTable(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		snprintf(fullPath, sizeof(fullPath), "%s/t_Position.csv", dir);
		std::fstream file(fullPath, std::fstream::in);
		if (!file)
		{
			throw std::string(fullPath) + " Open Failed.";
		}

		file.getline(HeaderBuffer, sizeof(HeaderBuffer), '\n');
		CSVRecord csv_record;
		if (!csv_record.AnalysisFieldName(HeaderBuffer))
		{
			throw std::string("AnalysisFieldName t_Position.csv failed");
		}
		while (!file.eof())
		{
			::memset(ContentBuffer, 0, sizeof(ContentBuffer));
			file.getline(ContentBuffer, sizeof(ContentBuffer), '\n');
			if (ContentBuffer[0] == '\0')
				break;
			if (!csv_record.AnalysisFieldContent(ContentBuffer))
			{
				throw std::string("AnalysisFieldContent t_Position.csv failed");
			}

			auto record = Position::Allocate();
			Utility::Strcpy(record->TradingDay, csv_record.GetFieldAsString("TradingDay"));
			Utility::Strcpy(record->AccountId, csv_record.GetFieldAsString("AccountId"));
			record->AccountType = static_cast<AccountTypeType>(csv_record.GetFieldAsInt("AccountType"));
			Utility::Strcpy(record->ExchangeId, csv_record.GetFieldAsString("ExchangeId"));
			Utility::Strcpy(record->InstrumentId, csv_record.GetFieldAsString("InstrumentId"));
			record->ProductClass = static_cast<ProductClassType>(csv_record.GetFieldAsInt("ProductClass"));
			record->PosiDirection = static_cast<PosiDirectionType>(csv_record.GetFieldAsInt("PosiDirection"));
			record->TotalPosition = csv_record.GetFieldAsInt64("TotalPosition");
			record->PositionFrozen = csv_record.GetFieldAsInt64("PositionFrozen");
			record->TodayPosition = csv_record.GetFieldAsInt64("TodayPosition");
			record->MarketValue = csv_record.GetFieldAsDouble("MarketValue");
			record->CashIn = csv_record.GetFieldAsDouble("CashIn");
			record->CashOut = csv_record.GetFieldAsDouble("CashOut");
			record->Margin = csv_record.GetFieldAsDouble("Margin");
			record->Commission = csv_record.GetFieldAsDouble("Commission");
			record->FrozenCash = csv_record.GetFieldAsDouble("FrozenCash");
			record->FrozenMargin = csv_record.GetFieldAsDouble("FrozenMargin");
			record->FrozenCommission = csv_record.GetFieldAsDouble("FrozenCommission");
			record->VolumeMultiple = csv_record.GetFieldAsInt("VolumeMultiple");
			record->CloseProfitByDate = csv_record.GetFieldAsDouble("CloseProfitByDate");
			record->CloseProfitByTrade = csv_record.GetFieldAsDouble("CloseProfitByTrade");
			record->PositionProfitByDate = csv_record.GetFieldAsDouble("PositionProfitByDate");
			record->PositionProfitByTrade = csv_record.GetFieldAsDouble("PositionProfitByTrade");
			record->SettlementPrice = csv_record.GetFieldAsDouble("SettlementPrice");
			record->PreSettlementPrice = csv_record.GetFieldAsDouble("PreSettlementPrice");
			mdb->position->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadPositionDetailTable(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		snprintf(fullPath, sizeof(fullPath), "%s/t_PositionDetail.csv", dir);
		std::fstream file(fullPath, std::fstream::in);
		if (!file)
		{
			throw std::string(fullPath) + " Open Failed.";
		}

		file.getline(HeaderBuffer, sizeof(HeaderBuffer), '\n');
		CSVRecord csv_record;
		if (!csv_record.AnalysisFieldName(HeaderBuffer))
		{
			throw std::string("AnalysisFieldName t_PositionDetail.csv failed");
		}
		while (!file.eof())
		{
			::memset(ContentBuffer, 0, sizeof(ContentBuffer));
			file.getline(ContentBuffer, sizeof(ContentBuffer), '\n');
			if (ContentBuffer[0] == '\0')
				break;
			if (!csv_record.AnalysisFieldContent(ContentBuffer))
			{
				throw std::string("AnalysisFieldContent t_PositionDetail.csv failed");
			}

			auto record = PositionDetail::Allocate();
			Utility::Strcpy(record->TradingDay, csv_record.GetFieldAsString("TradingDay"));
			Utility::Strcpy(record->AccountId, csv_record.GetFieldAsString("AccountId"));
			record->AccountType = static_cast<AccountTypeType>(csv_record.GetFieldAsInt("AccountType"));
			Utility::Strcpy(record->ExchangeId, csv_record.GetFieldAsString("ExchangeId"));
			Utility::Strcpy(record->InstrumentId, csv_record.GetFieldAsString("InstrumentId"));
			record->ProductClass = static_cast<ProductClassType>(csv_record.GetFieldAsInt("ProductClass"));
			record->PosiDirection = static_cast<PosiDirectionType>(csv_record.GetFieldAsInt("PosiDirection"));
			Utility::Strcpy(record->OpenDate, csv_record.GetFieldAsString("OpenDate"));
			Utility::Strcpy(record->TradeId, csv_record.GetFieldAsString("TradeId"));
			record->Volume = csv_record.GetFieldAsInt64("Volume");
			record->OpenPrice = csv_record.GetFieldAsDouble("OpenPrice");
			record->MarketValue = csv_record.GetFieldAsDouble("MarketValue");
			record->CashIn = csv_record.GetFieldAsDouble("CashIn");
			record->CashOut = csv_record.GetFieldAsDouble("CashOut");
			record->Margin = csv_record.GetFieldAsDouble("Margin");
			record->Commission = csv_record.GetFieldAsDouble("Commission");
			record->VolumeMultiple = csv_record.GetFieldAsInt("VolumeMultiple");
			record->CloseProfitByDate = csv_record.GetFieldAsDouble("CloseProfitByDate");
			record->CloseProfitByTrade = csv_record.GetFieldAsDouble("CloseProfitByTrade");
			record->PositionProfitByDate = csv_record.GetFieldAsDouble("PositionProfitByDate");
			record->PositionProfitByTrade = csv_record.GetFieldAsDouble("PositionProfitByTrade");
			record->SettlementPrice = csv_record.GetFieldAsDouble("SettlementPrice");
			record->PreSettlementPrice = csv_record.GetFieldAsDouble("PreSettlementPrice");
			record->CloseVolume = csv_record.GetFieldAsInt64("CloseVolume");
			record->CloseAmount = csv_record.GetFieldAsDouble("CloseAmount");
			mdb->positionDetail->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadOrderTable(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		snprintf(fullPath, sizeof(fullPath), "%s/t_Order.csv", dir);
		std::fstream file(fullPath, std::fstream::in);
		if (!file)
		{
			throw std::string(fullPath) + " Open Failed.";
		}

		file.getline(HeaderBuffer, sizeof(HeaderBuffer), '\n');
		CSVRecord csv_record;
		if (!csv_record.AnalysisFieldName(HeaderBuffer))
		{
			throw std::string("AnalysisFieldName t_Order.csv failed");
		}
		while (!file.eof())
		{
			::memset(ContentBuffer, 0, sizeof(ContentBuffer));
			file.getline(ContentBuffer, sizeof(ContentBuffer), '\n');
			if (ContentBuffer[0] == '\0')
				break;
			if (!csv_record.AnalysisFieldContent(ContentBuffer))
			{
				throw std::string("AnalysisFieldContent t_Order.csv failed");
			}

			auto record = Order::Allocate();
			Utility::Strcpy(record->TradingDay, csv_record.GetFieldAsString("TradingDay"));
			Utility::Strcpy(record->AccountId, csv_record.GetFieldAsString("AccountId"));
			record->AccountType = static_cast<AccountTypeType>(csv_record.GetFieldAsInt("AccountType"));
			Utility::Strcpy(record->ExchangeId, csv_record.GetFieldAsString("ExchangeId"));
			Utility::Strcpy(record->InstrumentId, csv_record.GetFieldAsString("InstrumentId"));
			record->ProductClass = static_cast<ProductClassType>(csv_record.GetFieldAsInt("ProductClass"));
			record->OrderId = csv_record.GetFieldAsInt("OrderId");
			Utility::Strcpy(record->OrderSysId, csv_record.GetFieldAsString("OrderSysId"));
			record->Direction = static_cast<DirectionType>(csv_record.GetFieldAsInt("Direction"));
			record->OffsetFlag = static_cast<OffsetFlagType>(csv_record.GetFieldAsInt("OffsetFlag"));
			record->OrderPriceType = static_cast<OrderPriceTypeType>(csv_record.GetFieldAsInt("OrderPriceType"));
			record->Price = csv_record.GetFieldAsDouble("Price");
			record->Volume = csv_record.GetFieldAsInt64("Volume");
			record->VolumeTotal = csv_record.GetFieldAsInt64("VolumeTotal");
			record->VolumeTraded = csv_record.GetFieldAsInt64("VolumeTraded");
			record->VolumeMultiple = csv_record.GetFieldAsInt("VolumeMultiple");
			record->OrderStatus = static_cast<OrderStatusType>(csv_record.GetFieldAsInt("OrderStatus"));
			Utility::Strcpy(record->OrderDate, csv_record.GetFieldAsString("OrderDate"));
			Utility::Strcpy(record->OrderTime, csv_record.GetFieldAsString("OrderTime"));
			Utility::Strcpy(record->CancelDate, csv_record.GetFieldAsString("CancelDate"));
			Utility::Strcpy(record->CancelTime, csv_record.GetFieldAsString("CancelTime"));
			record->SessionId = csv_record.GetFieldAsInt64("SessionId");
			record->ClientOrderId = csv_record.GetFieldAsInt("ClientOrderId");
			record->RequestId = csv_record.GetFieldAsInt("RequestId");
			record->OfferId = csv_record.GetFieldAsInt("OfferId");
			record->TradeGroupId = csv_record.GetFieldAsInt("TradeGroupId");
			record->RiskGroupId = csv_record.GetFieldAsInt("RiskGroupId");
			record->CommissionGroupId = csv_record.GetFieldAsInt("CommissionGroupId");
			record->FrozenCash = csv_record.GetFieldAsDouble("FrozenCash");
			record->FrozenMargin = csv_record.GetFieldAsDouble("FrozenMargin");
			record->FrozenCommission = csv_record.GetFieldAsDouble("FrozenCommission");
			record->RebuildMark = static_cast<bool>(csv_record.GetFieldAsInt("RebuildMark"));
			record->IsForceClose = static_cast<bool>(csv_record.GetFieldAsInt("IsForceClose"));
			mdb->order->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadTradeTable(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		snprintf(fullPath, sizeof(fullPath), "%s/t_Trade.csv", dir);
		std::fstream file(fullPath, std::fstream::in);
		if (!file)
		{
			throw std::string(fullPath) + " Open Failed.";
		}

		file.getline(HeaderBuffer, sizeof(HeaderBuffer), '\n');
		CSVRecord csv_record;
		if (!csv_record.AnalysisFieldName(HeaderBuffer))
		{
			throw std::string("AnalysisFieldName t_Trade.csv failed");
		}
		while (!file.eof())
		{
			::memset(ContentBuffer, 0, sizeof(ContentBuffer));
			file.getline(ContentBuffer, sizeof(ContentBuffer), '\n');
			if (ContentBuffer[0] == '\0')
				break;
			if (!csv_record.AnalysisFieldContent(ContentBuffer))
			{
				throw std::string("AnalysisFieldContent t_Trade.csv failed");
			}

			auto record = Trade::Allocate();
			Utility::Strcpy(record->TradingDay, csv_record.GetFieldAsString("TradingDay"));
			Utility::Strcpy(record->AccountId, csv_record.GetFieldAsString("AccountId"));
			record->AccountType = static_cast<AccountTypeType>(csv_record.GetFieldAsInt("AccountType"));
			Utility::Strcpy(record->ExchangeId, csv_record.GetFieldAsString("ExchangeId"));
			Utility::Strcpy(record->InstrumentId, csv_record.GetFieldAsString("InstrumentId"));
			record->ProductClass = static_cast<ProductClassType>(csv_record.GetFieldAsInt("ProductClass"));
			record->OrderId = csv_record.GetFieldAsInt("OrderId");
			Utility::Strcpy(record->OrderSysId, csv_record.GetFieldAsString("OrderSysId"));
			Utility::Strcpy(record->TradeId, csv_record.GetFieldAsString("TradeId"));
			record->Direction = static_cast<DirectionType>(csv_record.GetFieldAsInt("Direction"));
			record->OffsetFlag = static_cast<OffsetFlagType>(csv_record.GetFieldAsInt("OffsetFlag"));
			record->Price = csv_record.GetFieldAsDouble("Price");
			record->Volume = csv_record.GetFieldAsInt64("Volume");
			record->VolumeMultiple = csv_record.GetFieldAsInt("VolumeMultiple");
			record->TradeAmount = csv_record.GetFieldAsDouble("TradeAmount");
			record->Commission = csv_record.GetFieldAsDouble("Commission");
			Utility::Strcpy(record->TradeDate, csv_record.GetFieldAsString("TradeDate"));
			Utility::Strcpy(record->TradeTime, csv_record.GetFieldAsString("TradeTime"));
			mdb->trade->Insert(record);
		}
		file.close();
	}
}
