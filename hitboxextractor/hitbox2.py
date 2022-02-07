#!/usr/bin/python3

# Find all layer 1 rectangles
# Fill
# find bounding box
# use as rectangle

from sys import argv as args
from PIL import Image

directions = [
    [1, 0],  # Right
    [0, 1],  # Down
    [-1, 0], # Left
    [0, -1]  # Up
]

explored = set()
# Return extents (x1, y1, x2, y2) of bounding box
def FloodFill(x, y, col):
    
    if (x,y) in explored:
        return None

    minX = 9999
    maxX = 0
    minY = 9999
    maxY = 0

    frontier = set()
    frontier.add((x,y))

    while len(frontier) > 0:

        current = frontier.pop()
        explored.add(current)

        minX = minX if minX < current[0] else current[0]
        minY = minY if minY < current[1] else current[1]
        maxX = maxX if maxX > current[0] else current[0]
        maxY = maxY if maxY > current[1] else current[1]


        for d in directions:
            n = current[0] + d[0], current[1] + d[1]
            ncol = pix[n[1]*w + n[0]]
            if (ncol[:3] == col[:3]) and not (n in explored):
                frontier.add(n)


    if maxX - minX == 0 or maxY - minY == 0:
        return None
    return minX, minY, maxX, maxY


f:Image = Image.open(args[1])

pix = f.getdata()

w = f.width
h = f.height

hitboxes = []
hurtboxes = []
shoveboxes = []

RED   = [(167,143, 94), (160, 129, 68), (221, 68, 68)]
GREEN = [(167,143, 94), (66, 224, 68), (116, 213, 119), (160, 129, 68)]
WHITE = [(167,143, 94), (116, 213, 119), (194, 197, 197)]
COLORS = RED + GREEN + WHITE


def Main():
    for y in range(h):
        for x in range(w):
            col = pix[y*w + x]
            if col[3] == 0 or col[3] == 255:
                continue
        
            bound = FloodFill(x, y, col)
            if not bound:
                continue
            
            if col[:3] not in COLORS:
                print("Unknown color:", col[:3])
                return

            if col[:3] in RED:
                hitboxes.append(bound)
            if col[:3] in GREEN:
                hurtboxes.append(bound)
            if col[:3] in WHITE:
                shoveboxes.append(bound)

Main()

origin = w/2, h
# Scale by 10/2 before export
scale = 5


print ('"hitboxes": [', end="")

for hb in hitboxes:
    x, y, x2, y2 = hb
    w = x2-x
    h = y2-y

    x, y = x - origin[0], y - origin[1]

    x *= scale
    y *= scale
    w *= scale
    h *= scale

    print(f"[{x}, {y}, {w}, {h}],", end="")
print("],")

print ('"hurtboxes": [', end="")
for hb in hurtboxes:
    x, y, x2, y2 = hb
    w = x2-x
    h = y2-y

    x, y = x - origin[0], y - origin[1]

    x *= scale
    y *= scale
    w *= scale
    h *= scale
    print(f"[{x}, {y}, {w}, {h}],", end="")
print("],")

x1, y1, x2, y2 = 9999, 9999, 0, 0

for sb in shoveboxes:
    x1 = x1 if x1 < sb[0] else sb[0]
    y1 = y1 if y1 < sb[1] else sb[1]
    x2 = x2 if x2 > sb[2] else sb[2]
    y2 = y2 if y2 > sb[3] else sb[3]

print ('"shovebox": ', end="")
w = x2-x1
h = y2-y1

x1, y1 = x1 - origin[0], y1 - origin[1]

x1 *= scale
y1 *= scale
w *= scale
h *= scale
print(f"[{x1}, {y1}, {w}, {h}]")