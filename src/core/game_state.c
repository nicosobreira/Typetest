#include "core/game_state.h"

Data GameStateMachine_GetData(GameStateMachine* stateMachine)
{
	return (stateMachine->current->data);
}

Data GameStateMachine_GetDataByType(GameStateMachine* stateMachine, GameStateType type)
{
	return (stateMachine->states[type].data);
}

void GameStateMachine_Free(GameStateMachine* stateMachine)
{
	stateMachine->states[GAME_STATE_TYPING].Free(stateMachine);
	stateMachine->states[GAME_STATE_MENU].Free(stateMachine);
	stateMachine->states[GAME_STATE_SCORE].Free(stateMachine);
}

void GameStateMachine_Switch(GameStateMachine* stateMachine, GameStateType type)
{
	stateMachine->current->OnExit(stateMachine);

	stateMachine->current = &stateMachine->states[type];
	stateMachine->current->OnEnter(stateMachine);

	stateMachine->hasSwitch = true;
}

void GameStateMachine_Quit(GameStateMachine* stateMachine)
{
	stateMachine->isRunning = false;
}
