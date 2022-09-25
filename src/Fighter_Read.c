#include <json-c/json.h>

#include "Fighter_Read.h"
#include "Match.h"
#include "stretchy_buffer.h"
#include "circular_buffer.h"
#include "Action.h"
#include "Player.h"
#include "Json_Extension.h"

#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <unistd.h> // Unix only: getcwd

#include <Entities.h>

Texture background;
// extern Match currentMatch;

const char* Fighter_ReadActions(Fighter* fighter, struct json_object* parsed_json) {
    struct json_object* actions;
    bool defaulted;
    json_object_object_get_ex(parsed_json, "actions", &actions);

    fighter->actions = NULL;
    for(int i = 0; i < json_object_array_length(actions); i++) {

        struct json_object* action = json_object_array_get_idx(actions, i);
        
        char* name = json_get_string(action, "name");
        int canLinkAfter = json_get_default_int(action, "canLinkAfter", 0);
        int mustLinkAfter = json_get_default_int(action, "mustLinkAfter", 0);
        FighterFlags state = (FighterFlags)json_get_default_int(action, "state", -1);
        int priority = json_get_default_int(action, "priority", 0);
        
        char* animationName = json_get_default_string(action, "animation", name);
        int index = Animation_FindIndexByName(fighter->animations, sb_count(fighter->animations), animationName);
        if(index == -1) {
            printf("Animation [%s] not found\n", animationName);
            assert(false && "Animation index cannot be -1\n");
        }
        Animation* animation = fighter->animations[index];
        
        int damage = json_get_default_int(action, "damage", 0);
        int hitstun = json_get_default_int(action, "hitstun", 0);
        int hitStop = json_get_default_int(action, "hitStop", 0);
        bool forceAir = json_get_default_bool(action, "forceAir", false);
        int groundAir = json_get_default_int(action, "groundAir", -1);
        bool air = json_get_default_bool(action, "air", false);
        bool overrideGravity = json_get_default_bool(action, "overrideGravity", false);

        int knockback[2];
        json_get_default_int_array(action, "knockback", knockback, 2);
        
        int airKnockback[2];
        defaulted = json_get_default_int_array(action, "airKnockback", airKnockback, 2);
        if (defaulted) {
            airKnockback[0] =  4;
            airKnockback[1] = -5;
        }
        uint knockbackFrames = json_get_default_int(action, "knockbackFrames", 4);

        int selfKnockback[2];
        json_get_default_int_array(action, "selfKnockback", selfKnockback, 2);

        uint selfKnockbackFrames = json_get_default_int(action, "selfKnockbackFrames", 0);
        bool wallSplat = json_get_default_bool(action, "wallSplat", false);
        bool dunk = json_get_default_bool(action, "dunk", false);
        bool hardKnockdown = json_get_default_bool(action, "hardKnockdown", false);

        int step[2];
        json_get_default_int_array(action, "step", step, 2);
        uint stepDelay = json_get_default_int(action, "stepDelay", 0);

        int burnsJump = json_get_default_int(action, "burnsJump", 0);
        bool restoresJump = json_get_default_bool(action, "restoresJump", false);

        int _overrideSelfGravity[2];
        int* overrideSelfGravity = _overrideSelfGravity;

        defaulted = json_get_default_int_array(action, "overrideSelfGravity", overrideSelfGravity, 2);
        if(defaulted) {
            overrideSelfGravity = NULL;
        }

        int _overrideSelfVelocity[2];
        int* overrideSelfVelocity = _overrideSelfVelocity;
        defaulted = json_get_default_int_array(action, "overrideSelfVelocity", overrideSelfVelocity, 2);
        if(defaulted) {
            overrideSelfVelocity = NULL;
        }
        

        uint overrideSelfTime = json_get_default_int(action, "overrideSelfTime", 0);

        bool phase = json_get_default_bool(action, "phase", false);

        char* entity_name = json_get_default_string(action, "entity", NULL);
        int entity = Entities_GetEntityFromName(entity_name);

        Action* act = Action_Create(
            name,
            canLinkAfter,
            mustLinkAfter,
            state,
            priority,
            animation,
            damage,
            hitstun,
            wallSplat,
            dunk,
            hardKnockdown,
            forceAir,
            groundAir,
            overrideGravity,
            hitStop,
            knockback,
            airKnockback,
            knockbackFrames,
            selfKnockback,
            selfKnockbackFrames,
            step,
            stepDelay,
            burnsJump,
            restoresJump,
            overrideSelfGravity,
            overrideSelfVelocity,
            overrideSelfTime,
            i,
            phase,
            entity
        );
        sb_push(fighter->actions, act);
        
        ///////////
        // Box shit

        int shoveArray[4];
        json_get_default_int_array(action, "shovebox", shoveArray, 4);
        RectI shovebox_rect;
        shovebox_rect.pos.x  = shoveArray[0];
        shovebox_rect.pos.y  = shoveArray[1];
        shovebox_rect.size.x = shoveArray[2];
        shovebox_rect.size.y = shoveArray[3];
        Shovebox sb = Shovebox_Create(shovebox_rect);
        Action_SetShovebox(act, sb);

        
        struct json_object* ob_hitboxes;
        struct json_object* ob_hurtboxes;
        json_bool hitExists = json_object_object_get_ex(action, "hitboxes", &ob_hitboxes);
        json_bool hurtExists = json_object_object_get_ex(action, "hurtboxes", &ob_hurtboxes);


        if(hitExists) {
            for(int j = 0; j < json_object_array_length(ob_hitboxes); j++) {
                RectI rect;
                array_list* al = json_object_get_array(ob_hitboxes);
                // First dig into the array of arrays...
                array_list* rect_arr_list = json_object_get_array((const json_object*) array_list_get_idx(al, j));

                // And then into the array of ints.
                int hitArray[6];
                
                json_get_default_int_array((json_object*) rect_arr_list, "hitbox", hitArray, 6);

                rect.pos.x =  json_object_get_int((const json_object*) array_list_get_idx(rect_arr_list, 0));
                rect.pos.y =  json_object_get_int((const json_object*) array_list_get_idx(rect_arr_list, 1));
                rect.size.x = json_object_get_int((const json_object*) array_list_get_idx(rect_arr_list, 2));
                rect.size.y = json_object_get_int((const json_object*) array_list_get_idx(rect_arr_list, 3));
                // A hitbox is actually a 6tuple; x, y, w, h, activeOnFrame, offOnFrame.
                // If actOn isn't specified, set it to 0.
                // If offOn isn't specified, set it to infinity.
                int actOn = 0;
                int offOn = __INT_MAX__;
                if(array_list_length(rect_arr_list) > 4)
                    actOn = json_object_get_int((const json_object*) array_list_get_idx(rect_arr_list, 4));
                if(array_list_length(rect_arr_list) > 5)
                    offOn = json_object_get_int((const json_object*) array_list_get_idx(rect_arr_list, 5));

                Action_AddHitbox(act, Hitbox_Create(rect, actOn, offOn));
            }
        }

        if(hurtExists) {
            for(int j = 0; j < json_object_array_length(ob_hurtboxes); j++) {
                RectI rect;
                array_list* al = json_object_get_array(ob_hurtboxes);
                // First dig into the array of arrays...
                array_list* rect_arr = json_object_get_array((const json_object*) array_list_get_idx(al, j));
                // And then into the array of ints.
                rect.pos.x = json_object_get_int( array_list_get_idx(rect_arr, 0));
                rect.pos.y = json_object_get_int((const json_object*) array_list_get_idx(rect_arr, 1));
                rect.size.x = json_object_get_int((const json_object*) array_list_get_idx(rect_arr, 2));
                rect.size.y = json_object_get_int((const json_object*) array_list_get_idx(rect_arr, 3));
                Action_AddHurtbox(act, Hurtbox_Create(rect));
            }
        }

    }

    // Now that we have the actions created, we can link them.
    printf("Beginning links\n");
    for(int i = 0; i < sb_count(fighter->actions); i++) {
        struct json_object* action = json_object_array_get_idx(actions, i);
        Action* a = fighter->actions[i];
        a->linksTo = NULL;
        // sb_add(a->linksTo, 1);
        // Link to self by default; this may be cleared later
        sb_push(a->linksTo, a);

        json_object* nameArray = json_object_object_get(action, "linksTo");

        // Certain actions automatically link to others (e.g. landing to standing, rekka 2 to [rekka3high, rekka 3low])
        if (nameArray) {
            int length = json_object_array_length(nameArray);

            if (length > 0) {
                sb_free(a->linksTo);
                a->linksTo = NULL;

                // printf("%s links to", a->name);
                for(int j = 0; j < length; j++) {
                    char* linkName = (char *) json_object_get_string(json_object_array_get_idx(nameArray, j));
                    // printf("... %s", linkName);
                    if (linkName) {
                        int index = Action_FindIndexByName(fighter->actions, sb_count(fighter->actions), linkName);
                        // printf("%s\n", linkName);
                        assert(index >= 0 && "Could not find action by name");
                        sb_push(a->linksTo, fighter->actions[index]);
                    }
                }
                // printf(".\n");
            }
        }

        // Certain actions can only link from others (e.g. rekka 2 from rekka 1)
        char* linksFromName = json_get_default_string(action, "linksFrom", NULL);

        if (linksFromName) {
            int index = Action_FindIndexByName(fighter->actions, sb_count(fighter->actions), linksFromName);
            printf("%s\n", linksFromName);
            assert(index >= 0 && "Could not find action by name");
            Action_SetLinkFrom(a, fighter->actions[index]);
        }
    }
    return NULL;
}


const char* Fighter_ReadAnimations(Fighter* fighter, struct json_object* parsed_json) {
        struct json_object* animations;
        json_object_object_get_ex(parsed_json, "animations", &animations);

        fighter->animations = NULL;
        for(int i = 0; i < json_object_array_length(animations); i++) {
            struct json_object* animation = json_object_array_get_idx(animations, i);

            char* name = json_get_string(animation, "name");
            char* sprite = json_get_string(animation, "sprite");
            int frames = json_get_int(animation, "frames");
            int wait = json_get_int(animation, "wait");
            
            int loopStart = json_get_default_int(animation, "loopStart", 0);
            int cropInts[4]; 
            json_get_int_array(animation, "cropRect", cropInts);
            RectI cropRect = {cropInts[0], cropInts[1], cropInts[2], cropInts[3]};
        
            sb_push(fighter->animations, Animation_Create(name, sprite, frames, wait, cropRect, loopStart));
        }

        // Now that we have the animations created, we can link them.
        for(int i = 0; i < sb_count(fighter->animations); i++) {
            struct json_object* animation = json_object_array_get_idx(animations, i);
            char* linkName = json_get_default_string(animation, "linksTo", NULL);

            // If we don't link to anything, skip.
            if(!linkName)
                continue;

            Animation* a = fighter->animations[i];
            Animation* linksTo;
            int index = Animation_FindIndexByName(fighter->animations, sb_count(fighter->animations), linkName);
            linksTo = fighter->animations[index];
            Animation_SetLink(a, linksTo);
        }
    return NULL;
}

void Fighter_GetPalette(Fighter* f, int palIndex) {
    FILE* fp;
    fp = fopen("fighterData/superman.pal", "rb");
    
    fseek(fp, palIndex * 256 * 4, SEEK_SET);

    // Read the json file into a char array
    int buffer[256 * 4];
    fread(buffer, 256 * 4, 1, fp);
    fclose(fp);

    memcpy(f->entity->palette, buffer, 256 * 4);

    Image imBlank = GenImageColor(256, 1, BLANK);
    for(int x = 0; x < 256; x++)
        ImageDrawPixel(&imBlank, x, 0, f->entity->palette[x]);
    UnloadTexture(f->entity->paletteTexture);
    f->entity->paletteTexture = LoadTextureFromImage(imBlank);
    UnloadImage(imBlank);
    int shaderPalLoc = GetShaderLocation(f->entity->shader, "palette");
    SetShaderValueTexture(f->entity->shader, shaderPalLoc, f->entity->paletteTexture);
}

struct json_object* Fighter_GetParsedJson(char* path) {
    FILE* fp;
    fp = fopen(path, "r");
    if(!fp) {
        printf("Problem reading from path [%s]...\n", path);
        char cwdpath[80];
        getcwd(cwdpath, 80);
        printf("CWD:%s\n", cwdpath);
        perror("Error");
        assert(fp);
    }
    // Get the size of the json file
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    rewind(fp);

    // Read the json file into a char array
    char buffer[size];
    fread(buffer, size, 1, fp);
    fclose(fp);

    struct json_object* parsed_json = json_tokener_parse(buffer);
    return parsed_json;

}

// TODO(#9): Clean up Game_SpriteInit; it does way more than just sprites.
void Fighter_SpriteInit(Fighter* f, char* path) {


    struct json_object* parsed_json = Fighter_GetParsedJson(path);

    struct json_object* actions;
    struct json_object* motions;


    //////////
    // General
    f->entity->name = json_get_string(parsed_json, "name");
    f->maxJumps = json_get_int(parsed_json, "maxJumps");
    f->maxHealth = json_get_int(parsed_json, "maxHealth");
    f->maxMeter = json_get_int(parsed_json, "maxMeter");
    f->maxMana = json_get_int(parsed_json, "maxMana");
    f->gravity = json_get_int(parsed_json, "gravity");
    
    /////////////
    // Animations

    const char* result = Fighter_ReadAnimations(f, parsed_json);
    if(result) {
        printf("Error: %s", result);
        return;
    } else{
        puts("Made animations");
    }

    //////////
    // Actions
    result = Fighter_ReadActions(f, parsed_json);
    if(result) {
        printf("Error: %s", result);
        return;
    } else{
        puts("Made actions");
    }

    //////////
    // Motions
    result = Fighter_ReadMotions(f, parsed_json);
    if(result) {
        printf("Error: %s", result);
        return;
    } else{
        puts("Made motions");
    }

    f->portrait = LoadTexture("Graphics/Fighter/Shoto/aoko_portrait.png");
}


char* sfxPaths[] = {
    "audio/sfx/1.wav",
    "audio/sfx/2.wav",
    "audio/sfx/3.wav",
    "audio/sfx/4.wav",
    "audio/sfx/5.wav",
    "audio/sfx/6.wav",
    "audio/sfx/7.wav",
    "audio/sfx/8.wav",
    "audio/sfx/9.wav",
    "audio/sfx/10.wav",
    "audio/sfx/11.wav",
    "audio/sfx/sumo.wav"
};

// FIXME: For random
#include <stdlib.h>

void Game_Data_InitActions(Player* p) {

    // TODO(#10): Get sound effects. Currently, we just assign randomly.
    time_t t;
    srand((unsigned) time(&t));
    for(int i = 0; i < sb_count(p->pointCharacter->actions); i++) {
        
        char* path = sfxPaths[rand() % 12];
        // TODO: Actual SFX
        p->pointCharacter->actions[i]->audioChunk = LoadSound(path);
    }
}


const char* Fighter_ReadMotions(Fighter* fighter, struct json_object* parsed_json) {
    struct json_object* motions;
    json_object_object_get_ex(parsed_json, "motions", &motions);

    fighter->motions = NULL;
    for(int i = 0; i < json_object_array_length(motions); i++) {

        struct json_object* motion = json_object_array_get_idx(motions, i);

        const char* name        = json_get_string(motion, "name");
        const char* pattern     = json_get_string(motion, "pattern");
        int priority            = json_get_int(motion, "priority");
        bool airOk              = json_get_default_bool(motion, "airOk", false);
        const char* actionName  = json_get_default_string(motion, "action", name);
        int bufferSize          = json_get_default_int(motion, "bufferSize", 0);

        int len = sb_count(fighter->actions);
        int j = Action_FindIndexByName(fighter->actions, len, actionName);
        assert(j >= 0 && "Couldn't find action by name");

        Action* actionLink = fighter->actions[j];
        sb_push(fighter->motions, Motion_Create(priority, airOk, name, pattern, actionLink, bufferSize));
    }
    return NULL;
}

void Fighter_Destroy(Fighter* fighter) {


    //// Stretchy buffers
    for(int i = 0; i < sb_count(fighter->actions); i++)
        Action_Free(fighter->actions[i]);
    sb_free(fighter->actions);
    
    for(int i = 0; i < sb_count(fighter->animations); i++)
        Animation_Free(fighter->animations[i]);
    sb_free(fighter->animations);
    
    for(int i = 0; i < sb_count(fighter->motions); i++)
        free(fighter->motions[i]);
    
    sb_free(fighter->motions);
    cb_free(fighter->stateHistory);

}
