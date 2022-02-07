#!/usr/bin/env python

################################################################################
# Written 2020-7-30 by someone who doesn't believe in copyright.
# Do whatever you like with it, though I would appreciate keeping a link to itch.

from os import listdir
from os.path import isfile, join
from sys import stdout
import PIL
from PIL import Image

import argparse

parser = argparse.ArgumentParser(formatter_class=argparse.RawDescriptionHelpFormatter, description="""
Simple tools for manipulating paletted images. Originally written for palette swapping placeholder fighting game sprites.

Examples can be found at llamaq.itch.io
""")

parser.add_argument("-e", "--extract-palette", help="Export palette data from an image to a file (or stdout).", 
    nargs=2, metavar=("input", "output")) # Needs input file and output text file/"stdout"
parser.add_argument("-p", "--palettize", help="Create a copy of the first image, but with palette data changed to look like the second image.",
    nargs=3, metavar=("input", "input", "output")) # Needs input file 1, input file 2, and output file
parser.add_argument("-d", "--difference", help="Show the changes that would be made by --palettize, and save them to a file (or stdout).",
    nargs=3, metavar=("input", "input", "output")) # Needs input file 1, input file 2, and output file/"stdout"
args = parser.parse_args()

# Write out the palette to a file
def ExtractPalette(inPath, outPath):
    inFile = PIL.Image.open(inPath)
    if(inFile.mode != "P"):
        print("ERROR: Image must be in palette mode")
        return None


    ba = bytearray(inFile.palette.palette)


    output = stdout if outPath == "stdout" else open(outPath, "w")
    
    for i in range(0, len(ba), 3):
        output.write(f"{int(i/3)} | ({ba[i]}, {ba[i+1]}, {ba[i+2]})\n")



# Write the palette diff out to a file
def PaletteDiff(inPath1, inPath2, outPath):
    img1 = PIL.Image.open(inPath1)

    if img1.mode != "P":
        print("ERROR: Image 1 must be in palette mode")
        return None

    img1 = img1.convert("RGBA")
    img2 = PIL.Image.open(inPath2).convert("RGBA")

    if img1.width != img2.width and img1.height != img2.height:
        print("ERROR: Images must be the same size")
        return None

    colorDict = MakeDict(img1, img2)

    output = PIL.Image.open(inPath1)
    # For now, just show the palette
    ba = bytearray(output.palette.palette)
    
    lines1 = []
    lines2 = []

    for i in range(0, len(ba), 3):
        lines1.append(f"{int(i/3)} | ({ba[i]}, {ba[i+1]}, {ba[i+2]})")
        try:
            palCol = (ba[i], ba[i+1], ba[i+2])
            palCol = colorDict[palCol]
            ba[i] = palCol[0]
            ba[i+1] = palCol[1]
            ba[i+2] = palCol[2]
        except:
            pass
        lines2.append(f"{int(i/3)} | ({ba[i]}, {ba[i+1]}, {ba[i+2]})")
    outlines = []

    for i in range(len(lines1)):
        if lines1[i] == lines2[i]:
            pass
        else:
            outlines.append(f"{lines1[i]} > {lines2[i][lines2[i].find('|')+2:]}\n")
    
    out = stdout if outPath == "stdout" else open(outPath, "w")
    out.writelines(outlines)

def MakeDict(img1, img2):
    colorDict = {}
    img1Pix = img1.load()
    img2Pix = img2.load()

    for y in range(img1.height):
        for x in range(img1.width):
            ip = img1Pix[x,y][:3]
            op = img2Pix[x,y][:3]
            colorDict[ip] = op
    return colorDict

def Palettize(inPath1, inPath2, outPath):

    ## What do we have?
    # 1. An image that has a palette, img1
    # 2. A second image that does not have a palette, img2

    ## What do we want?
    # A paletted image that looks like img2, but with the same image data as img1

    ## How do we get there?
    # 1. Open img1 and img2 for reading, both in rgba mode.
    # 2. Create a (rgb)->(rgb) dictionary, and fill it by looping over x,y, using img1 as key and img2 as value.
    # 3. Make a copy of img1 in palette mode, call it output.
    # 4. Loop over output's palette data: if color is in dictionary's keys, replace it with the corrosponding value.
    # 5. Save output to a file.


    img1 = PIL.Image.open(inPath1)

    if img1.mode != "P":
        print("ERROR: Image 1 must be in palette mode")
        return None

    img1 = img1.convert("RGBA")
    img2 = PIL.Image.open(inPath2).convert("RGBA")

    if img1.width != img2.width and img1.height != img2.height:
        print("ERROR: Images must be the same size")
        return None

    colorDict = MakeDict(img1, img2)

    output = PIL.Image.open(inPath1)
    # For now, just show the palette
    ba = bytearray(output.palette.palette)
    
    # Replace palette info
    for i in range(0, len(ba), 3):
        try:
            palCol = (ba[i], ba[i+1], ba[i+2])
            palCol = colorDict[palCol]
            ba[i] = palCol[0]
            ba[i+1] = palCol[1]
            ba[i+2] = palCol[2]
        except:
            # The palette data may have unused colors. Skip these.
            pass
    output.putpalette(ba)
    
    output.save(outPath)

def Main():
    
    if args.difference:
        PaletteDiff(args.difference[0], args.difference[1], args.difference[2])
    elif args.extract_palette:
        ExtractPalette(args.extract_palette[0], args.extract_palette[1])
    elif args.palettize:
        Palettize(args.palettize[0], args.palettize[1], args.palettize[2])
    else:
        parser.print_help()

if __name__ == "__main__":
    Main()

