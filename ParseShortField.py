#coding:UTF-8
import xml.dom.minidom
import sys

class Item:
    def  __init__(self):
        self.Name = ""
        self.ID = ""
        self.Type = ""
        self.Desc = ""

class Field:
    def __init__(self):
        self.Name = ""
        self.ID = 0
        self.Desc = ""
        self.Items = []

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

def GetFields(fieldFile, items, fields):
    dom = xml.dom.minidom.parse(fieldFile)
    root = dom.documentElement
    lastID = 0
    for fieldNode in root.getElementsByTagName("field"):
        field = Field()
        field.Name = fieldNode.getAttribute("name")
        fieldID = fieldNode.getAttribute("id")
        if fieldID:
            field.ID = int(fieldID, 16)
            lastID = field.ID
        else:
            lastID += 1
            field.ID = lastID
        field.Desc = fieldNode.getAttribute("desc")
        for itemNode in fieldNode.getElementsByTagName("item"):
            itemName = itemNode.getAttribute("name")
            field.Items.append(items[itemName])
        fields[field.Name] = field
	
def AddItemNode(dom, parentNode, item):
    itemNode = dom.createElement('item')
    itemNode.setAttribute("name", item.Name)
    itemNode.setAttribute("id", item.ID)
    itemNode.setAttribute("type", item.Type)
    itemNode.setAttribute("desc", item.Desc)
    parentNode.appendChild(itemNode)

def AddFieldNode(dom, parentNode, field):
    fieldNode = dom.createElement('field')
    fieldNode.setAttribute("name", field.Name)
    fieldNode.setAttribute("id", f"0x{field.ID:04X}")
    fieldNode.setAttribute("desc", field.Desc)
    for item in field.Items:
        AddItemNode(dom, fieldNode, item)
    parentNode.appendChild(fieldNode)

def ReadXml(fieldFile, itemFile):
    items = {}
    fields = {}
    GetItems(itemFile, items)
    GetFields(fieldFile, items, fields)
    return fields
    
def WriteFieldsFile(destFieldFile, fields):
    sortedFields = sorted(fields.values(), key=lambda field : field.ID)
    impl = xml.dom.minidom.getDOMImplementation()
    dom = impl.createDocument(None, 'fields', None)
    root = dom.documentElement
    for field in sortedFields:
        AddFieldNode(dom, root, field)
    f = open(destFieldFile, 'w', encoding="UTF-8")
    dom.writexml(f, indent="", addindent='\t', newl='\n', encoding="UTF-8")
    f.close()
	
if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: ParseModel.py destField.xml srcField.xml srcItem.xml")
        exit(-1) 
    destFieldFile = sys.argv[1]
    srcFieldFile = sys.argv[2]
    srcItemFile = sys.argv[3]

    fields = ReadXml(srcFieldFile, srcItemFile)
    WriteFieldsFile(destFieldFile, fields)
