#ifndef CPI_VM_H
#define CPI_VM_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "ir.h"

// Virtual machine - Mirrors the kinds of statements the langauge can execute,
// but simpler.
//
// Dimensions of things to consider about the task of storage of variables:
// - Store program data (Mutable Variable, Immutable Literal)
// - With types (integer, real, string, char, date, boolean)
// - Has scope based lifetime. Can shadow.
// - As function parameter, may be passed by value or reference
// - Is an array type
//

// # Statement types
// - Declarations
// - Assignments
// - Branching
// - Loops
// - Definitions
// - Commands
//
// # Declarations
// Every variable or constant has a name. Therefore, the name can be used as a
// key to map it's value. Declarations dont come with assignments.
// - Variable
// - Constant
// - Array1D
// - Array2D
//
// # Assignments
// To simplify, this VM will not evaluate expressions. That is the job of the
// parser. The VM will instead evaluate a simpler form, such as "res" = "arg1"
// op "arg2" (except for the unary operator)
// ## Operations
// Every number is a double. Or a double with it's fractional part truncated and
// masquerading as an integer.
// - Addition    |--------------- Has lower predecence
// - Subtraction |
// - Multiplication |------------ Has higher predecence
// - Division       |
// - Greater than             |-- Has lowest predecence
// - Lesser than              |
// - Greater than or equal to |
// - Lesser than or equal to  |
// - Not equal to             |
// - Equal to                 |
// - Logical AND              |
// - Logical OR               |
// - Logical NOT              |
//
// # Branching
// if (condition) instr_ptr += offset;
// - If
// - If else
// - Case
// - Case otherwise
//
// # Loops
// - For
// - For with step
// - Repeat until
// - While
//
// # Definitions
// - Custom type. "foo.bar.baz" could be a key. Wait, everything's a double or a
// string.
// - Procedure (returns nothing)
// - Function (returns something)
//
// # Commands
// - Call function
// - Input... A num/string.
// - Output... A string. Interpreter's job to do the parsing.
// - File: open, read, write, close, seek
// - Record: get, put

enum StatementGuess {
    STMT_DECLARE,
    STMT_POSSIBLY_ASSIGNMENT,
};

char upper(char c);
void vm_guess_stmt_kind_from_first_word(char *stmt_ptr, enum StatementGuess *out_sg, size_t *out_stmt_len);

void grow(unsigned char **mem, size_t new_sz);

// Struct members are indices to a char* array instead of pointers
// Avoids pointer invalidation
struct Header {
    size_t next;
    size_t str;
    size_t strlen;
    size_t data;
    size_t datalen;
};

struct ProgramData {
    char *mem;
    size_t mem_sz;
    size_t latest_header;
};

void program_data_append(struct ProgramData *pd, char *zstr, void *data, size_t dat_len);

#define VAR_NAME_LEN ((size_t)8)
struct VmState {
    size_t one_above_top;
    size_t cap;
    struct Var {
        char name[VAR_NAME_LEN];
        char type[VAR_NAME_LEN];
        size_t valcnt; // Value count
        size_t val_arr_starting_idx;
        size_t valesz; // Value element size.
        void *valdat; // Value data
    } *vars;
};

void vm_exec_stmt(struct VmState *state, char *stmt_ptr);

#endif

// clang-format off
// Language grammar:
//
//<statement> ::=
//    | <var_decl>
//    | <const_decl>
//    | <array_decl>
//    | <input>
//    | <output>
//    | <openfile>
//    | <readfile>
//    | <writefile>
//    | <closefile>
//    | <seek>
//    | <getrecord>
//    | <putrecord>
//    | <custom_type_defn>
//    | <assignment>
//    | <conditional>
//    | <case>
//    | <for_loop>
//    | <repeat_until_loop>
//    | <while_loop>
//    | <proc_defn>
//    | <call>
//    | <fn_defn>
//
//<comment> ::= "//" <text> <newline>
//
//<atomic_typename> ::= "INTEGER" | "REAL" | "CHAR" | "STRING" | "BOOLEAN" | "DATE"
//
//<literal> ::=
//    | <integer_literal>
//    | <real_literal>
//    | <char_literal>
//    | <string_literal>
//    | <boolean_literal>
//    | <date_literal>
//
//<integer_literal> ::=
//    | <number>
//    | "-" <number>
//
//<real_literal> ::=
//    | <number> "." <number>
//    | "-" <number> "." <number>
//
//<char_literal> ::=
//    | "'" <character> "'"
//    | "'" "'" // Error case
//
//<string_literal> ::=
//    | """ <string_text> """
//    | """ """
//
//<boolean_literal> ::= "TRUE" | "FALSE"
//
//<date_literal> ::= <day> "/" <month> "/" <year>
//<day> ::= <digit><digit>
//<month> ::= <digit><digit>
//<year> ::= <digit><digit><digit><digit>
//
//<identifier> ::= <letter>, ( <letter> | <digit> )
//
//<var_decl> ::= DECLARE <identifier> : <datatype>
//
//<const_decl> ::= CONSTANT <identifier> = <literal>
//
//<assignment> ::= <expression> "<-" <expression>
//
//<array_decl> ::=
//    | "DECLARE" <identifier> ":" "ARRAY" "[" <l> ":" <u> "]" "OF" <datatype>
//    | "DECLARE" <identifier> ":" "ARRAY" "[" <l1> ":" <u1> "," <l2> ":" <u2> "]" "OF" <datatype>
//
//<custom_type_defn> ::= "TYPE" <identifier> <decl> "ENDTYPE"
//
//<input> ::= "INPUT" <identifier>
//
//<output> ::= "OUTPUT" <comma_seperated_values>
//<comma_seperated_values> ::= <value> "," <comma_seperated_values>
//
//<conditional> ::=
//    | "IF" <expr> "THEN" <statement> "ENDIF"
//    | "IF" <expr> "THEN" <statement> "ELSE" <statement> "ENDIF"
//
//<case> ::=
//    | "CASE" "OF" <identifier> <clause> "ENDCASE"
//    | "CASE" "OF" <identifier> <clause> "OTHERWISE" <statement> "ENDCASE"
//<clause> ::=
//    | <value> ":" <statement>
//    | <clause>
//
//<for_loop> ::=
//    | "FOR" <identifier> "<-" <identifier> "TO" <identifier> <statement> "ENDFOR" 
//    | "FOR" <identifier> "<-" <identifier> "TO" <identifier> "STEP" <identifier> <statement> "ENDFOR"
//
//<repeat_until_loop> ::= "REPEAT" <statement> "UNTIL" <expr>
//
//<while_loop> ::= "WHILE" <expr> "DO" <statement> "ENDWHILE"
//
//<proc_defn> ::=
//    | "PROCEDURE" <identifier> <statement> "ENDPROCEDURE"
//    | "PROCEDURE" <identifier> "(" <params> ")" <statement> "ENDPROCEDURE"
//<params> ::=
//    | <params> "," <params>
//    | <identifier> ":" <identifier>
//    | "BYREF" <identifier> ":" <identifier>
//    | "BYVALUE" <identifier> ":" <identifier>
//
//<call> ::=
//    | "CALL" <identifier>
//    | "CALL" <identifier> "(" <params> ")"
//
//<fn_defn> ::=
//    | "FUNCTION" <identifier> "RETURNS" <identifier> <statement> "ENDFUNCTION"
//    | "FUNCTION" <identifier> "(" <params> ")" "RETURNS" <identifier> <statement> "ENDFUNCTION"
//
//<openfile> ::= "OPENFILE" <identifier> "FOR" <filemode>
//<filemode> ::= "READ" | "WRITE" | "APPEND"
//
//<readfile> ::= "READFILE" <identifier> "," <variable>
//
//<writefile> ::= "WRITEFILE" <identifier> "," <variable>
//
//<closefile> ::= "CLOSEFILE" <identifier>
//
//<seek> ::= "SEEK" <identifier> "," <expr>
//
//<getrecord> ::= "GETRECORD" <identifier> "," <expr>
//
//<putrecord> ::= "PUTRECORD" <identifier> "," <expr>
//
//
//<datatype> ::= <identifier> | <atomic_typename>

