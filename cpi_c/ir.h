#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifndef IR_H
#define IR_H

typedef char *OwnedCstr;
typedef char *BorrowedCStr;

struct ParamDeclVar {
    OwnedCstr name;
    OwnedCstr type;
};

struct ParamDeclVar param_decl_var_new(OwnedCstr name, OwnedCstr type);
void param_decl_var_free(struct ParamDeclVar *);

struct ParamDeclConst {
    OwnedCstr name;
    OwnedCstr type;
};

struct ParamDeclArr1d {
    OwnedCstr name;
    OwnedCstr type;
    size_t bound_lower;
    size_t bound_upper;
};

struct ParamDeclArr2d {
    OwnedCstr name;
    OwnedCstr type;
    size_t dim0_bound_lower;
    size_t dim0_bound_upper;
    size_t dim1_bound_lower;
    size_t dim1_bound_upper;
};

// This should be a variable. Probably.
typedef char *TodoType;

// The operation type (add, sub, etc) should be derived from the kind of
// operation.
struct ParamAssignment {
    TodoType result;
    TodoType param0;
    TodoType param1;
};

// "If condition is true, execute this *reigon* of code. Else skip over that
// reigon."
struct ParamBrIf {
    bool condition;
    size_t if_reigon_len;
    // if (foo) ... else ... [The else conditoin cannot
    // exist without the if]
    size_t else_reigon_len;
};

struct ParamBrCase {
    TodoType condition;
    size_t reigon_len;

    TodoType *values;
    size_t *value_statement_starts;
    size_t *value_statement_lengths;
    size_t value_cnt;
};

struct ParamLoopFor {
    OwnedCstr identifier_name;
    OwnedCstr value0_name;
    OwnedCstr value1_name;
    OwnedCstr step_name;
    size_t reigon_len;
};

struct ParamLoopRepeatUntil {
    OwnedCstr condition_name;
    size_t reigon_len;
};

struct ParamLoopWhile {
    OwnedCstr condition_name;
    size_t reigon_len;
};

struct ParamDefnCustomType {
    OwnedCstr name;
    OwnedCstr type;
};

struct ParamDefnProcedure {
    OwnedCstr name;
    size_t reigon_len;
};

struct ParamDefnFunction {
    OwnedCstr name;
    OwnedCstr ret_value_name;
    size_t reigon_len;
};

struct ParamCmdCall {
    OwnedCstr name;
};

struct ParamCmdInput {
    OwnedCstr name;
};

struct ParamCmdOutput {
    OwnedCstr str;
};

struct ParamCmdFileOpen {
    OwnedCstr identifier;
    OwnedCstr mode;
};

struct ParamCmdFileRead {
    OwnedCstr identifier;
    OwnedCstr variable;
};

struct ParamCmdFileWrite {
    OwnedCstr identifier;
    OwnedCstr filename;
};

struct ParamCmdFileClose {
    TodoType identifier;
};

struct ParamCmdFileSeek {
    OwnedCstr identifier;
    TodoType address;
};

struct ParamCmdRecordGet {
    OwnedCstr identifier;
    TodoType variable;
};

struct ParamCmdRecordPut {
    OwnedCstr identifier;
    TodoType variable;
};

struct Instr {
    enum Kind {
        DECL_VAR,
        DECL_CONST,
        DECL_ARR1D,
        DECL_ARR2D,

        ASS_ADD,
        ASS_SUB,
        ASS_MUL,
        ASS_DIV,
        ASS_GEQ,
        ASS_LEQ,
        ASS_GT,
        ASS_LT,
        ASS_EQ,
        ASS_NEQ,
        ASS_AND,
        ASS_OR,
        ASS_NOT,

        BR_IF,
        BR_IF_ELSE,
        BR_CASE,
        BR_CASE_OTHERWISE,

        LOOP_FOR,
        LOOP_FOR_STEP,
        LOOP_REPEAT_UNTIL,
        LOOP_WHILE,

        DEFN_CUSTOM_TYPE,
        DEFN_PROCEDURE,
        DEFN_FUNCTION,

        CMD_CALL,
        CMD_INPUT,
        CMD_OUTPUT,
        CMD_FILE_OPEN,
        CMD_FILE_READ,
        CMD_FILE_WRITE,
        CMD_FILE_CLOSE,
        CMD_FILE_SEEK,
        CMD_RECORD_GET,
        CMD_RECORD_PUT,
    } kind;

    // All strings are null terminated.
    // The string may encode a numeric type.
    union Parameters {
        struct ParamDeclVar decl_var;
        struct ParamDeclConst decl_const;
        struct ParamDeclArr1d decl_arr1d;
        struct ParamDeclArr2d decl_arr2d;
        struct ParamAssignment ass;
        struct ParamBrIf br_if;
        struct ParamBrCase br_case;
        struct ParamLoopFor loop_for;
        struct ParamLoopRepeatUntil loop_repeat_until;
        struct ParamLoopWhile loop_while;
        struct ParamDefnCustomType defn_custom_type;
        struct ParamDefnProcedure defn_procedure;
        struct ParamDefnFunction defn_function;
        struct ParamCmdCall call;
        struct ParamCmdInput input;
        struct ParamCmdOutput output;
        struct ParamCmdFileOpen file_open;
        struct ParamCmdFileRead file_read;
        struct ParamCmdFileWrite file_write;
        struct ParamCmdFileClose file_close;
        struct ParamCmdFileSeek file_seek;
        struct ParamCmdRecordGet record_get;
        struct ParamCmdRecordPut record_put;
    } params;
};

bool type_is_custom(struct CustomTypes *ts, char *typename);
struct Instr instr_decl_var(char const *const name, char const *const type);
void instr_free(struct Instr);
#endif
