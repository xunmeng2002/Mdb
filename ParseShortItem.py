#!/usr/bin/env python
# -*- coding: utf-8 -*-
import xml.dom.minidom
import sys

class Item:
    def  __init__(self):
        self.Name = ""
        self.Id = 0
        self.Type = ""
        self.Desc = ""

def GetItems(itemFile, items):
    dom = xml.dom.minidom.parse(itemFile)
    root = dom.documentElement
    lastId = 0
    for itemNode in root.getElementsByTagName("item"):
        item = Item()
        item.Name = itemNode.getAttribute("name")
        itemId = itemNode.getAttribute("id")
        item.Type = itemNode.getAttribute("type")
        item.Desc = itemNode.getAttribute("desc")
        if itemId:
            item.Id = int(itemId, 16)
            lastId = item.Id
        else:
            lastId += 1
            item.Id = lastId
        items[item.Name] = item
	
def AddItemNode(dom, parentNode, item):
    itemNode = dom.createElement('item')
    itemNode.setAttribute("name", item.Name)
    itemNode.setAttribute("id", f"0x{item.Id:04X}")
    itemNode.setAttribute("type", item.Type)
    itemNode.setAttribute("desc", item.Desc)
    parentNode.appendChild(itemNode)

def ReadXml(itemFile):
    items = {}
    GetItems(itemFile, items)
    return items
    
def WriteItemsFile(destItemFile, items):
    sortedItems = sorted(items.values(), key=lambda item : item.Id)
    impl = xml.dom.minidom.getDOMImplementation()
    dom = impl.createDocument(None, 'items', None)
    root = dom.documentElement
    for item in sortedItems:
        AddItemNode(dom, root, item)
    f = open(destItemFile, 'w', encoding="UTF-8")
    dom.writexml(f, indent="", addindent='\t', newl='\n', encoding="UTF-8")
    f.close()
	
if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: ParseModel.py destItems.xml srcShortItem.xml")
        exit(-1) 
    destItemFile = sys.argv[1]
    srcItemFile = sys.argv[2]

    items = ReadXml(srcItemFile)
    WriteItemsFile(destItemFile, items)
