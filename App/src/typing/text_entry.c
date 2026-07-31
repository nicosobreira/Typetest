#include "typing/text_entry.h"

#include <stdlib.h> // rand, srand

#include "Core/data_structures/string.h"
#include "typing/samples.h"

static TextDifficulty getStringDifficulty(String *pString)
{
    size_t length = String_Length(pString);
    if (length < 15)
    {
        return TEXT_DIFFICULTY_EASY;
    }
    else if (length < 45)
    {

        return TEXT_DIFFICULTY_EASY;
    }
    else
    {

        return TEXT_DIFFICULTY_EASY;
    }
}

TextEntry TextEntry_RandomText(void)
{
    size_t randomEntry = rand() % TEXT_SAMPLES_LENGTH;

    wchar_t **pText = &TEXT_SAMPLES[randomEntry];

    String string;
    String_New(&string, *pText);

    TextDifficulty dif = getStringDifficulty(&string);

    TextEntry entry = {.text = string, .difficulty = dif};

    return entry;
}
