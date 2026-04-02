#include "string.h"

#include <wchar.h>

#include "Core/utils/error.h"

void String_New(String *pString, wchar_t *letters)
{
    pString->buffer.array = letters;
    pString->buffer.length = wcslen(letters);
    pString->buffer.capacity = pString->buffer.length;
}

wchar_t String_GetAt(String *pString, size_t index)
{
    if (!String_IsIndexValid(pString, index))
        ERROR(1, "%s", "Invalid index");

    return pString->buffer.array[index];
}

const wchar_t *String_GetPointerAt(String *pString, size_t index)
{
    if (!String_IsIndexValid(pString, index))
        ERROR(1, "%s", "Invalid index");

    return &pString->buffer.array[index];
}

size_t String_Length(String *pString)
{
    return pString->buffer.length;
}

bool String_IsIndexValid(String *pString, size_t index)
{
    return (index < String_Length(pString));
}

bool String_IsCharAtIndexEqual(String *pString, size_t index, wchar_t match)
{
    wchar_t character = String_GetAt(pString, index);

    return (character == match);
}

void String_Clear(String *pString)
{
    pString->buffer.array = NULL;
    pString->buffer.length = 0;
    pString->buffer.capacity = 0;
}
