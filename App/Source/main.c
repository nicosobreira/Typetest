#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1
#endif

#include "Core/game_state.h"
#include "states/typing.h"
#include "states/menu.h"
#include "states/score.h"

int main(void)
{
	Ncurses_Init();

	// TODO: See if it's a good idea to allocate this memory on the heap
	TypingData typingData;
	MenuData menuData;
	ScoreData scoreData;

	GameStateMachine stateMachine;

	stateMachine.states[GAME_STATE_TYPING] = Typing_Constructor(&typingData);
	stateMachine.states[GAME_STATE_MENU] = Menu_Constructor(&menuData);
	stateMachine.states[GAME_STATE_SCORE] = Score_Constructor(&scoreData, &typingData.score);

	// NOTE: You can't use GameStateMachine_Switch here because there is no
	// previous state to call OnExit
	stateMachine.current = &stateMachine.states[GAME_STATE_MENU];
	stateMachine.current->OnEnter(&stateMachine);

	GameStateMachine_MainLoop(&stateMachine);

	return 0;
}
