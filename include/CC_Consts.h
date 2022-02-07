#ifndef CC_CONSTS_H
#define CC_CONSTS_H

#define VIRT_SCREEN_SIZE_X 640
#define VIRT_SCREEN_SIZE_Y 480

// For ever visible pixel, there's 10 subunits
#define UNIT_TO_PIX (1.0f/10.0f)
#define PIX_TO_UNIT 10

// Stage is 2.5 screens wide
#define STAGE_W 16000
// Stage is a little over 2 screens tall
#define STAGE_H 10000
#define FLOOR_OFFSET (STAGE_H - 400)

#define LEFT_WALL 0
#define RIGHT_WALL STAGE_W

// (Stage width/2) +/- 800
#define LEFT_SPAWN 7200
#define RIGHT_SPAWN 8800

#define true 1
#define false 0

#define TRACE printf("Trace %s %d\n",__FILE__ , __LINE__);

#define MAX_REWIND 30

#endif
