#ifndef SCORE_H
#define SCORE_H

#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1
#endif

#include <ncursesw/ncurses.h>

#include "Core/scenes/scene.h"

#include "scenes/typing.h"

#define SCENE_SCORE ("score")

typedef struct ScoreData
{
    TypingScore *pTypingScore;
    WINDOW *windowText;
} ScoreData;

void Score_OnEnter(void *gm);

void Score_OnExit(void *gm);
void Score_Input(void *gm);
void Score_Update(void *gm);
void Score_Draw(void *gm);

void Score_Free(void *gm);

Scene Score_Scene(TypingScore *pTypingScore);

#endif // SCORE_H
