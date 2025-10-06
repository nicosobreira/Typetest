#include "game_state.h"

#include <locale.h>
#include <ncursesw/ncurses.h>
#include <stdlib.h> // rand, srand
#include <time.h> // time
#include <sys/time.h>
#include <unistd.h> // usleep

#include "constants/frames.h"
#include "utils/error.h"

static long long getCurrentTimeMs()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000;
}

void GameStateMachine_MainLoop(GameStateMachine* sm)
{
	sm->isRunning = true;
	sm->hasSwitch = false;

	long long previousTime = getCurrentTimeMs();
	double lag = 0.0;

	while (sm->isRunning)
	{
		long long currentTime = getCurrentTimeMs();
		double elapsedTime = (double)(currentTime - previousTime);
		previousTime = currentTime;
		lag += elapsedTime;

		sm->current->Input(sm);

		while (lag >= MS_PER_UPDATE)
		{
			sm->current->Update(sm);

			if (sm->hasSwitch)
			{
				sm->hasSwitch = false;
				continue;
			}

			lag -= MS_PER_UPDATE;
		}

		sm->current->Draw(sm);

		if (lag < MS_PER_UPDATE)
		{
			double sleepTimeUs = (MS_PER_UPDATE - lag) * 1000.0;
			usleep((useconds_t)sleepTimeUs);
		}
	}

	GameStateMachine_Free(sm);

	endwin();

}

Data GameStateMachine_GetData(GameStateMachine* sm)
{
	return (sm->current->data);
}

Data GameStateMachine_GetDataByType(GameStateMachine* stateMachine, GameStateType type)
{
	return (stateMachine->states[type].data);
}

void GameStateMachine_Free(GameStateMachine* stateMachine)
{
	stateMachine->states[GAME_STATE_TYPING].Free(stateMachine);
	stateMachine->states[GAME_STATE_MENU].Free(stateMachine);
	stateMachine->states[GAME_STATE_SCORE].Free(stateMachine);
}

void GameStateMachine_Switch(GameStateMachine* stateMachine, GameStateType type)
{
	stateMachine->current->OnExit(stateMachine);

	stateMachine->current = &stateMachine->states[type];
	stateMachine->current->OnEnter(stateMachine);

	stateMachine->hasSwitch = true;
}

void GameStateMachine_Quit(GameStateMachine* stateMachine)
{
	stateMachine->isRunning = false;
}

void Ncurses_Init(void)
{
	setlocale(LC_ALL, "");

	srand((unsigned int)time(NULL));

	initscr();
	if (stdscr == NULL)
		HANDLE_ERROR(1, "%s", "Failed to initiate the screen");

	cbreak();
	noecho();

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

