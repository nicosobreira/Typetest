#ifndef TYPES_H
#define TYPES_H

typedef void* Data;

typedef struct GameManager GameManager;

// Game Screen

typedef void (*GameScreenFunc)(GameManager* gm);

typedef struct GameScreen
{
	GameScreenFunc OnEnter;
	GameScreenFunc OnExit;
	GameScreenFunc Input;
	GameScreenFunc Update;
	GameScreenFunc Draw;
	GameScreenFunc Free;
	Data data;
} GameScreen;

// Game Pool

typedef struct GameScreenPool
{
	GameScreen* data;
	int size;
} GameScreenPool;

// Game Manager

typedef enum GameManagerStatus
{
	GAME_MANAGER_RUNNING,
	GAME_MANAGER_SWITCH,
	GAME_MANAGER_QUIT,
} GameManagerStatus;

typedef struct GameManager
{
	GameScreenPool screens;
	GameScreen* current;
	GameManagerStatus status;
} GameManager;

#endif // TYPES_H
