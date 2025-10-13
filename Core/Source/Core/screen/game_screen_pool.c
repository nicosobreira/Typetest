#include "Core/screen/game_screen_pool.h"

#include <stdlib.h>

#include "Core/utils/error.h"

static void validateId(GameScreenPool* pPool, int id)
{
	if (id < 0 || id >= pPool->size)
		HANDLE_ERROR(1, "Invalid ID \"%d\"", id);
}

void GameScreenPool_Init(GameScreenPool* pPool, int size)
{
	pPool->data = (GameScreen *) malloc(size * sizeof(GameScreen));
	if (pPool->data == NULL)
		HANDLE_ERROR(1, "%s", "Allocation failed");

	pPool->size = size;
}

void GameScreenPool_SetScreen(GameScreenPool* pPool, GameScreen* pState, int id)
{
	validateId(pPool, id);

	pPool->data[id] = *pState;
}

GameScreen* GameScreenPool_GetScreen(GameScreenPool* pPool, int id)
{
	validateId(pPool, id);

	return (&pPool->data[id]);
}

Data GameScreenPool_GetDataById(GameScreenPool* pPool, int id)
{
	validateId(pPool, id);

	return (pPool->data[id].data);
}

void GameScreenPool_Free(GameScreenPool* pPool, GameManager *pStateMachine)
{
	for (int i = GAME_SCREEN_POOL_FIRST; i < pPool->size; i++)
	{
		pPool->data[i].Free(pStateMachine);
	}

	free(pPool->data);

	pPool->data = NULL;
	pPool->size = 0;
}
