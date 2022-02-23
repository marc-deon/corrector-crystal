#ifndef FIGHTER_READ_H
#define FIGHTER_READ_H

#include "Player.h"
#include <json-c/json.h>

// typedef struct Player;

void Fighter_SpriteInit();

// Call this from main
void Game_Data_InitActions(Player* p);


// Call these internally
const char* Fighter_ReadAnimations(Fighter* fighter, struct json_object* parsed_json, Color colors[256]);
const char* Fighter_ReadActions(Fighter* fighter, struct json_object* parsed_json);
const char* Fighter_ReadMotions(Fighter* fighter, struct json_object* parsed_json);

#endif /* FIGHTER_READ_H */
