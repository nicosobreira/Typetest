#include "states/menu.h"

#include <ncursesw/ncurses.h>

#include "constants/key_codes.h"
#include "ui/window.h"

void Menu_OnEnter(GameStateMachine* sm)
{
	MenuData* data = (MenuData *)GameStateMachine_GetData(sm);

	curs_set(0);

	mvwprintw(data->windowText, 1, 1, "%s", data->title);
	mvwprintw(data->windowText, 2, 1, "Press [p] to Play");
	mvwprintw(data->windowText, 3, 1, "Press [q] to Exit");

	wrefresh(data->windowText);
}

void Menu_OnExit(GameStateMachine* sm)
{
	MenuData* data = (MenuData *)GameStateMachine_GetData(sm);

	werase(data->windowText);
	wrefresh(data->windowText);
}

void Menu_Input(GameStateMachine* sm)
{
	MenuData* data = (MenuData *)GameStateMachine_GetData(sm);

	char key = (char)wgetch(data->windowText);

	switch (key)
	{
		case 'p':
			GameStateMachine_Switch(sm, GAME_STATE_TYPING);
			break;
		case KEY_CODE_BACKSPACE:
		case 'q':
			GameStateMachine_Quit(sm);
			break;
	}
}

void Menu_Update(GameStateMachine* sm, double delta)
{
	(void)sm;
	(void)delta;
}

void Menu_Draw(GameStateMachine* sm)
{
	(void)sm;
}

void Menu_Free(GameStateMachine* sm)
{
	MenuData* data = (MenuData *)GameStateMachine_GetData(sm);

	delwin(data->windowText);
}

GameState Menu_Constructor(MenuData* data)
{
	GameState menu;

	menu.OnEnter = Menu_OnEnter;
	menu.OnExit = Menu_OnExit;
	menu.Input = Menu_Input;
	menu.Update = Menu_Update;
	menu.Draw = Menu_Draw;
	menu.Free = Menu_Free;

	data->windowText = Window_New(stdscr, WINDOW_LAYOUT_CENTER, WINDOW_ALIGN_NULL);

	nodelay(data->windowText, TRUE);
	notimeout(data->windowText, TRUE);

	data->title = "Typetest";

	menu.data = data;

	return menu;
}
