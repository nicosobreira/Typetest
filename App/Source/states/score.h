#ifndef SCORE_H
#define SCORE_H

#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1
#endif

#include <ncursesw/ncurses.h>

#include "Core/common/definitions.h"

#include "states/typing.h"

typedef struct ScoreData
{
	TypingScore *pTypingScore;
	WINDOW *windowText;
} ScoreData;

void Score_OnEnter(GameManager* gm);

void Score_OnExit(GameManager* gm);
void Score_Input(GameManager* gm);
void Score_Update(GameManager* gm);
void Score_Draw(GameManager* gm);

void Score_Free(GameManager* gm);

GameScreen Score_Constructor(ScoreData* data, TypingScore* pTypingScore);

#endif // SCORE_H
