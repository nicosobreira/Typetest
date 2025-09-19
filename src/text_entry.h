#ifndef TEXT_H
#define TEXT_H

#include <wchar.h>

#include "my_string.h"

typedef enum TextDifficulty
{
	TEXT_DIFFICULTY_EASY,
	TEXT_DIFFICULTY_MEDIUM,
	TEXT_DIFFICULTY_HARD,
} TextDifficulty;

typedef struct TextEntry
{
	wchar_t* text;
	size_t length;
	TextDifficulty difficulty;
} TextEntry;

void Sample_Loader(String* pSentence, size_t randomEntry);

#endif // TEXT_H
