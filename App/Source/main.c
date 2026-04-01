#include "Core/manager/game_manager.h"

#include "states/id.h"
#include "states/menu.h"
#include "states/score.h"
#include "states/typing.h"

int main(void)
{
    TypingData typingData;
    MenuData menuData;
    ScoreData scoreData;

    GameManager gameManager;

    GameManager_Init(&gameManager, SCREEN_TOTAL);

    gameManager.screens.data[SCREEN_TYPING] = Typing_Constructor(&typingData);
    gameManager.screens.data[SCREEN_MENU] = Menu_Constructor(&menuData);
    gameManager.screens.data[SCREEN_SCORE] = Score_Constructor(&scoreData, &typingData.score);

    gameManager.current = &gameManager.screens.data[SCREEN_MENU];
    gameManager.current->OnEnter(&gameManager);

    GameManager_MainLoop(&gameManager);

    return 0;
}
