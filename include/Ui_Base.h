#ifndef UI_BASE
#define UI_BASE

#include "Vec2I.h"
#include "Stick.h"

enum ui_type {
    UI_TYPE_NONE,
    UI_TYPE_LABEL,
    UI_TYPE_BUTTON,
    UI_TYPE_SPRITE,
    UI_TYPE_MENU,
    UI_TYPE_MATCH,
    UI_TYPE_LIST,
    UI_TYPE_PREVIEW
};

enum bake_type {
    BAKE_TYPE_DEFAULT,
    BAKE_TYPE_FLOATING,
    BAKE_TYPE_SET_X_SIZE,
    BAKE_TYPE_SET_X_SPACING,
    BAKE_TYPE_SET_Y_SIZE,
    BAKE_TYPE_SET_Y_SPACING,
};

#define ui_preamble          \
enum ui_type type;           \
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

typedef struct callback_info {
    int player;
    UiElement* target;
    int arg;
} CallbackInfo;

typedef struct callback_function {
    void (*function)();
    CallbackInfo info;
} CallbackFunction;


#define invoke(cbfunc) (cbfunc.function(cbfunc.info))
#define invokea(cbfunc, a) (cbfunc.info.arg=a, invoke(cbfunc))
#define invokep(cbfunc, p) (cbfunc.info.player=p, invoke(cbfunc))
#define invokepa(cbfunc, p, a) (cbfunc.info.player=p, cbfunc.arg=a, invoke(cbfunc))

Vec2I UiElement_Size(UiElement* elem);
# define ui_x(e) (e->position.x)
# define ui_y(e) (e->position.y)
# define ui_w(e) (UiElement_Size(mqui_as_element(e)).x)
# define ui_h(e) (UiElement_Size(mqui_as_element(e)).y)

void** Ui_GetFocusStack();

#endif /* UI_BASE */
