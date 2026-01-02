#ifndef CPI_VM_H
#define CPI_VM_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

// Virtual machine - Mirrors the kinds of statements the langauge can execute, but simpler.
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
// Every variable or constant has a name. Therefore, the name can be used as a key to map it's value.
// Declarations dont come with assignments.
// - Variable
// - Constant
// - Array1D
// - Array2D
//
// # Assignments
// To simplify, this VM will not evaluate expressions. That is the job of the parser.
// The VM will instead evaluate a simpler form, such as "res" = "arg1" op "arg2" (except for the unary operator)
// ## Operations
// Every number is a double. Or a double with it's fractional part truncated and masquerading as an integer.
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
// - Custom type. "foo.bar.baz" could be a key. Wait, everything's a double or a string.
// - Procedure (returns nothing)
// - Function (returns something)
//
// # Commands
// - Call function
// - Input... A num/string.
// - Output... A string. Interpreter's job to do the parsing.
// - File: open, read, write, close, seek
// - Record: get, put

struct Instr {
    enum Kind {
        DECL_VAR, DECL_CONST, DECL_ARR1D, DECL_ARR2D,

        ASS_ADD, ASS_SUB, ASS_MUL, ASS_DIV, ASS_GEQ, ASS_LEQ, ASS_GT, ASS_LT, ASS_EQ, ASS_NEQ, ASS_AND, ASS_OR, ASS_NOT,

        BR_IF, BR_IF_ELSE, BR_CASE, BR_CASE_OTHERWISE,

        LOOP_FOR, LOOP_FOR_STEP, LOOP_REPEAT_UNTIL, LOOP_WHILE,

        DEFN_CUSTOM_TYPE, DEFN_PROCEDURE, DEFN_FUNCTION,

        CMD_CALL, CMD_INPUT, CMD_OUTPUT, CMD_FILE_OPEN, CMD_FILE_READ, CMD_FILE_WRITE, CMD_FILE_CLOSE, CMD_FILE_SEEK, CMD_RECORD_GET, CMD_RECORD_PUT,
    } kind;

    // Parameters. Use whatever parameter from here where appropriate.
    // Note: Strings are null terminated.
    char *param_decl_var_name;
    char *param_decl_var_type;

    char *param_decl_const_name;
    char *param_decl_const_type;

    char *param_decl_arr1d_name;
    char *param_decl_arr1d_type;
    char *param_decl_arr1d_bound_lower;
    char *param_decl_arr1d_bound_upper;

    char *param_decl_arr2d_name;
    char *param_decl_arr2d_type;
    char *param_decl_arr2d_dim0_bound_lower;
    char *param_decl_arr2d_dim0_bound_upper;
    char *param_decl_arr2d_dim1_bound_lower;
    char *param_decl_arr2d_dim1_bound_upper;

    // The operation type (add, sub, etc) should be derived from the kind of operation.
    char *param_ass_result_name;
    char *param_ass_param0_name;
    char *param_ass_param1_name;

    // "If condition is true, execute this *reigon* of code. Else skip over that reigon.
    char *param_br_if_condition;
    char *param_br_if_reigon_len;
    char *param_br_if_else_reigon_len; // if (foo) ... else ... [The else conditoin cannot exist without the if]

    char *param_br_case_condition;
    char *param_br_case_reigon_len;
    char **param_br_case_values;
    char **param_br_case_value_reigons;
    char *param_br_case_value_cnt;

    char *loop_for_identifier_name;
    char *loop_for_value0_name;
    char *loop_for_value1_name;
    char *loop_for_step_name;
    char *loop_for_reigon_len;

    char *loop_repeat_until_condition_name;
    char *loop_repeat_until_reigon_len;

    char *loop_while_condition_name;
    char *loop_while_reigon_len;

    char *defn_custom_type_name;
    char *defn_custom_type_type;

    char *defn_procedure_name;
    char *defn_procedure_reigon_len;

    char *defn_function_name;
    char *defn_function_ret_value_name;
    char *defn_function_reigon_len;

    char *cmd_call_name;
    char *cmd_input_name;
    char *cmd_output_str;

    char *cmd_file_open_identifier;
    char *cmd_file_open_mode;
    char *cmd_file_read_identifier;
    char *cmd_file_read_variable;
    char *cmd_file_write_identifier;
    char *cmd_file_write_filename;
    char *cmd_file_close_identifier;
    char *cmd_file_seek_identifier;
    char *cmd_file_seek_address;
    char *cmd_record_get_identifier;
    char *cmd_record_get_variable;
    char *cmd_record_put_identifier;
    char *cmd_record_put_variable;
};


struct VmState {
    struct Var {
        char *name;
        char *value;
        char *typename;
    } *vars;
    size_t var_count;
};

void vm_exe_instr(struct VmState *state, struct Instr *instr);

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

