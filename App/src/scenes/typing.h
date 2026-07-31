#ifndef TYPING_H
#define TYPING_H

#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1
#endif

#include <ncursesw/ncurses.h>

#include "Core/clock.h"
#include "Core/constants.h"
#include "Core/data_structures/point.h"
#include "Core/data_structures/stack_char.h"
#include "Core/scenes.h"

#include "typing/text_entry.h"

#define SCENE_TYPING ("typing")
#define GAME_TICK_MS (MS_PER_UPDATE * 10)

typedef struct TypingScore
{
    Clock totalTimeMs;
    double charsPerSecond;
    double wordsPerMinute;
    int wrongLetters;
    int correctLetters;
} TypingScore;

typedef struct TypingData
{
    TypingScore score;
    StackChar input;
    TextEntry entry;
    WINDOW *windowStatus;
    WINDOW *windowText;
    Point cursor;
    int pointerText;
    bool shouldDraw;
} TypingData;

void Typing_OnEnter(void *data);
void Typing_OnExit(void *data);
void Typing_Input(void *data);
void Typing_Update(void *data);
void Typing_Draw(void *data);

void Typing_Free(void *data);

Scene Typing_Scene(void);

#endif // TYPING_H
