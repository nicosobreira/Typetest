#ifndef STRING_H
#define STRING_H

#include <stdbool.h>
#include <stddef.h>
#include <wchar.h>

typedef struct String
{
	int length;
	size_t _allocated;
	wchar_t *letters;
} String;

void String_New(String* string, wchar_t* letters);

wchar_t String_GetChar(String *pString, int index);

bool String_IsIndexValid(String *pString, int index);

bool String_IsCharAtIndexEqual(String *pString, int index, wchar_t match);

void String_AllocateMemory(String *pString, size_t allocate);

void String_AddChar(String *pString, wchar_t character);

void String_PopChar(String *pString);

void String_Free(String* pString);

#endif // STRING_H
