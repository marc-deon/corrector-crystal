#!/usr/bin/python

from os import listdir
from os.path import isfile, join
from sys import argv

def Main():
    if len(argv) < 2:
        print("Itterate non-recursively through files in given folders and print TODO and FIXMEs.\
             \n\nUsage: TodoGen.py (folderPaths)\n\te.g. TodoGen.py src include")
        return

    for path in argv[1:]:

        onlyfiles = [f for f in listdir(path) if isfile(join(path, f))]
        paths = [join(path, fullpath) for fullpath in onlyfiles]
        
        for fullpath in paths:
            hasPrint = False
            with open(fullpath, 'r') as f:
                line = f.readline()
                while line:
                    if "TODO:" in line or "FIXME:" in line:
                        if not hasPrint:
                            print(fullpath, ":")
                            hasPrint = True
                        print("\t", line.strip())
                    line = f.readline()


Main()
