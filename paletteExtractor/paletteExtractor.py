#!/usr/bin/python

from os import listdir
from os.path import isfile, join
from sys import argv
import PIL
from PIL import Image

def Main(i):
    inpath = f"aokopals/{i:02}"

    red = PIL.Image.open("red.png") # Official paletted version
    blueTrue = PIL.Image.open(inpath + ".png") # Bootleg rgb to palettize
    redTrue = red.convert("RGBA")
    blue = blueTrue.convert("P")

    # addressable by index
    redpal = red.palette.palette
    bluepal = blue.palette.palette
    red.palette.save("red.txt")

    redPix = redTrue.load()
    bluePix = blueTrue.load()

    # Make a color->color dict by going over every pixel
    trueColorDict = {}
    for y in range(red.height):
        for x in range(red.width):
            rp = redPix[x, y]
            bp = bluePix[x, y]
            # print(rp, bp)
            trueColorDict[rp[:3]] = bp[:3]

    print(trueColorDict)

    # copy the colors (palette definition) over
    redpalba = bytearray(red.palette.palette)
    for x in range(0, len(redpal), 3):
        redRgb = (redpal[x], redpal[x+1], redpal[x+2])
        try:
            redpalba[x+0] = trueColorDict[redRgb][0]
        except:
            pass
        try:
            redpalba[x+1] = trueColorDict[redRgb][1]
        except:
            pass
        try:
            redpalba[x+2] = trueColorDict[redRgb][2]
        except:
            pass

    red.palette.palette = bytes(redpalba)

    red.palette.save(inpath + ".txt")




for i in range(34):
    Main(i)


def Post(i):
    f = open(f"aokopals/{i:02}.txt", 'r')
    lines = f.readlines()[2:]
    f.close()
    f = open(f"aokopals/{i:02}.txt", 'w')
    f.write("[")

    for line in lines:
        line = line.strip()
        line = line.split(" ")
        r,g,b = line[1:]
        f.write(f"[{r:>3}, {g:>3}, {b:>3}],")

    f.write("],")
    f.write("\n")



for i in range(34):
    Post(i)