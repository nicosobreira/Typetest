#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <ncursesw/ncurses.h>

#include "core/text_entry.h"
#include "utils/stack_char.h"

typedef struct Typing Game;
typedef struct GameState GameState;

typedef struct Typing
{
	GameState* state;
} Game;

typedef void* data;

typedef struct GameState
{
	void (*Enter)(data*);
	void (*Start)(data*);
	void (*Input)(data*);
	void (*Update)(data*);
	void (*Draw)(data*);
} GameState;

typedef struct TypingData
{
	TextEntry* pTextEntry;
	StackChar inputBuffer;
	WINDOW* windowStatus;
	WINDOW* windowText;
	int pointerText;
	int correctLetters;
	GameState state;
} TypingData;

void Typing_Enter(Game* pGame);

#endif // GAME_STATE_H
