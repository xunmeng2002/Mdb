#!python2
#coding:utf-8

import xml.etree.cElementTree as ET
import sys
import re
import os


def Search(path, destFileName, excludes, destPaths):
    for fileName in os.listdir(path):
        if fileName in excludes:
            continue
        fullFileName = os.path.join(path, fileName)
        if os.path.isfile(fullFileName) and fileName == destFileName:
            destPaths.append(fullFileName)
        if os.path.isdir(fullFileName):
            Search(fullFileName, destFileName, excludes, destPaths)
			
def Clear(pumpfile):
    root = ET.parse(pumpfile).getroot()
    for node in root:
        dest = node.get("dest")
        split = node.get("split")
        if split == "true":
            type = node.get("type")
            targetsFile = node.get("targets")
            targetsRoot = ET.parse(targetsFile).getroot()
            for targetNode in targetsRoot:
                targetName = targetNode.get("name")
                split_dest = dest + targetName + "." + type
                Remove(split_dest)
        else:
            Remove(dest)

def Remove(destFile):
    if os.path.exists(destFile):
        print("delete %s" % destFile)
        os.remove(destFile)
	
if __name__ == "__main__":
    excludes = ['.sv', '.vs', 'build', 'out', "Branches"]
    includes = []
    parsefiles = []
    Search(".","parselist.xml", excludes, parsefiles)
    for include in includes:
        Search(include, "parselist.xml", excludes, parsefiles)
    for parsefile in parsefiles:
        Clear(parsefile)

    pumpfiles = []
    Search(".","pumplist.xml", excludes, pumpfiles)
    for include in includes:
        Search(include, "pumplist.xml", excludes, pumpfiles)
    for pumpfile in pumpfiles:
        Clear(pumpfile)