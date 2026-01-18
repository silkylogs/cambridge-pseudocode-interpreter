#include "vm.h"
#include <assert.h>
#include <stdlib.h>

#define TODO assert(!"Todo");

static char *str_dup(const char *const str) {
    if (!str) return NULL;

    size_t len = strlen(str) + 1;
    char *dup = malloc(len);
    assert(dup);

    strcpy(dup, str);

    return dup;
}

static bool type_is_atomic(char *t) {
    if (0 == strcmp(t, "INTEGER")
        || 0 == strcmp(t, "REAL")
        || 0 == strcmp(t, "CHAR")
        || 0 == strcmp(t, "STRING")
        || 0 == strcmp(t, "BOOLEAN")
        || 0 == strcmp(t, "DATE")
        ) {
        return true;
    } else return false;
}

static char *atomic_type_default_value(char *t) {
    if (0 == strcmp(t, "INTEGER")) return "0";
    else if (0 == strcmp(t, "REAL")) return "0.0";
    else if (0 == strcmp(t, "CHAR")) return "\0";
    else if (0 == strcmp(t, "STRING")) return "";
    else if (0 == strcmp(t, "BOOLEAN")) return "FALSE";
    else if (0 == strcmp(t, "DATE")) return "00/00/0000";
    else assert(!"Error: Attempt to construct default value of illegal atomic type");
}

// -----------------------------------------------------------------

static void vm_add_atomic_var(struct VmState *state, struct Var var) {
    //assert(vm_alloc_var(*state));
    
    size_t new_len = state->variables.len + 1;
    state->variables.vars = realloc(state->variables.vars, sizeof(struct Var) * new_len);
    assert(state->variables.vars);
    state->variables.vars[state->variables.len] = var;
    state->variables.len = new_len;
}

static void vm_add_custom_type_var(struct VmState *state, struct CustomTypes *custom_types, char *typename) {
    TODO;
}

// -----------------------------------------------------------------

static void vm_decl_var(struct VmState *state, struct Instr *instr) {
    struct Var var = { 0 };
    var.name = str_dup(instr->params.decl_var.name);
    var.typename = str_dup(instr->params.decl_var.type);

    if (type_is_atomic(var.typename)) {
        var.value = str_dup(atomic_type_default_value(var.typename));
        vm_add_atomic_var(state, var);
    } else if (/*type_is_custom(&state->custom_types, var.typename)*/ 0) {
        vm_add_custom_type_var(state, &state->custom_types, var.typename);
    } else {
        assert(!"Error: Attempt to declare value of invalid type");
    }

}

static void vm_decl_const(struct VmState *state, struct Instr *instr) {

}

static void vm_decl_arr1d(struct VmState *state, struct Instr *instr) {

}

static void vm_decl_arr2d(struct VmState *state, struct Instr *instr) {

}

static void vm_ass_add(struct VmState *state, struct Instr *instr) {

}

static void vm_ass_sub(struct VmState *state, struct Instr *instr) {

}

static void vm_ass_mul(struct VmState *state, struct Instr *instr) {

}

static void vm_ass_div(struct VmState *state, struct Instr *instr) {

}

static void vm_ass_geq(struct VmState *state, struct Instr *instr) {

}

static void vm_ass_leq(struct VmState *state, struct Instr *instr) {

}

static void vm_ass_gt(struct VmState *state, struct Instr *instr) {

}

static void vm_ass_lt(struct VmState *state, struct Instr *instr) {

}

static void vm_ass_eq(struct VmState *state, struct Instr *instr) {

}

static void vm_ass_neq(struct VmState *state, struct Instr *instr) {

}

static void vm_ass_and(struct VmState *state, struct Instr *instr) {

}

static void vm_ass_or(struct VmState *state, struct Instr *instr) {

}

static void vm_ass_not(struct VmState *state, struct Instr *instr) {

}

static void vm_br_if(struct VmState *state, struct Instr *instr) {

}

static void vm_br_if_else(struct VmState *state, struct Instr *instr) {

}

static void vm_br_case(struct VmState *state, struct Instr *instr) {

}

static void vm_br_case_otherwise(struct VmState *state, struct Instr *instr) {

}

static void vm_loop_for(struct VmState *state, struct Instr *instr) {

}

static void vm_loop_for_step(struct VmState *state, struct Instr *instr) {

}

static void vm_loop_repeat_until(struct VmState *state, struct Instr *instr) {

}

static void vm_loop_while(struct VmState *state, struct Instr *instr) {

}

static void vm_defn_custom_type(struct VmState *state, struct Instr *instr) {

}

static void vm_defn_procedure(struct VmState *state, struct Instr *instr) {

}

static void vm_defn_function(struct VmState *state, struct Instr *instr) {

}

static void vm_cmd_call(struct VmState *state, struct Instr *instr) {

}

static void vm_cmd_input(struct VmState *state, struct Instr *instr) {

}

static void vm_cmd_output(struct VmState *state, struct Instr *instr) {

}

static void vm_cmd_file_open(struct VmState *state, struct Instr *instr) {

}

static void vm_cmd_file_read(struct VmState *state, struct Instr *instr) {

}

static void vm_cmd_file_write(struct VmState *state, struct Instr *instr) {

}

static void vm_cmd_file_close(struct VmState *state, struct Instr *instr) {

}

static void vm_cmd_file_seek(struct VmState *state, struct Instr *instr) {

}

static void vm_cmd_record_get(struct VmState *state, struct Instr *instr) {

}

static void vm_cmd_record_put(struct VmState *state, struct Instr *instr) {

}

// -----------------------------------------------------------------


struct Instr instr_decl_var(char const *const name, char const *const type) {
    struct Instr i = { 0 };
    i.kind = DECL_VAR;
    i.params.decl_var.name = str_dup(name);
    i.params.decl_var.type = str_dup(type);
    return i;
}

void instr_free(struct Instr i) {
    TODO; // Actually free it
}

struct VmState vm_init() {
    struct VmState vm = { 0 };

    vm.variables.len = 0;
    vm.variables.vars = NULL;

    vm.custom_types.len = 0;
    vm.custom_types.types = NULL;

    return vm;
}

void vm_free(struct VmState *vm) {
    size_t i;

    for (i = 0; i < vm->variables.len; i += 1) {
        free(vm->variables.vars[i].name);
        free(vm->variables.vars[i].value);
        free(vm->variables.vars[i].typename);

        vm->variables.vars[i].name = NULL;
        vm->variables.vars[i].value = NULL;
        vm->variables.vars[i].typename = NULL;
    }
    free(vm->variables.vars);
    vm->variables.vars = NULL;

    TODO; // free custom types
}

struct Var *vm_search_var_by_name(struct VmState vm, const char *const name) {
    size_t i;

    for (i = 0; i < vm.variables.len; i += 1) {
        if (0 == strcmp(vm.variables.vars[i].name, name)) {
            return &vm.variables.vars[i];
        }
    }

    return NULL;
}

void vm_exe_instr(struct VmState *state, struct Instr *instr) {
    switch (instr->kind) {
        case DECL_VAR: { vm_decl_var(state, instr); break; }
        case DECL_CONST: { vm_decl_const(state, instr); break; }
        case DECL_ARR1D: { vm_decl_arr1d(state, instr); break; }
        case DECL_ARR2D: { vm_decl_arr2d(state, instr); break; }
        case ASS_ADD: { vm_ass_add(state, instr); break; }
        case ASS_SUB: { vm_ass_sub(state, instr); break; }
        case ASS_MUL: { vm_ass_mul(state, instr); break; }
        case ASS_DIV: { vm_ass_div(state, instr); break; }
        case ASS_GEQ: { vm_ass_geq(state, instr); break; }
        case ASS_LEQ: { vm_ass_leq(state, instr); break; }
        case ASS_GT: { vm_ass_gt(state, instr); break; }
        case ASS_LT: { vm_ass_lt(state, instr); break; }
        case ASS_EQ: { vm_ass_eq(state, instr); break; }
        case ASS_NEQ: { vm_ass_neq(state, instr); break; }
        case ASS_AND: { vm_ass_and(state, instr); break; }
        case ASS_OR: { vm_ass_or(state, instr); break; }
        case ASS_NOT: { vm_ass_not(state, instr); break; }
        case BR_IF: { vm_br_if(state, instr); break; }
        case BR_IF_ELSE: { vm_br_if_else(state, instr); break; }
        case BR_CASE: { vm_br_case(state, instr); break; }
        case BR_CASE_OTHERWISE: { vm_br_case_otherwise(state, instr); break; }
        case LOOP_FOR: { vm_loop_for(state, instr); break; }
        case LOOP_FOR_STEP: { vm_loop_for_step(state, instr); break; }
        case LOOP_REPEAT_UNTIL: { vm_loop_repeat_until(state, instr); break; }
        case LOOP_WHILE: { vm_loop_while(state, instr); break; }
        case DEFN_CUSTOM_TYPE: { vm_defn_custom_type(state, instr); break; }
        case DEFN_PROCEDURE: { vm_defn_procedure(state, instr); break; }
        case DEFN_FUNCTION: { vm_defn_function(state, instr); break; }
        case CMD_CALL: { vm_cmd_call(state, instr); break; }
        case CMD_INPUT: { vm_cmd_input(state, instr); break; }
        case CMD_OUTPUT: { vm_cmd_output(state, instr); break; }
        case CMD_FILE_OPEN: { vm_cmd_file_open(state, instr); break; }
        case CMD_FILE_READ: { vm_cmd_file_read(state, instr); break; }
        case CMD_FILE_WRITE: { vm_cmd_file_write(state, instr); break; }
        case CMD_FILE_CLOSE: { vm_cmd_file_close(state, instr); break; }
        case CMD_FILE_SEEK: { vm_cmd_file_seek(state, instr); break; }
        case CMD_RECORD_GET: { vm_cmd_record_get(state, instr); break; }
        case CMD_RECORD_PUT: { vm_cmd_record_put(state, instr); break; }
        default: { assert(!"panic! unreachable instruction\n"); break; }
    }
}

// -----------------------------------------------------------------

