#include "states/menu.h"

#include <ncursesw/ncurses.h>

#include "Core/manager/game_manager.h"
#include "Core/constants/key_codes.h"
#include "Core/ui/window.h"

#include "states/id.h"

void Menu_OnEnter(GameManager* sm)
{
	MenuData* data = (MenuData *)GameManager_GetData(sm);

	curs_set(FALSE);

	mvwprintw(data->windowText, 1, 1, "%s", data->title);
	mvwprintw(data->windowText, 2, 1, "Press [p] to Play");
	mvwprintw(data->windowText, 3, 1, "Press [q] to Exit");

	wrefresh(data->windowText);
}

void Menu_OnExit(GameManager* sm)
{
	MenuData* data = (MenuData *)GameManager_GetData(sm);

	werase(data->windowText);
	wrefresh(data->windowText);
}

void Menu_Input(GameManager* sm)
{
	MenuData* data = (MenuData *)GameManager_GetData(sm);

	char key = (char)wgetch(data->windowText);

	switch (key)
	{
		case 'p':
			GameManager_Switch(sm, SCREEN_TYPING);
			break;
		case KEY_CODE_BACKSPACE:
		case 'q':
			GameManager_Quit(sm);
			break;
	}
}

void Menu_Update(GameManager* sm)
{
	(void)sm;
}

void Menu_Draw(GameManager* sm)
{
	(void)sm;
}

void Menu_Free(GameManager* sm)
{
	MenuData* data = (MenuData *)GameManager_GetDataByType(sm, SCREEN_MENU);

	delwin(data->windowText);
}

GameScreen Menu_Constructor(MenuData* data)
{
	GameScreen menu;

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
