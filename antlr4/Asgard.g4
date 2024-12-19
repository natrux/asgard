grammar Asgard;

DOT: '.' ;
COMMA: ',' ;
EQUALS: '=' ;
PARENTHESIS_OPEN: '(' ;
PARENTHESIS_CLOSE: ')' ;
LESS_THAN: '<' ;
GREATER_THAN: '>' ;
ASTERISK: '*' ;
QUESTION_MARK: '?' ;
EXTENDS: 'extends' ;
CONST: 'const' ;
STATIC: 'static' ;
MUTABLE: 'mut' ;
VOID: 'void' ;
HANDLES: 'handles' ;
BOOL: 'true' | 'false' ;
NANOSECONDS: 'ns' ;
MICROSECONDS: 'us' ;
MILLISECONDS: 'ms' ;
SECONDS: 's' ;
MINUTES: 'm' ;
HOURS: 'h' ;
WHITESPACE: [ \t\n\r\f]+ -> skip ;

STRING
	: '"' (ESC | SAFECODEPOINT)* '"'
	;
fragment ESC
	: '\\' ["\\nt]
	;
fragment SAFECODEPOINT
	: ~ ["\\\u0000-\u001F]
	;

ID: [a-zA-Z_][a-zA-Z_0-9]* ;

NUMBER
	: '-'? NATURAL ('.' [0-9]+)?
	| '0x' [0-9a-fA-F]+
	;
fragment NATURAL
	: '0'
	| [1-9][0-9]*
	;




value
	: NUMBER
	| BOOL
	| STRING
	| duration
	;



duration: duration_unit+ ;
duration_unit
	: NUMBER 'ns'
	| NUMBER 'us'
	| NUMBER 'ms'
	| NUMBER 's'
	| NUMBER 'm'
	| NUMBER 'h'
	;

type
	: (ID '.')* ID
	| type '*'
	| type '?'
	| type '<' type (',' type)* '>'
	;

return_type
	: 'void'
	| type
	;

declaration: type ID ('=' value)? ;

types
	: '(' ')'
	| '(' type (',' type)* ','? ')'
	;

declarations
	: '(' ')'
	| '(' declaration (',' declaration)* ')'
	;

member: 'mut'? declaration ;
function: 'static' return_type ID declarations ;
method: 'const'? return_type ID declarations ;

extends: 'extends' '(' type ')' ;
handles: 'handles' types ;


module: extends (member | function | method)* handles EOF;
class: extends (declaration | function | method)* EOF;
enum: ID* EOF;


