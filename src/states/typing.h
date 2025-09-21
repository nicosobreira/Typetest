#ifndef TYPING_H
#define TYPING_H

#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1
#endif

#include <ncursesw/ncurses.h>

#include "core/game_state.h"
#include "core/text_entry.h"
#include "utils/stack_char.h"

// FIX: Separate the int cursor into two variables
// 1. Virtual screen position
// 2. Current letter in pTextEntry->text
typedef struct TypingData
{
	TextEntry* pTextEntry;
	StackChar inputBuffer;
	WINDOW* windowStatus;
	WINDOW* windowText;
	int pointerText;
	int correctLetters;
} TypingData;

void Typing_OnEnter(GameStateMachine* sm);
void Typing_OnExit(GameStateMachine* sm);
void Typing_Input(GameStateMachine* sm);
void Typing_Update(GameStateMachine* sm);
void Typing_Draw(GameStateMachine* sm);

GameState Typing_Constructor(TypingData* data);

#endif // TYPING_H
