#include "Core/manager/game_manager.h"

#include "states/id.h"
#include "states/typing.h"
#include "states/menu.h"
#include "states/score.h"

// TODO: Alocar as `Data` na heap
// TODO: Abstrair a alocação das diferentes `Data`

int main(void)
{
	// TODO: See if it's a good idea to allocate this memory on the heap
	TypingData typingData;
	MenuData menuData;
	ScoreData scoreData;

	GameManager gameManager;

	// FIX: this whole declaration in kind ugly
	stateMachine.states[GAME_STATE_TYPING] = Typing_Constructor(&typingData);
	stateMachine.states[GAME_STATE_MENU] = Menu_Constructor(&menuData);
	stateMachine.states[GAME_STATE_SCORE] = Score_Constructor(&scoreData, &typingData.score);

	gameManager.screens.data[SCREEN_TYPING] = Typing_Constructor(&typingData);
	gameManager.screens.data[SCREEN_MENU] = Menu_Constructor(&menuData);
	gameManager.screens.data[SCREEN_SCORE] = Score_Constructor(
		&scoreData,
		&typingData.score
	);

	gameManager.current = &gameManager.screens.data[SCREEN_MENU];
	gameManager.current->OnEnter(&gameManager);

	GameManager_MainLoop(&gameManager);

	return 0;
}
