#include "manager.h"

#include <locale.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Core/constants/frames.h"
#include "Core/utils/time.h"

static SManager g_SceneManager = {0};

static void Ncurses_Init(void)
{
    setlocale(LC_ALL, "");

    srand((unsigned int)time(NULL));

    initscr();
    if (stdscr == NULL)
        exit(1);
    // HANDLE_ERROR(1, "%s", "Failed to initiate the screen");

    cbreak();
    noecho();

    if (!has_colors())
        exit(1);
    // HANDLE_ERROR(9, "%s", "Your terminal don't support colors");

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

void SceneManager_Init(void)
{
    Ncurses_Init();

    g_SceneManager.count = 0;
    g_SceneManager.pCurrent = NULL;
    g_SceneManager.shouldClose = false;
}

void SceneManager_Free(void)
{
    for (size_t i = 0; i < g_SceneManager.count; i++)
    {
        void *pData = g_SceneManager.scenes[i].pData;
        g_SceneManager.scenes[i].Free(pData);
    }
}

void SceneManager_Switch(const char *name)
{
    for (size_t i = 0; i < g_SceneManager.count; i++)
    {
        if (strcmp(name, g_SceneManager.scenes[i].name) == 0)
        {
            Scene_OnExit();
            g_SceneManager.pCurrent = &g_SceneManager.scenes[i];
            Scene_OnEnter();
            return;
        }
    }

    // TraceLog(LOG_ERROR, "Unknow scene \"%s\"", name);
    exit(4);
}

void SceneManager_Loop(void)
{
    double previousTime = getCurrentTimeMs();
    double lag = 0.0;
    while (!g_SceneManager.shouldClose)
    {
        double currentTime = getCurrentTimeMs();
        double elapsedTime = (double)(currentTime - previousTime);
        previousTime = currentTime;
        lag += elapsedTime;

        Scene_Input();

        while (lag >= MS_PER_UPDATE)
        {
            Scene_Update();

            lag -= MS_PER_UPDATE;
        }

        Scene_Draw();
    }

    SceneManager_Free();
}

void SceneManager_EndLoop(void)
{
    g_SceneManager.shouldClose = true;
}

void SceneManager_Register(Scene (*SceneConstructor)(void))
{
    if (g_SceneManager.count + 1 > SCENES_MAX)
    {
        // TraceLog(LOG_ERROR, "SceneManager overflow");
        exit(2);
    }

    int index = g_SceneManager.count;
    g_SceneManager.scenes[index] = SceneConstructor();

    g_SceneManager.count++;
}

void Scene_Free(void)
{
    void *pData = g_SceneManager.pCurrent->pData;
    g_SceneManager.pCurrent->Free(pData);
}

void Scene_OnEnter(void)
{
    void *pData = g_SceneManager.pCurrent->pData;
    g_SceneManager.pCurrent->OnEnter(pData);
}

void Scene_OnExit(void)
{
    void *pData = g_SceneManager.pCurrent->pData;
    g_SceneManager.pCurrent->OnExit(pData);
}

void Scene_Input(void)
{
    void *pData = g_SceneManager.pCurrent->pData;
    g_SceneManager.pCurrent->Input(pData);
}

void Scene_Update(void)
{
    void *pData = g_SceneManager.pCurrent->pData;
    g_SceneManager.pCurrent->Update(pData);
}

void Scene_Draw(void)
{
    void *pData = g_SceneManager.pCurrent->pData;
    g_SceneManager.pCurrent->Draw(pData);
}
