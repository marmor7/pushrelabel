#include "Utils.h"

string Utils::printValue(int val)
{
	char s[15];
	if (val == INFINITY)
		return "INF";
	else
		return itoa(val, s, 10);
}