#ifndef UI_BASE
#define UI_BASE

#include "Vec2I.h"
#include "Stick.h"
#include "Types.h"

enum ui_type {
    UI_TYPE_NONE,   // 0
    UI_TYPE_LABEL,  // 1 
    UI_TYPE_BUTTON, // 2
    UI_TYPE_SPRITE, // 3
    UI_TYPE_MENU,   // 4
    UI_TYPE_MATCH,  // 5
    UI_TYPE_LIST,   // 6
    UI_TYPE_PREVIEW // 7
};

enum bake_type {
    BAKE_TYPE_DEFAULT,
    BAKE_TYPE_FLOATING,
    BAKE_TYPE_SET_X_SIZE,
    BAKE_TYPE_SET_X_SPACING,
    BAKE_TYPE_SET_Y_SIZE,
    BAKE_TYPE_SET_Y_SPACING,
};

extern int uiidcount;

#define ui_preamble          \
enum ui_type type;           \
int id;                      \
int hcentered;               \
int vcentered;               \
Vec2I position;              \
struct ui_element* up;       \
struct ui_element* down;     \
struct ui_element* left;     \
struct ui_element* right;    \
struct ui_element* next;     /*right bumper to tab right */ \
struct ui_element* previous; /*left bumper to tab left */   \

typedef struct ui_element {
    ui_preamble
} UiElement;


// Invoke
#define invoke(cbfunc) (cbfunc.function(cbfunc.info))
// Invoke with set arg
#define invokea(cbfunc, a) (cbfunc.info.arg=a, invoke(cbfunc))
// Invoke with set player
#define invokep(cbfunc, p) (cbfunc.info.player=p, invoke(cbfunc))
// Invoke with set player and arg
#define invokepa(cbfunc, p, a) (cbfunc.info.player=p, cbfunc.arg=a, invoke(cbfunc))

Vec2I UiElement_Size(UiElement* elem);
# define ui_x(e) (e->position.x)
# define ui_y(e) (e->position.y)
# define ui_w(e) (UiElement_Size(mqui_as_element(e)).x)
# define ui_h(e) (UiElement_Size(mqui_as_element(e)).y)

void** Ui_GetFocusStack();

#endif /* UI_BASE */
