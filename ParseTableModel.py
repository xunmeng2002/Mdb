#coding:UTF-8
import xml.dom.minidom
import sys

class Item:
    def  __init__(self):
        self.Name = ""
        self.ID = ""
        self.Type = ""
        self.Desc = ""

class Table:
    def __init__(self):
        self.Name = ""
        self.ID = 0
        self.Desc = ""
        self.Batch = ""
        self.Session = ""
        self.Items = []
        self.PrimaryKey = []
        self.Uniquekeys = {}
        self.Indexes = {}

def GetItems(itemFile, items):
    dom = xml.dom.minidom.parse(itemFile)
    root = dom.documentElement
    for itemNode in root.getElementsByTagName("item"):
        item = Item()
        item.Name = itemNode.getAttribute("name")
        item.ID = itemNode.getAttribute("id")
        item.Type = itemNode.getAttribute("type")
        item.Desc = itemNode.getAttribute("desc")
        items[item.Name] = item

def GetTables(tableFile, items, tables):
    dom = xml.dom.minidom.parse(tableFile)
    root = dom.documentElement
    lastID = 0
    for tableNode in root.getElementsByTagName("table"):
        table = Table()
        table.Name = tableNode.getAttribute("name")
        tableID = tableNode.getAttribute("id")
        if tableID:
            table.ID = int(tableID, 16)
            lastID = table.ID
        else:
            lastID += 1
            table.ID = lastID
        table.Desc = tableNode.getAttribute("desc")
        table.Batch = tableNode.getAttribute("batch")
        table.Session = tableNode.getAttribute("session")
        for itemsNode in tableNode.getElementsByTagName("items"):
            for itemNode in itemsNode.getElementsByTagName("item"):
                itemName = itemNode.getAttribute("name")
                table.Items.append(items[itemName])
        for primarykeyNode in tableNode.getElementsByTagName("primarykey"):
            for itemNode in primarykeyNode.getElementsByTagName("item"):
                itemName = itemNode.getAttribute("name")
                table.PrimaryKey.append(items[itemName])
        for uniquekeyesNode in tableNode.getElementsByTagName("uniquekeys"):
            for uniquekeyNode in uniquekeyesNode.getElementsByTagName("uniquekey"):
                uniquekeyName = uniquekeyNode.getAttribute("name")
                table.Uniquekeys[uniquekeyName] = []
                for itemNode in uniquekeyNode.getElementsByTagName("item"):
                    itemName = itemNode.getAttribute("name")
                    table.Uniquekeys[uniquekeyName].append(items[itemName])
        for indexesNode in tableNode.getElementsByTagName("indexes"):
            for indexNode in indexesNode.getElementsByTagName("index"):
                indexName = indexNode.getAttribute("name")
                table.Indexes[indexName] = []
                for itemNode in indexNode.getElementsByTagName("item"):
                    itemName = itemNode.getAttribute("name")
                    table.Indexes[indexName].append(items[itemName])
        tables.append(table)
	
def AddItemNode(dom, parentNode, item):
    itemNode = dom.createElement('item')
    itemNode.setAttribute("name", item.Name)
    itemNode.setAttribute("id", item.ID)
    itemNode.setAttribute("type", item.Type)
    itemNode.setAttribute("desc", item.Desc)
    parentNode.appendChild(itemNode)

def AddTableNode(dom, parentNode, table):
    tableNode = dom.createElement('table')
    tableNode.setAttribute("name", table.Name)
    tableNode.setAttribute("id", f"0x{table.ID:04X}")
    tableNode.setAttribute("desc", table.Desc)
    tableNode.setAttribute("batch", table.Batch)
    tableNode.setAttribute("session", table.Session)
    itemsNode = dom.createElement('items')
    for item in table.Items:
        AddItemNode(dom, itemsNode, item)
    tableNode.appendChild(itemsNode)
    
    primarykeyNode = dom.createElement('primarykey')
    for item in table.PrimaryKey:
        AddItemNode(dom, primarykeyNode, item)
    tableNode.appendChild(primarykeyNode)
    
    uniquekeysNode = dom.createElement('uniquekeys')
    for uniquekeyName in table.Uniquekeys.keys():
        uniquekeyNode = dom.createElement('uniquekey')
        uniquekeyNode.setAttribute("name", uniquekeyName)
        for item in table.Uniquekeys[uniquekeyName]:
            AddItemNode(dom, uniquekeyNode, item)
        uniquekeysNode.appendChild(uniquekeyNode)
    tableNode.appendChild(uniquekeysNode)
    
    indexesNode = dom.createElement('indexes')
    for indexName in table.Indexes.keys():
        indexNode = dom.createElement('index')
        indexNode.setAttribute("name", indexName)
        for item in table.Indexes[indexName]:
            AddItemNode(dom, indexNode, item)
        indexesNode.appendChild(indexNode)
    tableNode.appendChild(indexesNode)
    
    parentNode.appendChild(tableNode)

def ReadXml(tableFile, itemFile):
    items = {}
    tables = []
    destFields = {}
    GetItems(itemFile, items)
    GetTables(tableFile, items, tables)
    return tables

def WriteTablesFile(destTableFile, tables):
    impl = xml.dom.minidom.getDOMImplementation()
    dom = impl.createDocument(None, 'tables', None)
    root = dom.documentElement
    for table in tables:
        AddTableNode(dom, root, table)
    f = open(destTableFile, 'w', encoding="UTF-8")
    dom.writexml(f, indent="", addindent='\t', newl='\n', encoding="UTF-8")
    f.close()

def WriteFullDBTablesFile(fullDBTablesFile, tables):
    impl = xml.dom.minidom.getDOMImplementation()
    dom = impl.createDocument(None, 'dbtables', None)
    root = dom.documentElement
    for table in tables:
        tableNode = dom.createElement('table')
        tableNode.setAttribute("name", table.Name)
        root.appendChild(tableNode)
    f = open(fullDBTablesFile, 'w', encoding="UTF-8")
    dom.writexml(f, indent="", addindent='\t', newl='\n', encoding="UTF-8")
    f.close()

if __name__ == "__main__":
    if len(sys.argv) < 4:
        print("Usage: ParseTableModel.py destTable.xml fullDBTable.xml shortTable.xml items.xml")
        exit(-1) 
    destTableFile = sys.argv[1]
    fullDBTableFile = sys.argv[2]
    shortTableFile = sys.argv[3]
    itemFile = sys.argv[4]

    tables = ReadXml(shortTableFile, itemFile)
    WriteTablesFile(destTableFile, tables)
    WriteFullDBTablesFile(fullDBTableFile, tables)
