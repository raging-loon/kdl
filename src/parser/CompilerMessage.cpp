#include "parser/CompilerMessage.h"
#ifdef _WIN32
# include <windows.h>

#endif // _WIN32
using kdl::CompilerMessage;
using kdl::CTokenPtr;
using kdl::message_class_t;

int CompilerMessage::m_len = -1;
const char* CompilerMessage::m_source = nullptr;
const char* CompilerMessage::m_file = nullptr;

void CompilerMessage::setSource(const char* source, const char* file, int len)
{
    m_len = len;
    m_source = source;
    m_file = file;
#ifdef _WIN32
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD consoleMode;
    GetConsoleMode(handle, &consoleMode);
    SetConsoleMode(handle, consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif // _win32

}

void CompilerMessage::error(message_class_t mc, const char* message, CTokenPtr offendingToken)
{

    error(mc, offendingToken->lineNumber, offendingToken->lineStart, offendingToken->startPos, offendingToken->val.length(), message);
}
void kdl::CompilerMessage::error(message_class_t mc, CTokenPtr offendingToken, const char* message, ...)
{
    va_list args;
    va_start(args, message);
    error(mc, offendingToken->lineNumber, offendingToken->lineStart, offendingToken->startPos, offendingToken->val.length(), message, args);
    va_end(args);

}

void CompilerMessage::error(message_class_t mc, int line, int lineStart, int startChar, int endChar, const char* message, ...)
{
    printf("\033[31;1mError\033[0;0;0m\n%s\n", msgClassToStr(mc));

    va_list args;
    va_start(args, message);
    vprintf(message, args);
    va_end(args);
    printf(": %s:%d:%d\n", m_file, line, startChar);

    print(message, line, lineStart, startChar, endChar);

}

void CompilerMessage::warning(const char* message, CTokenPtr offendingToken)
{
    printf("\033[1;98;33mWarning\033[0m\n");
    printf("%s: %s:%d:%d\n", message, m_file, offendingToken->lineNumber, offendingToken->startPos);

    print(message, offendingToken->lineNumber, offendingToken->lineStart, offendingToken->startPos, offendingToken->val.length());


}


void CompilerMessage::print(const char* message, int lineNo, int lineStart, int startChar, int endChar)
{
    if (!m_source)
    {
        printf("CompileMessage::print called with no source set\n");
        return;
    }
    if (endChar != startChar)
        endChar += startChar - 1;


    int pad = printf("\t %d | ", lineNo);

    for (int i = lineStart - 1; i < m_len; i++)
    {
        if (i >= startChar && i <= endChar)
            printf("\033[;4m");

        putchar(m_source[i]);

        if (i >= endChar)
            printf("\033[0m");

        if (m_source[i] == '\n')
            break;
    }
    putchar('\t');

    for (int i = 0; i < (pad - 3); i++)
        putchar('\x20');
    putchar('|');
    putchar(' ');

    for (int i = lineStart; i <= startChar; i++)
    {
        if (m_source[i - 1] == '\t')
            putchar('\t');
        else
            putchar('\x20');
    }

    printf("^\n\033[0m");


}

const char* CompilerMessage::msgClassToStr(message_class_t mc)
{
    switch (mc)
    {
        case message_class_t::UNKNOWN_ERROR:
            return "Unknown Error";
        case message_class_t::INVALID_SYNTAX:
            return "Invalid Syntax";
        case message_class_t::INVALID_SEMANTICS:
            return "Invalid Semantic";
        case message_class_t::UNKNOWN_VARIABLE:
            return "Unknown Variable";
        default:
            return "Invalid Error Code From Compiler Message";
    }
}
