#define _XOPEN_SOURCE_EXTENDED 1

#include <locale.h>
#include <ncursesw/ncurses.h>
#include <wchar.h>
#include <stdlib.h> // rand, srand
#include <time.h> // time

#include "core/text_entry.h"
#include "utils/error.h"
#include "utils/my_string.h"
#include "ui/window.h"
#include "utils/stack_char.h"

#include "assets/samples.h"

#define FPS (60.0f)
#define MS_PER_SECONDS (int)(1.0f / FPS)

// TODO: Change the String sentence in game to a TextEntry

// NOTE: Screen updates (refresh) are made only when necessary

#define COLOR_DEFAULT (-1)

typedef enum GameState
{
	GAME_STATE_RUNNING,
	GAME_STATE_QUIT,
} GameState;

typedef struct Point
{
	int x;
	int y;
} Point;

// FIX: Separate the int cursor into two variables
// 1. Virtual screen position
// 2. Current letter in pTextEntry->text
typedef struct Game
{
	TextEntry* pTextEntry;
	StackChar inputBuffer;
	WINDOW* windowStatus;
	WINDOW* windowText;
	Point cursor;
	int pointerText;
	int correctLetters;
	GameState state;
} Game;

void compareInputText(Game* pGame, wint_t key);

void Game_ColorInit(void);

void Game_Init(Game* pGame);

void Game_Input(Game* pGame);

void Game_Update(Game* pGame);

void Game_Start(Game* pGame);

int main(void)
{
	setlocale(LC_ALL, "");

	srand((unsigned int)time(NULL));

	initscr();
	if (stdscr == NULL)
		HANDLE_ERROR(1, "%s", "Failed to initiate the screen");

	curs_set(1);
	cbreak();
	noecho();

	Game game;
	Game* pGame = &game;

	// TODO: Add a new partida function
	Game_Init(pGame);

	Game_Start(pGame);

	while (pGame->state != GAME_STATE_QUIT)
	{
		Game_Input(pGame);

		Game_Update(pGame);

		napms(MS_PER_SECONDS);
	}

	StackChar_Free(&pGame->inputBuffer);

	delwin(pGame->windowText);
	delwin(pGame->windowStatus);
	endwin();

	return 0;
}

void Game_Init(Game* pGame)
{
	Game_ColorInit();

	pGame->windowText = Window_New(stdscr, WINDOW_LAYOUT_CENTER, WINDOW_ALIGN_NULL);

	nodelay(pGame->windowText, TRUE);
	notimeout(pGame->windowText, TRUE);

	pGame->windowStatus = Window_New(pGame->windowText, WINDOW_LAYOUT_ON_TOP, WINDOW_ALIGN_LEFT);

	StackChar_New(&pGame->inputBuffer);
}

void Game_Input(Game* pGame)
{
	wint_t key;
	int returnValue = wget_wch(pGame->windowText, &key);

	if (returnValue == ERR)
		return;

	compareInputText(pGame, key);

	// FIX: Move this elsewhere

	mvwprintw(pGame->windowStatus, 1, 1, "Correct Letters: %d", pGame->correctLetters);

	wmove(pGame->windowText, 0, pGame->pointerText);

	wrefresh(pGame->windowStatus);
	wrefresh(pGame->windowText);
}

void Game_Update(Game* pGame)
{
}

void Game_Start(Game* pGame)
{
	// Cleanup previous screen
	erase();

	pGame->state = GAME_STATE_RUNNING;
	// pGame->pTextEntry = TextEntry_RandomText();
	pGame->pTextEntry = &TEXT_SAMPLES[0];
	pGame->pointerText = 0;
	pGame->correctLetters = 0;
	pGame->cursor = (Point){ .x = 0, .y = 0 };

	// mvwaddwstr(pGame->windowText, 1, 1, pGame->pTextEntry->text.letters);
	waddwstr(pGame->windowText, pGame->pTextEntry->text.letters);

	wrefresh(pGame->windowStatus);
	wrefresh(pGame->windowText);
}

void handleBackspace(Game* pGame) {
	if (!String_IsIndexValid(&pGame->pTextEntry->text, pGame->pointerText - 1))
		return;

	pGame->pointerText--;

	wchar_t textChar = String_GetChar(&pGame->pTextEntry->text, pGame->pointerText);

	if (textChar == StackChar_Top(&pGame->inputBuffer)) {
		pGame->correctLetters--;
	}

	StackChar_Pop(&pGame->inputBuffer);

	mvwaddnwstr(pGame->windowText, 0, pGame->pointerText, &textChar, 1);
}

void handleCharacterInput(Game* pGame, wint_t key) {
	if (!String_IsIndexValid(&pGame->pTextEntry->text, pGame->pointerText + 1))
		return;

	wchar_t character = (wchar_t)key;
	StackChar_Push(&pGame->inputBuffer, character);

	wchar_t textChar = String_GetChar(&pGame->pTextEntry->text, pGame->pointerText);

	if (character == textChar) {
		wattron(pGame->windowText, COLOR_PAIR(COLOR_GREEN));
		pGame->correctLetters++;
	} else {
		wattron(pGame->windowText, COLOR_PAIR(COLOR_RED));
	}

	mvwaddnwstr(pGame->windowText, 0, pGame->pointerText, &character, 1);
	wattron(pGame->windowText, COLOR_PAIR(COLOR_DEFAULT));

	// Game win
	if (pGame->correctLetters >= pGame->pTextEntry->text.length) {
		pGame->state = GAME_STATE_QUIT;
		return;
	}

	pGame->pointerText++;
}

// TODO: Add support for ENTER
void compareInputText(Game* pGame, wint_t key) {
	switch (key)
	{
		case 127: // Backspace (DEL)
			handleBackspace(pGame);
			break;
		case 10: // Enter
			break;
		default:
			handleCharacterInput(pGame, key);
			break;
	}
}

void Game_ColorInit(void) {
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
