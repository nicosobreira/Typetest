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

bool Clock_Tick(Clock* pClock, double step);

void Clock_Reset(Clock* pClock);

#endif // CLOCK_H
