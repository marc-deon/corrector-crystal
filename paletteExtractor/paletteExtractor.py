#!/usr/bin/python

########################################################
# This program takes any number of directories as input.
# For each directory, it will stack all png images in the directory on top of one another.
# (NOTE: each image is intended to be the same size; if they are not, then this will likely crash.)
# From this stack, it will find the bounding box of non-transparent pixels.
# The images are then cropped to that bounding box, concatanated horizontally,
# and the result is saved with the width of the box in the filename.
#
# Written 2020-7-20 by someone who doesn't believe in copyright.

from os import listdir
from os.path import isfile, join
from sys import argv
import PIL
from PIL import Image

def Main():

    red = PIL.Image.open("red.png")
    blueTrue = PIL.Image.open("blue.png")
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

    red.palette.save("redalt.txt")




Main()

