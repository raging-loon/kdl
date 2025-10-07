grammar KDLGrammar;
import KDLLexer;

startRule
    : top_decls+ EOF;

top_decls
    : rule_decl+;

rule_decl
    :   RULE IDENTIFIER 
        OPEN_BRACE
            event_source
            predicate_section
            condition_section
            action_section
        CLOSE_BRACE

    ;

event_source
    : EVT_SOURCE COLON IDENTIFIER SEMI_COLON
    ;

predicate_section
    : PREDICATE COLON statement+
    ;
condition_section
    : CONDITION COLON expr
    ;

action_section
    : ACTION COLON (IDENTIFIER (',' IDENTIFIER)*) SEMI_COLON
    ;
statement
    : assignment
    ;

assignment
    : var ASSIGN expr SEMI_COLON
    ;

expr
    : expr AND expr
    | expr OR expr
    | expr IN expr
    | expr NOT IN expr
    | primary
    ;

primary:
    | literal
    | array_literal
    | var
    | IDENTIFIER
    | field_access
    | OPEN_PAREN primary CLOSE_PAREN
    ;

field_access
    : IDENTIFIER (DOT IDENTIFIER)*
    ;
array_literal
    : OPEN_BRACKET
        (valid_array_types (',' valid_array_types)*)?    
      CLOSE_BRACKET

    ;

valid_array_types
    : literal 
    | var 
    | IDENTIFIER
    ;

var
    : VAR IDENTIFIER
    ;

literal
    : STRING
    | INTEGER
    ;