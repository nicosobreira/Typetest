#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdbool.h>

// Total GameStateType possible values
#define GAME_STATES_MAX (2)

typedef struct GameStateMachine GameStateMachine;

typedef void (*GameStateFunc)(GameStateMachine* sm);
typedef void (*GameStateFuncUpdate)(GameStateMachine* sm, double delta);

typedef void* Data;

typedef enum GameStateType
{
	GAME_STATE_TYPING,
	GAME_STATE_MENU,
} GameStateType;

typedef struct GameState
{
	GameStateFunc OnEnter;
	GameStateFunc OnExit;
	GameStateFunc Input;
	GameStateFuncUpdate Update;
	GameStateFunc Draw;
	GameStateFunc Free;
	Data data;
} GameState;

typedef struct GameStateMachine
{
	GameState states[GAME_STATES_MAX];
	GameState* current;
	bool isRunning;
} GameStateMachine;

// Helper function
Data GameStateMachine_GetData(GameStateMachine* sm);

void GameStateMachine_Switch(GameStateMachine* sm, GameStateType type);

void GameStateMachine_Quit(GameStateMachine* sm);

#endif // GAME_STATE_H
