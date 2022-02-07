#ifndef FIGHTERFLAGS_H
#define FIGHTERFLAGS_H

typedef enum {
    FF_NONE        = 0,
    FF_AIR         = 1<<0, // 0 if grounded
    FF_HARD_KD     = 1<<1,
    FF_ATTK_INVULN = 1<<2,
    FF_PROJ_INVULN = 1<<3,
    FF_GRAB_INVULN = 1<<4,
    FF_FULL_INVULN = FF_ATTK_INVULN + FF_PROJ_INVULN + FF_GRAB_INVULN,
    // Others?

} FighterFlags;

#endif