#include "Ui_Label.h"
#include "stretchy_buffer.h"
#include <string.h>
#include "SDL_ttf.h"
#include "SDL2/SDL.h"


TTF_Font* mmlabelfont = NULL;
const SDL_Color mmlWHITE = (SDL_Color){255, 255, 255, 255};

UiLabel* Label_New(SDL_Renderer* gRenderer, UiMenu* menu, char* text){
    if (!mmlabelfont){
        mmlabelfont = TTF_OpenFont("Graphics/Fonts/Recursive-Bold.ttf", 32);
        
    }

    UiLabel* label = malloc(sizeof(UiLabel));
    label->hcentered = 1;
    label->up = label->down = label->left = label->right = NULL;

    SDL_Surface* tempsurf;

    label->string_text = (char*)malloc(strlen(text) * sizeof(char) + 1);
    label->surface_text = TTF_RenderText_Blended(mmlabelfont, text, mmlWHITE);
    label->texture_text = SDL_CreateTextureFromSurface(gRenderer, label->surface_text);
    strcpy(label->string_text, text);
    label->type = UI_TYPE_LABEL;
    sb_push(menu->elements, (UiElement*)label);

    return label;
}

void Label_Draw(SDL_Renderer* gRenderer, UiMenu* menu, UiLabel* label){
    SDL_Rect rect;

    rect = (SDL_Rect) {label->position.x, label->position.y, label->surface_text->w, label->surface_text->h};
    SDL_RenderCopy(gRenderer, label->texture_text, NULL, &rect);
}

void Label_Free(UiLabel* l){
    free(l->string_text);
    free(l);
}

Vector2 Label_Size(SDL_Renderer* ren, UiLabel* label){
    return (Vector2) {label->surface_text->w, label->surface_text->h};
}