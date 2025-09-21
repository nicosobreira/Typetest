#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1
#endif

#include <locale.h>
#include <ncursesw/ncurses.h>
#include <wchar.h>
#include <stdlib.h> // rand, srand
#include <time.h> // time

#include "core/game_state.h"
#include "states/typing.h"
#include "states/menu.h"
#include "utils/error.h"

#define FPS (60.0f)
#define MS_PER_SECONDS (int)(1.0f / FPS)

// TODO: Change the String sentence in game to a TextEntry

// NOTE: Screen updates (refresh) are made only when necessary

void Game_Init(void);

void Game_ColorInit(void);

int main(void)
{
	Game_Init();

	static TypingData typingData;
	static MenuData menuData;

	GameStateMachine stateMachine;

	stateMachine.states[GAME_STATE_TYPING] = Typing_Constructor(&typingData);
	stateMachine.states[GAME_STATE_MENU] = Menu_Constructor(&menuData);

	GameStateMachine_Switch(&stateMachine, GAME_STATE_TYPING);

	stateMachine.isRunning = true;

	while (stateMachine.isRunning)
	{
		stateMachine.current->Input(&stateMachine);
		stateMachine.current->Update(&stateMachine);
		stateMachine.current->Draw(&stateMachine);

		napms(MS_PER_SECONDS);
	}

	stateMachine.states[GAME_STATE_TYPING].OnExit(&stateMachine);
	stateMachine.states[GAME_STATE_MENU].OnExit(&stateMachine);

	endwin();

	return 0;
}

void Game_Init(void)
{
	setlocale(LC_ALL, "");

	srand((unsigned int)time(NULL));

	initscr();
	if (stdscr == NULL)
		HANDLE_ERROR(1, "%s", "Failed to initiate the screen");

	curs_set(1);
	cbreak();
	noecho();

	Game_ColorInit();
}

void Game_ColorInit(void)
{
	if (!has_colors())
		HANDLE_ERROR(9, "%s", "Your terminal don't support colors");

	start_color();
	if (COLORS < 256)
		HANDLE_ERROR(10, "%s", "Your terminal don't support 256 colors");

	use_default_colors();
	for (short i = 0; i < COLORS; i++) {
		// Index, foreground, background
		init_pair(i, i, -1);
	}
}
