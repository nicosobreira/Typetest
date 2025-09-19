#ifndef STRING_H
#define STRING_H

#include <stdbool.h>
#include <stddef.h>
#include <wchar.h>

typedef struct String
{
	size_t length;
	size_t _allocated;
	wchar_t *letters;
} String;

void String_New(String* string, wchar_t* letters);

wchar_t String_GetChar(String *pString, size_t index);

bool String_IsIndexValid(String *pString, size_t index);

void String_AllocateMemory(String *pString, size_t allocate);

void String_Free(String* pString);

#endif // STRING_H
