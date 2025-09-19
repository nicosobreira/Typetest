#define _XOPEN_SOURCE_EXTENDED 1

#include <locale.h>
#include <ncursesw/ncurses.h>
#include <wchar.h>
#include <stdlib.h> // rand, srand
#include <time.h> // time

#include "samples.h"
#include "text_entry.h"
#include "error.h"
#include "my_string.h"
#include "window.h"

#define FPS (int)(1.0f / 60.0f)

// TODO: Change the String sentence in game to a TextEntry

typedef enum GameState
{
	GAME_STATE_RUNNING,
	GAME_STATE_QUIT
} GameState;

typedef struct Game
{
	String sentence;
	String* pSentence;
	WINDOW* windowStatus;
	WINDOW* windowText;
	size_t index;
	GameState state;
} Game;

void compareInputText(Game* pGame, wchar_t character);
void selectRandomText(Game* pGame);

int main(void)
{
	setlocale(LC_ALL, "");

	srand((unsigned int)time(NULL));

	initscr();
	if (stdscr == NULL)
		HANDLE_ERROR(1, "%s", "Failed to init the screen");

	curs_set(1);
	cbreak();
	noecho();

	// nodelay(stdscr, TRUE);
	// notimeout(stdscr, TRUE);

	Game game;
	Game* pGame = &game;

	pGame->windowText = Window_New(stdscr, WINDOW_LAYOUT_CENTER);

	pGame->windowStatus = Window_New(pGame->windowText, WINDOW_LAYOUT_ON_TOP);

	wrefresh(pGame->windowStatus);
	wrefresh(pGame->windowText);

	nodelay(pGame->windowText, TRUE);
	notimeout(pGame->windowText, TRUE);

	pGame->state = GAME_STATE_RUNNING;
	pGame->pSentence = &pGame->sentence;
	pGame->index = 0;

	selectRandomText(pGame);

	wchar_t character;
	while (pGame->state != GAME_STATE_QUIT)
	{
		int returnValue = wget_wch(pGame->windowText, (wint_t *)&character);

		if (returnValue == ERR) continue;

		compareInputText(pGame, character);

		wrefresh(pGame->windowStatus);
		wrefresh(pGame->windowText);

		napms(FPS);
	}

	delwin(pGame->windowText);
	endwin();

	String_Free(pGame->pSentence);

	return 0;
}

void compareInputText(Game* pGame, wchar_t character)
{
	if (!String_IsIndexValid(pGame->pSentence, pGame->index))
	{
		pGame->state = GAME_STATE_QUIT;
		return;
	}

	if (character == String_GetChar(pGame->pSentence, pGame->index))
	{
		waddnwstr(pGame->windowText, &character, 1);
	}
	else
{
		wprintw(pGame->windowText, "x");
	}

	pGame->index++;
}

void selectRandomText(Game* pGame)
{
	size_t randomEntry = rand() % TEXT_SAMPLES_LENGTH;
	Sample_Loader(pGame->pSentence, randomEntry);
}
