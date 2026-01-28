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

static bool streqci(char *a, char *b) {
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
    if (streqci(stmt_ptr, "DECLARE")) {
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
    if (state->one_above_top >= state->cap) {
        if (state->cap) state->cap *= 2;
        else state->cap = 8;

        size_t alloc_sz = state->cap * sizeof(state->vars[0]);
        struct Var *ptr = realloc(state->vars, alloc_sz);
        assert(ptr);
        memset(ptr, 0, alloc_sz);
        state->vars = ptr;
    }
    state->one_above_top += 1;
}

static void vm_add_var(
    struct VmState *state,
    char *name, size_t name_len,
    char *type, size_t type_len
) {
    struct Var *top = &state->vars[state->one_above_top - 1];

    // Init name container
    memset(top->name, '\0', VAR_NAME_LEN);
    memset(top->type, '\0', VAR_NAME_LEN);

    // Copy name to container
    size_t max = VAR_NAME_LEN - 1;
    memcpy(top->name, name, (name_len >= max) ? max : name_len);
    memcpy(top->type, type, (type_len >= max) ? max : type_len);

    // Assign default value. TODO take custom values into account.
    if (streqci(type, "INTEGER")) {
        top->valcnt = 1;
        top->val_arr_starting_idx = 0;
        top->valesz = sizeof(int);

        size_t alloc_sz = top->valcnt * top->valesz;
        top->valdat = malloc(alloc_sz);
        assert(top->valdat);

        *(int *)top->valdat = 0;
    } else if (streqci(type, "REAL")) {
        top->valcnt = 1;
        top->val_arr_starting_idx = 0;
        top->valesz = sizeof(double);

        size_t alloc_sz = top->valcnt * top->valesz;
        top->valdat = malloc(alloc_sz);
        assert(top->valdat);

        *(double *)top->valdat = 0.0;
    } else if (streqci(type, "CHAR")) {
        top->valcnt = 1;
        top->val_arr_starting_idx = 0;
        top->valesz = sizeof(char);

        size_t alloc_sz = top->valcnt * top->valesz;
        top->valdat = malloc(alloc_sz);
        assert(top->valdat);

        *(char *)top->valdat = '\0';
    } else if (streqci(type, "STRING")) {
        top->valcnt = 1;
        top->val_arr_starting_idx = 0;
        top->valesz = sizeof("");

        size_t alloc_sz = top->valcnt * top->valesz;
        top->valdat = malloc(alloc_sz);
        assert(top->valdat);

        *(char *)top->valdat = "";
    } else if (streqci(type, "BOOLEAN")) {
        top->valcnt = 1;
        top->val_arr_starting_idx = 0;
        top->valesz = sizeof(bool);

        size_t alloc_sz = top->valcnt * top->valesz;
        top->valdat = malloc(alloc_sz);
        assert(top->valdat);

        *(bool *)top->valdat = false;
    } else if (streqci(type, "DATE")) {
        top->valcnt = 1;
        top->val_arr_starting_idx = 0;
        top->valesz = sizeof("00/00/0000");

        size_t alloc_sz = top->valcnt * top->valesz;
        top->valdat = malloc(alloc_sz);
        assert(top->valdat);

        memcpy(top->valdat, "00/00/0000", sizeof "00/00/0000");
    }
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

