#encoding:utf-8

import codecs
from doctest import REPORTING_FLAGS
import os
import re
import select
import sys
import xml.etree.ElementTree as ET

class Type:
    def __init__(self, name, type, sqlType, desc) -> None:
        self.Name = name
        self.Type = type
        self.SqlType = sqlType
        self.Desc = desc
    def __str__(self) -> str:
        return "Name:%s, Type:%s, SqlType:%s Desc:%s" % (self.Name, self.Type, self.SqlType, self.Desc)
    def __eq__(self, name) -> bool:
        return self.Name == name

class Field:
    def __init__(self, name, type, desc) -> None:
        self.Name = name
        self.Type = type
        self.SqlType = ""
        self.Desc = desc
    def __str__(self) -> str:
        return "Name:%s, Type:%s, SqlType:%s, Desc:%s" %(self.Name, self.Type, self.SqlType, self.Desc)
    def __eq__(self, field) -> bool:
        return self.Name == field.Name

class Table:
    def __init__(self, name, tid, desc) -> None:
        self.Name = name
        self.Tid = tid
        self.Desc = desc
        self.Fields = []
        self.PrimaryKeys = []
    def __str__(self) -> str:
        return "Name:%s, Tid:%s, Desc:%s, FieldCount:%d, PrimaryKeyCount:%d" %(self.Name, self.Tid, self.Desc, len(self.Fields), len(self.PrimaryKeys))
    def __eq__(self, table) -> bool:
        return self.Name == table.Name

def GetTypes(typeFile):
    types = []
    tree = ET.parse(typeFile)
    root = tree.getroot()
    for typesNode in root:
        typesSqlType = typesNode.get("sqltype")
        for typeNode in typesNode:
            type = Type(typeNode.get("name"), typeNode.tag, typeNode.get("sqltype"), typeNode.get("desc"))
            if typeNode.get("sqltype") is None:
                type.SqlType = typesSqlType
            types.append(type)
    return types

def GetTables(tableFiles, types):
    tables = []
    tree = ET.parse(tableFiles)
    root = tree.getroot()
    for tableNode in root:
        table = Table(tableNode.get("name"), tableNode.get("tid"), tableNode.get("desc"))
        for fieldNode in tableNode.find("fields"):
            field = Field(fieldNode.get("name"), fieldNode.get("type"), fieldNode.get("desc"))
            index = types.index(field.Type)
            field.SqlType = types[index].SqlType
            table.Fields.append(field)
        for primarykeyNode in tableNode.find("primarykey"):
            table.PrimaryKeys.append(primarykeyNode.get("name"))
        tables.append(table)
    return tables

def ComparePrimaryKeys(lowTable, highTable):
    sql = ""
    changed = False
    for name in lowTable.PrimaryKeys:
        if name not in highTable.PrimaryKeys:
            changed = True
    for name in highTable.PrimaryKeys:
        if name not in lowTable.PrimaryKeys:
            changed = True
    if changed:
        sql += "Alter Table `t_%s` Drop Primary Key;\n" % highTable.Name
        sql += "Alter Table `t_%s` Add Primary Key(\n" % highTable.Name
        for name in highTable.PrimaryKeys:
            sql += "`%s`, " % name
        sql = sql[:-2]
        sql += ");\n"
    return sql

def CompareFields(lowTable, highTable):
    sql = ""
    for field in lowTable.Fields:
        if field not in highTable.Fields:
            sql += "Alter Table `t_%s` Drop Column `%s`;\n" % (highTable.Name, field.Name)
        else:
            highIndex = highTable.Fields.index(field)
            highField = highTable.Fields[highIndex]
            if field.SqlType != highField.SqlType:
                sql += "Alter Table `t_%s` Modify Column `%s` %s;\n" % (highTable.Name, highField.Name, highField.SqlType)
    for field in highTable.Fields:
        if field not in lowTable.Fields:
            sql += "Alter Table `t_%s` Add Column `%s` %s Comment '%s' " % (highTable.Name, field.Name, field.SqlType, field.Desc)
            index = highTable.Fields.index(field)
            if index == 0:
                sql += "First;\n"
            else:
                sql += "After %s;\n" % highTable.Fields[index - 1].Name
    return sql

def GetPreHighIndex(lowTable, highTable, highIndex):
    preHighIndex = highIndex - 1
    while preHighIndex >= 0:
        preHighField = highTable.Fields[preHighIndex]
        if preHighField not in lowTable.Fields:
            preHighIndex -= 1
        else:
            break
    return preHighIndex
    
def CompareFieldOrder(lowTable, highTable):
    sql = ""
    for lowField in lowTable.Fields:
        if lowField in highTable.Fields:
            lowIndex = lowTable.Fields.index(lowField)
            highIndex = highTable.Fields.index(lowField)
            highField = highTable.Fields[highIndex]
            if highIndex == 0 and lowIndex == 0:
                pass
            elif highIndex == 0 and lowIndex != 0:
                sql += "Alter Table `t_%s` Modify Column `%s` %s First;\n" % (highTable.Name, highField.Name, highField.SqlType)
            elif highIndex != 0 and lowIndex == 0:
                preHighIndex = GetPreHighIndex(lowTable, highTable, highIndex)
                if preHighIndex < 0:
                    sql += "Alter Table `t_%s` Modify Column `%s` %s First;\n" % (highTable.Name, highField.Name, highField.SqlType)
                else:
                    preHighField = highTable.Fields[preHighIndex]
                    sql += "Alter Table `t_%s` Modify Column `%s` %s After `%s`;\n" % (highTable.Name, highField.Name, highField.SqlType, preHighField.Name)
            else:
                preLowField = lowTable.Fields[lowIndex - 1]
                preHighField = highTable.Fields[highIndex - 1]
                if preLowField.Name != preHighField.Name:
                    preHighIndex = GetPreHighIndex(lowTable, highTable, highIndex)
                    if preHighIndex < 0:
                        sql += "Alter Table `t_%s` Modify Column `%s` %s First;\n" % (highTable.Name, highField.Name, highField.SqlType)
                    else:
                        preHighField = highTable.Fields[preHighIndex]
                        sql += "Alter Table `t_%s` Modify Column `%s` %s After `%s`;\n" % (highTable.Name, highField.Name, highField.SqlType, preHighField.Name)
    return sql

def CreateTable(table):
    sql = "\nCreate Table If Not Exists `t_%s`(\n" % table.Name
    for field in table.Fields:
        sql += "`%s` %s Comment '%s', \n" % (field.Name, field.SqlType, field.Desc)
    sql += "Primary Key("
    for name in table.PrimaryKeys:
        if table.PrimaryKeys.index(name) > 0:
            sql += ", "
        sql += name
    sql += ")\n"
    sql += ") ENGINE=MyISAM DEFAULT COLLATE='utf8mb4_bin' COMMENT='%s';\n\n" % table.Desc
    return sql

def CompareTables(lowTables, highTables):
    sql = ""
    for lowTable in lowTables:
        if lowTable not in highTables:
            sql += "Drop Table `t_%s`;\n" % lowTable.Name
        else:
            highIndex = highTables.index(lowTable)
            highTable = highTables[highIndex]
            sql += CompareFieldOrder(lowTable, highTable)
            sql += CompareFields(lowTable, highTable)
            sql += ComparePrimaryKeys(lowTable, highTable)
    for highTable in highTables:
        if highTable not in lowTables:
            sql += CreateTable(highTable)
    return sql

def GenerateIncrementTables(lowTypes, highTypes, lowVersionDir, highVersionDir, dbName):
    if dbName == "Init":
        lowModelFileName = "%s/Model/Tables/Sync.xml" % (lowVersionDir)
        highModelFileName = "%s/Model/Tables/Sync.xml" % (highVersionDir)
    else:
        lowModelFileName = "%s/Model/Tables/%s.xml" % (lowVersionDir, dbName)
        highModelFileName = "%s/Model/Tables/%s.xml" % (highVersionDir, dbName)
    lowTables = GetTables(lowModelFileName, lowTypes)
    highTables = GetTables(highModelFileName, highTypes)
    incFileName = "%s/Sql/Increment/%sInc.sql" % (highVersionDir, dbName)
    incFile = open(incFileName, mode="w", encoding="utf8")
    sql = CompareTables(lowTables, highTables)
    incFile.write(sql)
    incFile.close()
    GenerateIncrementFunctions(lowVersionDir, highVersionDir, dbName)

def GenerateIncrementFunctions(lowVersionDir, highVersionDir, dbName):
    incFileName = "%s/Sql/Increment/%sInc.sql" % (highVersionDir, dbName)
    lowDropFunctionFileName = "%s/Sql/Drop%sDBFunctions.sql" % (lowVersionDir, dbName)
    highCreateFunctionFileName = "%s/Sql/Create%sDBFunctions.sql" % (highVersionDir, dbName)
    highCreateTruncateTablesFunctionFileName = "%s/Sql/Create%sDBTruncateTablesFunction.sql" % (highVersionDir, dbName)
    CatFiles([incFileName, lowDropFunctionFileName, highCreateFunctionFileName, highCreateTruncateTablesFunctionFileName])

def CatFiles(files):
    destFile = open(files[0], mode="a", encoding="utf8")
    for i in range(1, len(files)):
        file = open(files[i], mode="rb")
        #检查是否有utf8-bom，如果有，则去掉
        s = file.read(3)
        isBom = s == codecs.BOM_UTF8
        file.close()
        file = open(files[i], mode="r", encoding="utf8")
        if isBom:
            file.read(1)
            destFile.write(file.read())
        else:
            destFile.write(file.read())
        file.close()
    destFile.close()

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python geninc.py [lowVersionDir] [highVersionDir]")
        exit(-1)
    lowVersionDir = sys.argv[1]
    highVersionDir = sys.argv[2]

    lowTypes = GetTypes(lowVersionDir + "/Model/Types.xml")
    highTypes = GetTypes(highVersionDir + "/Model/Types.xml")

    GenerateIncrementTables(lowTypes, highTypes, lowVersionDir, highVersionDir, "Admin")
    GenerateIncrementTables(lowTypes, highTypes, lowVersionDir, highVersionDir, "History")
    GenerateIncrementTables(lowTypes, highTypes, lowVersionDir, highVersionDir, "Init")
    GenerateIncrementTables(lowTypes, highTypes, lowVersionDir, highVersionDir, "Sync")
