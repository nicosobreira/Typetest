#include "core/text_entry.h"

#include <stdlib.h> // rand, srand

#include "assets/samples.h"

TextEntry* TextEntry_RandomText(void)
{
	size_t randomEntry = rand() % TEXT_SAMPLES_LENGTH;

	TextEntry* pEntry = &TEXT_SAMPLES[randomEntry];

	return pEntry;
}
