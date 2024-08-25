#include "parser/lex.h"
#include "utils.h"

#include "parser/CompilerMessage.h"
using namespace kdl;

KeywordMap Lexer::m_keywords =
{
    {"rule", token_t::SI_RULES},
    {"meta", token_t::SI_META},
    {"condition", token_t::SI_CONDITIONS},
    {"content", token_t::SI_CONTENT},
    {"and", token_t::KW_AND},
    {"or", token_t::KW_OR},
    {"nocase", token_t::MOD_NOCASE},
    {"ascii", token_t::MOD_ASCII},
    {"wide", token_t::MOD_WIDE},
    {"of", token_t::CND_OF},
    {"them", token_t::CND_THEM},
    {"any", token_t::CND_ANY},
    {"all", token_t::CND_ALL},
    {"at", token_t::CND_AT},
};


Lexer::Lexer(const char* source, int len)
    : m_source(source), m_srcLen(len), m_tokens({})
{
}

int Lexer::scan()
{
    while (!atEnd())
    {
        if (m_wasError)
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
        case '(':
        {

            addToken(token_t::OPEN_PARENTHESIS); break;
        }
        case ')': addToken(token_t::CLOSE_PARENTHESIS); break;
        case '{': addToken(token_t::OPEN_BRACE); break;
        case '}': addToken(token_t::CLOSE_BRACE); break;
        case ':': addToken(token_t::COLON); break;
        case '$': addToken(token_t::TI_VARIABLE); break;
        case '=':
        {
            // test for ==
            match('=') ?
                addToken(token_t::KW_EQUALITY) :
                addToken(token_t::ASSIGNMENT);
            break;
        }
        case '<':
        {
            // test for <=
            match('=') ?
                addToken(token_t::KW_LEQ) :
                addToken(token_t::KW_LT);
            break;
        }
        case '>':
        {
            // test for >=
            match('=') ?
                addToken(token_t::KW_GEQ) :
                addToken(token_t::KW_GT);
            break;
        }
        case '*': addToken(token_t::STAR); break;

        case ' ':
        case '\t':
        case '\0':
        case '\r':
            break;
        case '\n':
            m_curLine++;
            m_lineStart = m_current + 1;
            break;

        case '"': scanString();		break;
        case '|': scanByteSearch(); break;
        case '/': scanRegex();		break;
        default:

            if (utils::isNumeric(c))
                scanInteger();
            else if (utils::isAlpha(c))
                scanIdentifierOrKeyword();
            else
                showError("Unknown token");
    }
}
char Lexer::peek()
{

    if (atEnd()) return '\0';

    return m_source[m_current];
}

bool Lexer::match(char n)
{
    if (atEnd()) return false;

    if (m_source[m_current] != n) return false;

    m_current++;
    return true;
}
char Lexer::previous()
{
    if (m_current == 0)
        return -1;
    return m_source[m_curLine - 1];
}
// todo: remove code duplication
void Lexer::addToken(token_t tok)
{
    m_tokens.push_back({
        .t = tok,
        .startPos = m_start,
        .lineStart = m_lineStart,
        .lineNumber = m_curLine

        });
}

void Lexer::addToken(token_t tok, std::string& val)
{
    m_tokens.push_back({
        .t = tok,
        .val = val,
        .startPos = m_start,
        .lineStart = m_lineStart,
        .lineNumber = m_curLine
        });
}

void Lexer::addToken(token_t tok, int startPos, int endPos)
{
    m_tokens.push_back({
        .t = tok,
        .val = std::string(m_source + startPos, m_source + endPos),
        .startPos = m_start,
        .lineStart = m_lineStart,
        .lineNumber = m_curLine


        });
}

void Lexer::scanIdentifierOrKeyword()
{
    while (utils::isAlphaNumeric(peek())) advance();

    std::string test((m_source + m_start), (m_source + m_current));

    //printf("%s s:%d e:%d\n", test.c_str(), m_start, m_current);
    auto posKw = m_keywords.find(test);

    // is this a keyword?
    if (posKw != m_keywords.end())
    {
        addToken(
            posKw->second
        );
    }
    // if not it must be an identifier or variable
    else
    {
        // match something like '$str*' as in 'all of $str*'
        if (peek() == '*')
        {
            addToken(
                token_t::MULTI_VAR_IDENTIFIER,
                test
            );
            advance();
        }
        else
        {
            // match a single variable like '$str1'

            // -2 because of the advance in the loop. -1 is the name, 0 is the next symbol
            // $str = 1 
            //      ^ we are here
            if (m_source[m_start - 1] == '$')
                addToken(token_t::SINGLE_VAR_IDENTIFIER, test);
            else
                addToken(token_t::IDENTIFIER, test);

        }
    }
}

void Lexer::scanInteger()
{
    while (utils::isNumeric(peek())) advance();

    if (utils::isAlpha(peek()))
        scanSize();
    else
        addToken(token_t::INTEGER, m_start, m_current);
}

void Lexer::scanSize()
{
    char n = peek();

    int curCopy = m_current;

    char sizeIndicator[3] = { 0,0, 0 };

    // byte, e.g. 10B
    if (n == 'B' || n == 'b')
    {
        sizeIndicator[0] = n;
        advance();
    }
    // KB - GB
    else if (n == 'M' || n == 'm' || n == 'K' ||
        n == 'k' || n == 'g' || n == 'G')
    {
        advance();
        sizeIndicator[0] = n;

        char sb = peek();

        if (sb == 'B' || sb == 'b')
        {
            sizeIndicator[1] = sb;
            advance();
        }

        else
        {
            showError("Invalid Size indicator. Must end with 'B' or 'b'");
            return;
        }
    }
    else
    {
        showError("Invalid Size indicator. Must start with one of these characters: 'KkMmGg'");
        return;
    }


    int target = formatSize(m_start, curCopy, sizeIndicator);
    // TODO vvvv optimize
    auto str = std::to_string(target);
    addToken(token_t::INTEGER, str);
    return;
}


void Lexer::scanString()
{
    int curline = m_curLine;
    int start = m_start;
    int curlinestart = m_lineStart;
    scanSection('"');
    if (m_curLine != curline)
    {
        m_wasError = true;
        CompilerMessage::error(
            message_class_t::INVALID_SYNTAX,
            curline, curlinestart, start, start,
            "No closing quote found"
        );
        return;
    }
    addToken(token_t::STRING, m_start + 1, m_current - 1);
}

void kdl::Lexer::scanRegex()
{
    scanSection('/');
    // we want to keep the '/''s. 
    // They are necessary for processing
    addToken(token_t::REGEXP, m_start, m_current);
}

void kdl::Lexer::scanByteSearch()
{
    scanSection('|');

    int substringStart = m_start + 1;
    int substringEnd = m_current;

    // Strip leading and ending white-spaces
    for (int i = substringStart; i < substringEnd; i++)
    {
        if (m_source[i] != ' ') break;
        substringStart++;
    }
    for (int i = substringEnd - 1; i > substringStart; --i)
    {
        if (m_source[i] != ' ' && m_source[i] != '|') break;
        substringEnd--;
    }

    addToken(token_t::BYTE_SEQ, substringStart, substringEnd);

    int possibleErrorOffset = formatByteSequence(m_tokens.back().val);

    if (possibleErrorOffset != -1)
    {
        m_start += possibleErrorOffset;
        showError("Invalid hexadecimal");

    }
}

void Lexer::scanSection(char target)
{

    while (!atEnd())
    {
        if (peek() == target)
        {
            if (m_source[m_current - 1] == '\\')
            {
                advance();
                continue;
            }
            break;
        }
        else if (peek() == '\n')
            m_curLine++;
        advance();
    }

    if (atEnd())
        assert(false);

    advance();

}

void Lexer::showError(const char* message)
{
    m_wasError = true;

    CompilerMessage::error(
        message_class_t::INVALID_SYNTAX,
        m_curLine, m_lineStart, m_start, m_current,
        "\nUnknown token/operator/identifier"
    );

}

bool Lexer::validateRegex()
{
    return false;
}

int Lexer::formatByteSequence(std::string& input)
{
    int charsParsed = 0;
    int position = 0;

    char* formatted = new char[input.size()];
    memset(formatted, 0, input.size());
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] != ' ')
        {
            int num = utils::hexToInt(input[i]);

            if (num == -1)
                return i;

            if (charsParsed % 2 == 0)
            {
                // set the upper nibble
                formatted[position] = (num << 4);
            }
            else
            {
                // set the lower nibble
                formatted[position] |= num;
                ++position;
            }

            ++charsParsed;
        }
    }

    input.clear();
    input.assign(formatted, charsParsed / 2);
    delete[] formatted;

    return -1;
}

uint64_t Lexer::formatSize(int numStart, int numEnd, char input[3])
{
    // expect that error checking has already occured
    // assume that mainnum is a byte quantity


    char numberBuffer[10];
    int i;
    for (i = 0; i < (numEnd - numStart) && i < 9; i++)
        numberBuffer[i] = m_source[numStart + i];

    numberBuffer[i] = 0;

    int mainnum = std::atoi(numberBuffer);

    switch (input[0])
    {
        case 'B':
        case 'b':
            return mainnum;
        case 'K':
        case 'k':
            return mainnum * (2 << 9);
        case 'M':
        case 'm':
            return mainnum * (2 << 19);
        case 'G':
        case 'g':
            return mainnum * (2 << 29);
            /*	case 'T':
                case 't':
                    return mainnum * (2 << 39);*/
        default:
            return -1;
    }

}





void Lexer::dumpTokens()
{
    printf("\n[=================]\n");
    for (auto& tok : m_tokens)
    {

        printf("[%s]", getTokenName(tok.t));
        if (!tok.val.empty())
            printf(" %s", tok.val.c_str());
        putchar('\n');

    }
    printf("[=================]\n\n");

}

