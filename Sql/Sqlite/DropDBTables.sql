-- 本文件由 ../Templates/Sql/Sqlite/DropTables.sql.tpl 生成；请勿手改，改动请改模板后重跑 pumpall.py
DROP TABLE IF EXISTS TradingDay;
DROP TABLE IF EXISTS Exchange;
DROP TABLE IF EXISTS Product;
DROP TABLE IF EXISTS Instrument;
DROP INDEX PrimaryAccountOfferId;
DROP TABLE IF EXISTS PrimaryAccount;
DROP TABLE IF EXISTS Account;
DROP INDEX CapitalTradingDay;
DROP TABLE IF EXISTS Capital;
DROP INDEX PositionAccount;
DROP INDEX PositionTradingDay;
DROP TABLE IF EXISTS Position;
DROP INDEX PositionDetailTradeMatch;
DROP INDEX PositionDetailTradingDay;
DROP TABLE IF EXISTS PositionDetail;
DROP TABLE IF EXISTS Order;
DROP TABLE IF EXISTS Trade;
