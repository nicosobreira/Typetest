#ifndef GAME_SCREEN_POOL_H
#define GAME_SCREEN_POOL_H

#include "Core/common/definitions.h"

#define GAME_SCREEN_POOL_FIRST (0)

void GameScreenPool_Init(GameScreenPool* pPool, int size);

void GameScreenPool_SetScreen(GameScreenPool* pPool, GameScreen* pState, int id);

GameScreen* GameScreenPool_GetScreen(GameScreenPool* pPool, int id);

Data GameScreenPool_GetDataById(GameScreenPool* pPool, int id);

void GameScreenPool_Free(GameScreenPool* pPool, GameManager *pStateMachine);

#endif // GAME_SCREEN_POOL_H
