-- 本文件由 ../Templates/Sql/Duckdb/DropTables.sql.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
DROP TABLE IF EXISTS TradingDays;
DROP TABLE IF EXISTS Exchanges;
DROP TABLE IF EXISTS Products;
DROP TABLE IF EXISTS Instruments;
DROP INDEX PrimaryAccountsOfferId;
DROP TABLE IF EXISTS PrimaryAccounts;
DROP TABLE IF EXISTS Accounts;
DROP INDEX CapitalsTradingDay;
DROP TABLE IF EXISTS Capitals;
DROP INDEX PositionsAccount;
DROP INDEX PositionsTradingDay;
DROP TABLE IF EXISTS Positions;
DROP INDEX PositionDetailsTradeMatch;
DROP INDEX PositionDetailsTradingDay;
DROP TABLE IF EXISTS PositionDetails;
DROP TABLE IF EXISTS Orders;
DROP TABLE IF EXISTS Trades;
	
