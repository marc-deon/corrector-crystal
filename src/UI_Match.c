#include "CC_Consts.h"
#include "Ui_Match.h"
#include "stretchy_buffer.h"
#include "circular_buffer.h"
#include <raylib.h>

#define TIMERFONTSIZE 30
#define NAMEFONTSIZE 18


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

Texture p1wing;
Texture p2wing;
Texture p1nameplate;
Texture p2nameplate;

int shaderPalLoc;

void Ui_Match_MakeRects() {
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

void Ui_Match_MakeColors() {
}

bool Ui_Match_MakeFonts() {
    return true;
}
bool Ui_Match_MakeNames(char* p1Name, char* p2Name) {
    return true;
}


bool Ui_Match_Init(Match* m) {

    Fighter* f1 = m->players[0].pointCharacter;
    Fighter* f2 = m->players[1].pointCharacter;

    // shaderPalLoc = GetShaderLocation(fighterShader, "palette");
    // SetShaderValueTexture(fighterShader, shaderPalLoc, p1->pointCharacter->paletteTexture);
    // SetShaderValueTexture(fighterShader, shaderPalLoc, p2->pointCharacter->paletteTexture);

    p1wing = LoadTexture("Graphics/Ui/wing-2-feather-left.png");
    p2wing = LoadTexture("Graphics/Ui/wing-2-feather-right.png");

    Image img;

    img = LoadImage("Graphics/Ui/NamePalettePlate.png");
    ImageColorReplace(&img, (Color) {255,0,0,  255}, f1->entity->palette[1]);
    ImageColorReplace(&img, (Color) {0,255,0,  255}, f1->entity->palette[2]);
    ImageColorReplace(&img, (Color) {0,0,255,  255}, f1->entity->palette[3]);
    ImageColorReplace(&img, (Color) {255,0,255,255}, f1->entity->palette[4]);
    p1nameplate = LoadTextureFromImage(img);
    UnloadImage(img);

    img = LoadImage("Graphics/Ui/NamePalettePlate.png");
    ImageColorReplace(&img, (Color) {255,0,0,  255}, f2->entity->palette[1]);
    ImageColorReplace(&img, (Color) {0,255,0,  255}, f2->entity->palette[2]);
    ImageColorReplace(&img, (Color) {0,0,255,  255}, f2->entity->palette[3]);
    ImageColorReplace(&img, (Color) {255,0,255,255}, f2->entity->palette[4]);
    ImageFlipHorizontal(&img);
    p2nameplate = LoadTextureFromImage(img);
    UnloadImage(img);

    Ui_Match_MakeColors();

    Ui_Match_MakeRects();

    if(!Ui_Match_MakeFonts())
        return false;

    Ui_Match_MakeNames(f1->entity->name, f2->entity->name);

    return true;
}



void DrawHUD(Match* m) {

    uint time  = cb_last(m->history).currentTime / 60;
    Player* p1 = &m->players[0];
    Player* p2 = &m->players[1];

    short p1health = cb_last(p1->pointCharacter->entity->history).currentHealth;
    short p2health = cb_last(p2->pointCharacter->entity->history).currentHealth;

    const float widthFactor = 0.39f;
    const int portraitSize = 48;

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
    // Draw the plate twice; once without shader for the border and background, then again for the colors
    DrawTexture(p1nameplate, 0, 55, WHITE);
    BeginShaderMode(p1->pointCharacter->entity->shader);
    DrawTexture(p1nameplate, 0, 55, WHITE);
    EndShaderMode();
    DrawText(p1->pointCharacter->entity->name, 5+p1nameOffset.x, 55+p1nameOffset.y, NAMEFONTSIZE, WHITE);
    v = MeasureTextEx(GetFontDefault(), p2->pointCharacter->entity->name, NAMEFONTSIZE, 2);
    DrawTexture(p2nameplate, VIRT_SCREEN_SIZE_X-p2nameplate.width, 55, WHITE);
    BeginShaderMode(p2->pointCharacter->entity->shader);
    DrawTexture(p2nameplate, VIRT_SCREEN_SIZE_X-p2nameplate.width, 55, WHITE);
    EndShaderMode();
    DrawText(p2->pointCharacter->entity->name, VIRT_SCREEN_SIZE_X - v.x - 5 - p2nameOffset.x, 55 + p2nameOffset.y, NAMEFONTSIZE, WHITE);

    // #pragma region Portaits
    
    DrawTexture(p1->pointCharacter->portrait, 0, 0, WHITE);
    DrawTexture(p2->pointCharacter->portrait, VIRT_SCREEN_SIZE_X - p2->pointCharacter->portrait.width, 0, WHITE);

    #pragma endregion

    #pragma region Optional
    if(true)
        DrawHUD_Training(m);
    #pragma endregion
}

void DrawHUD_Training(Match* m) {
    // TODO(#18): P2 training gui
    #pragma region Action names
    
   #pragma endregion
}