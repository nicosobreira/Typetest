#ifndef SCORE_H
#define SCORE_H

#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1
#endif

#include <ncursesw/ncurses.h>

#include "states/typing.h"

typedef struct ScoreData
{
	TypingScore *pTypingScore;
	WINDOW *windowText;
} ScoreData;

void Score_OnEnter(GameStateMachine* sm);
void Score_OnExit(GameStateMachine* sm);
void Score_Input(GameStateMachine* sm);
void Score_Update(GameStateMachine* sm);
void Score_Draw(GameStateMachine* sm);

void Score_Free(GameStateMachine* sm);

GameState Score_Constructor(ScoreData* data, TypingScore* pTypingScore);

#endif // SCORE_H
