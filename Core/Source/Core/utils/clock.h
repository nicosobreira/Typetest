#ifndef CLOCK_H
#define CLOCK_H

#include <stdbool.h>

typedef struct Clock
{
	double target;
	double previous;
	double total;
} Clock;

double Clock_Get(Clock* pClock);

void Clock_Set(Clock* pClock, double target);

// The clock step is MS_PER_UPDATE
bool Clock_Tick(Clock* pClock);

void Clock_Reset(Clock* pClock);

#endif // CLOCK_H
