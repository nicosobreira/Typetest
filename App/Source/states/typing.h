#ifndef TYPING_H
#define TYPING_H

#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1
#endif

#include <ncursesw/ncurses.h>

#include "Core/common/definitions.h"

#include "Core/utils/point.h"
#include "Core/utils/stack_char.h"
#include "Core/utils/clock.h"

#include "core/text_entry.h"

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

void Typing_OnEnter(GameManager* sm);
void Typing_OnExit(GameManager* sm);
void Typing_Input(GameManager* sm);
void Typing_Update(GameManager* sm);
void Typing_Draw(GameManager* sm);

void Typing_Free(GameManager* sm);

GameScreen Typing_Constructor(TypingData* data);

#endif // TYPING_H
