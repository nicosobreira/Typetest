#ifndef TYPING_H
#define TYPING_H

#include <ncursesw/ncurses.h>

#include "core/text_entry.h"
#include "utils/stack_char.h"

typedef struct Point
{
	int x;
	int y;
} Point;

// FIX: Separate the int cursor into two variables
// 1. Virtual screen position
// 2. Current letter in pTextEntry->text
typedef struct TypingData
{
	TextEntry* pTextEntry;
	StackChar inputBuffer;
	WINDOW* windowStatus;
	WINDOW* windowText;
	Point cursor;
	int pointerText;
	int correctLetters;
} TypingData;

void compareInputText(TypingData* pTyping, wint_t key);

void Typing_ColorInit(void);

void Typing_Start(TypingData* pTyping);

void Typing_Input(TypingData* pTyping);

void Typing_Update(TypingData* pTyping);


#endif // TYPING_H
