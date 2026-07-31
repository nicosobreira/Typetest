#ifndef STRING_H
#define STRING_H

#include <stdbool.h>
#include <stddef.h>
#include <wchar.h>

#include "Core/data_structures/stack_char.h"

/// A StackChar wrapper that doesn't own it's memory
typedef struct String
{
    StackChar buffer;
} String;

void String_New(String *string, wchar_t *letters);

wchar_t String_GetAt(String *pString, size_t index);

const wchar_t *String_GetPointerAt(String *pString, size_t index);

size_t String_Length(String *pString);

bool String_IsIndexValid(String *pString, size_t index);

bool String_IsCharAtIndexEqual(String *pString, size_t index, wchar_t match);

void String_Clear(String *pString);

#endif // STRING_H
