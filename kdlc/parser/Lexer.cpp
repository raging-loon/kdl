#include "Lexer.h"

#include <cstdio>
#include <unordered_map>
#include <format>
#include "context/CompilerContext.h"
#include "interface/Reporting.h"

namespace kdl
{

static inline bool IsNumeric(char c)
{
    return c >= '0' && c <= '9';
}

static inline bool IsNumOrHex(char c)
{
    return IsNumeric(c)
        || (c >= 'a' && c <= 'f')
        || (c >= 'A' && c <= 'F');
}

static inline bool IsAlphaNumeric(char c)
{
    return IsNumeric(c)
        || (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || (c == '_');
}




Lexer::Lexer(FileID id)
    : m_source{ nullptr },
    m_fileID{id},
    m_tokens{ },
    m_current{ 0 },
    m_start{ 0 },
    m_currentLine{ 0 },
    m_lineStartPos{ 0 },
    m_error{ false }
{
    m_source = SRC_MGR.getSourceFile(id);
    assert(m_source);
}

int Lexer::scan()
{
    while (!atEnd())
    {
        if (m_error)
            return -1;

        m_start = m_current;
        
        scanToken();
    }

    return m_tokens.size();
}


void Lexer::scanToken()
{
    char c = advance();

    switch (c)
    {
        case '(': addToken(KDL_T_OPEN_PARENTHESIS); break;
        case ')': addToken(KDL_T_CLOSE_PARENTHESIS); break;
        
        case '{': addToken(KDL_T_OPEN_BRACE); break;
        case '}': addToken(KDL_T_CLOSE_BRACE); break;
        
        case '[': addToken(KDL_T_OPEN_BRACKET); break;
        case ']': addToken(KDL_T_CLOSE_BRACKET); break;

        case ':': addToken(KDL_T_COLON); break;
        case ';': addToken(KDL_T_SEMI_COLON); break;

        case ',': addToken(KDL_T_COMMA); break;
        case '=': {
            // test for '=='
            match('=')
                ? addToken(KDL_T_EQUALS)
                : addToken(KDL_T_ASSIGNMENT);

            break;
        }

        case '>': {
            // test for '>='
            match('=')
                ? addToken(KDL_T_GEQ)
                : addToken(KDL_T_GT);

            break;
        }

        case '<': {
            // test for '<='
            match('=')
                ? addToken(KDL_T_LEQ)
                : addToken(KDL_T_LT);

            break;
        }

        case '!': {
            // test for '!='
            match('=')
                ? addToken(KDL_T_NE)
                : addToken(KDL_T_NOT);
            break;
        }
        case '$': addToken(KDL_T_VARIABLE); break;
        case '*': addToken(KDL_T_ASTERISK); break;
        case '.': addToken(KDL_T_DOT); break;
        case ' ':
        case '\t':
        case '\r':
            break;
        
        case '\n': nextLine(); break;
        
        case '#': scanComment(); break;
        
        case '\'':
        case '"':
            scanString();
            break;
        default:

            // scan number first to avoid variable names 
            // that start with numbers
            if (IsNumeric(c))
                scanNumber();
            else if (IsAlphaNumeric(c))
                scanIdentifierOrKeyword();
            else
                showError("Unknown Token", m_start);
            break;
    }

}

char Lexer::peek()
{
    if (atEnd())
        return '\0';

    return m_source->contents[m_current];
}

bool Lexer::match(char n)
{
    if (peek() == n)
    {
        m_current++;
        return true;
    }
    return false;
}

char Lexer::previous()
{
    if (m_current == 0)
        return '\0';
    return m_source->contents[m_current - 1];
}

void Lexer::nextLine()
{
    m_currentLine++;
    m_lineStartPos = m_current + 1;
}

void Lexer::scanComment()
{
    while (peek() != '\n' && !atEnd())
        advance();

    nextLine();
}

void Lexer::scanSection(char target, bool errorOnNewline)
{
    while (!atEnd())
    {
        if (peek() == target)
        {
            // target is escaped, so include it
            if (previous() == '\\')
            {
                advance();
                continue;
            }
            break;
        }
        else if (peek() == '\n')
        {
            if (errorOnNewline)
            {
                m_error = true;
                return;
            }
            else
                nextLine();
        }


        advance();
    }

    if (atEnd())
        assert(false);

    advance();


}

void Lexer::scanString()
{
    char strChar = previous();

    assert(strChar == '\'' || strChar == '"');
    // start of string, for substr or error
    int startChar  = m_start;

    scanSection(strChar, true);

    if (m_error)
    {
        showError("No closing quote found", startChar);
        return;
    }

    addTokenSubstring(
        KDL_T_STRING,
        // +1/-1 to get rid of quotes
        startChar + 1, m_current - 1
    );
    

}

void Lexer::scanNumber()
{
    int radix = 10;
    int numStart = m_start;

    if (previous() == '0')
    {
        char next = peek();

        if (next == 'b' || next == 'B')
        {
            radix = 2;
            numStart += 2;
            advance();
        }
        if (next == 'x' || next == 'X')
        {
            radix = 16;
            numStart += 2;
            advance();
        }
    }

    while (IsNumOrHex(peek()))
        advance();

    // now validate
    auto num = getSubView(numStart, m_current);
    long ret = 0;
    auto result = std::from_chars(
        num.data(),
        num.data() + num.size(),
        ret,
        radix
    );

    if (result.ec != std::errc{})
    {
        showNumericError(result.ec, radix);
        return;
    }

    if (result.ptr != num.data() + num.size())
    {
        showError("Invalid number or identifier", m_start, m_current);
        return;
    }
    addTokenString(KDL_T_INTEGER, num);

}

void Lexer::scanIdentifierOrKeyword()
{
    while (IsAlphaNumeric(peek()))
        advance();

    auto text = getSubView(m_start, m_current);

    // test if it's a key word. 
    // if so, add the appropriate token, otherwise it's an identifier

    token_t kw = GetKeyWord(text);
    
    if (kw != KDL_T_ERROR)
        addToken(kw);
    else {
        addTokenString(
            KDL_T_IDENTIFIER,
            text
        );
    }
}

std::string_view Lexer::getSubView(size_t start, size_t end)
{
    std::string_view view{
        m_source->contents.c_str() + start,
        (end - start)
    };
    return view;
}

void Lexer::addToken(token_t tok)
{
    m_tokens.push_back({
        .token = tok,
        .startPosition = m_start,
        .lineStart = m_lineStartPos,
        .lineNumber = m_currentLine
    });
}

void Lexer::addTokenString(token_t tok, std::string_view str)
{
    m_tokens.push_back({
           .token = tok,
           .value = str,
           .startPosition = m_start,
           .lineStart = m_lineStartPos,
           .lineNumber = m_currentLine
    });
}

void Lexer::addTokenSubstring(
    token_t tok,
    size_t start,
    size_t end
)
{
    std::string_view view = getSubView(start, end);
    addTokenString(
        tok, view
    );
}

void Lexer::dumpTokens()
{
    std::printf("\n[==========================]\n");

    for (const auto& token : m_tokens)
    {
        std::printf("[%s]", GetTokenName(token.token).data());

        if (!token.value.empty())
        {
            std::printf(
                " %s", std::format("{}", token.value).c_str()
            );
        }

        std::putchar('\n');

    }

    std::printf("\n[==========================]\n");
}

ReportInfo& Lexer::getReportInfo(int startChar, int endChar)
{
    SourceLocation sl{};
    
    sl.lineNumber = m_currentLine;
    sl.lineOffset = m_lineStartPos;
    sl.locationStart = startChar;
    sl.locationEnd = endChar;

    ReportInfo ri{};
    ri.fileID = m_fileID;
    ri.location = sl;
    return ri;
}

void Lexer::showError(
    const std::string_view& message,
    int startChar,
    int endChar
)
{
    
    ReportInfo ri = getReportInfo(startChar, endChar);
    Report::Error(
        ri,
        message
    );
    m_error = true;
}

void Lexer::showNumericError(std::errc& ec, int radix)
{
    std::string_view message{};

    switch (ec)
    {
        case std::errc::invalid_argument:
            message = "Invalid input for base {} number";
            break;
        case std::errc::result_out_of_range:
            message = "Base {} number out of range";
            break;
        default:
            message = "Unknown error while parsing base {} number";
            break;
    }

    std::string formatted = std::vformat(message, std::make_format_args(radix));

    showError(
        formatted,
        m_start,
        m_current
    );
}

} // kdl