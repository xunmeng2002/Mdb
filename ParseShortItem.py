#!/usr/bin/env python
# -*- coding: utf-8 -*-
import xml.dom.minidom
import sys

class Item:
    def  __init__(self):
        self.Name = ""
        self.ID = 0
        self.Type = ""
        self.Desc = ""

def GetItems(itemFile, items):
    dom = xml.dom.minidom.parse(itemFile)
    root = dom.documentElement
    lastID = 0
    for itemNode in root.getElementsByTagName("item"):
        item = Item()
        item.Name = itemNode.getAttribute("name")
        itemID = itemNode.getAttribute("id")
        item.Type = itemNode.getAttribute("type")
        item.Desc = itemNode.getAttribute("desc")
        if itemID:
            item.ID = int(itemID, 16)
            lastID = item.ID
        else:
            lastID += 1
            item.ID = lastID
        items[item.Name] = item
	
def AddItemNode(dom, parentNode, item):
    itemNode = dom.createElement('item')
    itemNode.setAttribute("name", item.Name)
    itemNode.setAttribute("id", f"0x{item.ID:04X}")
    itemNode.setAttribute("type", item.Type)
    itemNode.setAttribute("desc", item.Desc)
    parentNode.appendChild(itemNode)

def ReadXml(itemFile):
    items = {}
    GetItems(itemFile, items)
    return items
    
def WriteItemsFile(destItemFile, items):
    sortedItems = sorted(items.values(), key=lambda item : item.ID)
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
