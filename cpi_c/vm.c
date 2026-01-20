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

static bool case_sensitive_open_ended_strequ(char *a, char *b) {
    while (*a && *b) {
        if (*a == *b) {
            a++;
            b++;
        } else return false;
    }
    return true;
}

enum StatementGuess vm_guess_stmt_kind_from_first_word(char *stmt_ptr) {
    if (case_sensitive_open_ended_strequ(stmt_ptr, "DECLARE")) {
        return STMT_DECLARE;
    } else {
        return STMT_POSSIBLY_ASSIGNMENT;
    }
}

void vm_decl_var_in_current_scope() {
    TODO;
}

void vm_exec_stmt(struct VmState *state, char **stmt_ptr) {
    switch (vm_guess_stmt_kind_from_first_word(stmt_ptr)) {
        case STMT_DECLARE:
        {
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

