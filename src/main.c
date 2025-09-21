#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1
#endif

#include <locale.h>
#include <ncursesw/ncurses.h>
#include <stdlib.h> // rand, srand
#include <time.h> // time
#include <sys/time.h>
#include <unistd.h> // usleep

#include "core/game_state.h"
#include "states/typing.h"
#include "states/menu.h"
#include "utils/error.h"

// 60 ticks per second
const double MS_PER_UPDATE = 1000.0 / 60.0;

// TODO: Change the String sentence in game to a TextEntry

// NOTE: Screen updates (refresh) are made only when necessary

static long long getCurrentTimeMs()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000;
}

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

	// NOTE: You can't use GameStateMachine_Switch here because there is no
	// previous state to call OnExit
	stateMachine.current = &stateMachine.states[GAME_STATE_MENU];
	stateMachine.current->OnEnter(&stateMachine);

	stateMachine.isRunning = true;

	long long previousTime = getCurrentTimeMs();
	double lag = 0.0;

	while (stateMachine.isRunning)
	{
		long long currentTime = getCurrentTimeMs();
		double elapsedTime = (double)(currentTime - previousTime);
		previousTime = currentTime;
		lag += elapsedTime;

		stateMachine.current->Input(&stateMachine);

		while (lag >= MS_PER_UPDATE)
		{
			stateMachine.current->Update(&stateMachine, MS_PER_UPDATE);
			lag -= MS_PER_UPDATE;
		}

		stateMachine.current->Draw(&stateMachine);

		if (lag < MS_PER_UPDATE)
		{
			double sleepTimeUs = (MS_PER_UPDATE - lag) * 1000;
			usleep((useconds_t)sleepTimeUs);
		}
	}

	stateMachine.current->Free(&stateMachine);

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
