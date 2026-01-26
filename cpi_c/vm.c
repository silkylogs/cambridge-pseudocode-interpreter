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

static bool skip_whitespace(char **pstr) {
    bool found = false;
    do {
        char c = **pstr;
        if (!(c == ' ' && c != '\0')) return found;
        *pstr += 1;
        found = true;
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

static bool extract_skip_var_name(char **pstr, char **out_var_name, size_t *out_var_len) {
    *out_var_name = get_var_name(*pstr, out_var_len);
    *pstr += *out_var_len;
    return (*out_var_name != 0);
}

static bool extract_skip_var_type(char **pstr, char **out_var_name, size_t *out_var_len) {
    return extract_skip_var_name(pstr, out_var_name, out_var_len);
}

static bool skip_colon(char **pstr) {
    if (**pstr == ':') {
        *pstr += 1;
        return true;
    } else {
        return false;
    }
}

static bool skip_newline(char **pstr) {
    if (**pstr == '\n') {
        *pstr += 1;
        return true;
    } else {
        return false;
    }
}

static bool skip_nul(char **pstr) {
    if (**pstr == '\0') {
        *pstr += 1;
        return true;
    } else {
        return false;
    }
}

static void vm_alloc_var(struct VmState *state) {
    if (state->top >= state->cap) {
        if (state->cap) state->cap *= 2;
        else state->cap = 8;

        size_t alloc_sz = state->cap * sizeof(state->vars[0]);
        struct Var *ptr = realloc(state->vars, alloc_sz);
        assert(ptr);
        state->vars = ptr;
    }
}

static void vm_add_var(
    struct VmState *state,
    char *name, size_t name_len,
    char *type, size_t type_len
) {
    // Initialize name container.
    size_t name_size_in_db = 8;
    size_t top = state->top;
    memset(state->vars[top].name, '\0', name_size_in_db);
    memset(state->vars[top].type, '\0', name_size_in_db);

    // Copy name to container.
    size_t max_sz = name_size_in_db - 1;
    size_t sz = (name_len >= max_sz) ? max_sz : name_len;
    memcpy(state->vars[top].name, name, sz);
    sz = (type_len >= max_sz) ? max_sz : type_len;
    memcpy(state->vars[top].type, type, sz);

    state->top += 1; // Bump the top.
}

static void vm_decl_var_in_current_scope(
    struct VmState *state,
    char *name, size_t name_len,
    char *type, size_t type_len
) {
    vm_alloc_var(state);
    vm_add_var(state, name, name_len, type, type_len);
}

void vm_exec_stmt(struct VmState *state, char *stmt_ptr) {
    size_t len;
    enum StatementGuess guess;

    vm_guess_stmt_kind_from_first_word(stmt_ptr, &guess, &len);
    stmt_ptr += len;

    switch (guess) {
        case STMT_DECLARE:
        {
            // <optional space> DECLARE <space> VarName <optional space> : <optional space> TypeName <optional space> <EOL>
            //                          ^ You are here.

            // Note: Replace asserts with macro exiting with a good error message.

            assert(skip_whitespace(&stmt_ptr) && "Expected whitespace");

            char *var_name; size_t var_name_len;
            assert(extract_skip_var_name(&stmt_ptr, &var_name, &var_name_len) && "Expected variable name");

            skip_whitespace(&stmt_ptr);

            assert(skip_colon(&stmt_ptr) && "Expected colon");
            
            skip_whitespace(&stmt_ptr);

            char *var_type; size_t var_type_len;
            assert(extract_skip_var_type(&stmt_ptr, &var_type, &var_type_len) && "Expected variable type");

            skip_whitespace(&stmt_ptr);

            assert(skip_newline(&stmt_ptr) || skip_nul(&stmt_ptr) && "Expected newline or nul terminator");

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

