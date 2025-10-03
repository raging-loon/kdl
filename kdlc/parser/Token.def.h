

#ifndef KDL_TOKEN
#   define KDL_TOKEN(name) name
#endif

#ifndef KDL_SYMBOL_TOKEN
#   define KDL_SYMBOL_TOKEN(name, sym) KDL_TOKEN(name)
#endif

#ifndef KDL_KEYWORD_TOKEN
#   define KDL_KEYWORD_TOKEN(name, sym) KDL_TOKEN(name)
#endif

KDL_TOKEN(ERROR)

KDL_SYMBOL_TOKEN(OPEN_PARENTHESIS,      "(")
KDL_SYMBOL_TOKEN(CLOSE_PARENTHESIS,     ")")
KDL_SYMBOL_TOKEN(OPEN_BRACE,            "{")
KDL_SYMBOL_TOKEN(CLOSE_BRACE,           "}")
KDL_SYMBOL_TOKEN(OPEN_BRACKET,          "[")
KDL_SYMBOL_TOKEN(CLOSE_BRACKET,         "[")
KDL_SYMBOL_TOKEN(COMMA,                 ",")
KDL_SYMBOL_TOKEN(SEMI_COLON,            ";")
KDL_SYMBOL_TOKEN(COLON,                 ":")
KDL_SYMBOL_TOKEN(DOT,                   ".")
KDL_SYMBOL_TOKEN(ASSIGNMENT,            "=")
KDL_SYMBOL_TOKEN(ASTERISK,              "*")
KDL_SYMBOL_TOKEN(PLUS,                  "+")
KDL_SYMBOL_TOKEN(MINUS,                 "-")
KDL_SYMBOL_TOKEN(LOGICAL_NOT,           "!")
KDL_SYMBOL_TOKEN(EQUALS,                "==")
KDL_SYMBOL_TOKEN(LEQ,                   "<=")
KDL_SYMBOL_TOKEN(LT,                    "<")
KDL_SYMBOL_TOKEN(NE,                    "!=")
KDL_SYMBOL_TOKEN(GT,                    ">")
KDL_SYMBOL_TOKEN(GEQ,                   ">=")
KDL_SYMBOL_TOKEN(VARIABLE,              "$")
KDL_KEYWORD_TOKEN(RULE,                 "rule")
KDL_KEYWORD_TOKEN(EVT_SOURCE,           "evt_source")
KDL_KEYWORD_TOKEN(CONDITION,            "condition")
KDL_KEYWORD_TOKEN(ACTION,               "action")
KDL_KEYWORD_TOKEN(PREDICATE,            "predicate")
KDL_KEYWORD_TOKEN(META,                 "meta")
KDL_KEYWORD_TOKEN(NOT,                  "not")
KDL_KEYWORD_TOKEN(IN,                   "in")
KDL_KEYWORD_TOKEN(CONST,                "const")
KDL_KEYWORD_TOKEN(AND,                  "and")
KDL_KEYWORD_TOKEN(OR,                   "or")
KDL_KEYWORD_TOKEN(ANY,                  "any")
KDL_KEYWORD_TOKEN(ALL,                  "all")
KDL_KEYWORD_TOKEN(OF,                   "of")
KDL_KEYWORD_TOKEN(THEM,                 "them")


KDL_TOKEN(STRING)
KDL_TOKEN(REGEX)
KDL_TOKEN(INTEGER)
KDL_TOKEN(IDENTIFIER)



#undef KDL_TOKEN
#undef KDL_SYMBOL_TOKEN
#undef KDL_KEYWORD_TOKEN