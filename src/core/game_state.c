#include "core/game_state.h"

Data GameStateMachine_GetData(GameStateMachine* stateMachine)
{
	return (stateMachine->current->data);
}

void GameStateMachine_Switch(GameStateMachine* stateMachine, GameStateType type)
{
	stateMachine->current->OnExit(stateMachine);

	stateMachine->current = &stateMachine->states[type];
	stateMachine->current->OnEnter(stateMachine);
}

void GameStateMachine_Quit(GameStateMachine* stateMachine)
{
	stateMachine->isRunning = false;
}
