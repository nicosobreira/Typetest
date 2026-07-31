#ifndef SCENES_H
#define SCENES_H

#include <stdbool.h>
#include <stddef.h>

#define SCENES_MAX 16

typedef struct Scene
{
    void (*Free)(void *pData);

    void (*OnEnter)(void *pData);
    void (*OnExit)(void *pData);

    void (*Input)(void *pData);
    void (*Update)(void *pData);
    void (*Draw)(void *pData);

    /// Heap allocated data pointer.
    /// This `Scene` manages the lifetime.
    void *pData;

    /// Name of the scene.
    const char *name;
} Scene;

/** Array of Scenes
 */
typedef struct SceneManager
{
    /// Array of possible scenes.
    /// Has size of `count`.
    Scene scenes[SCENES_MAX];
    Scene *pCurrent;

    /// Total of possible scenes.
    size_t count;
    bool shouldClose;
} SceneManager;

void SceneManager_Init(void);

void SceneManager_Free(void);

void SceneManager_Switch(const char *name);

void SceneManager_Loop(void);

void SceneManager_Close(void);

/** Get `pData` from a scene
 * @return The `pData` void pointer (need to cast!)
 */
void *SceneManager_GetDataByName(const char *name);

/// Add a Scene to `scenes`.
/// Also call the `Init` function.

void SceneManager_Register(Scene (*SceneConstructor)(void));

void SceneManager_RegisterScene(Scene *pScene);

void Scene_Free(void);

void Scene_OnEnter(void);

void Scene_OnExit(void);

void Scene_Input(void);

void Scene_Update(void);

void Scene_Draw(void);

#endif // SCENES_H
