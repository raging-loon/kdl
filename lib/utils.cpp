#include "utils.h"
#include <string>
using namespace kdl;




bool utils::isNumeric(char c)
{
	return (c >= '0' && c <= '9');
}

bool utils::isSignedNumeric(char c)
{
	return ((c >= 0x30 && c <= 0x39) || c == '-' || c == '+');
}

bool utils::isAlpha(char c)
{
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool utils::isAlphaNumeric(char c)
{
	return utils::isNumeric(c) || utils::isAlpha(c) || c == '_';
}

void utils::toLowerCase(std::string& str)
{
	for (int i = 0; i < str.size(); i++)
	{
		char cur = str.data()[i];

		if (cur >= 'Z')
			// todo: see if a shift would make this faster
			str.data()[i] = cur + 32;
	}
}

int utils::hexToInt(char c)
{
	int res = 0;

	if (utils::isNumeric(c))
		res = c - '0';
	else if (c >= 'a' && c <= 'f')
		res = (c - 'a') + 10;
	else if (c >= 'A' && c <= 'F')
		res = (c - 'A') + 10;
	else
		return -1;

	return res;
}
