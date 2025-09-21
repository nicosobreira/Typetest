#ifndef MENU_H
#define MENU_H

#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1
#endif

#include <ncursesw/ncurses.h>

#include "core/game_state.h"

typedef struct MenuData
{
	int test;
} MenuData;

void Menu_OnEnter(GameStateMachine* sm);
void Menu_OnExit(GameStateMachine* sm);
void Menu_Input(GameStateMachine* sm);
void Menu_Update(GameStateMachine* sm);
void Menu_Draw(GameStateMachine* sm);

GameState Menu_Constructor(MenuData* data);

#endif // MENU_H
