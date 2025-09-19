#include "text_entry.h"

#include "samples.h"
#include "error.h"

// BUG: Need to check if randomEntry is valid
void Sample_Loader(String* pSentence, size_t randomEntry)
{
	if (randomEntry >= TEXT_SAMPLES_LENGTH)
		HANDLE_ERROR(1, "%s", "Random entry is larger than the samples length");

	String_New(pSentence, TEXT_SAMPLES[randomEntry].text);
}
