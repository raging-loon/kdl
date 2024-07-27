#ifndef PARSER_ERROR_PRINTER_H_
#define PARSER_ERROR_PRINTER_H_

#include "parser/parser.h"
namespace kdl
{

// TODO: Fix this when multithreading is added


///
/// PURPOSE
///		Print errors based on line starts and character positions
/// 
class ErrorPrinter
{
public:
	enum ErrorType
	{
		UNKNOWN_TOKEN,
		SYNTAX_ERROR,
		UNKNOWN_SYMBOL
	} ;


	static void printViaToken(
		ErrorType t,
		const char* message,
		CTokenPtr targetToken
	);

	static void print(
		ErrorType t,
		const char* message,
		int lineNumber,
		int lineStart,
		int startCharacter,
		int endCharacter = 0
	);

	static void setCurrentSource(const char* filename, const char* source, int len);

private:
	static const char* m_curSource;
	static int m_sourceLen;
	static const char* m_filename;

	static const char* fromErrorCode(ErrorType t);

};

} // kdl


#endif // PARSER_ERROR_PRINTER_H_