#include "vm.h"
#include <assert.h>
#include <stdlib.h>

#define TODO assert(!"Todo");


//static void vm_decl_var_in_current_scope() {
//    assert(type_registry_exists(type));
//    assert(var_registry_exists_current_scope(name));
//    var_registry_add_current_scope(name, type);    
//}
//

char upper(char c) {
    if (c >= 'a' && c <= 'z') {
        c -= 'a' - 'A';
    }
    return c;
}

static bool streq(char *a, char *b) {
    while (*a && *b) {
        char ua = upper(*a);
        char ub = upper(*b);
        if (ua == ub) {
            a++;
            b++;
        } else return false;
    }
    return true;
}

void vm_guess_stmt_kind_from_first_word(char *stmt_ptr, enum StatementGuess *out_sg, size_t *out_stmt_len) {
    if (streq(stmt_ptr, "DECLARE")) {
        *out_sg = STMT_DECLARE;
        *out_stmt_len = sizeof "DECLARE" - 1;
        return;
    }
}

static void whitespace_len(char *str, size_t *out_len) {
    char c;
    *out_len = 0;
    do {
        c = str[*out_len];
        if (!(c == ' ' && c != '\0')) return;
        *out_len += 1;
    } while (true);
}

bool is_in_var_charset(char c) {
    return (c >= 'A' && c <= 'Z')
        || (c >= 'a' && c <= 'z')
        || (c >= '0' && c <= '9')
        ;
}

char *get_var_name(char *str, size_t *out_len) {
    char c;
    *out_len = 0;
    do {
        c = str[*out_len];
        if (!is_in_var_charset(c)) break;
        *out_len += 1;
    } while (true);

    return str;
}

char *get_var_type(char *str, size_t *out_len) {
    char c;
    *out_len = 0;
    do {
        c = str[*out_len];
        if (!is_in_var_charset(c)) break;
        *out_len += 1;
    } while (true);

    return str;
}

// TODO: memory management
static void vm_decl_var_in_current_scope(
    struct VmState *state,
    char *name, size_t name_len,
    char *type, size_t type_len
) {
    if (state->top == 0) {
        if (state->cap == 0) {
            state->cap = 1;
            state->vars = NULL;
        } else {
            state->cap *= 2;
        }
    } else {
        state->top += 1;
        state->cap *= 2;
    }
    assert(state->top < state->cap);
    size_t alloc_sz = state->cap * sizeof(state->vars[0]);
    struct Var *ptr = realloc(state->vars, alloc_sz);
    assert(ptr);
    state->vars = ptr;

    size_t name_size_in_db = 8;

    memset(state->vars[state->top].name, '\0', name_size_in_db);
    memset(state->vars[state->top].type, '\0', name_size_in_db);

    size_t max_sz = name_size_in_db - 1;

    size_t sz = (name_len >= max_sz) ? max_sz : name_len;
    memcpy(state->vars[state->top].name, name, sz);

    sz = (type_len >= max_sz) ? max_sz : type_len;
    memcpy(state->vars[state->top].type, type, sz);

    if (state->top == 0) state->top = 1;
}

void vm_exec_stmt(struct VmState *state, char *stmt_ptr) {
    size_t len;
    enum StatementGuess guess;

    vm_guess_stmt_kind_from_first_word(stmt_ptr, &guess, &len);
    stmt_ptr += len;

    switch (guess) {
        case STMT_DECLARE:
        {
            // <space> DECLARE <space> VarName <space> : <space> TypeName <space> <EOL>

            whitespace_len(stmt_ptr, &len);
            stmt_ptr += len;

            char *var_name = get_var_name(stmt_ptr, &len);
            size_t var_name_len = len;
            stmt_ptr += len;

            whitespace_len(stmt_ptr, &len);
            stmt_ptr += len;

            assert(*stmt_ptr == ':');
            stmt_ptr += 1;

            whitespace_len(stmt_ptr, &len);
            stmt_ptr += len;

            char *var_type = get_var_type(stmt_ptr, &len);
            size_t var_type_len = len;
            stmt_ptr += len;

            whitespace_len(stmt_ptr, &len);
            stmt_ptr += len;

            assert(*stmt_ptr == '\n' || *stmt_ptr == '\0');

            vm_decl_var_in_current_scope(state, var_name, var_name_len, var_type, var_type_len);
            break;
        }
        case STMT_POSSIBLY_ASSIGNMENT: // Fallthrough
        default:
        {
            TODO;
            break;
        }
    }
}

// -----------------------------------------------------------------

