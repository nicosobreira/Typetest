#ifndef TEXT_H
#define TEXT_H

#include "utils/my_string.h"

typedef enum TextDifficulty
{
	TEXT_DIFFICULTY_EASY,
	TEXT_DIFFICULTY_MEDIUM,
	TEXT_DIFFICULTY_HARD,
} TextDifficulty;

typedef struct TextEntry
{
	String text;
	TextDifficulty difficulty;
} TextEntry;

TextEntry* TextEntry_RandomText(void);

#endif // TEXT_H
