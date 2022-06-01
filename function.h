#pragma once
void Clamp(float& n, float max=75.0f, float min=-75.0f)
{
	if (n > max) { n = max; }
	if (n < min) { n = min; }
}

bool isOver(float n, float max=75.0f, float min=-75.0f)
{
	if (n > max || n < min) { return 1; }
}