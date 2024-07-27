#ifndef UTILS_H_
#define UTILS_H_

#include <string>

namespace kdl
{

namespace utils
{


bool isNumeric(char c);

bool isSignedNumeric(char c);

bool isAlpha(char c);

bool isAlphaNumeric(char c);

void toLowerCase(std::string& str);

int hexToInt(char c);
} // utils

} // kdl


#endif // UTILS_H_