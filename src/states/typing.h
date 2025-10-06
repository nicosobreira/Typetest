#ifndef TYPING_H
#define TYPING_H

#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1
#endif

#include <ncursesw/ncurses.h>

#include "utils/clock.h"

#include "utils/point.h"
#include "core/game_state.h"
#include "core/text_entry.h"
#include "utils/stack_char.h"

typedef struct TypingScore
{
	Clock miliSeconds;
	double charsPerSecond;
	double wordsPerMinute;
	int wrongLetters;
	int correctLetters;
} TypingScore;

typedef struct TypingData
{
	TypingScore score;
	StackChar inputBuffer;
	TextEntry* pTextEntry;
	WINDOW* windowStatus;
	WINDOW* windowText;
	Point cursor;
	int pointerText;
	bool shouldDraw;
} TypingData;

void Typing_OnEnter(GameStateMachine* sm);
void Typing_OnExit(GameStateMachine* sm);
void Typing_Input(GameStateMachine* sm);
void Typing_Update(GameStateMachine* sm);
void Typing_Draw(GameStateMachine* sm);

void Typing_Free(GameStateMachine* sm);

GameState Typing_Constructor(TypingData* data);

#endif // TYPING_H
