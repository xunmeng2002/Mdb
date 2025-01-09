#!coding:utf-8

import xml.etree.cElementTree as ET
import sys
import re
import os
import shutil


def NeedPump(model, tpl, dest):
    if not os.path.exists(dest):
        return True
    destMtime = os.stat(dest).st_mtime
    if os.stat(tpl).st_mtime > destMtime:
        return True
    for item in model.split(" "):
        if os.stat(item).st_mtime > destMtime:
            return True
    return False

def Search(path, destFileName, excludes, destPaths):
    for fileName in os.listdir(path):
        if fileName in excludes:
            continue
        fullFileName = os.path.join(path, fileName)
        if os.path.isfile(fullFileName) and fileName == destFileName:
            destPaths.append(fullFileName)
        if os.path.isdir(fullFileName):
            Search(fullFileName, destFileName, excludes, destPaths)

def HandlePumpFile(pumpFile):
    root = ET.parse(pumpFile).getroot()
    for node in root:
        model = node.get("model")
        tpl = node.get("tpl")
        dest = node.get("dest")
        split = node.get("split")
        #print(node.attrib)
        if split == "true":
            type = node.get("type")
            targetsFile = node.get("targets")
            targetsRoot = ET.parse(targetsFile).getroot()
            for targetNode in targetsRoot:
                targetName = targetNode.get("name")
                split_dest = dest + targetName + "." + type
                DoPump(split_dest, tpl, targetName, model)
        else:
            DoPump(dest, tpl, "target", model)

def DoPump(dest, tpl, target, model):
    if NeedPump(model, tpl, dest):
        print("pump.py %s %s %s %s" % (dest, tpl, target, model))
        if os.system("python pump.py %s %s %s %s" % (dest, tpl, target, model)) != 0:
            exit()

if __name__ == "__main__":
    excludes = ['.sv', '.vs', 'build', 'out', "Branches"]
    includes = []
    pumpfiles = []
    Search(".", "pumplist.xml", excludes, pumpfiles)
    for include in includes:
        Search(include, "pumplist.xml", excludes, pumpfiles)
    for pumpfile in pumpfiles:
        HandlePumpFile(pumpfile)
