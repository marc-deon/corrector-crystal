#!/usr/bin/env python

################################################################################
# Written 2020-7-29 by someone who doesn't believe in copyright.
# Do whatever you like with it, though I would appreciate keeping a link to itch.
# Last updated 2020-8-4

# Version 2.1 todo
# - [x] Export as gif or apng

# Version 2.2 todo
# - [ ] Self contained executable
# - [ ] option for how to deal with different base sizes
# - [ ] grid layout

from os import listdir
from os.path import isfile, join
import PIL
from PIL import Image


import argparse
parser = argparse.ArgumentParser(formatter_class=argparse.RawDescriptionHelpFormatter, description="""
This program is used to automatically generate uniformly sized spritesheets from individual images. The output filename is formatted as:
[directory_name frame_width:frame_height (frame_count).png]

At the moment, it may not work properly if images of difference sizes.

Please make sure that the base images are named/numbered alphanumerically, otherwise the resulting spritesheet will be out of order.

Examples can be found at llamaq.itch.io
""")

parser.add_argument("--input-type", help="Input filetype, e.x. \".jpg\", \".png\". Defaults to all files in directory.", 
    default="")
parser.add_argument("--output-type", help="Output filetype, e.x. \".gif\", \".apng\", \".png\". Defaults to \".png\".", 
    default=".png")
parser.add_argument("--crop", help="Crop the input images to a shared bounding box before concatinating. Use multiple times to crop from more than one direction.",
    choices=('left', 'right', 'top', 'bottom'), action='append', default=[])
parser.add_argument("--color" , help="Background color to use when cropping, e.x. \"255,0,255\" for magenta. 't' can be used for transparency (default).",
    default='t')
parser.add_argument("--cwd" , help="Create output file in current working directory.",
    action='store_true')
parser.add_argument("--path", help="Create output file in given directory.",
    action='store_true')
parser.add_argument("--layout", help="Create sprite sheet horizontally or vertically.",
    choices=('h', 'v'), default='h')
parser.add_argument("paths", help="Input directories.", type=str, nargs="+")
args = parser.parse_args()

print(args)
if args.color != "t":
    split = args.color.split(',')
    args.color = (int(split[0]), int(split[1]), int(split[2]))
    print("Color converted to ", args.color)


def Animated(cropRect, size, paths):
    frames = []
    for fullpath in paths:
        frames.append(PIL.Image.open(fullpath).crop(cropRect))
    
    if len(frames) == 1:
        print(f"Only 1 frame in {fullpath}? Saving anyway...")
    
    elif len(frames) == 0:
        print(f"No frames in {fullpath}? Aborting save...")
        return
    
    f = "PNG" if args.output_type.lower() == ".apng" else "GIF"
    # APNG doesn't like a disposal of 2
    d = 0 if args.output_type.lower() == ".apng" else 2

    if args.cwd:
        filename = f"{paths[0].split('/')[::-1][1]} {size[0]}:{size[1]} ({len(paths)}){args.output_type}"
        print(filename)
        frames[0].save(filename, format=f, append_images=frames[1:], save_all=True, duration=2/60, loop=0, disposal=d)

    # Save in image source directory
    if args.path:
        filename = f"{path}/{paths[0].split('/')[::-1][1]} {size[0]}:{size[1]} ({len(paths)}){args.output_type}"
        print(filename)
        frames[0].save(filename, format=f, append_images=frames[1:], save_all=True, duration=2/60, loop=0, disposal=d)


    pass

def Sheet(cropRect, size, paths):
    # Spritesheet image
    sheet = None
    i = 0
    for fullpath in paths:
        # Initialize the spritesheet to be the goal size
        if sheet == None:
            sheet = PIL.Image.open(fullpath)
            if args.layout == 'h':
                sheet = sheet.resize((size[0] * len(paths), size[1]))
            elif args.layout == 'v':
                sheet = sheet.resize((size[0], size[1] * len(paths)))

        img = PIL.Image.open(fullpath).crop(cropRect)
        box = (0, 0)
        if args.layout == 'h':
            box = (i*size[0], 0)
        elif args.layout == 'v':
            box = (0, i*size[1])
        sheet.paste(img, box)
        i+=1
        
    # Save in current working directory
    if args.cwd:
        filename = f"{paths[0].split('/')[::-1][1]} {size[0]}:{size[1]} ({len(paths)}){args.output_type}"
        print(filename)
        sheet.save(filename)

    # Save in image source directory
    if args.path:
        filename = f"{path}/{paths[0].split('/')[::-1][1]} {size[0]}:{size[1]} ({len(paths)}){args.output_type}"
        print(filename)
        sheet.save(filename)
    pass

def Main():

    for path in args.paths:
        print (f"concatinating files in [{path}]...")
        onlyfiles = [f for f in listdir(path) if f.lower().endswith(args.input_type)]
        paths = [join(path, fullpath) for fullpath in onlyfiles]
        paths.sort()

        merged = None
        for fullpath in paths:
            img = PIL.Image.open(fullpath).convert("RGBA")

            # If we're using an RGB color as the background color, do some numpy magic to replace that with real transparency.
            if args.color != 't':
                import numpy as np
                arr = np.array(img)
                r, g, b = arr[:,:,0], arr[:,:,1], arr[:,:,2]
                mask = (r == args.color[0]) & (g == args.color[1]) & (b == args.color[2])
                arr[:,:,:4][mask] = [0,0,0,0]
                img = Image.fromarray(arr)

            if merged == None:
                merged = img
                continue

            merged.alpha_composite(img.convert("RGBA"))


        # Left values will be modified by if-for-for loop
        minX = merged.width-1  if 'left' in args.crop   else 0
        minY = merged.height-1 if 'top' in args.crop    else 0
        maxX = 0               if 'right' in args.crop  else merged.width-1
        maxY = 0               if 'bottom' in args.crop else merged.height-1

        #Get the proper min and max of x and y given our crop settings
        if args.crop != []:
            pix = merged.load()
            for y in range(merged.height):
                for x in range(merged.width):
                    p = pix[x, y]
                    # Not transparent
                    if p[3] != 0:
                        if 'left' in args.crop:
                            minX = x if x < minX else minX
                        if 'top' in args.crop:
                            minY = y if y < minY else minY
                        if 'right' in args.crop:
                            maxX = x if x > maxX else maxX
                        if 'bottom' in args.crop:
                            maxY = y if y > maxY else maxY



        # Static rectangle to take out of original images
        cropRect = [minX, minY, maxX, maxY]

        # Size of target image segment
        size = [maxX-minX, maxY-minY]
        
        if(args.output_type.lower() == ".gif" or args.output_type.lower() == ".apng"):
            Animated(cropRect, size, paths)

        else:
            Sheet(cropRect, size, paths)

 

if __name__ == "__main__":
    Main()

