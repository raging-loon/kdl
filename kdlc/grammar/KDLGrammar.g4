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
    : IDENTIFIER ASSIGN expr SEMI_COLON
    ;

expr
    : expr binaryOp expr
    | primary
    ;

binaryOp
    : IN
    | NOT IN
    | ('<' | '<=' | '>' | '>=' | '==' | '!=')
    | AND
    | OR
    ;

primary:
    | literal
    | array_literal
    | IDENTIFIER
    | field_access
    | OPEN_PAREN primary CLOSE_PAREN
    ;

field_access
    : IDENTIFIER (DOT IDENTIFIER)*
    ;
array_literal
    : OPEN_BRACKET
        ((literal | IDENTIFIER) (',' (literal | IDENTIFIER))*)?    
      CLOSE_BRACKET

    ;

literal
    : STRING
    | INTEGER
    ;