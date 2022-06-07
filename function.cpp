#include "function.h"

void Clamp(float& n, float max, float min)
{
	if (n > max) { n = max; }
	if (n < min) { n = min; }
}

bool isOver(float n, float max, float min)
{
	if (n > max || n < min) { return 1; }
	return 0;
}