#ifndef STACK_CHAR_H
#define STACK_CHAR_H

#include <wchar.h>
#include <stdbool.h>

typedef struct NodeChar
{
	wchar_t character;
	struct NodeChar* next;
} NodeChar;

typedef struct StackChar
{
	NodeChar* head;
	int size;
} StackChar;

void StackChar_New(StackChar* pStack);

void StackChar_Push(StackChar* pStack, wchar_t newChar);

void StackChar_Pop(StackChar* pStack);

wchar_t StackChar_Top(StackChar* pStack);

bool StackChar_IsEmpty(StackChar* pStack);

int StackChar_Size(StackChar* pStack);

void StackChar_Free(StackChar* pStack);

#endif // STACK_CHAR_H
