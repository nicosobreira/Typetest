#include "time.h"

#include <stddef.h>

#ifdef _WIN32

#include <windows.h>

double getCurrentTimeMs(void)
{
    return (double)GetTickCount64();
}

#else

#include <sys/time.h>

double getCurrentTimeMs(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return (double)((long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000);
}
#endif
