#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <stddef.h>
#include <stdbool.h>

#include "Core/common/definitions.h"

// Helper functions

void GameManager_Init(GameManager* gm, int screensTotal);

void GameManager_MainLoop(GameManager* gm);

Data GameManager_GetData(GameManager* gm);

Data GameManager_GetDataByType(GameManager* gm, int type);

void GameManager_Free(GameManager* gm);

void GameManager_Switch(GameManager* gm, int type);

void GameManager_Quit(GameManager* gm);

void Ncurses_Init(void);

#endif // GAME_SCREEN_H
