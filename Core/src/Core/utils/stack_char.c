#include "Core/utils/stack_char.h"

#include <stdlib.h>

#include "Core/utils/error.h"

void StackChar_Init(StackChar *pStack)
{
    pStack->array = NULL;
    pStack->length = 0;
    pStack->capacity = STACK_CAPACITY;

    pStack->array = malloc(sizeof(wchar_t) * pStack->capacity);
    if (!pStack->array)
        ERROR(10, "%s", "Alocation failed");
}

void StackChar_Free(StackChar *pStack)
{
    free(pStack->array);
    pStack->array = NULL;
    pStack->length = 0;
    pStack->capacity = 0;
}

void StackChar_Clear(StackChar *pStack)
{
    pStack->length = 0;
}

void StackChar_Push(StackChar *pStack, wchar_t new)
{
    if (pStack->length + 1 > pStack->capacity)
    {
        pStack->capacity *= 2;

        pStack->array = realloc(pStack->array, pStack->capacity);
        if (!pStack->array)
            ERROR(10, "%s", "Reallocation failed");
    }

    pStack->length++;

    pStack->array[pStack->length] = new;
}

void StackChar_Pop(StackChar *pStack)
{
    if (pStack->length == 0)
        return;

    pStack->length--;
}

wchar_t StackChar_Top(StackChar *pStack)
{
    return pStack->array[pStack->length];
}

bool StackChar_IsEmpty(StackChar *pStack)
{
    return (pStack->length == 0);
}

size_t StackChar_Size(StackChar *pStack)
{
    return (pStack->length);
}
