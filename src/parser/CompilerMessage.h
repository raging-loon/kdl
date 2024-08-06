#ifndef PARSER_COMPILER_MESSAGE_H_
#define PARSER_COMPILER_MESSAGE_H_

#include "parser.h"

namespace kdl
{
enum class message_class_t 
{
	UNKNOWN_ERROR,
	INVALID_SYNTAX,
	INVALID_SEMANTICS,
	INVALID_LITERAL,
	UNKNOWN_VARIABLE
};

class CompilerMessage
{
public:

	static void setSource(const char* source, const char* file, int len);

	static void error(message_class_t, const char* message, CTokenPtr offendingToken);
	static void error(message_class_t, CTokenPtr offendingToken, const char* message, ...);
	
	static void error(message_class_t, int line, int lineStart, int startChar, int endChar, const char* message, ...);
	static void warning(const char* message, CTokenPtr offendingToken);

	static void print(const char* message, int lineNo, int lineStart, int startChar, int endChar);
	static const char* msgClassToStr(message_class_t mc);
private:

	static int m_len;
	static const char* m_source;
	static const char* m_file;

};



} // kdl


#endif // PARSER_COMPILER_MESSAGE_H_