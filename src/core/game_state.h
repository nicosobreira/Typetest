#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdbool.h>

// Total GameStateType possible values
#define GAME_STATES_MAX (2)

typedef struct GameStateMachine GameStateMachine;

typedef enum GameStateType
{
	GAME_STATE_TYPING,
	GAME_STATE_MENU,
} GameStateType;

typedef void* Data;

typedef struct GameState
{
	void (*OnEnter)(GameStateMachine* sm);
	void (*OnExit)(GameStateMachine* sm);
	void (*Input)(GameStateMachine* sm);
	void (*Update)(GameStateMachine* sm);
	void (*Draw)(GameStateMachine* sm);
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
