#include "Core/scenes/manager.h"

#include <locale.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#include "Core/utils/error.h"

#include "scenes/menu.h"
#include "scenes/score.h"
#include "scenes/typing.h"

// Teste: O som foi adicionado

// Exemplo: As partículas foram adicionadas

static void ncursesInit(void)
{
    setlocale(LC_ALL, "");

    srand((unsigned int)time(NULL));

    initscr();
    if (stdscr == NULL)
        ERROR(1, "%s", "Failed to initiate the screen");

    cbreak();
    noecho();

    if (!has_colors())
        ERROR(9, "%s", "Your terminal don't support colors");

    start_color();

    use_default_colors();
    for (short i = 0; i < COLORS; i++)
    {
        init_pair(i, // Index
                  i, // Foreground
                  -1 // Background
        );
    }
}

int main(void)
{
    ncursesInit();

    SceneManager_Init();

    SceneManager_Register(Typing_Scene);
    SceneManager_Register(Menu_Scene);

    TypingData *typingData = (TypingData *)SceneManager_GetDataByName(SCENE_TYPING);
    TypingScore *typingScore = &typingData->score;

    Scene score = Score_Scene(typingScore);

    SceneManager_RegisterScene(&score);

    SceneManager_Switch(SCENE_MENU);

    SceneManager_Loop();

    endwin();

    return 0;
}
