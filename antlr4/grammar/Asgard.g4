grammar Asgard;

DOT: '.' ;
COMMA: ',' ;
EQUALS: '=' ;
PARENTHESIS_OPEN: '(' ;
PARENTHESIS_CLOSE: ')' ;
BRACKET_OPEN: '[' ;
BRACKET_CLOSE: ']' ;
BRACE_OPEN: '{' ;
BRACE_CLOSE: '}' ;
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

TYPE_SIZE_T: 'size_t' ;
TYPE_U8: 'u8' ;
TYPE_I8: 'i8' ;
TYPE_I32: 'i32' ;
TYPE_I64: 'i64' ;
TYPE_STRING: 'string' ;


ID: [a-zA-Z_][a-zA-Z_0-9]* ;

STRING: '"' (ESC | SAFECODEPOINT)* '"' ;
fragment ESC : '\\' ["\\nt] ;
fragment SAFECODEPOINT: ~ ["\\\u0000-\u001F] ;

NATURAL
	: '0'
	| [1-9][0-9]*
	;

NUMBER
	: '-'? NATURAL ('.' [0-9]+)?
	| '0x' [0-9a-fA-F]+
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

name_space: (ID '.')* ID ;
class_path: (name_space '.')? ID ;

type
	: class_path
	| primitive_type
	| type '*'                                          // pointer
	| type '?'                                          // optional
	| type '[' ']'                                      // dynamic size array (aka vector)
	| type '[' NATURAL ']'                              // fixed size array
	| type '{' type '}'                                 // ordered map
	| type '{' '}'                                      // ordered set
	| type '(' type ')'                                 // unordered map
	| type '(' ')'                                      // unordered set
	;

primitive_type
	: TYPE_SIZE_T
	| TYPE_U8
	| TYPE_I8
	| TYPE_I32
	| TYPE_I64
	| TYPE_STRING
	// etc
	;

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

extends: 'extends' '(' class_path ')' ;
handles: 'handles' '(' class_path* ')' ;


module: extends (module_member | function | method)* handles? EOF;
class: extends (static_member | member | function | method)* EOF;
enum: ID* EOF;


