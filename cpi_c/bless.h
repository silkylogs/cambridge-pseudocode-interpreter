#ifndef BLESS_H
#define BLESS_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

struct StringSlice {
	char *start;
	size_t len;
};

double eval_expr(struct StringSlice str);

struct Int32Result {
	int32_t val;
	uint32_t res;
};

bool char_in(char c, const char *set);
size_t count_longest_sequence_of_char_in_set(const char *str, size_t sz, const char *set);
struct Int32Result ascii_string_to_int32(const char *str, size_t sz);
float ascii_string_to_float(char const *str, size_t sz);

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

struct Literal try_parse_literal(const char *str, size_t sz);

/* TODO: Implement as ECS, current approach is not maintainable.
// Idea: Have a container of atomic datatypes,
struct ProgramData {
	int32_t integers[SZ];
	float floats[SZ];
	char chars[SZ];
	struct String strings[SZ];
	bool booleans[SZ];
	struct Date dates[SZ];

	char *identifier_or_literal_names;
};
// And another of custom types
struct CustomType {
	
};*/


enum StatementType {
	STMT_VAR_DECL,
	STMT_CONST_DECL,
	STMT_1D_ARRAY_DECL,
	STMT_2D_ARRAY_DECL,
	STMT_INPUT,
	STMT_OUTPUT,
	STMT_OPENFILE,
	STMT_READFILE,
	STMT_WRITEFILE,
	STMT_CLOSEFILE,
	STMT_SEEK,
	STMT_GETRECORD,
	STMT_PUTRECORD,
	STMT_CUSTOM_TYPE_DEFN,
	STMT_ASSIGNMENT,
	STMT_CONDITIONAL,
	STMT_CASE,
	STMT_FOR_LOOP,
	STMT_REPEAT_UNTIL_LOOP,
	STMT_WHILE_LOOP,
	STMT_PROC_DEFN,
	STMT_CALL,
	STMT_FN_DEFN,
};

struct StatementTypeAndBounds {
	enum StatementType type;
	char *start;
	size_t size;
};

// Inteneded loop:
// stmt = get_statement(ptr);
// exec_stmt(stmt);
// ptr += stmt.size;
struct StatementTypeAndBounds get_statement(const char *ptr);
void exec_stmt(struct StatementTypeAndBounds stmt);

#endif


/**** Language grammar: ****

<statement> ::=
	| <var_decl>
	| <const_decl>
	| <array_decl>
	| <input>
	| <output>
	| <openfile>
	| <readfile>
	| <writefile>
	| <closefile>
	| <seek>
	| <getrecord>
	| <putrecord>
	| <custom_type_defn>
	| <assignment>
	| <conditional>
	| <case>
	| <for_loop>
	| <repeat_until_loop>
	| <while_loop>
	| <proc_defn>
	| <call>
	| <fn_defn>

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
	| "'" <character> "'"
	| "'" "'" // Error case

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


<datatype> ::= <identifier> | <atomic_typename>
*/

