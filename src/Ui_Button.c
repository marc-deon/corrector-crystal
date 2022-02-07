#include "Ui_Button.h"
#include "stretchy_buffer.h"
#include <string.h>
#include "SDL_ttf.h"
#include "SDL2/SDL.h"


TTF_Font* mmbuttfont = NULL;
const SDL_Color mmb_WHITE = (SDL_Color){255, 255, 255, 255};

SDL_Surface* _button_base_surface_normal = NULL;
SDL_Texture* _button_base_texture_normal = NULL;
SDL_Surface* _button_base_surface_p1hover = NULL;
SDL_Texture* _button_base_texture_p1hover = NULL;
SDL_Surface* _button_base_surface_p2hover = NULL;
SDL_Texture* _button_base_texture_p2hover = NULL;
SDL_Surface* _button_base_surface_bothhover = NULL;
SDL_Texture* _button_base_texture_bothhover = NULL;
SDL_Surface* _button_base_surface_pressed = NULL;
SDL_Texture* _button_base_texture_pressed = NULL;

UiButton* Button_New(SDL_Renderer* gRenderer, UiMenu* menu, char* text, int fontSize){
    if (!mmbuttfont){
        // TODO: Actually support multiple font sizes... somehow
        mmbuttfont = TTF_OpenFont("Graphics/Fonts/Recursive-Bold.ttf", 20);
    }

    if (!_button_base_surface_normal){
        _button_base_surface_normal = IMG_Load("Graphics/Ui/button1.png");
        _button_base_texture_normal = SDL_CreateTextureFromSurface(gRenderer, _button_base_surface_normal);
    }
    if (!_button_base_surface_p1hover){
        _button_base_surface_p1hover = IMG_Load("Graphics/Ui/button2.png");
        _button_base_texture_p1hover = SDL_CreateTextureFromSurface(gRenderer, _button_base_surface_p1hover);
    }
    if (!_button_base_surface_p2hover){
        _button_base_surface_p2hover = IMG_Load("Graphics/Ui/button2.png");
        _button_base_texture_p2hover = SDL_CreateTextureFromSurface(gRenderer, _button_base_surface_p2hover);
    }
    if (!_button_base_surface_bothhover){
        _button_base_surface_bothhover = IMG_Load("Graphics/Ui/button2.png");
        _button_base_texture_bothhover = SDL_CreateTextureFromSurface(gRenderer, _button_base_surface_bothhover);
    }

    if (!_button_base_surface_pressed){
        _button_base_surface_pressed = IMG_Load("Graphics/Ui/button3.png");
        _button_base_texture_pressed = SDL_CreateTextureFromSurface(gRenderer, _button_base_surface_pressed);
    }

    UiButton* butt = malloc(sizeof(UiButton));
    butt->hcentered = 0;
    butt->up    = NULL;
    butt->down  = NULL;
    butt->left  = NULL;
    butt->right = NULL;
    butt->position = (Vector2){0,0};
    butt->on_confirm    = (CallbackFunction){};
    butt->on_hold_left  = (CallbackFunction){};
    butt->on_hold_right = (CallbackFunction){};

    butt->surface_normal = _button_base_surface_normal;
    butt->texture_normal = _button_base_texture_normal;
    butt->surface_p1hover = _button_base_surface_p1hover;
    butt->texture_p1hover = _button_base_texture_p1hover;
    butt->surface_p2hover = _button_base_surface_p2hover;
    butt->texture_p2hover = _button_base_texture_p2hover;
    butt->surface_bothhover = _button_base_surface_bothhover;
    butt->texture_bothhover = _button_base_texture_bothhover;
    butt->surface_pressed = _button_base_surface_pressed;
    butt->texture_pressed = _button_base_texture_pressed;

    butt->string_text = (char*)malloc(strlen(text) * sizeof(char) + 1);
    butt->surface_text = TTF_RenderText_Blended(mmbuttfont, text, mmb_WHITE);
    butt->texture_text = SDL_CreateTextureFromSurface(gRenderer, butt->surface_text);
    strcpy(butt->string_text, text);
    butt->type = UI_TYPE_BUTTON;
    sb_push(menu->elements, (UiElement*)butt);

    return butt;
}

void Button_Draw(SDL_Renderer* gRenderer, UiMenu* menu, UiButton* butt){
    SDL_Rect rect, buttrect;
    SDL_Surface* surf = butt->surface_normal;
    SDL_Texture* tex = butt->texture_normal;

    if(menu->p1focused == butt && menu->p2focused == butt){
        surf = butt->surface_bothhover;
        tex = butt->texture_bothhover;
    }

    else if(menu->p1focused == butt){
        surf = butt->surface_p1hover;
        tex = butt->texture_p1hover;
    }

    else if(menu->p2focused == butt){
        surf = butt->surface_p2hover;
        tex = butt->texture_p2hover;
    }


    buttrect = rect = (SDL_Rect) {butt->position.x, butt->position.y, surf->w, surf->h};
    SDL_RenderCopy(gRenderer, tex, NULL, &rect);

    if(butt->texture_text){
        rect = (SDL_Rect) {butt->position.x, butt->position.y, butt->surface_text->w, butt->surface_text->h};
        rect.x = butt->position.x + buttrect.w/2 - rect.w/2;
        rect.y = butt->position.y + buttrect.h/2 - rect.h/2;

        SDL_RenderCopy(gRenderer, butt->texture_text, NULL, &rect);
    }
}

void Button_Free(UiButton* b){
    free(b->string_text);
    free(b);
}

Vector2 Button_Size(SDL_Renderer* ren, UiButton* butt){
    return (Vector2) {butt->surface_normal->w, butt->surface_normal->h};
}