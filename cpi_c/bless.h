#ifndef BLESS_H
#define BLESS_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>


struct Int32Result {
	int32_t val;
	uint32_t res;
};

bool char_in(char c, const char *set);
size_t count_longest_sequence_of_char_in_set(const char *str, size_t sz, const char *set);
struct Int32Result ascii_string_to_int32(const char *str, size_t sz);

enum AtomicDataType { ADT_INTEGER, ADT_REAL, ADT_CHAR, ADT_STRING, ADT_BOOLEAN, ADT_DATE };

struct Date { int32_t day; int32_t month; int32_t year; };

struct Literal {
	enum AtomicDataType kind;
	union LiteralValue {
		int32_t as_int32;
		float as_float;
		char as_char;
		char *as_string;
		bool as_boolean;
		struct Date as_date;
	} val;
	bool is_valid;
};

bool literal_shallow_equality(struct Literal lit1, struct Literal lit2);

// TODO: Implement as ECS, current approach is not maintainable.
struct Literal try_parse_literal(const char *str, size_t sz);

#endif


/**** Language grammar: ****

<comment> ::= "//" <text> <newline>

<atomic_typename> ::= "INTEGER" | "REAL" | "CHAR" | "STRING" | "BOOLEAN" | "DATE"

<literal> ::=
	| <integer_literal>
	| <real_literal>
	| <char_literal>
	| <string_literal>
	| <boolean_literal>
	| <date_literal>

<integer_literal> ::=
	| <number>
	| "-" <number>

<real_literal> ::=
	| <number> "." <number>
	| "-" <number> "." <number>

<char_literal> ::=
	"'" <character> "'"
	"'" "'" // Error case

<string_literal> ::=
	| """ <string_text> """
	| """ """

<boolean_literal> ::= "TRUE" | "FALSE"

<date_literal> ::= <day> "/" <month> "/" <year>
<day> ::= <digit><digit>
<month> ::= <digit><digit>
<year> ::= <digit><digit><digit><digit>

<identifier> ::= <letter>, ( <letter> | <digit> )

<var_decl> ::= DECLARE <identifier> : <datatype>

<const_decl> ::= CONSTANT <identifier> = <literal>

<assignment> ::= <expression> "<-" <expression>

<array_decl> ::=
	| "DECLARE" <identifier> ":" "ARRAY" "[" <l> ":" <u> "]" "OF" <datatype>
	| "DECLARE" <identifier> ":" "ARRAY" "[" <l1> ":" <u1> "," <l2> ":" <u2> "]" "OF" <datatype>

<custom_type_defn> ::= "TYPE" <identifier> <decl> "ENDTYPE"

<input> ::= "INPUT" <identifier>

<output> ::= "OUTPUT" <comma_seperated_values>
<comma_seperated_values> ::= <value> "," <comma_seperated_values>

<conditional> ::=
	| "IF" <expr> "THEN" <statement> "ENDIF"
	| "IF" <expr> "THEN" <statement> "ELSE" <statement> "ENDIF"

<case> ::=
	| "CASE" "OF" <identifier> <clause> "ENDCASE"
	| "CASE" "OF" <identifier> <clause> "OTHERWISE" <statement> "ENDCASE"
<clause> ::=
	| <value> ":" <statement>
	| <clause>

<for_loop> ::=
	| "FOR" <identifier> "<-" <identifier> "TO" <identifier> <statement> "ENDFOR"
	| "FOR" <identifier> "<-" <identifier> "TO" <identifier> "STEP" <identifier> <statement> "ENDFOR"

<repeat_until_loop> ::= "REPEAT" <statement> "UNTIL" <expr>

<while_loop> ::= "WHILE" <expr> "DO" <statement> "ENDWHILE"

<proc_defn> ::=
	| "PROCEDURE" <identifier> <statement> "ENDPROCEDURE"
	| "PROCEDURE" <identifier> "(" <params> ")" <statement> "ENDPROCEDURE"
<params> ::=
	| <params> "," <params>
	| <identifier> ":" <identifier>
	| "BYREF" <identifier> ":" <identifier>
	| "BYVALUE" <identifier> ":" <identifier>

<call> ::=
	| "CALL" <identifier>
	| "CALL" <identifier> "(" <params> ")"

<fn_defn> ::=
	| "FUNCTION" <identifier> "RETURNS" <identifier> <statement> "ENDFUNCTION"
	| "FUNCTION" <identifier> "(" <params> ")" "RETURNS" <identifier> <statement> "ENDFUNCTION"

<openfile> ::= "OPENFILE" <identifier> "FOR" <filemode>
<filemode> ::= "READ" | "WRITE" | "APPEND"

<readfile> ::= "READFILE" <identifier> "," <variable>

<writefile> ::= "WRITEFILE" <identifier> "," <variable>

<closefile> ::= "CLOSEFILE" <identifier>

<seek> ::= "SEEK" <identifier> "," <expr>

<getrecord> ::= "GETRECORD" <identifier> "," <expr>

<putrecord> ::= "PUTRECORD" <identifier> "," <expr>

<statement> ::=
	| <statement> <comment>
	| <newline>
	| <comment>
	| <var_decl>
	| <const_decl>
	| <assignment>
	| <array_decl>
	| <custom_type_defn>
	| <input>
	| <output>
	| <conditional>
	| <case>
	| <for_loop>
	| <repeat_until_loop>
	| <while_loop>
	| <proc_defn>
	| <call>
	| <fn_defn>
	| <openfile>
	| <readfile>
	| <writefile>
	| <closefile>
	| <seek>
	| <getrecord>
	| <putrecord>
	| <statement>

<datatype> ::= <identifier> | <atomic_typename>
*/

