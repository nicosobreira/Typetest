#include "Core/scenes/manager.h"

#include "scenes/menu.h"
#include "scenes/score.h"
#include "scenes/typing.h"

int main(void)
{
    SceneManager_Init();

    SceneManager_Register(Menu_Scene);
    SceneManager_Register(Typing_Scene);

    // SceneManager_Register(Score_Scene);

    SceneManager_Loop();

    return 0;
}
