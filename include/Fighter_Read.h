#ifndef FIGHTER_READ_H
#define FIGHTER_READ_H

#include "Player.h"
#include <json-c/json.h>

void Fighter_SpriteInit(Fighter* f, char* path);

// Call this from main
void Game_Data_InitActions(Player* p);


// Call these internally
const char* Fighter_ReadAnimations(Fighter* fighter, struct json_object* parsed_json);
const char* Fighter_ReadActions(Fighter* fighter, struct json_object* parsed_json);
const char* Fighter_ReadMotions(Fighter* fighter, struct json_object* parsed_json);
void Fighter_GetPalette(Fighter* f, int palIndex);
struct json_object* Fighter_GetParsedJson(char* path);

#endif /* FIGHTER_READ_H */
