#ifndef UI_TYPES
#define UI_TYPES

#include "Ui_Button.h"
#include "Ui_Label.h"
#include "Ui_Sprite.h"
#include "Ui_Menu.h"
#include "Ui_Match.h"

#define mqui_as_element(a) ((UiElement*)(a))
#define mqui_as_button(a) ((UiButton*)(a))
#define mqui_as_label(a) ((UiLabel*)(a))
#define mqui_as_sprite(a) ((UiSprite*)(a))
#define mqui_as_menu(a) ((UiMenu*)(a))
#define mqui_as_match(a) ((UiMatch*)(a))

#endif /* UI_TYPES */
