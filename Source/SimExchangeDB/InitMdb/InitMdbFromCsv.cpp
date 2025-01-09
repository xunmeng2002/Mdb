#include "InitMdbFromCsv.h"
#include "CsvRecord.h"
#include "Utility.h"
#include <fstream>
#include <assert.h>

using namespace std;


namespace mdb
{
	static char HeaderBuffer[1024] = { 0 };
	static char ContentBuffer[64 * 1024] = { 0 };

	void InitMdbFromCsv::LoadTables(Mdb* mdb, const char* dir)
	{

		LoadTradingDayTable(mdb, dir);
		LoadExchangeTable(mdb, dir);
		LoadProductTable(mdb, dir);
		LoadInstrumentTable(mdb, dir);
		LoadAccountTable(mdb, dir);
		LoadPositionTable(mdb, dir);
		LoadOrderTable(mdb, dir);
		LoadTradeTable(mdb, dir);
		LoadMdTickTable(mdb, dir);
	}

	void InitMdbFromCsv::LoadTradingDayTable(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		sprintf(fullPath, "%s/t_TradingDay.csv", dir);
		fstream file(fullPath, fstream::in);
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

			auto record = new TradingDay();
			record->PK = csv_record.GetFieldAsInt("PK");
			Strcpy(record->CurrTradingDay, csv_record.GetFieldAsString("CurrTradingDay"));
			Strcpy(record->PreTradingDay, csv_record.GetFieldAsString("PreTradingDay"));
			mdb->t_TradingDay->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadExchangeTable(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		sprintf(fullPath, "%s/t_Exchange.csv", dir);
		fstream file(fullPath, fstream::in);
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

			auto record = new Exchange();
			Strcpy(record->ExchangeID, csv_record.GetFieldAsString("ExchangeID"));
			Strcpy(record->ExchangeName, csv_record.GetFieldAsString("ExchangeName"));
			mdb->t_Exchange->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadProductTable(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		sprintf(fullPath, "%s/t_Product.csv", dir);
		fstream file(fullPath, fstream::in);
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

			auto record = new Product();
			Strcpy(record->ExchangeID, csv_record.GetFieldAsString("ExchangeID"));
			Strcpy(record->ProductID, csv_record.GetFieldAsString("ProductID"));
			Strcpy(record->ProductName, csv_record.GetFieldAsString("ProductName"));
			record->ProductClass = (ProductClassType)csv_record.GetFieldAsInt("ProductClass");
			record->VolumeMultiple = csv_record.GetFieldAsInt("VolumeMultiple");
			record->PriceTick = csv_record.GetFieldAsDouble("PriceTick");
			record->MaxMarketOrderVolume = csv_record.GetFieldAsInt("MaxMarketOrderVolume");
			record->MinMarketOrderVolume = csv_record.GetFieldAsInt("MinMarketOrderVolume");
			record->MaxLimitOrderVolume = csv_record.GetFieldAsInt("MaxLimitOrderVolume");
			record->MinLimitOrderVolume = csv_record.GetFieldAsInt("MinLimitOrderVolume");
			Strcpy(record->SessionName, csv_record.GetFieldAsString("SessionName"));
			mdb->t_Product->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadInstrumentTable(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		sprintf(fullPath, "%s/t_Instrument.csv", dir);
		fstream file(fullPath, fstream::in);
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

			auto record = new Instrument();
			Strcpy(record->ExchangeID, csv_record.GetFieldAsString("ExchangeID"));
			Strcpy(record->InstrumentID, csv_record.GetFieldAsString("InstrumentID"));
			Strcpy(record->InstrumentName, csv_record.GetFieldAsString("InstrumentName"));
			Strcpy(record->ProductID, csv_record.GetFieldAsString("ProductID"));
			record->ProductClass = (ProductClassType)csv_record.GetFieldAsInt("ProductClass");
			record->VolumeMultiple = csv_record.GetFieldAsInt("VolumeMultiple");
			record->PriceTick = csv_record.GetFieldAsDouble("PriceTick");
			record->MaxMarketOrderVolume = csv_record.GetFieldAsInt("MaxMarketOrderVolume");
			record->MinMarketOrderVolume = csv_record.GetFieldAsInt("MinMarketOrderVolume");
			record->MaxLimitOrderVolume = csv_record.GetFieldAsInt("MaxLimitOrderVolume");
			record->MinLimitOrderVolume = csv_record.GetFieldAsInt("MinLimitOrderVolume");
			Strcpy(record->SessionName, csv_record.GetFieldAsString("SessionName"));
			record->DeliveryYear = csv_record.GetFieldAsInt("DeliveryYear");
			record->DeliveryMonth = csv_record.GetFieldAsInt("DeliveryMonth");
			mdb->t_Instrument->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadAccountTable(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		sprintf(fullPath, "%s/t_Account.csv", dir);
		fstream file(fullPath, fstream::in);
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

			auto record = new Account();
			Strcpy(record->TradingDay, csv_record.GetFieldAsString("TradingDay"));
			Strcpy(record->AccountID, csv_record.GetFieldAsString("AccountID"));
			Strcpy(record->AccountName, csv_record.GetFieldAsString("AccountName"));
			Strcpy(record->Password, csv_record.GetFieldAsString("Password"));
			record->PreBalance = csv_record.GetFieldAsDouble("PreBalance");
			record->Balance = csv_record.GetFieldAsDouble("Balance");
			record->CloseProfitByDate = csv_record.GetFieldAsDouble("CloseProfitByDate");
			record->PositionProfitByDate = csv_record.GetFieldAsDouble("PositionProfitByDate");
			record->PositionProfitByTrade = csv_record.GetFieldAsDouble("PositionProfitByTrade");
			record->PremiumIn = csv_record.GetFieldAsDouble("PremiumIn");
			record->PremiumOut = csv_record.GetFieldAsDouble("PremiumOut");
			record->MarketValue = csv_record.GetFieldAsDouble("MarketValue");
			mdb->t_Account->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadPositionTable(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		sprintf(fullPath, "%s/t_Position.csv", dir);
		fstream file(fullPath, fstream::in);
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

			auto record = new Position();
			Strcpy(record->TradingDay, csv_record.GetFieldAsString("TradingDay"));
			Strcpy(record->AccountID, csv_record.GetFieldAsString("AccountID"));
			Strcpy(record->ExchangeID, csv_record.GetFieldAsString("ExchangeID"));
			Strcpy(record->InstrumentID, csv_record.GetFieldAsString("InstrumentID"));
			record->ProductClass = (ProductClassType)csv_record.GetFieldAsInt("ProductClass");
			record->PosiDirection = (PosiDirectionType)csv_record.GetFieldAsInt("PosiDirection");
			record->TotalPosition = csv_record.GetFieldAsInt("TotalPosition");
			record->TodayPosition = csv_record.GetFieldAsInt("TodayPosition");
			record->FrozenPosition = csv_record.GetFieldAsInt("FrozenPosition");
			record->CloseProfitByDate = csv_record.GetFieldAsDouble("CloseProfitByDate");
			record->CloseProfitByTrade = csv_record.GetFieldAsDouble("CloseProfitByTrade");
			record->PositionProfitByDate = csv_record.GetFieldAsDouble("PositionProfitByDate");
			record->PositionProfitByTrade = csv_record.GetFieldAsDouble("PositionProfitByTrade");
			record->PremiumIn = csv_record.GetFieldAsDouble("PremiumIn");
			record->PremiumOut = csv_record.GetFieldAsDouble("PremiumOut");
			record->MarketValue = csv_record.GetFieldAsDouble("MarketValue");
			record->VolumeMultiple = csv_record.GetFieldAsInt("VolumeMultiple");
			record->SettlementPrice = csv_record.GetFieldAsDouble("SettlementPrice");
			record->PreSettlementPrice = csv_record.GetFieldAsDouble("PreSettlementPrice");
			mdb->t_Position->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadOrderTable(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		sprintf(fullPath, "%s/t_Order.csv", dir);
		fstream file(fullPath, fstream::in);
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

			auto record = new Order();
			Strcpy(record->TradingDay, csv_record.GetFieldAsString("TradingDay"));
			Strcpy(record->AccountID, csv_record.GetFieldAsString("AccountID"));
			Strcpy(record->ExchangeID, csv_record.GetFieldAsString("ExchangeID"));
			Strcpy(record->InstrumentID, csv_record.GetFieldAsString("InstrumentID"));
			record->OrderID = csv_record.GetFieldAsInt("OrderID");
			record->ClientOrderID = csv_record.GetFieldAsInt("ClientOrderID");
			record->Direction = (DirectionType)csv_record.GetFieldAsInt("Direction");
			record->OffsetFlag = (OffsetFlagType)csv_record.GetFieldAsInt("OffsetFlag");
			record->OrderPriceType = (OrderPriceTypeType)csv_record.GetFieldAsInt("OrderPriceType");
			record->Price = csv_record.GetFieldAsDouble("Price");
			record->Volume = csv_record.GetFieldAsInt("Volume");
			record->VolumeRemain = csv_record.GetFieldAsInt("VolumeRemain");
			record->VolumeTraded = csv_record.GetFieldAsInt("VolumeTraded");
			record->VolumeMultiple = csv_record.GetFieldAsInt("VolumeMultiple");
			record->OrderStatus = (OrderStatusType)csv_record.GetFieldAsInt("OrderStatus");
			Strcpy(record->OrderDate, csv_record.GetFieldAsString("OrderDate"));
			Strcpy(record->OrderTime, csv_record.GetFieldAsString("OrderTime"));
			Strcpy(record->CancelDate, csv_record.GetFieldAsString("CancelDate"));
			Strcpy(record->CancelTime, csv_record.GetFieldAsString("CancelTime"));
			mdb->t_Order->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadTradeTable(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		sprintf(fullPath, "%s/t_Trade.csv", dir);
		fstream file(fullPath, fstream::in);
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

			auto record = new Trade();
			Strcpy(record->TradingDay, csv_record.GetFieldAsString("TradingDay"));
			Strcpy(record->AccountID, csv_record.GetFieldAsString("AccountID"));
			Strcpy(record->ExchangeID, csv_record.GetFieldAsString("ExchangeID"));
			Strcpy(record->InstrumentID, csv_record.GetFieldAsString("InstrumentID"));
			record->OrderID = csv_record.GetFieldAsInt("OrderID");
			Strcpy(record->TradeID, csv_record.GetFieldAsString("TradeID"));
			record->Direction = (DirectionType)csv_record.GetFieldAsInt("Direction");
			record->OffsetFlag = (OffsetFlagType)csv_record.GetFieldAsInt("OffsetFlag");
			record->Price = csv_record.GetFieldAsDouble("Price");
			record->Volume = csv_record.GetFieldAsInt("Volume");
			record->TradeAmount = csv_record.GetFieldAsDouble("TradeAmount");
			record->PremiumIn = csv_record.GetFieldAsDouble("PremiumIn");
			record->PremiumOut = csv_record.GetFieldAsDouble("PremiumOut");
			Strcpy(record->TradeDate, csv_record.GetFieldAsString("TradeDate"));
			Strcpy(record->TradeTime, csv_record.GetFieldAsString("TradeTime"));
			mdb->t_Trade->Insert(record);
		}
		file.close();
	}
	void InitMdbFromCsv::LoadMdTickTable(Mdb* mdb, const char* dir)
	{
		char fullPath[260];
		sprintf(fullPath, "%s/t_MdTick.csv", dir);
		fstream file(fullPath, fstream::in);
		if (!file)
		{
			throw std::string(fullPath) + " Open Failed.";
		}

		file.getline(HeaderBuffer, sizeof(HeaderBuffer), '\n');
		CSVRecord csv_record;
		if (!csv_record.AnalysisFieldName(HeaderBuffer))
		{
			throw std::string("AnalysisFieldName t_MdTick.csv failed");
		}
		while (!file.eof())
		{
			::memset(ContentBuffer, 0, sizeof(ContentBuffer));
			file.getline(ContentBuffer, sizeof(ContentBuffer), '\n');
			if (ContentBuffer[0] == '\0')
				break;
			if (!csv_record.AnalysisFieldContent(ContentBuffer))
			{
				throw std::string("AnalysisFieldContent t_MdTick.csv failed");
			}

			auto record = new MdTick();
			Strcpy(record->TradingDay, csv_record.GetFieldAsString("TradingDay"));
			Strcpy(record->ExchangeID, csv_record.GetFieldAsString("ExchangeID"));
			Strcpy(record->InstrumentID, csv_record.GetFieldAsString("InstrumentID"));
			record->LastPrice = csv_record.GetFieldAsDouble("LastPrice");
			record->PreSettlementPrice = csv_record.GetFieldAsDouble("PreSettlementPrice");
			record->PreClosePrice = csv_record.GetFieldAsDouble("PreClosePrice");
			record->PreOpenInterest = csv_record.GetFieldAsInt("PreOpenInterest");
			record->OpenPrice = csv_record.GetFieldAsDouble("OpenPrice");
			record->HighestPrice = csv_record.GetFieldAsDouble("HighestPrice");
			record->LowestPrice = csv_record.GetFieldAsDouble("LowestPrice");
			record->Volume = csv_record.GetFieldAsInt("Volume");
			record->Turnover = csv_record.GetFieldAsDouble("Turnover");
			record->OpenInterest = csv_record.GetFieldAsInt("OpenInterest");
			record->UpperLimitPrice = csv_record.GetFieldAsDouble("UpperLimitPrice");
			record->LowerLimitPrice = csv_record.GetFieldAsDouble("LowerLimitPrice");
			Strcpy(record->UpdateTime, csv_record.GetFieldAsString("UpdateTime"));
			record->UpdateMillisec = csv_record.GetFieldAsInt("UpdateMillisec");
			record->AskPrice1 = csv_record.GetFieldAsDouble("AskPrice1");
			record->AskPrice2 = csv_record.GetFieldAsDouble("AskPrice2");
			record->AskPrice3 = csv_record.GetFieldAsDouble("AskPrice3");
			record->AskPrice4 = csv_record.GetFieldAsDouble("AskPrice4");
			record->AskPrice5 = csv_record.GetFieldAsDouble("AskPrice5");
			record->AskVolume1 = csv_record.GetFieldAsInt("AskVolume1");
			record->AskVolume2 = csv_record.GetFieldAsInt("AskVolume2");
			record->AskVolume3 = csv_record.GetFieldAsInt("AskVolume3");
			record->AskVolume4 = csv_record.GetFieldAsInt("AskVolume4");
			record->AskVolume5 = csv_record.GetFieldAsInt("AskVolume5");
			record->BidPrice1 = csv_record.GetFieldAsDouble("BidPrice1");
			record->BidPrice2 = csv_record.GetFieldAsDouble("BidPrice2");
			record->BidPrice3 = csv_record.GetFieldAsDouble("BidPrice3");
			record->BidPrice4 = csv_record.GetFieldAsDouble("BidPrice4");
			record->BidPrice5 = csv_record.GetFieldAsDouble("BidPrice5");
			record->BidVolume1 = csv_record.GetFieldAsInt("BidVolume1");
			record->BidVolume2 = csv_record.GetFieldAsInt("BidVolume2");
			record->BidVolume3 = csv_record.GetFieldAsInt("BidVolume3");
			record->BidVolume4 = csv_record.GetFieldAsInt("BidVolume4");
			record->BidVolume5 = csv_record.GetFieldAsInt("BidVolume5");
			record->AveragePrice = csv_record.GetFieldAsDouble("AveragePrice");
			mdb->t_MdTick->Insert(record);
		}
		file.close();
	}
}
