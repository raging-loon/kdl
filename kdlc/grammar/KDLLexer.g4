lexer grammar KDLLexer;


/// LEXING RULES
AND             : 'and';
OR              : 'or';
RULE            : 'rule';
EVT_SOURCE      : 'evt_source' | 'event_source';
CONDITION       : 'condition';
PREDICATE       : 'predicate';
META            : 'meta';
IN              : 'in';
NOT             : 'not';
ACTION          : 'action';

OPEN_PAREN      : '(';
CLOSE_PAREN     : ')';
OPEN_BRACE      : '{';
CLOSE_BRACE     : '}';
OPEN_BRACKET    : '[';
DOT             : '.';
CLOSE_BRACKET   : ']';
COLON           : ':';
SEMI_COLON      : ';';
ASSIGN          : '=';
VAR             : '$';
IDENTIFIER      : [a-zA-Z] [a-zA-Z0-9_]* '*'?;
INTEGER         : [0-9]+;
COMMENT         : '#' ~[\r\n]* -> channel(HIDDEN);
STRING          : '"' (~[\\"] | ESCAPE_CHARS)* '"';
WS              : [ \t\r\n]+ -> skip;

fragment ESCAPE_CHARS   : '\\' [tnr"\\];
fragment HEX_DIGIT      : [0-9a-fA-F];
fragment DEC_DIGIT      : [0-9];
fragment ALPHANUM_UNDERSCORE    : [a-zA-Z0-9_];
