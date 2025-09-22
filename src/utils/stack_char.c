#include "utils/stack_char.h"

#include <stdlib.h>

#include "utils/error.h"

void StackChar_New(StackChar* pStack)
{
	pStack->head = NULL;
	pStack->size = 0;
}

void StackChar_Push(StackChar* pStack, wchar_t newChar)
{
	NodeChar* node = (NodeChar *)malloc(sizeof(NodeChar));
	if (node == NULL)
		HANDLE_ERROR(1, "%s", "Failed to create a new stack node");

	node->character = newChar;

	node->next = pStack->head;
	pStack->head = node;

	pStack->size++;
}

void StackChar_Pop(StackChar* pStack)
{
	if (pStack->head == NULL)
		return;

	NodeChar* node = NULL;

	node = pStack->head->next;
	free(pStack->head);
	pStack->head = node;

	pStack->size--;
}

wchar_t StackChar_Top(StackChar* pStack)
{
	return pStack->head->character;
}

bool StackChar_IsEmpty(StackChar* pStack)
{
	return (pStack->size <= 0);
}

int StackChar_Size(StackChar* pStack)
{
	return (pStack->size);
}

void StackChar_Free(StackChar* pStack)
{
	if (StackChar_IsEmpty(pStack))
		return;

	while (pStack->head->next != NULL)
		StackChar_Pop(pStack);

	pStack->size = 0;
	pStack = NULL;
}
