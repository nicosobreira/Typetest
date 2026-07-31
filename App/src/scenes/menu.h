#ifndef MENU_H
#define MENU_H

#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1
#endif

#include <ncursesw/ncurses.h>

#include "Core/scenes.h"

#define SCENE_MENU ("menu")

typedef struct MenuData
{
    WINDOW *windowText;
} MenuData;

void Menu_OnEnter(void *data);
void Menu_OnExit(void *data);
void Menu_Input(void *data);
void Menu_Update(void *data);
void Menu_Draw(void *data);

void Menu_Free(void *data);

Scene Menu_Scene(void);

#endif // MENU_H
