#ifndef MENU_H
#define MENU_H

#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1
#endif

#include <ncursesw/ncurses.h>

#include "Core/common/definitions.h"

typedef struct MenuData
{
	WINDOW* windowText;
	char* title;
} MenuData;

void Menu_OnEnter(GameManager* sm);
void Menu_OnExit(GameManager* sm);
void Menu_Input(GameManager* sm);
void Menu_Update(GameManager* sm);
void Menu_Draw(GameManager* sm);

void Menu_Free(GameManager* sm);

GameScreen Menu_Constructor(MenuData* data);

#endif // MENU_H
