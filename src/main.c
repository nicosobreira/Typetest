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
#include "states/score.h"
#include "utils/error.h"

#include "constants/frames.h"

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

	TypingData typingData;
	MenuData menuData;
	ScoreData scoreData;

	GameStateMachine stateMachine;

	stateMachine.states[GAME_STATE_TYPING] = Typing_Constructor(&typingData);
	stateMachine.states[GAME_STATE_MENU] = Menu_Constructor(&menuData);
	stateMachine.states[GAME_STATE_SCORE] = Score_Constructor(&scoreData, &typingData.score);

	// NOTE: You can't use GameStateMachine_Switch here because there is no
	// previous state to call OnExit
	stateMachine.current = &stateMachine.states[GAME_STATE_MENU];
	stateMachine.current->OnEnter(&stateMachine);

	stateMachine.isRunning = true;
	stateMachine.hasSwitch = false;

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
			stateMachine.current->Update(&stateMachine);

			if (stateMachine.hasSwitch)
			{
				stateMachine.hasSwitch = false;
				continue;
			}

			lag -= MS_PER_UPDATE;
		}

		stateMachine.current->Draw(&stateMachine);

		if (lag < MS_PER_UPDATE)
		{
			double sleepTimeUs = (MS_PER_UPDATE - lag) * 1000.0;
			usleep((useconds_t)sleepTimeUs);
		}
	}

	GameStateMachine_Free(&stateMachine);

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

	cbreak();
	noecho();

	Game_ColorInit();
}

void Game_ColorInit(void)
{
	if (!has_colors())
		HANDLE_ERROR(9, "%s", "Your terminal don't support colors");

	start_color();

	use_default_colors();
	for (short i = 0; i < COLORS; i++) {
		init_pair(
			i,  // Index
			i,  // Foreground
			-1  // Background
		);
	}
}
