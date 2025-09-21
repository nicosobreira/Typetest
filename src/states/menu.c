#include "states/menu.h"

void Menu_OnEnter(GameStateMachine* sm)
{
	(void)sm;
}

void Menu_OnExit(GameStateMachine* sm)
{
	(void)sm;
}

void Menu_Input(GameStateMachine* sm)
{
	(void)sm;
}

void Menu_Update(GameStateMachine* sm)
{
	(void)sm;
}

void Menu_Draw(GameStateMachine* sm)
{
	(void)sm;
}

GameState Menu_Constructor(MenuData* data)
{
	GameState menu;

	menu.OnEnter = Menu_OnEnter;
	menu.OnExit = Menu_OnExit;
	menu.Input = Menu_Input;
	menu.Update = Menu_Update;
	menu.Draw = Menu_Update;

	menu.data = data;

	return menu;
}
