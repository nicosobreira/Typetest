#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdbool.h>

// Total GameStateType possible values
#define GAME_STATES_MAX (3)

typedef struct GameStateMachine GameStateMachine;

typedef void (*GameStateFunc)(GameStateMachine* sm);

typedef void* Data;

typedef enum GameStateType
{
	GAME_STATE_TYPING = 0,
	GAME_STATE_MENU,
	GAME_STATE_SCORE,
	GAME_STATE_TOTAL_SIZE,
} GameStateType;

typedef struct GameState
{
	GameStateFunc OnEnter;
	GameStateFunc OnExit;
	GameStateFunc Input;
	GameStateFunc Update;
	GameStateFunc Draw;
	GameStateFunc Free;
	Data data;
} GameState;

typedef struct GameStateMachine
{
	GameState states[GAME_STATES_MAX];
	GameState* current;
	bool isRunning;
	bool hasSwitch;
} GameStateMachine;

// Helper functions

void GameStateMachine_MainLoop(GameStateMachine* sm);

Data GameStateMachine_GetData(GameStateMachine* sm);

Data GameStateMachine_GetDataByType(GameStateMachine* stateMachine, GameStateType type);

void GameStateMachine_Free(GameStateMachine* stateMachine);

void GameStateMachine_Switch(GameStateMachine* sm, GameStateType type);

void GameStateMachine_Quit(GameStateMachine* sm);

void Ncurses_Init(void);

#endif // GAME_STATE_H
