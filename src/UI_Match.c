#include "CC_Consts.h"
#include "Ui_Match.h"
#include "stretchy_buffer.h"
#include "circular_buffer.h"
#include <raylib.h>

#define TIMERFONTSIZE 30
#define NAMEFONTSIZE 18

TTF_Font* clockFont;
TTF_Font* nameFont;
TTF_Font* debugFont;

Texture clockTexture;

RectI p1NameRect;
RectI p2NameRect;
Texture p1NameTexture;
Texture p2NameTexture;

RectI p1ActionRect;
RectI p2ActionRect;
Texture p1ActionTexture;
Texture p2ActionTexture;

RectI p1HealthRectUnder;
RectI p2HealthRectUnder;

RectI p1AdvantageRect;
RectI p2AdvantageRect;

RectI p1PortraitRect;
RectI p2PortraitRect;

RectI p1MeterRect;
RectI p2MeterRect;

RectI p1HealthRectOver;
RectI p2HealthRectOver;

SDL_Color winColor = {40, 184, 72, 255};
SDL_Color loseColor = {184, 40, 40, 255};

Texture p1wing;
Texture p2wing;
Texture p1nameplate;
Texture p2nameplate;

void Ui_Match_MakeRects(){
    p1HealthRectUnder.x = 50; 
    p1HealthRectUnder.y = 10;
    p1HealthRectUnder.w = 220;
    p1HealthRectUnder.h = 40;

    p2HealthRectUnder.x = VIRT_SCREEN_SIZE_X - 50;
    p2HealthRectUnder.y = 10;
    p2HealthRectUnder.w = -220;
    p2HealthRectUnder.h = 40;

    p1AdvantageRect.x = 270;
    p1AdvantageRect.y = 10;
    p1AdvantageRect.w = 10;
    p1AdvantageRect.h = 40;

    p2AdvantageRect.x = 360;
    p2AdvantageRect.y = 10;
    p2AdvantageRect.w = 10;
    p2AdvantageRect.h = 40;

    p1PortraitRect.x = 0;
    p1PortraitRect.y = 0;
    p1PortraitRect.w = 50;
    p1PortraitRect.h = 50;

    p2PortraitRect.x = VIRT_SCREEN_SIZE_X - 50;
    p2PortraitRect.y = 0;
    p2PortraitRect.w = 50;
    p2PortraitRect.h = 50;

    p1MeterRect.x = 10;
    p1MeterRect.y = VIRT_SCREEN_SIZE_Y - 10 - 20;
    p1MeterRect.w = 160;
    p1MeterRect.h = 20;

    p2MeterRect.x = VIRT_SCREEN_SIZE_X - 10 - 160;
    p2MeterRect.y = VIRT_SCREEN_SIZE_Y - 10 - 20;
    p2MeterRect.w = 160;
    p2MeterRect.h = 20;

    p1HealthRectOver.x = 50 + 220; 
    p1HealthRectOver.y = 10;
    p1HealthRectOver.h = 40;

    p2HealthRectOver.x =  VIRT_SCREEN_SIZE_X - 50 - 220;
    p2HealthRectOver.y = 10;
    p2HealthRectOver.h = 40;
}

void Ui_Match_MakeColors(){
}

bool Ui_Match_MakeFonts(){
    return true;
}
bool Ui_Match_MakeNames(char* p1Name, char* p2Name){
    return true;
}


bool Ui_Match_Init(Match* m){

    Fighter* f1 = m->players[0].pointCharacter;
    Fighter* f2 = m->players[1].pointCharacter;

    p1wing = LoadTexture("Graphics/Ui/wing-2-feather-left.png");
    p2wing = LoadTexture("Graphics/Ui/wing-2-feather-right.png");

    Image img;

    img = LoadImage("Graphics/Ui/NamePalettePlate.png");
    ImageColorReplace(&img, (Color){255,0,0,  255}, f1->palette[1]);
    ImageColorReplace(&img, (Color){0,255,0,  255}, f1->palette[2]);
    ImageColorReplace(&img, (Color){0,0,255,  255}, f1->palette[3]);
    ImageColorReplace(&img, (Color){255,0,255,255}, f1->palette[4]);
    p1nameplate = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("Graphics/Ui/NamePalettePlate.png");
    ImageColorReplace(&img, (Color){255,0,0,  255}, f1->palette[1]);
    ImageColorReplace(&img, (Color){0,255,0,  255}, f1->palette[2]);
    ImageColorReplace(&img, (Color){0,0,255,  255}, f1->palette[3]);
    ImageColorReplace(&img, (Color){255,0,255,255}, f1->palette[4]);
    ImageFlipHorizontal(&img);
    p2nameplate = LoadTextureFromImage(img);
    UnloadImage(img);

    Ui_Match_MakeColors();

    Ui_Match_MakeRects();

    if(!Ui_Match_MakeFonts())
        return false;

    Ui_Match_MakeNames(f1->name, f2->name);

    return true;
}



void DrawHUD(Match* m){

    uint time  = cb_last(m->history).currentTime / 60;
    Player* p1 = &m->players[0];
    Player* p2 = &m->players[1];

    short p1health = cb_last(p1->pointCharacter->stateHistory).health;
    short p2health = cb_last(p2->pointCharacter->stateHistory).health;

    const float widthFactor = 0.39f;
    const portraitSize = 48;

    const int maxWidth = VIRT_SCREEN_SIZE_X * widthFactor - portraitSize;
    const int height = 30;
    const Vector2 p1nameOffset = {68, 2};
    const Vector2 p2nameOffset = {59, 2};

    int width; 

    DrawCircle(VIRT_SCREEN_SIZE_X/2, 25, 25, WHITE);


    // Draw P1 healthbar
    DrawRectangle(portraitSize + 0, 10, maxWidth, height, GRAY);
    width = maxWidth * p1health / (float)(p1->pointCharacter->maxHealth);
    DrawRectangle(portraitSize + maxWidth-width, 10, width, height, WHITE);

    // Draw p2 Healthbar
    DrawRectangle(VIRT_SCREEN_SIZE_X * (1-widthFactor), 10, maxWidth, height, GRAY);
    width = maxWidth * p2health / (float)(p2->pointCharacter->maxHealth);
    DrawRectangle(VIRT_SCREEN_SIZE_X * (1-widthFactor), 10, width, height, WHITE);

    // Draw timer
    DrawTexture(p1wing, VIRT_SCREEN_SIZE_X/2 - p1wing.width/2, 6, p1health < p2health ? DARKGRAY : WHITE);
    DrawTexture(p2wing, VIRT_SCREEN_SIZE_X/2 - p2wing.width/2, 6, p2health < p1health ? DARKGRAY : WHITE);
    const char* times = TextFormat("%d", time);
    Vector2 v = MeasureTextEx(GetFontDefault(), times, TIMERFONTSIZE, 2);
    DrawText(times, VIRT_SCREEN_SIZE_X/2 - v.x/2, 25 - v.y/2, TIMERFONTSIZE, BLACK);

    // Draw Names
    DrawTexture(p1nameplate, 0, 55, WHITE);
    // DrawTexture(p2nameplate, VIRT_SCREEN_SIZE_X - p2nameplate.width, 55, WHITE);
    DrawText(p1->pointCharacter->name, 5+p1nameOffset.x, 55+p1nameOffset.y, NAMEFONTSIZE, WHITE);
    v = MeasureTextEx(GetFontDefault(), p2->pointCharacter->name, NAMEFONTSIZE, 2);
    DrawTexture(p2nameplate, VIRT_SCREEN_SIZE_X-p2nameplate.width, 55, WHITE);
    DrawText(p2->pointCharacter->name, VIRT_SCREEN_SIZE_X - v.x - 5 - p2nameOffset.x, 55 + p2nameOffset.y, NAMEFONTSIZE, WHITE);

    // #pragma region Portaits
    
    DrawTexture(p1->pointCharacter->portrait, 0, 0, WHITE);
    DrawTexture(p2->pointCharacter->portrait, VIRT_SCREEN_SIZE_X - p2->pointCharacter->portrait.width, 0, WHITE);
    


    // SDL_RenderCopyEx(ren, p1->pointCharacter->portrait, NULL, &p1PortraitRect, 0, 0, 0);
    // SDL_RenderCopyEx(ren, p2->pointCharacter->portrait, NULL, &p2PortraitRect, 0, 0, SDL_FLIP_HORIZONTAL);
    // #pragma endregion

    // #pragma region Super meters
    

    // SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

    // SDL_RenderFillRect(ren, &p1MeterRect);
    // SDL_RenderFillRect(ren, &p2MeterRect);


    // #pragma endregion

    // // TODO(#15): Meters in UI
    // #pragma region Magic meters

    // SDL_SetRenderDrawColor(ren, 129, 40, 184, 255);
    // // SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    // SDL_RenderDrawLine(ren, 70, VIRT_SCREEN_SIZE_Y - 10 - 20, 170, VIRT_SCREEN_SIZE_Y - 10); // Diag
    // SDL_RenderDrawLine(ren, 95, VIRT_SCREEN_SIZE_Y - 10 - 20, 170, VIRT_SCREEN_SIZE_Y - 15); // Diag
    // SDL_RenderDrawLine(ren, 120, VIRT_SCREEN_SIZE_Y - 10 - 20, 170, VIRT_SCREEN_SIZE_Y - 20); // Diag
    // SDL_RenderDrawLine(ren, 145, VIRT_SCREEN_SIZE_Y - 10 - 20, 170, VIRT_SCREEN_SIZE_Y - 25); // Diag
    // SDL_RenderDrawLine(ren, 170, VIRT_SCREEN_SIZE_Y - 10, 170, VIRT_SCREEN_SIZE_Y - 30); // Vert
    // SDL_RenderDrawLine(ren, 170, VIRT_SCREEN_SIZE_Y - 30, 70, VIRT_SCREEN_SIZE_Y - 10 - 20); // Horz

    // #pragma endregion

    // #pragma region Color palette, name, winmarks, etc
    // // TODO(#16): Color palette in UI
    // // SDL_RenderCopy(ren, p1NameTexture, NULL, &p1NameRect);
    // // SDL_RenderCopy(ren, p2NameTexture, NULL, &p2NameRect);
    // // TODO(#17): Winmarks, etc in UI
    
    
    // // SDL_SetRenderDrawColor(ren, winColor.r, winColor.g, winColor.b, 255);
    // // P1 wins
    // for(int i = 0; i < m->currentRounds[0]; i++){
    //     RectI r;

    //     r.x =VIRT_SCREEN_SIZE_X/2 - 50 - i*20;
    //     r.y = 60;
    //     r.w = 10;
    //     r.h = 30;
    //     // SDL_RenderFillRect(ren, &r);
    // }

    // // P2 wins
    // for(int i = 0; i < m->currentRounds[1]; i++){
    //     // RectI r;
    //     r.x = VIRT_SCREEN_SIZE_X/2 + 40 + i*20;
    //     r.y = 60;
    //     r.w = 10;
    //     r.h = 30;
    //     // SDL_RenderFillRect(ren, &r);
    // }

    // // SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    // // P1 empty wins
    // for(int i = 0; i < m->maxRounds; i++){
    //     RectI r;

    //     r.x =VIRT_SCREEN_SIZE_X/2 - 50 - i*20;
    //     r.y = 60;
    //     r.w = 10;
    //     r.h = 30;
    //     // SDL_RenderDrawRect(ren, &r);
    // }

    // // P2 empty wins
    // for(int i = 0; i < m->maxRounds; i++){
    //     // RectI r;
    //     r.x = VIRT_SCREEN_SIZE_X/2 + 40 + i*20;
    //     r.y = 60;
    //     r.w = 10;
    //     r.h = 30;
    //     // SDL_RenderDrawRect(ren, &r);
    // }

    #pragma endregion
    
    #pragma region Timer

    // time/=60;
    
    // Pad
    // "12\0"
    // char timeStr[3];

    // if (time > 9)
    //     snprintf(timeStr, 3, "%d", (int)time);
    // else
    //     snprintf(timeStr, 3, "0%d", (int)time);


    // No memeory leaks allowed!
    // if(clockTexture){
    //     SDL_FreeSurface(clockTexture);
    // }
    // if(clockTexture){
        // Don't do this. This isn't real.
        // SDL_FreeTexture(clockTexture);
        // SDL_DestroyTexture(clockTexture);
    // }
    // clockTexture = TTF_RenderText_Blended(clockFont, timeStr, WHITE);
    // clockTexture = SDL_CreateTextureFromSurface(ren, clockTexture);

    // RectI timerRect = {VIRT_SCREEN_SIZE_X/2 - clockTexture->w/2, 0, clockTexture->w, clockTexture->h};
    // RectI timerBgRect = {VIRT_SCREEN_SIZE_X/2 - 80/2, 0, 80, 80};
    
    // SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    // SDL_RenderFillRect(ren, &timerBgRect);

    // SDL_RenderCopy(ren, clockTexture, NULL, &timerRect);
    #pragma endregion

    #pragma region Optional
    if(true)
        DrawHUD_Training(m);
    #pragma endregion
}

void DrawHUD_Training(Match* m){
    // TODO(#18): P2 training gui
    #pragma region Action names
    // if(p1ActionSurface)
    //     SDL_FreeSurface(p1ActionSurface);
    
    // if(p1ActionTexture)
    //     SDL_DestroyTexture(p1ActionTexture);


    // // Display "ID: Action Name"
    // // p1
    // {
    // char* name = cb_last(m->players[0].pointCharacter->stateHistory).action->name;
    // char num[strlen(name)+5+2+2];
    // char state = Fighter_Air(m->players[0].pointCharacter) ? 'a' : 'g';


    // int state2 = cb_last(m->players[0].pointCharacter->stateHistory).stateFlags;
    // sprintf(num, "%d %c %d: %s", state2, state, cb_last(m->players[0].pointCharacter->stateHistory).action->index, name);
    
    // p1ActionSurface = TTF_RenderText_Blended(debugFont, num, WHITE);

    // p1ActionRect.x = 10;
    // p1ActionRect.y = 90;

    // p1ActionRect.w = p1ActionSurface->w;
    // p1ActionRect.h = p1ActionSurface->h;

    // p1ActionTexture = SDL_CreateTextureFromSurface(ren, p1ActionSurface);

    // SDL_RenderCopy(ren, p1ActionTexture, NULL, &p1ActionRect);
    // }

    // // p2
    // {
    // char* name = cb_last(m->players[1].pointCharacter->stateHistory).action->name;
    // char num[strlen(name)+5];
    // sprintf(num, "%d: %s", cb_last(m->players[1].pointCharacter->stateHistory).action->index, name);
    
    // p2ActionSurface = TTF_RenderText_Blended(debugFont, num, WHITE);

    // p2ActionRect.x = VIRT_SCREEN_SIZE_X - 10 - p2ActionSurface->w;
    // p2ActionRect.y = 90;

    // p2ActionRect.w = p2ActionSurface->w;
    // p2ActionRect.h = p2ActionSurface->h;

    // p2ActionTexture = SDL_CreateTextureFromSurface(ren, p2ActionSurface);

    // SDL_RenderCopy(ren, p2ActionTexture, NULL, &p2ActionRect);
    // }

    // For reference
    /*
    SDL_Surface* p1NameSurface = TTF_RenderText_Blended(nameFont, p1Name, WHITE);
    
    p1NameRect.x = 10;
    p1NameRect.y = 45;
    p1NameRect.w = p1NameSurface->w;
    p1NameRect.h = p1NameSurface->h;
    
    p1NameTexture = SDL_CreateTextureFromSurface(ren, p1NameSurface);
    */
   #pragma endregion
}