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


ID: [a-zA-Z_][a-zA-Z_0-9]* ;

STRING: '"' (ESC | SAFECODEPOINT)* '"' ;
fragment ESC : '\\' ["\\nt] ;
fragment SAFECODEPOINT: ~ ["\\\u0000-\u001F] ;

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

classname: (ID '.')* ID ;

type
	: classname
	| template_type
	| type '*'
	| type '?'
	;

template_type: classname '<' type (',' type)* '>' ;

return_type
	: 'void'
	| type
	;

declaration: type ID ('=' value)? ;

parameters
	: '(' ')'
	| '(' declaration (',' declaration)* ')'
	;

member: declaration ;
static_member: 'static' declaration ;
module_member: 'mut'? declaration ;
function: 'static' return_type ID parameters ;
method: 'const'? return_type ID parameters ;

extends: 'extends' '(' classname ')' ;
handles: 'handles' '(' classname* ')' ;


module: extends (module_member | function | method)* handles? EOF;
class: extends (static_member | member | function | method)* EOF;
enum: ID* EOF;


