#ifndef FIGHTER_WRITE_H
#define FIGHTER_WRITE_H
#include <stdbool.h>
#include <json-c/json.h>
#include "Fighter.h"

bool Fighter_Save(Fighter* f, const char* path);
// bool _Fighter_SaveAnimations(Fighter*f, json_object* job);
// bool _Fighter_SaveActions(Fighter*   f, json_object* job);
// bool _Fighter_SaveMotions(Fighter*   f, json_object* job);

#endif