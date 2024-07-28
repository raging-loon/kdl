#include "parser/ErrorPrinter.h"

using kdl::ErrorPrinter;
using kdl::CTokenPtr;


const char* ErrorPrinter::m_curSource = nullptr;
const char* ErrorPrinter::m_filename = nullptr;
int ErrorPrinter::m_sourceLen = -1;


void ErrorPrinter::printViaToken(ErrorType t, const char* message, CTokenPtr targetToken)
{
	print(t, message, targetToken->lineNumber, targetToken->lineStart, targetToken->startPos);

}

void ErrorPrinter::print(ErrorType t, const char* message, int lineNumber, int lineStart, int startCharacter, int endCharacter)
{
	if (!m_curSource)
	{
		printf("ErrorPrinter called. No Source has been set\n");
		return;
	}

	printf("%s\n",fromErrorCode(t));
	printf("%s: %s:%d:%d\n",message, m_filename, lineNumber, startCharacter);

	int pad =printf("\t %d | ", lineNumber);

	for (int i = lineStart-1; i < m_sourceLen; i++)
	{
		putchar(m_curSource[i]);
		if (m_curSource[i] == '\n')
			break;
	}
	putchar('\t');

	for(int i = 0; i < (pad - 3); i++)
		putchar('\x20');
	putchar('|');
	putchar(' ');

	for (int i = lineStart; i <= startCharacter; i++)
	{
		if (m_curSource[i-1] == '\t')
			putchar('\t');
		else
			putchar('\x20');
	}

	putchar('^');
	putchar('\n');
}

void ErrorPrinter::setCurrentSource(const char* filename, const char* source, int len)
{

	m_curSource = source;
	m_sourceLen = len;
	m_filename = filename;
}

const char* ErrorPrinter::fromErrorCode(ErrorType t)
{
	switch (t)
	{
		case SYNTAX_ERROR: 
			return "Syntax Error";
		case UNKNOWN_SYMBOL: 
			return "Unknown Symbol";
		case UNKNOWN_TOKEN: 
			return "Unknown Token";
		default: 
			return "Unknown Error";


	}
}
