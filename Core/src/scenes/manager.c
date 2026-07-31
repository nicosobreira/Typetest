#include "Core/scenes.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "Core/constants.h"
#include "Core/error.h"

#include "time.h"

static SceneManager g_Manager = {0};

void SceneManager_Init(void)
{
    g_Manager.count = 0;
    g_Manager.pCurrent = NULL;
    g_Manager.shouldClose = false;
}

void SceneManager_Free(void)
{
    for (size_t i = 0; i < g_Manager.count; i++)
    {
        void *pData = g_Manager.scenes[i].pData;
        g_Manager.scenes[i].Free(pData);
    }
}

void SceneManager_Switch(const char *name)
{
    for (size_t i = 0; i < g_Manager.count; i++)
    {
        Scene *pScene = &g_Manager.scenes[i];
        if (strcmp(name, pScene->name) == 0)
        {
            if (g_Manager.pCurrent)
            {
                Scene_OnExit();
            }

            g_Manager.pCurrent = &g_Manager.scenes[i];
            Scene_OnEnter();
            return;
        }
    }

    ERROR(3, "Unknow scene \"%s\"", name);
}

void SceneManager_Loop(void)
{
    double previousTime = getTimeMs();
    double lag = 0.0;
    while (!g_Manager.shouldClose)
    {
        double currentTime = getTimeMs();
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

void SceneManager_Close(void)
{
    g_Manager.shouldClose = true;
}

void SceneManager_Register(Scene (*SceneConstructor)(void))
{
    if (g_Manager.count + 1 > SCENES_MAX)
    {
        ERROR(2, "%s", "Manager array overflow");
    }

    int index = g_Manager.count;
    g_Manager.scenes[index] = SceneConstructor();

    g_Manager.count++;
}

void SceneManager_RegisterScene(Scene *pScene)
{
    if (g_Manager.count + 1 > SCENES_MAX)
    {
        ERROR(2, "%s", "Manager array overflow");
    }

    int index = g_Manager.count;
    g_Manager.scenes[index] = *pScene;

    g_Manager.count++;
}

void *SceneManager_GetDataByName(const char *name)
{
    for (size_t i = 0; i < g_Manager.count; i++)
    {
        Scene *pScene = &g_Manager.scenes[i];
        if (strcmp(name, pScene->name) == 0)
        {
            return pScene->pData;
        }
    }

    ERROR(3, "Unknow scene \"%s\"", name);
    return NULL;
}

void Scene_Free(void)
{
    void *pData = g_Manager.pCurrent->pData;
    g_Manager.pCurrent->Free(pData);
}

void Scene_OnEnter(void)
{
    void *pData = g_Manager.pCurrent->pData;
    g_Manager.pCurrent->OnEnter(pData);
}

void Scene_OnExit(void)
{
    void *pData = g_Manager.pCurrent->pData;
    g_Manager.pCurrent->OnExit(pData);
}

void Scene_Input(void)
{
    void *pData = g_Manager.pCurrent->pData;
    g_Manager.pCurrent->Input(pData);
}

void Scene_Update(void)
{
    void *pData = g_Manager.pCurrent->pData;
    g_Manager.pCurrent->Update(pData);
}

void Scene_Draw(void)
{
    void *pData = g_Manager.pCurrent->pData;
    g_Manager.pCurrent->Draw(pData);
}
