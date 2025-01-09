#!encoding:utf-8


import xml.etree.cElementTree as ET
import sys
import re
import os
import shutil


def NeedCopy(src, dest):
    if not os.path.exists(dest):
        return True
    destMtime = os.stat(dest).st_mtime
    if os.stat(src).st_mtime > destMtime:
        return True
    return False

def DoCopy(copylist):
    if not os.path.exists(copylist):
        return
    root = ET.parse(copylist).getroot()
    for node in root:
        name = node.get("name")
        dest = node.get("dest")
        src = node.get("src")
        if NeedCopy(src + name, dest + name):
            print("shutil.copy %s %s" % (src + name, dest))
            shutil.copy(src + name, dest)

def Search(path, destFileName):
    destPaths = []
    for fileName in os.listdir(path):
        fullFileName = os.path.join(path, fileName)
        if os.path.isfile(fullFileName) and fileName == destFileName:
            destPaths.append(fullFileName)
    return destPaths

if __name__ == "__main__":
    copyfiles = Search(".", "headerlist.xml")
    for copyfile in copyfiles:
        DoCopy(copyfile)
