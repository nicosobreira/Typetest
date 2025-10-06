#include "Core/utils/my_string.h"

#include <string.h>
#include <stdlib.h>

#include "Core/utils/error.h"

// NOTE: Need to initialize the length and _allocated
void String_New(String* pString, wchar_t* letters)
{
	if (pString == NULL)
		HANDLE_ERROR(1, "%s", "Failed to load string");

	pString->length = 0;
	pString->_allocated = 0;

	// FIX: The current way WON'T WORK with text that are not in the sample
	// Need to add dynamic allocation

	// String_AllocateMemory(pString, length);
	// wcscpy(pString->letters, letters);
	size_t length = wcslen(letters);
	pString->length = (int)length;
	pString->letters = letters;
}

void String_AllocateMemory(String *pString, size_t allocate)
{
	if (pString->_allocated > allocate)
		return;

	pString->letters = (wchar_t *)malloc(allocate * sizeof(wchar_t));
	if (pString->letters == NULL)
		HANDLE_ERROR(1, "%s", "Memory allocation failed");

	pString->_allocated += allocate;
}

wchar_t String_GetChar(String *pString, int index)
{
	if (!String_IsIndexValid(pString, index))
		HANDLE_ERROR(1, "%s", "Invalid index");

	return pString->letters[index];
}

bool String_IsIndexValid(String *pString, int index)
{
	return (index >= 0 && index < (int)pString->length + 1);
}

bool String_IsCharAtIndexEqual(String *pString, int index, wchar_t match)
{
	wchar_t character = String_GetChar(pString, index);

	return (character == match);
}

void String_Free(String* pString)
{
	// free(pString->letters);
	pString->letters = NULL;
	pString->length = 0;
	pString->_allocated = 0;
	pString = NULL;
}
