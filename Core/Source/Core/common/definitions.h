#ifndef TYPES_H
#define TYPES_H

typedef void (*GameScreenFunc)(void *data);

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

#endif // TYPES_H
