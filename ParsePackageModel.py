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

class Package:
    def __init__(self):
        self.Name = ""
        self.ID = 0
        self.Desc = ""
        self.Fields = []

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

def GetPackages(packageFile, fields, packages, destFields):
    dom = xml.dom.minidom.parse(packageFile)
    root = dom.documentElement
    lastID = 0
    for packageNode in root.getElementsByTagName("package"):
        package = Package()
        package.Name = packageNode.getAttribute("name")
        packageID = packageNode.getAttribute("id")
        if packageID:
            package.ID = int(packageID, 16)
            lastID = package.ID
        else:
            lastID += 1
            package.ID = lastID
        package.Desc = packageNode.getAttribute("desc")
        for fieldNode in packageNode.getElementsByTagName("field"):
            fieldName = fieldNode.getAttribute("name")
            package.Fields.append(fields[fieldName])
            destFields[fieldName] = fields[fieldName]
        packages.append(package)
	
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

def AddPackageNode(dom, parentNode, package):
    packageNode = dom.createElement('package')
    packageNode.setAttribute("name", package.Name)
    packageNode.setAttribute("id", f"0x{package.ID:04X}")
    packageNode.setAttribute("desc", package.Desc)
    for field in package.Fields:
        AddFieldNode(dom, packageNode, field)
    parentNode.appendChild(packageNode)

def ReadXml(packageFile, fieldFile, itemFile):
    items = {}
    fields = {}
    packages = []
    destFields = {}
    GetItems(itemFile, items)
    GetFields(fieldFile, items, fields)
    GetPackages(packageFile, fields, packages, destFields)
    return packages, destFields

def WritePackagesFile(destPackageFile, packages):
    impl = xml.dom.minidom.getDOMImplementation()
    dom = impl.createDocument(None, 'packages', None)
    root = dom.documentElement
    for package in packages:
        AddPackageNode(dom, root, package)
    f = open(destPackageFile, 'w', encoding="UTF-8")
    dom.writexml(f, indent="", addindent='\t', newl='\n', encoding="UTF-8")
    f.close()
    
def WriteFullApiPackagesFile(fullApiPackageFile, packages):
    impl = xml.dom.minidom.getDOMImplementation()
    dom = impl.createDocument(None, 'apipackages', None)
    root = dom.documentElement
    for package in packages:
        packageNode = dom.createElement('package')
        packageNode.setAttribute("name", package.Name)
        root.appendChild(packageNode)
    f = open(fullApiPackageFile, 'w', encoding="UTF-8")
    dom.writexml(f, indent="", addindent='\t', newl='\n', encoding="UTF-8")
    f.close()
    
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
    if len(sys.argv) < 5:
        print("Usage: ParseModel.py destPackage.xml fullApiPackage.xml destField.xml srcPackage.xml srcField.xml srcItem.xml")
        exit(-1) 
    destPackageFile = sys.argv[1]
    fullApiPackageFile = sys.argv[2]
    destFieldFile = sys.argv[3]
    srcPackageFile = sys.argv[4]
    srcFieldFile = sys.argv[5]
    srcItemFile = sys.argv[6]

    packages, fields = ReadXml(srcPackageFile, srcFieldFile, srcItemFile)
    WritePackagesFile(destPackageFile, packages)
    WriteFullApiPackagesFile(fullApiPackageFile, packages)
    WriteFieldsFile(destFieldFile, fields)
