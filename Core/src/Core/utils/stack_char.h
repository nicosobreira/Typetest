#ifndef STACK_CHAR_H
#define STACK_CHAR_H

#include <stdbool.h>
#include <wchar.h>

#define STACK_CAPACITY (1024)

typedef struct StackChar
{
    wchar_t *array;
    size_t length;
    size_t capacity;
} StackChar;

void StackChar_Init(StackChar *pStack);

void StackChar_Free(StackChar *pStack);

void StackChar_Clear(StackChar *pStack);

void StackChar_Push(StackChar *pStack, wchar_t new);

void StackChar_Pop(StackChar *pStack);

wchar_t StackChar_Top(StackChar *pStack);

bool StackChar_IsEmpty(StackChar *pStack);

size_t StackChar_Size(StackChar *pStack);

#endif // STACK_CHAR_H
