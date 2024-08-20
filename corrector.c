// TODO(#19): Free stuff (animations, motions, etc) when done.

// #define DRAWPOINT
// #define DRAWCOLLISIONBOX
#define DRAWHURTBOXES
#define DRAWHITBOXES
#define DRAWBLOCKBOXES
//#define SUPERMAN "fighterData/superman_stable.jsonc"
#define SUPERMAN "fighterData/superman_unstable.json"

int asamiya = 0;

#include <assert.h>
#include <raylib.h>

#define PLATFORM_DESKTOP
#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

#include "Action.h"
#include "CC_Audio.h"
#include "CC_Consts.h"
#include "CC_Input.h"
#include "circular_buffer.h"
#include "FighterFlags.h"
#include "Fighter_Read.h"
#include "main.h"
#include "Match.h"
#include "QFramework_Input.h"
#include "TestMenus.h"
#include "Menus_Pause.h"
#include "Ui_Match.h"
#include "Ui_Types.h"
#include "Entity.h"
#include "Json_Extension.h"

bool stopGame = 0;

RectI camera;
Match currentMatch;

const int maxpal = 31;
int p1Pal = 9;
int p2Pal = 18;

/*  Rollback flowchart:

    Somewhere, we:
    1. Get remote inputs
    2. Check against local guesses, count wrong
    3. For each wrong, re-simulate a frame

    ...

    Update stick states
    Advance action frames, auto-advance action if neccesary
    unshove
    Update positions
    Update hitboxes
    Check for overlaps in hitboxes
    Deal damage
    Check for win

    Render
*/


typedef struct save_state {
    unsigned int currentTime;
    unsigned int hitStop;
    FighterState fighterStates[2];
} SaveState;

SaveState trainingState;
int isStateSet = 0;


void Training_SaveState() {
    isStateSet = 1;
    trainingState.currentTime = cb_last(currentMatch.history).currentTime;
    trainingState.hitStop     = cb_last(currentMatch.history).hitStop;
    trainingState.fighterStates[0] = cb_last(currentMatch.players[0].pointCharacter->stateHistory);
    trainingState.fighterStates[1] = cb_last(currentMatch.players[1].pointCharacter->stateHistory);
    // Do puppets
    // Do projectiles
}


void Training_LoadState() {
    if (!isStateSet)
        return;
    cb_last(currentMatch.history).currentTime = trainingState.currentTime;
    cb_last(currentMatch.history).hitStop     = trainingState.hitStop;
    cb_last(currentMatch.players[0].pointCharacter->stateHistory) = trainingState.fighterStates[0];
    cb_last(currentMatch.players[1].pointCharacter->stateHistory) = trainingState.fighterStates[1];
    // Do puppets
    // Do projectiles
}

// returns -1, 0, or 1 based on the sign of i.
#define sign(i) (i > 0 | -(i<0))


bool CC_INIT() {
    bool videoSuccess;
    bool audioSuccess;

    SetTraceLogLevel(LOG_NONE);
    InitWindow(VIRT_SCREEN_SIZE_X, VIRT_SCREEN_SIZE_Y, "Corrector Raylib");
    SetTargetFPS(60);
    // TODO: Render to a texture, then stretch that texture to window size
    SetWindowState(FLAG_WINDOW_RESIZABLE);


    videoSuccess = IsWindowReady();
    audioSuccess = CC_Audio_Init();

    if (videoSuccess) {
        printf("Video init error: %d\n", videoSuccess);
    } else {
        printf("Video OK\n");
    }

    if (videoSuccess) {
        printf("Audio init error: %d\n", audioSuccess);
    } else {
        printf("Audio OK\n");
    }

    return videoSuccess & audioSuccess;
}


void CC_CLOSE() {
    // Destroy Window

    free(previousKeyStates);
}


void Game_PlayerInit() {
    currentMatch.players[0].stick =          &p1Stick;
    currentMatch.players[0].pointCharacter = Fighter_Create(SUPERMAN);
    printf("During init p1 is %p\nDuring init p2 is %p\n", &p1Stick, &p2Stick);
    void* parsed = json_get_parsed_json(SUPERMAN);
    Fighter_GetPalette(currentMatch.players[0].pointCharacter, p1Pal);
    currentMatch.players[1].stick =          &p2Stick;
    currentMatch.players[1].pointCharacter = Fighter_Create(SUPERMAN);

    Fighter_GetPalette(currentMatch.players[1].pointCharacter, p2Pal);
}


void Game_DataInit() {
    currentMatch.players[0].pointCharacter->opponent = currentMatch.players[1].pointCharacter;
    currentMatch.players[1].pointCharacter->opponent = currentMatch.players[0].pointCharacter;
    for (int i = 0; i < 2; i++) {
        Player* p = &currentMatch.players[i];
        Game_Data_InitActions(p);

        p->pointCharacter->ownerIndex = i+1;
    }
    currentMatch.players[0].pointCharacter->opponent = currentMatch.players[1].pointCharacter;
    currentMatch.players[1].pointCharacter->opponent = currentMatch.players[0].pointCharacter;
    ResetRound();
    currentMatch.players[0].pointCharacter->opponent = currentMatch.players[1].pointCharacter;
    currentMatch.players[1].pointCharacter->opponent = currentMatch.players[0].pointCharacter;

}


Match* GameInit(int p1CharaIndex, int p2CharaIndex) {
    background = LoadTexture("Graphics/Background/training-big.png");

    currentMatch = Match_Init();

    Game_PlayerInit();

    Game_DataInit(p1CharaIndex, p2CharaIndex);

    Ui_Match_Init(&currentMatch);
    return &currentMatch;
}


void CC_Render() {

     Rectangle r = {
         camera.x * UNIT_TO_PIX,
         camera.y * UNIT_TO_PIX,
         camera.w,
         camera.h,
     };

    DrawTextureRec(background, r, (Vector2) {0,0}, WHITE);

    // Draw sprites
    for (int i = 0; i < 2; i++) {
        Player* p = &currentMatch.players[i];
        FighterState* fs = &cb_last(p->pointCharacter->stateHistory);
        Fighter_DrawSprite(p->pointCharacter, camera);

    }

    // Draw debug components
    // We keep these as two loops because we want everything to be drawn on top of sprites, regardless of player
    for (int i = 0; i < 2; i++) {
        Player* p = &currentMatch.players[i];
        #ifdef DRAWPOINT
            Fighter_DrawPoint(p->pointCharacter, camera);
        #endif
        #ifdef DRAWCOLLISIONBOX
            Fighter_DrawCollisionbox(p->pointCharacter, camera);
        #endif
        #ifdef DRAWHURTBOXES
            Fighter_DrawHurtboxes(p->pointCharacter, camera);
        #endif
        #ifdef DRAWHITBOXES
            Fighter_DrawHitboxes(p->pointCharacter, camera);
        #endif
        #ifdef DRAWBLOCKBOXES
            Fighter_DrawBlockboxes(p->pointCharacter, camera);
        #endif
    }
    DrawHUD(&currentMatch);
}

////////////////////////////////////////////
//                                        //
//                                        //
//               Main Loops               //
//                                        //
//                                        //
////////////////////////////////////////////

void CC_ProcessCamera() {
    camera.x = 0;
    for (int i = 0; i < 2; i++)
        camera.x += cb_last(currentMatch.players[i].pointCharacter->entity->history).position.x;
    camera.x -= camera.w * PIX_TO_UNIT;
    camera.x /= 2;

    camera.y = STAGE_H;
    for (int i = 0; i < 2; i++)
        camera.y = min(camera.y, cb_last(currentMatch.players[i].pointCharacter->entity->history).position.y);
    camera.y /= 2;
    camera.y += 2000;

    camera.x = max(0, camera.x);
    if (camera.x + camera.w * PIX_TO_UNIT > STAGE_W)
        camera.x = STAGE_W - camera.w * PIX_TO_UNIT;

    camera.y = max(0, camera.y);
    if (camera.y + camera.h * PIX_TO_UNIT > STAGE_H)
        camera.y = STAGE_H - camera.h * PIX_TO_UNIT;

}


int CC_ProcessInput() {

    if (Stick_IsButtonJustDown(&p1Stick, STICK_START) || Stick_IsButtonJustDown(&p2Stick, STICK_START)) {
        printf("paused\n");
        UiMenu* menu = MakePauseMenu();
        currentMatch.paused = true;
        return 0;
    }

    if (QF_IsKeyJustDown(KEY_F5)) {
        Training_SaveState();
    }

    else if (QF_IsKeyJustDown(KEY_F6)) {
        Training_LoadState();
    }

    int p1Pal_old = p1Pal;
    int p2Pal_old = p2Pal;

    p1Pal += QF_IsKeyJustDown(KEY_RIGHT_BRACKET) - QF_IsKeyJustDown(KEY_LEFT_BRACKET);
    p1Pal = (maxpal + p1Pal) % maxpal;
    p2Pal += QF_IsKeyJustDown(KEY_APOSTROPHE) - QF_IsKeyJustDown(KEY_SEMICOLON);
    p2Pal = (maxpal + p2Pal) % maxpal;

    if (p1Pal != p1Pal_old || p2Pal != p2Pal_old) {
        void* parsed = json_get_parsed_json(SUPERMAN);
        Fighter_GetPalette(currentMatch.players[0].pointCharacter, p1Pal);
        Fighter_GetPalette(currentMatch.players[1].pointCharacter, p2Pal);
    }

    for (int i = 0; i < 2; i++) {
        Player* p = &currentMatch.players[i];
        StickState* ss = Stick_GetState(p->stick);

        assert(ss && "Stick state must not be null");

        // Look for motions
        if (! cb_last(currentMatch.history).hitStop) {
            int i = Player_GetFirstPattern(p);
            if (i != -1) {
                Action* a = p->pointCharacter->motions[i]->action;

                // Start new actions
                if (a != cb_last(p->pointCharacter->entity->history).currentAction) {
                    Fighter_StartAction(p->pointCharacter, a, 0);
                }
            }
        }
    }

    return 1;
}


void CC_RewindFrames(int x) {
    // What histories do we have?
    // P1 history
    // P1 Entities histories
    // P2 history
    // P2 Entities histories
    // Match history
}


struct Collision_Hit {
    Entity* attacker;
    Entity* defender;
    Action* attacker_action;
    Action* defender_action;
};


Entity** CC_ProcessCheckBlocks(struct Collision_Hit* hits) {
    // Stretchy buffer of Entity pointers
    Entity** results = NULL;

    for (int i = 0; i < sb_count(hits); i++) {
        struct Collision_Hit hit = hits[i];
        bool allowed_to_block = true;

        // Retrieve a copy of attacker and defender flags
        uint attacker = hit.attacker_action->attacker_flags; 
        uint defender = hit.defender_action->defender_flags; 
        // Separate air component
        bool attacker_air = attacker && BLOCK_FLAG_AERIAL;   
        bool defender_air = defender && BLOCK_FLAG_AERIAL;   
        // Separate grounded components
        attacker &= 0b0111;                                  
        defender &= 0b0111;                                  

        // If the defender is in the air but the attack is not air blockable
        if (defender_air && !attacker_air)
            allowed_to_block = false;

        // If this is valid defense, then attacker will be a subset of defender (I think)
        if ((attacker | defender) != defender)
            allowed_to_block = false;

        if (!allowed_to_block)
            continue;

        // Only fighters can block
        if (hit.defender->fighter == NULL)
            continue;

        // Check stick direction
        bool holding_away;
        Fighter* us_f = ((Fighter*)(hit.defender->fighter));
        Stick* stick = currentMatch.players[us_f->ownerIndex-1].stick;

        int us_x = cb_last(hit.defender->history).position.x;
        int op_x = cb_last(hit.attacker->history).position.x;

        if (us_x < op_x)
            holding_away = Stick_IsButtonDown(stick, STICK_LEFT);
        else if (us_x > op_x)
            holding_away = Stick_IsButtonDown(stick, STICK_RIGHT);
        else
            holding_away = Stick_IsButtonDown(stick, STICK_LEFT | STICK_RIGHT);

        if (holding_away) {
            sb_push(results, hit.defender);
        }
    }

    return results;
}

// Returns larget hitstop
uint CC_Process_ApplyHits(struct Collision_Hit* hits) {
    uint hitstop = 0;

    for (int i = 0; i < sb_count(hits); i++) {
        struct Collision_Hit hit = hits[i];
        if (hit.defender == NULL) {
            printf("Skipped because block\n");
            continue;
        }

        printf("Appply Hits\n");

        // Calculate hitstop to return
        // TODO: Different on block?
        hitstop = max(hitstop, hit.attacker_action->hitStop);

        // Disable involved hitboxes
        for (int hit = 0; hit < sb_count(hits[i].attacker_action->hitboxes); hit++)
            hits[i].attacker_action->hitboxes[hit]->active = HB_DISABLED;

        // Point fighter specific
        if (hit.defender->fighter)
            Fighter_Damage(hit.defender->fighter, hit.attacker_action);
        // Deal damage
        Entity_Damage(hit.attacker, hit.defender, hit.attacker_action);
    }

    return hitstop;
}

struct Collision_Hit* CC_Process_CheckHitboxes_Pairs(Entity** eList1, Entity** eList2, struct Collision_Hit* hits) {
    // To traverse the entities list depth first, just loop i, j from len to 0
    for (int i = sb_count(eList1)-1; i >= 0; i--) {
        for (int j = sb_count(eList2)-1; j >= 0; j--) {

            // Don't check self-self
            if ((eList1 == eList2) && (i == j)) 
                continue;

            Action* hit = Entity_ShouldHit(eList1[i], eList2[j]);

            // We want to prevent actions from hitting multiple targets... or do we? I'll leave this commented out for now.
            bool action_already_used = false;
            /*
            for (int k = 0; k < stb_len(hits); k++) {
                action_already_used |= hit == hits[k].action;
            }
            */

            if (hit && !action_already_used) {
                struct Collision_Hit chit = {
                    .attacker = eList1[i],
                    .defender = eList2[j],
                    .attacker_action = hit,
                    .defender_action = cb_last(eList2[j]->history).currentAction
                };
                sb_push(hits, chit);
                break;
            }
        }    
    }
    // sb_push may change hits's address inplace, which isn't propogated out to our caller
    return hits;
}

struct Collision_Hit* CC_Process_CheckHitboxes(Fighter* f1, Fighter* f2) {
    struct Collision_Hit* hits = NULL;
    hits = sb_add(hits, 0);

    // 1) Get all entities for P1, P2.
    Entity** p1Entities = Entity_GetRecursive(f1->entity, NULL);
    Entity** p2Entities = Entity_GetRecursive(f2->entity, NULL);

    // 2) Check P1-P1 collision DF (in case of necomech type interactions)
    // hits = CC_Process_CheckHitboxes_Pairs(p1Entities, p1Entities, hits);
    // 3) Do same for P2-P2
    // hits = CC_Process_CheckHitboxes_Pairs(p2Entities, p2Entities, hits);

    // 4) Check P1-P2 collisions RDF
    hits = CC_Process_CheckHitboxes_Pairs(p1Entities, p2Entities, hits);
    // 5 Do same for P2-P1
    hits = CC_Process_CheckHitboxes_Pairs(p2Entities, p1Entities, hits);

    // Return and apply
    return hits;
}


bool CC_ProcessFrame() {
    if (cb_last(currentMatch.history).hitStop) {
        cb_last(currentMatch.history).hitStop--;
        return true;
    }
    else {
        cb_last(currentMatch.history).currentTime--;
    }

    CC_ProcessCamera();


    // Advance action frames, auto-advance action if neccesary
    Fighter_Process_Advance(currentMatch.players[0].pointCharacter);
    Fighter_Process_Advance(currentMatch.players[1].pointCharacter);
    
    // unshove
    Fighter* f1 = currentMatch.players[0].pointCharacter;
    Fighter* f2 = currentMatch.players[1].pointCharacter;
    if (!(cb_last(f1->entity->history).currentAction->phase || cb_last(f2->entity->history).currentAction->phase)) {

        int p1delta = 0;
        int p2delta = 0;

        RectI rect1 = Fighter_FacingRight(f1) ?
            Rect_Flip(cb_last(f1->entity->history).currentAction->shovebox.rect)
            : cb_last(f1->entity->history).currentAction->shovebox.rect;
        rect1.pos = Vec2_Add(rect1.pos, cb_last(f1->entity->history).position);

        RectI rect2 = Fighter_FacingRight(f2) ?
            Rect_Flip(cb_last(f2->entity->history).currentAction->shovebox.rect)
            : cb_last(f2->entity->history).currentAction->shovebox.rect;
        rect2.pos = Vec2_Add(rect2.pos, cb_last(f2->entity->history).position);

        RectI r1 = rect1;
        RectI r2 = rect2;
        int dir;
        while (dir = Rect_ShortestEscape(r1, r2))
        {
            p1delta += Fighter_OnRight(f1) ? 1 : -1;
            p2delta += Fighter_OnRight(f2) ? 1 : -1;

            r1 = rect1; r1.pos.x += p1delta;
            r2 = rect2; r2.pos.x += p2delta;
        }

        cb_last(f1->entity->history).position.x = cb_last(f1->entity->history).position.x + (1 * p1delta);
        cb_last(f2->entity->history).position.x = cb_last(f2->entity->history).position.x + (1 * p2delta);

        Fighter_CheckWalls(f1);
        Fighter_CheckWalls(f2);
    }

    // Update positions
    for (int i = 0; i < 2; i++) {
        Player* p = &currentMatch.players[i];
        Player* op = &currentMatch.players[(i+1)%2];
        if (Fighter_Air(p->pointCharacter)) {
            Fighter_MoveAir(p->pointCharacter, &p->stick->buffer[0], camera);
        }
        else {
            Fighter_MoveGround(p->pointCharacter, &p->stick->buffer[0], &op->stick->buffer[0], camera);
        }
        Fighter_UpdateSubstate(p->pointCharacter);
        Fighter_CheckWalls(p->pointCharacter);

    }

    Entity_Process_Hitbox(currentMatch.players[0].pointCharacter->entity);
    Entity_Process_Hitbox(currentMatch.players[1].pointCharacter->entity);

    // You know what, ignore blockboxes for now. They're really only needed for a grounded state,
    // And therefore not universal.
    // auto val = CC_Process_CheckBlockboxes(f1, f2);

    struct Collision_Hit* hits = CC_Process_CheckHitboxes(f1, f2);

    Entity** entities_to_block = CC_ProcessCheckBlocks(hits);

    for (int i = 0; i < sb_count(entities_to_block); i++) {
        Entity* e = entities_to_block[i];

        // Start blocking
        // TODO: Don't use hard coded 60 frames of blockstun
        Entity_BeginBlock(e, 60);

        //// Update the hits list
        for(int j = 0; j < sb_count(hits); j++) {
            if (hits[j].defender == e) {
                hits[j].defender = NULL;
            }
        }
    }

    uint hitStop = CC_Process_ApplyHits(hits);

    // Make copy of current, store the original, and edit new on the next loop
    for (int i = 0; i < 2; i++) {
        Player* p = &currentMatch.players[i];
        cb_push(p->pointCharacter->stateHistory, cb_last(p->pointCharacter->stateHistory));
        cb_push(p->pointCharacter->entity->history, cb_last(p->pointCharacter->entity->history));
    }

    // int hitStop =
    //     p1HitByAct && p2HitByAct ? max(p1HitByAct->hitStop, p2HitByAct->hitStop)
    //     : p1HitByAct             ? p1HitByAct->hitStop
    //     : p2HitByAct             ? p2HitByAct->hitStop
    //     : 0
    // ;

    if (hitStop || cb_last(currentMatch.history).hitStop)
        cb_last(currentMatch.history).hitStop = max(hitStop, cb_last(currentMatch.history).hitStop);

    { // Check for win
        int result = Match_CheckRoundWin(&currentMatch);
        if (result == 1 || result == 2) {
            currentMatch.currentRounds[result-1]++;
        }
        else if (result == 3) {
            // Tie
            printf("tie\n");
            currentMatch.currentRounds[0]++;
            if (currentMatch.currentRounds[0] >= currentMatch.maxRounds)
                currentMatch.currentRounds[0] = currentMatch.maxRounds - 1;
            currentMatch.currentRounds[1]++;
            if (currentMatch.currentRounds[1] >= currentMatch.maxRounds)
                currentMatch.currentRounds[1] = currentMatch.maxRounds - 1;

        }

        if (result) {
            ResetRound();
        }
    }
}


bool CC_Update() {

    // Update stick states
    int success = CC_ProcessInput();

    if (success == -1)
        return false;

    if (success > 0)
        CC_ProcessFrame();

    return true;
}


void ResetRound() {
    puts("Resetting round");
    camera = (RectI) {0, STAGE_H - VIRT_SCREEN_SIZE_Y, VIRT_SCREEN_SIZE_X, VIRT_SCREEN_SIZE_Y};
    cb_last(currentMatch.history).currentTime = currentMatch.maxTime;
    for (int i = 0; i < 2; i++) {
        Player* p = &(currentMatch.players[i]);
        Fighter* f = p->pointCharacter;

        // Clear Subentities....
        for (int i = 0; i < sb_count(f->entity->subEntities); i++) {
            Entity_Destroy(f->entity->subEntities[i]);
        }

        // Init buffer...
        cb_init(f->stateHistory, MAX_REWIND);
        // Push empty...
        cb_push(f->stateHistory, (FighterState) {});
        // get last...
        FighterState* fs = &cb_last(f->stateHistory);
        // Fill...
        fs->jumps   = f->maxJumps;
        fs->mana    = f->maxMana;
        fs->meter   = f->maxMeter;
        fs->stateFlags = 0;
        fs->tempGravity = (Vec2I) {6969, 6969};
        // Done with fs.

        // Init buffer...
        cb_init(f->entity->history, MAX_REWIND);
        // Push empty...
        cb_push(f->entity->history, *EntityState_Create());
        // Get last...
        EntityState* es = &cb_last(f->entity->history);
        // Fill...
        es->currentHealth = f->maxHealth;
        es->valid = true;
        es->velocity = VEC2_ZERO;
        es->facingRight = i == 0;
        es->currentHealth = f->maxHealth;
        es->frame = 0;
        es->subframe = 0;
        // Done with es.
        
        Fighter_StartActionIndex(p->pointCharacter, 0, -1);
    }

    cb_last(currentMatch.players[0].pointCharacter->entity->history).position.x = LEFT_SPAWN;
    cb_last(currentMatch.players[0].pointCharacter->entity->history).position.y = FLOOR_OFFSET;
    cb_last(currentMatch.players[1].pointCharacter->entity->history).position.x = RIGHT_SPAWN;
    cb_last(currentMatch.players[1].pointCharacter->entity->history).position.y = FLOOR_OFFSET;

}

// Texture mmbg_texture;


int main(int argc, char* args[]) {

    if (!CC_INIT()) { puts("Failed to initialize!"); return -1; }

    // Main menu
    UiMenu* menu = MakeMainMenu();
    CC_Audio_Play_Music(musics[7]);

    int r,g,b;

    while(!WindowShouldClose()) {

        UpdateMusicStream(currentMusic);
        UpdateInput();
        BeginDrawing();

        ClearBackground(SKYBLUE);

        menu = Ui_GetTopFocus();
        if (menu) {
            if (menu->type == UI_TYPE_MATCH) {
                CC_Update();
                CC_Render();
            }
            else if (Menu_Input(menu, &p1Stick, &p2Stick)) {
                // See if we can find a Match...
                // UiMenu** stack = mqui_as_menu(Ui_GetFocusStack());
                UiMenu** stack = (UiMenu**)Ui_GetFocusStack();
                for (int i = 0; i < sb_count(stack); i++) {
                    if (stack[i]->type == UI_TYPE_MATCH)
                        CC_Render();
                }
                // Regardless, draw menu on top
                Menu_Draw(Ui_GetTopFocus(), (Vec2I) {0,0});
            }
        }

        DrawFPS(0,0);
        EndDrawing();
    }

    CloseWindow();
    // CC_CLOSE();

    printf("Exitting gracefully\n");

    return 0;
}
