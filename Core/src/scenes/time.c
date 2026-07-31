#include "time.h"

#ifdef _WIN32
#include <windows.h>

double getTimeMs(void)
{
    static LARGE_INTEGER frequency;
    static int initialized = 0;

    if (!initialized)
    {
        QueryPerformanceFrequency(&frequency);
        initialized = 1;
    }

    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    return (double)(counter.QuadPart * 1000.0 / frequency.QuadPart);
}

#else

#include <sys/time.h>
#include <time.h>

double getTimeMs(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return (double)((double)tv.tv_sec * 1000.0 + (double)tv.tv_usec / 1000.0);
}

#endif // _WIN32
