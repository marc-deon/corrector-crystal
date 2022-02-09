
// TODO(#19): Free stuff (animations, motions, etc) when done.

#include <assert.h>
#include <raylib.h>

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

bool stopGame = 0;

RectI camera;
Match currentMatch;

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

void Training_SaveState(){
    isStateSet = 1;
    trainingState.currentTime = cb_last(currentMatch.history).currentTime;
    trainingState.hitStop     = cb_last(currentMatch.history).hitStop;
    trainingState.fighterStates[0] = cb_last(currentMatch.players[0].pointCharacter->stateHistory);
    trainingState.fighterStates[1] = cb_last(currentMatch.players[1].pointCharacter->stateHistory);
    // Do puppets
    // Do projectiles
}

void Training_LoadState(){
    if(!isStateSet)
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

bool CC_INIT(){
    bool success = true;

    InitWindow(VIRT_SCREEN_SIZE_X, VIRT_SCREEN_SIZE_Y, "Corrector Raylib");
    SetTargetFPS(60);
    // TODO:
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    // SetWindowState(FLAG_FULLSCREEN_MODE);
    
    // if (SDL_Init(SDL_INIT_VIDEO < 0)){
    //     printf("SDL could not initialize! SDL ERROR: %S\n", SDL_GetError());
    //     success = false;
    // }
    // else{
    //     gWindow = SDL_CreateWindow("original content by tim",
    //         SDL_WINDOWPOS_UNDEFINED,
    //         SDL_WINDOWPOS_UNDEFINED,
    //         VIRT_SCREEN_SIZE_X,
    //         VIRT_SCREEN_SIZE_Y,
    //         SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE //| SDL_WINDOW_FULLSCREEN_DESKTOP
    //     );
    //     if (gWindow == NULL){
    //         printf("Window could not be created! SDL ERROR: %S\n", SDL_GetError());
    //         success = false;
    //         return success;
    //     }

    //     // TODO(#20): Find a way to force 60fps, instead of doing vsync
    //     gRenderer = SDL_CreateRenderer(
    //         gWindow, 
    //         -1, 
    //         SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    //      );
    //     SDL_RenderSetLogicalSize(VIRT_SCREEN_SIZE_X, VIRT_SCREEN_SIZE_Y);
    //     SDL_SetRenderDrawBlendMode(SDL_BLENDMODE_BLEND);
    //     TTF_Init();
    // }

    // if (SDL_Init(SDL_INIT_AUDIO < 0)){
    //     printf("SDL could not initialize! SDL ERROR: %S\n", SDL_GetError());
    // }
    // CC_Audio_Init();

    // TODO(#21): Move to another file?
    // previousKeyStates = (uint8_t*) calloc(285, sizeof(uint8_t));


    return success;
}

void CC_CLOSE(){
    // Destroy Window

    free(previousKeyStates);

    // Quit SDL subsystems
}

void Game_PlayerInit(){
    currentMatch.players[0].stick =          &p1Stick;
    currentMatch.players[0].pointCharacter = Fighter_Create();
    currentMatch.players[1].stick =          &p2Stick;
    currentMatch.players[1].pointCharacter = Fighter_Create();
}


void Game_DataInit(){
    currentMatch.players[0].pointCharacter->opponent = currentMatch.players[1].pointCharacter;
    currentMatch.players[1].pointCharacter->opponent = currentMatch.players[0].pointCharacter;
    for(int i = 0; i < 2; i++){
        Player* p = &currentMatch.players[i];
        Game_Data_InitActions(p);

        p->pointCharacter->owner = i+1;
    }
    currentMatch.players[0].pointCharacter->opponent = currentMatch.players[1].pointCharacter;
    currentMatch.players[1].pointCharacter->opponent = currentMatch.players[0].pointCharacter;
    ResetRound();
    currentMatch.players[0].pointCharacter->opponent = currentMatch.players[1].pointCharacter;
    currentMatch.players[1].pointCharacter->opponent = currentMatch.players[0].pointCharacter;

}


Match* GameInit(int p1CharaIndex, int p2CharaIndex){
    currentMatch = Match_Init();

    Game_PlayerInit();

    Game_SpriteInit(p1CharaIndex, p2CharaIndex);

    Game_DataInit(p1CharaIndex, p2CharaIndex);

    Ui_Match_Init(&currentMatch);
    return &currentMatch;
}


void CC_Render(){
    
     Rectangle r = {
         camera.x * UNIT_TO_PIX,
         camera.y * UNIT_TO_PIX,
         camera.w,
         camera.h,
     };

    DrawTextureRec(background, r, (Vector2){0,0}, WHITE);

    // Draw sprites
    for(int i = 0; i < 2; i++){
        Player* p = &currentMatch.players[i];
        FighterState* fs = &cb_last(p->pointCharacter->stateHistory);
        Fighter_DrawSprite(p->pointCharacter, camera);
        
    }

    // Draw debug components
    // We keep these as two loops because we want everything to be drawn on top of sprites, regardless of player
    for(int i = 0; i < 2; i++){
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

int min(int x, int y){
    return x < y ? x : y;
}

int max(int x, int y){
    return x > y ? x : y;
}

void CC_ProcessCamera(){
    camera.x = 0; 
    for (int i = 0; i < 2; i++)
        camera.x += cb_last(currentMatch.players[i].pointCharacter->stateHistory).position.x;
    camera.x -= camera.w * PIX_TO_UNIT;
    camera.x /= 2;

    camera.y = STAGE_H; 
    for (int i = 0; i < 2; i++)
        camera.y = min(camera.y, cb_last(currentMatch.players[i].pointCharacter->stateHistory).position.y);
    camera.y /= 2;
    camera.y += 2000;

    camera.x = max(0, camera.x);
    if (camera.x + camera.w * PIX_TO_UNIT > STAGE_W)
        camera.x = STAGE_W - camera.w * PIX_TO_UNIT;
    
    camera.y = max(0, camera.y);
    if (camera.y + camera.h * PIX_TO_UNIT > STAGE_H)
        camera.y = STAGE_H - camera.h * PIX_TO_UNIT;

}

int CC_ProcessInput(){


    if(Stick_IsButtonJustDown(&p1Stick, STICK_START) || Stick_IsButtonJustDown(&p2Stick, STICK_START)){
        printf("paused\n");
        UiMenu* menu = MakePauseMenu();
        currentMatch.paused = true;
        return 0;
    }

    if (QF_IsKeyJustDown(KEY_F5)){
        Training_SaveState();
    }
    
    else if (QF_IsKeyJustDown(KEY_F6)){
        Training_LoadState();
    }

    for(int i = 0; i < 2; i++){
        Player* p = &currentMatch.players[i];
        StickState* ss = Stick_GetState(p->stick);

        assert(ss && "Stick state must not be null");

        // Look for motions
        if(! cb_last(currentMatch.history).hitStop){
            int i = Player_GetFirstPattern(p);
            if(i != -1){
                Action* a = p->pointCharacter->motions[i]->action;

                // Start new actions
                if(a != cb_last(p->pointCharacter->stateHistory).action){
                    Fighter_StartAction(p->pointCharacter, a, 0);
                }
            }
        }
    }

    return 1;
}

void CC_RewindFrames(int x){
    // What histories do we have?
    // P1 history
    // P2 history
    // Match history
}


bool CC_ProcessFrame(){
    if (cb_last(currentMatch.history).hitStop){
        cb_last(currentMatch.history).hitStop--;
        return true;
    }
    else{
        cb_last(currentMatch.history).currentTime--;
    }

    CC_ProcessCamera();

    for(int i = 0; i < 2; i++){
        Player* p = &currentMatch.players[i];
    }

    // Advance action frames, auto-advance action if neccesary
    for(int i = 0; i < 2; i++){
        Player* p = &currentMatch.players[i];
        FighterState* state = &cb_last(p->pointCharacter->stateHistory);
        state->frame++;

        if(state->frame == state->action->mustLinkAfter){
            if(state->stateFlags & FF_HARD_KD){
                Fighter_StartAction(p->pointCharacter, state->action, 0);
            }
            else{
                Fighter_StartAction(p->pointCharacter, state->action->linksTo[0], 0);
            }
        }
        else if(state->action->mustLinkAfter >= 0 && state->frame > state->action->mustLinkAfter){
            printf("%s: %d >%d\n", state->action->name, state->frame, state->action->mustLinkAfter);
            printf("This shouldn't happen. %s (%d)\n", __FILE__, __LINE__);
            return false;
        }
    }

    // unshove
    Fighter* f1 = currentMatch.players[0].pointCharacter;
    Fighter* f2 = currentMatch.players[1].pointCharacter;

    if (!(cb_last(f1->stateHistory).action->phase) && !(cb_last(f2->stateHistory).action->phase)){

        int p1delta = 0;
        int p2delta = 0;
        
        RectI rect1 = Fighter_OnRight(f1) ?
            Rect_Flip(cb_last(f1->stateHistory).action->shovebox.rect)
            : cb_last(f1->stateHistory).action->shovebox.rect;
        rect1.pos = Vec2_Add(rect1.pos, cb_last(f1->stateHistory).position);

        RectI rect2 = Fighter_OnRight(f2) ?
            Rect_Flip(cb_last(f2->stateHistory).action->shovebox.rect)
            : cb_last(f2->stateHistory).action->shovebox.rect;
        rect2.pos = Vec2_Add(rect2.pos, cb_last(f2->stateHistory).position);

        RectI r1 = rect1;
        RectI r2 = rect2;
        int dir;
        while(dir = Rect_ShortestEscape(r1, r2))
        {
            p1delta += Fighter_OnRight(f1) ? 1 : -1;
            p2delta += Fighter_OnRight(f2) ? 1 : -1;

            r1 = rect1; r1.pos.x += p1delta;
            r2 = rect2; r2.pos.x += p2delta;
        }

        cb_last(f1->stateHistory).position.x = cb_last(f1->stateHistory).position.x + (1 * p1delta);
        cb_last(f2->stateHistory).position.x = cb_last(f2->stateHistory).position.x + (1 * p2delta);

        Fighter_CheckWalls(f1);
        Fighter_CheckWalls(f2);
    }
    
    // Update positions
    for(int i = 0; i < 2; i++){
        Player* p = &currentMatch.players[i];
        Player* op = &currentMatch.players[(i+1)%2];
        if(Fighter_Air(p->pointCharacter)) {
            Fighter_MoveAir(p->pointCharacter, &p->stick->buffer[0], camera);
        }
        else{
            Fighter_MoveGround(p->pointCharacter, &p->stick->buffer[0], &op->stick->buffer[0], camera);
        }
        Fighter_UpdateSubstate(p->pointCharacter);
        Fighter_CheckWalls(p->pointCharacter);
    }

    // Update hitboxes
    for(int i = 0; i < 2; i++){
        Player* p = &currentMatch.players[i];

        Hitbox** hitboxes = cb_last(p->pointCharacter->stateHistory).action->hitboxes;

        for(int hit = 0; hit < sb_count(hitboxes); hit++){
            Hitbox_UpdateTimer(hitboxes[hit]);
        }
    }
    ////////
    // Check for overlaps in hitboxes
    Action* p1GotHitBy = NULL;
    Action* p2GotHitBy = NULL;
    for(int i = 0; i < 2; i++){
        Player* p = &currentMatch.players[i];

        FighterState* fs = &cb_last(p->pointCharacter->stateHistory);
        
        // Flag to exit double-loop whenever active hitbox-hurtbox collision happens
        bool madeContact = false;

        // Iterate over every hitbox, or until we make contact
        Hitbox** hitboxes = fs->action->hitboxes;
        for(int hit = 0; (!madeContact) && (hit < sb_count(hitboxes)); hit++){
            if (hitboxes[hit]->active != HB_ACTIVE){
                continue;
            }

            // Iterate over every opponent hurtbox, or until we make contact
            Hurtbox** hurtboxes = cb_last(p->pointCharacter->opponent->stateHistory).action->hurtboxes;
            for(int hurt = 0; (!madeContact) && (hurt < sb_count(hurtboxes)); hurt++){
                // Adjust our hitbox according to our position and side
                RectI r1 = hitboxes[hit]->rect;
                if(Fighter_OnRight(p->pointCharacter))
                    r1 = Rect_Flip(r1);
                r1.pos.x += fs->position.x;
                r1.pos.y += fs->position.y;

                // Adjust opponent's hurtbox according to their position and side
                RectI r2 = hurtboxes[hurt]->rect;
                if(Fighter_OnRight(p->pointCharacter->opponent))
                    r2 = Rect_Flip(r2);
                r2.pos.x += cb_last(p->pointCharacter->opponent->stateHistory).position.x;
                r2.pos.y += cb_last(p->pointCharacter->opponent->stateHistory).position.y;

                if(Rect_Overlap(r1, r2)){
                    madeContact = true;

                    // Save the action so we can apply hitstun later
                    if (i == 0){
                        p2GotHitBy = fs->action;
                    }
                    else{
                        p1GotHitBy = fs->action;
                    }

                    // Disable our action's hitboxes so we don't hit more than once
                    // (use multiple actions for multi hit moves)
                    for(int k = 0; k < sb_count(hitboxes); k++){
                        hitboxes[k]->active = HB_DISABLED;
                    }
                    break;
                }
            }
        }
    }
    
    Fighter_Damage(currentMatch.players[0].pointCharacter, p1GotHitBy, Fighter_OnRight(currentMatch.players[0].pointCharacter));
    Fighter_Damage(currentMatch.players[1].pointCharacter, p2GotHitBy, Fighter_OnRight(currentMatch.players[1].pointCharacter));

    // Make copy of current, store the original, and edit new on the next loop
    for(int i = 0; i < 2; i++){
        Player* p = &currentMatch.players[i];
        FighterState* fs = &cb_last(p->pointCharacter->stateHistory);
        cb_push(p->pointCharacter->stateHistory, cb_last(p->pointCharacter->stateHistory));
        FighterState* fs2 = &cb_last(p->pointCharacter->stateHistory);
    }

    int hitStop = 0;

    if(p1GotHitBy && p2GotHitBy){
        hitStop = max(p1GotHitBy->hitStop, p2GotHitBy->hitStop);
    }
    else if (p1GotHitBy){
        hitStop = p1GotHitBy->hitStop;
    }
    else if (p2GotHitBy){
        hitStop = p2GotHitBy->hitStop;
    }

    if(hitStop || cb_last(currentMatch.history).hitStop)
    cb_last(currentMatch.history).hitStop = max(hitStop, cb_last(currentMatch.history).hitStop);

 
    // Check for win
    int result = Match_CheckRoundWin(&currentMatch);
    if(result == 1 || result == 2){
        currentMatch.currentRounds[result-1]++;
    }
    else if(result == 3){
        // Tie
        printf("tie\n");
        currentMatch.currentRounds[0]++;
        if (currentMatch.currentRounds[0] >= currentMatch.maxRounds)
            currentMatch.currentRounds[0] = currentMatch.maxRounds - 1;
        currentMatch.currentRounds[1]++;
        if (currentMatch.currentRounds[1] >= currentMatch.maxRounds)
            currentMatch.currentRounds[1] = currentMatch.maxRounds - 1;
        
    }

    if(result){
        ResetRound();
    }
}

bool CC_Update(){

    // Update stick states
    int success = CC_ProcessInput();

    if(success == -1)
        return false;

    if (success > 0)
        CC_ProcessFrame();

    return true;
}


void ResetRound(){
    puts("Resetting round");
    camera = (RectI) {0, STAGE_H - VIRT_SCREEN_SIZE_Y, VIRT_SCREEN_SIZE_X, VIRT_SCREEN_SIZE_Y};
    cb_last(currentMatch.history).currentTime = currentMatch.maxTime;
    for(int i = 0; i < 2; i++){
        Player* p = &(currentMatch.players[i]);
        FighterState* fs;
        p->pointCharacter->stateHistory = cb_init(p->pointCharacter->stateHistory, MAX_REWIND);
        cb_push(p->pointCharacter->stateHistory, (FighterState){});
        fs = &cb_last(p->pointCharacter->stateHistory);
        fs->stateFlags              = 0;
        fs->tempGravity.x           = 6969;
        fs->tempGravity.y           = 6969;
        fs->subframe                = 0;
        fs->animation = p->pointCharacter->animations[0];
        fs->animation->currentFrame = 0;

        fs->jumps   = p->pointCharacter->maxJumps;
        fs->health  = p->pointCharacter->maxHealth;
        fs->mana    = p->pointCharacter->maxMana;
        fs->meter   = p->pointCharacter->maxMeter;
        fs->action  = p->pointCharacter->actions[0];

        
        fs->health = p->pointCharacter->maxHealth;       
        Fighter_StartActionIndex(p->pointCharacter, 0, -1);
    }

    cb_last(currentMatch.players[0].pointCharacter->stateHistory).position.x = LEFT_SPAWN;
    cb_last(currentMatch.players[0].pointCharacter->stateHistory).position.y = FLOOR_OFFSET;
    cb_last(currentMatch.players[1].pointCharacter->stateHistory).position.x = RIGHT_SPAWN;
    cb_last(currentMatch.players[1].pointCharacter->stateHistory).position.y = FLOOR_OFFSET;

}

Texture mmbg_texture;

int main(int argc, char* args[]){

    if(!CC_INIT()){ puts("Failed to initialize!"); return -1; }
    
    // Main menu
    UiMenu* menu = MakeMainMenu();
    // CC_Audio_Play_Music(musics[2]);

    int r,g,b;

    while(!WindowShouldClose()){
        UpdateInput();
        BeginDrawing();

        ClearBackground(SKYBLUE);

        menu = Ui_GetTopFocus();
        if(menu){
            if (menu->type == UI_TYPE_MATCH){
                CC_Update();
                CC_Render();
            }
            else if(Menu_Input(menu, &p1Stick, &p2Stick)){
                // See if we can find a Match...
                UiMenu** stack = Ui_GetFocusStack();
                for(int i = 0; i < sb_count(stack); i++){
                    if(stack[i]->type == UI_TYPE_MATCH)
                        CC_Render();
                }
                // Regardless, draw menu on top
                Menu_Draw(Ui_GetTopFocus());
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
