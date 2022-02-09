#include "CC_Consts.h"
#include "Ui_Match.h"
#include "stretchy_buffer.h"
#include "circular_buffer.h"

// These must be declared in the C file, for reasons.

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

    // nameFont = TTF_OpenFont("Graphics/Fonts/BadScript-Regular.ttf", 20);
    // if(!nameFont)
    // {
    //     printf("Invalid font! %s (%d)\n", __FILE__, __LINE__);
    //     return false;
    // }
    // clockFont = TTF_OpenFont("Graphics/Fonts/FrederickatheGreat-Regular.ttf", 80);
    // if(!clockFont){
    //     printf("Invalid font! %s (%d)\n", __FILE__, __LINE__);
    //     return false;
    // }
    // debugFont = TTF_OpenFont("Graphics/Fonts/Recursive-Bold.ttf", 20);
    // if(!debugFont){
    //     printf("Invalid font! %s (%d)\n", __FILE__, __LINE__);
    //     return false;
    // }
    return true;

}
bool Ui_Match_MakeNames(char* p1Name, char* p2Name){
    

    // SDL_Surface* p1NameSurface = TTF_RenderText_Blended(nameFont, p1Name, WHITE);
    

    p1NameRect.x = 10;
    p1NameRect.y = 45;
    // p1NameRect.w = p1NameSurface->w;
    // p1NameRect.h = p1NameSurface->h;

    // SDL_Surface* p2NameSurface = TTF_RenderText_Blended(nameFont, p2Name, WHITE);
    

    // p2NameRect.x = VIRT_SCREEN_SIZE_X - p2NameSurface->w - 10;
    p2NameRect.y = 45;
    // p2NameRect.w = p2NameSurface->w;
    // p2NameRect.h = p2NameSurface->h;

    
    // // p1NameTexture = SDL_CreateTextureFromSurface(ren, p1NameSurface);
    // // p2NameTexture = SDL_CreateTextureFromSurface(ren, p2NameSurface);
    
    return true;
}


bool Ui_Match_Init(Match* m){

    Fighter* f1 = m->players[0].pointCharacter;
    Fighter* f2 = m->players[1].pointCharacter;

    Ui_Match_MakeColors();

    Ui_Match_MakeRects();

    if(!Ui_Match_MakeFonts())
        return false;

    Ui_Match_MakeNames(f1->name, f2->name);

    return true;
}

void DrawHUD(Match* m){

    // uint time  = cb_last(m->history).currentTime;
    // Player* p1 = &m->players[0];
    // Player* p2 = &m->players[1];

    // #pragma region Health bars
    // p1HealthRectOver.w = 220 * -Fighter_HealthPercent(p1->pointCharacter);
    // p2HealthRectOver.w = 220 * Fighter_HealthPercent(p2->pointCharacter);
    
    // SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    // SDL_RenderFillRect(ren, &p1HealthRectUnder);
    // SDL_RenderFillRect(ren, &p2HealthRectUnder);

    // SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    // SDL_RenderFillRect(ren, &p1HealthRectOver);
    // SDL_RenderFillRect(ren, &p2HealthRectOver);
    // #pragma endregion

    // #pragma region Health advantage

    // if (Fighter_HealthPercent(p1->pointCharacter) >= Fighter_HealthPercent(p2->pointCharacter))
    //     SDL_SetRenderDrawColor(ren, winColor.r, winColor.g, winColor.b, 255);
    // else
    //     SDL_SetRenderDrawColor(ren, loseColor.r, loseColor.g, loseColor.b, 255);

    // SDL_RenderFillRect(ren, &p1AdvantageRect);
    

    // if (Fighter_HealthPercent(p1->pointCharacter) <= Fighter_HealthPercent(p2->pointCharacter))
    //     SDL_SetRenderDrawColor(ren, winColor.r, winColor.g, winColor.b, 255);
    // else
    //     SDL_SetRenderDrawColor(ren, loseColor.r, loseColor.g, loseColor.b, 255);

    // SDL_RenderFillRect(ren, &p2AdvantageRect);
    // #pragma endregion

    // #pragma region Portaits
    
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