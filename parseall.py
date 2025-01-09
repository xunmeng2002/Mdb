import xml.etree.cElementTree as ET
import sys
import re
import os

def NeedPump(script, dests, sources):
    for dest in dests.split(" "):
        if not os.path.exists(dest):
            return True
        destMtime = os.stat(dest).st_mtime
        if os.stat(script).st_mtime > destMtime:
            return True
        for source in sources.split(" "):
            if os.stat(source).st_mtime > destMtime:
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

def DoParse(fileName):
    root = ET.parse(fileName).getroot()
    for node in root:
        script = node.get("script")
        dests = node.get("dest")
        sources = node.get("sources")
        if NeedPump(script, dests, sources):
            print("%s %s %s" % (script, dests, sources))
            if os.system("python %s %s %s" % (script, dests, sources)) != 0:
                exit()

if __name__ == "__main__":
    excludes = ['.sv', '.vs', 'build', 'out', "Branches"]
    includes = []
    parsefiles = []
    Search(".", "parselist.xml", excludes, parsefiles)
    for include in includes:
        Search(include, "parselist.xml", excludes, parsefiles)
    for parsefile in parsefiles:
        DoParse(parsefile)
