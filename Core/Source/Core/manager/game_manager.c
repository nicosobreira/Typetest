#include "Core/manager/game_manager.h"

#include <locale.h>
#include <ncursesw/ncurses.h>
#include <stdlib.h> // rand, srand
#include <time.h> // time
#include <sys/time.h>
#include <unistd.h> // usleep

#include "Core/screen/game_screen_pool.h"
#include "Core/constants/frames.h"
#include "Core/utils/error.h"

static long long getCurrentTimeMs()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000;
}

void GameManager_Init(GameManager* gm, int screensTotal)
{
	Ncurses_Init();

	GameScreenPool_Init(&gm->screens, screensTotal);

	gm->current = NULL;
	gm->status = GAME_MANAGER_RUNNING;
}

void GameManager_MainLoop(GameManager* gm)
{
	long long previousTime = getCurrentTimeMs();
	double lag = 0.0;

	while (gm->status != GAME_MANAGER_QUIT)
	{
		long long currentTime = getCurrentTimeMs();
		double elapsedTime = (double)(currentTime - previousTime);
		previousTime = currentTime;
		lag += elapsedTime;

		gm->current->Input(gm);

		if (gm->status == GAME_MANAGER_SWITCH)
		{
			gm->status = GAME_MANAGER_RUNNING;
			continue;
		}

		while (lag >= MS_PER_UPDATE)
		{
			gm->current->Update(gm);

			lag -= MS_PER_UPDATE;
		}

		gm->current->Draw(gm);

		if (lag < MS_PER_UPDATE)
		{
			double sleepTimeUs = (MS_PER_UPDATE - lag) * 1000.0;
			usleep((useconds_t)sleepTimeUs);
		}
	}

	GameManager_Free(gm);

	endwin();
}

Data GameManager_GetData(GameManager* gm)
{
	return (gm->current->data);
}

Data GameManager_GetDataByType(GameManager* gm, int type)
{
	GameScreenPool* pPool = &gm->screens;
	return (GameScreenPool_GetDataById(pPool, type));
}

void GameManager_Free(GameManager* gm)
{
	GameScreenPool* pPool = &gm->screens;
	GameScreenPool_Free(pPool, gm);

	gm->current = NULL;
}

void GameManager_Switch(GameManager* gm, int type)
{
	gm->current->OnExit(gm);

	GameScreenPool* pPool = &gm->screens;
	gm->current = GameScreenPool_GetScreen(pPool, type);
	gm->current->OnEnter(gm);

	gm->status = GAME_MANAGER_SWITCH;
}

void GameManager_Quit(GameManager* gm)
{
	gm->status = GAME_MANAGER_QUIT;
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
			i, // Index
			i, // Foreground
			-1 // Background
		);
	}
}
