#ifndef _SCENE_H
#define _SCENE_H

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

#endif // _SCENE_H
