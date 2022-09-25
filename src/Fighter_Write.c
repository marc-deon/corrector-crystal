#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h> // Unix only: getcwd
#include <json-c/json_types.h>

#include "Animation.h"
#include "Fighter_Write.h"
#include "stretchy_buffer.h"

bool _Fighter_WriteBasic(Fighter* f, json_object* job){
    json_object_object_add(job, "name", json_object_new_string(f->entity->name));
    json_object_object_add(job, "maxJumps", json_object_new_int(f->maxJumps));
    json_object_object_add(job, "maxHealth", json_object_new_int(f->maxHealth));
    json_object_object_add(job, "maxMeter", json_object_new_int(f->maxMeter));
    json_object_object_add(job, "maxMana", json_object_new_int(f->maxMana));
    json_object_object_add(job, "gravity", json_object_new_int(f->gravity));
}

bool _Fighter_SaveAnimations(Fighter* f, json_object* job){

    json_object* animList = json_object_new_array_ext(sb_count(f->animations));
    json_object_object_add(job, "animations", animList);


    for(int i = 0; i < sb_count(f->animations); i++){

        json_object* animData = json_object_new_object();

        json_object* name;
        // TODO: Texture filename?
        // Texture texture;
        // char* textureName;
        json_object* frames;
        json_object* wait; // 1/frameRate. The number of frames to wait before incrementing the displayed frame. "Animated on X's".
        json_object* loopStart;
        json_object* spriteClips = json_object_new_array_ext(4); // Stretchy buffer; RectIs for each sprite frame
        json_object* linksToName;

        Animation* anim = f->animations[i];

        name = json_object_new_string(anim->name);
        frames = json_object_new_int(anim->frameCount);
        wait = json_object_new_int(anim->frameWait);
        loopStart = json_object_new_int(anim->loopStart);
        // for(int j = 0; j < 4; j ++)
        //     json_object_array_add(spriteClips, json_object_new_int(anim->spriteClips[i][j]));
        linksToName = json_object_new_string(anim->linksTo->name);

        json_object_object_add(animData, "name", name);
        json_object_object_add(animData, "frames", frames);
        json_object_object_add(animData, "wait", wait);
        json_object_object_add(animData, "loopStart", loopStart);
        json_object_object_add(animData, "linksTo", linksToName);

        json_object_array_add(animList, animData);

    }

    return true;
}

bool _Fighter_SaveActions(Fighter* f, json_object* job){

    json_object* animList = json_object_new_array_ext(sb_count(f->actions));
    json_object_object_add(job, "actions", animList);


    for(int i = 0; i < sb_count(f->actions); i++){

        Action* act = f->actions[i];
        json_object* actData = json_object_new_object();

        json_object* name;
        json_object* canLinkAfter;
        json_object* mustLinkAfter;
        json_object* state;
        json_object* priority;
        json_object* animation;
        json_object* damage;
        json_object* hitstun;
        json_object* wallSplat;
        json_object* dunk;
        json_object* hardKnockdown;
        json_object* forceAir;
        json_object* groundAir;
        json_object* overrideGravity;
        json_object* hitStop;
        json_object* knockback = json_object_new_array_ext(2);
        json_object* airKnockback = json_object_new_array_ext(2);
        json_object* knockbackFrames;
        json_object* selfKnockback = json_object_new_array_ext(2);
        json_object* selfKnockbackFrames;
        json_object* step = json_object_new_array_ext(2);
        json_object* stepDelay;
        json_object* burnsJump;
        json_object* restoresJump;
        json_object* overrideSelfGravity = json_object_new_array_ext(2);
        json_object* overrideSelfVelocity = json_object_new_array_ext(2);
        json_object* overrideSelfTime;
        json_object* index;
        json_object* phase;

        name = json_object_new_string(act->name);
        canLinkAfter = json_object_new_int(act->canLinkAfter);
        mustLinkAfter = json_object_new_int(act->mustLinkAfter);
        state = json_object_new_int(act->state);
        priority = json_object_new_int(act->priority);
        animation = json_object_new_string(act->animation->name);
        damage = json_object_new_int(act->damage);
        hitstun = json_object_new_int(act->hitstun);
        wallSplat = json_object_new_boolean(act->wallSplat);
        dunk = json_object_new_boolean(act->dunk);
        hardKnockdown = json_object_new_boolean(act->hardKnockdown);
        forceAir = json_object_new_boolean(act->forceAir);
        groundAir = json_object_new_boolean(act->groundAir);
        overrideGravity = json_object_new_boolean(act->overrideGravity);
        hitStop = json_object_new_int(act->hitStop);
        for(int i = 0; i < 2; i ++) json_object_array_add(knockback, json_object_new_int(act->knockback[i]));
        for(int i = 0; i < 2; i ++) json_object_array_add(airKnockback, json_object_new_int(act->airKnockback[i]));
        knockbackFrames = json_object_new_int(act->knockbackFrames);
        for(int i = 0; i < 2; i ++) json_object_array_add(selfKnockback, json_object_new_int(act->selfKnockback[i]));
        selfKnockbackFrames = json_object_new_int(act->selfKnockbackFrames);
        for(int i = 0; i < 2; i ++) json_object_array_add(step, json_object_new_int(act->step[i]));
        stepDelay = json_object_new_int(act->stepDelay);
        burnsJump = json_object_new_int(act->burnsJump);
        restoresJump = json_object_new_boolean(act->restoresJump);
        for(int i = 0; act->overrideSelfGravity && (i < 2); i ++)
        {
            int val = act->overrideSelfGravity[i];
            json_object* val2 = json_object_new_int(val);
            json_object_array_add(overrideSelfGravity, val2);
        }
        for(int i = 0; act->overrideSelfVelocity && (i < 2); i ++)
        {
            int val = act->overrideSelfVelocity[i];
            json_object* val2 = json_object_new_int(val);
            json_object_array_add(overrideSelfVelocity, val2);
        }
        overrideSelfTime = json_object_new_int(act->overrideSelfTime);
        index = json_object_new_int(act->index);
        phase = json_object_new_boolean(act->phase);

        json_object_object_add(actData, "name", name);
        json_object_object_add(actData, "canLinkAfter", canLinkAfter);
        json_object_object_add(actData, "mustLinkAfter", mustLinkAfter);
        json_object_object_add(actData, "state", state);
        json_object_object_add(actData, "priority", priority);
        json_object_object_add(actData, "animation", animation);
        json_object_object_add(actData, "damage", damage);
        json_object_object_add(actData, "hitstun", hitstun);
        json_object_object_add(actData, "wallSplat", wallSplat);
        json_object_object_add(actData, "dunk", dunk);
        json_object_object_add(actData, "hardKnockdown", hardKnockdown);
        json_object_object_add(actData, "forceAir", forceAir);
        json_object_object_add(actData, "groundAir", groundAir);
        json_object_object_add(actData, "overrideGravity", overrideGravity);
        json_object_object_add(actData, "hitStop", hitStop);
        json_object_object_add(actData, "knockback", knockback);
        json_object_object_add(actData, "airKnockback", airKnockback);
        json_object_object_add(actData, "knockbackFrames", knockbackFrames);
        json_object_object_add(actData, "selfKnockback", selfKnockback);
        json_object_object_add(actData, "selfKnockbackFrames", selfKnockbackFrames);
        json_object_object_add(actData, "step", step);
        json_object_object_add(actData, "stepDelay", stepDelay);
        json_object_object_add(actData, "burnsJump", burnsJump);
        json_object_object_add(actData, "restoresJump", restoresJump);
        if(act->overrideSelfGravity)
            json_object_object_add(actData, "overrideSelfGravity", overrideSelfGravity);
        if(act->overrideSelfVelocity)
            json_object_object_add(actData, "overrideSelfVelocity", overrideSelfVelocity);
        json_object_object_add(actData, "overrideSelfTime", overrideSelfTime);
        // json_object_object_add(actData, "i", i);
        json_object_object_add(actData, "phase", phase);

        json_object_array_add(animList, actData);

    }

    return true;
}

bool _Fighter_SaveMotions(Fighter* f, json_object* job){

    json_object* motionList = json_object_new_array_ext(sb_count(f->motions));
    json_object_object_add(job, "motions", motionList);

    for(int i = 0; i < sb_count(f->motions); i++){
        Motion* m = f->motions[i];
        json_object* motionData = json_object_new_object();

        json_object* priority;   // int
        json_object* airOk;      // int
        json_object* name;       // char*
        json_object* pattern;    // char*  
        json_object* action;     // Action* -> char*
        json_object* bufferSize; // uint   

        priority = json_object_new_int(m->priority);
        airOk = json_object_new_boolean(m->airOk);
        name = json_object_new_string(m->name);
        pattern = json_object_new_string(m->pattern);
        if(m->action->name)
            action = json_object_new_string(m->action->name);
        bufferSize = json_object_new_int(m->bufferSize);

        json_object_object_add(motionData, "priority", priority);
        json_object_object_add(motionData, "airOk", airOk);
        json_object_object_add(motionData, "name", name);
        json_object_object_add(motionData, "pattern", pattern);
        if(m->action->name)
            json_object_object_add(motionData, "action", action);
        json_object_object_add(motionData, "bufferSize", bufferSize);

        json_object_array_add(motionList, motionData);
    }

    return true;
}

bool Fighter_Save(Fighter* f, const char* path){

    printf("Beginning to save fighter [\%s] to [%s]\n", /*f->entity->name,*/ path);

    // Construct the json_object
    struct json_object *job = json_object_new_object();

    _Fighter_WriteBasic(f, job);

    _Fighter_SaveAnimations(f, job);
    _Fighter_SaveActions(f, job);
    _Fighter_SaveMotions(f, job);

    // Write to file
    FILE* fp;
    fp = fopen(path, "w");
    if(!fp){
        printf("Problem opening file to write: [%s]...\n", path);
        char cwdpath[80];
        getcwd(cwdpath, 80);
        printf("CWD: %s\n", cwdpath);
        perror("Error");
        assert(fp);
    }
    size_t length;

    const char* str = json_object_to_json_string_length(job, JSON_C_TO_STRING_PRETTY, &length);
    printf("%s\n", str);

    fwrite(str, sizeof(char), length, fp);

    fclose(fp);

}