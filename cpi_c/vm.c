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

enum StatementGuess vm_guess_stmt_kind_from_first_word(char *stmt_ptr) {
    if (streq(stmt_ptr, "DECLARE")) {
        return STMT_DECLARE;
    } else {
        return STMT_POSSIBLY_ASSIGNMENT;
    }
}

static void vm_decl_var_in_current_scope(void) {
    TODO;
}

void vm_exec_stmt(struct VmState *state, char **stmt_ptr) {
    switch (vm_guess_stmt_kind_from_first_word(*stmt_ptr)) {
        case STMT_DECLARE:
        {
            // TODO: get statement length
            // TODO: get var name
            // TODO: get var type
            vm_decl_var_in_current_scope();
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

