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


enum StatementGuess vm_guess_stmt_kind_from_first_word(char *stmt_ptr) {
    // Case insensitive, open-ended strequ
    char *c = stmt_ptr;
    if (c[0] == 'D'
        && c[1] == 'E'
        && c[2] == 'C'
        && c[3] == 'L'
        && c[4] == 'A'
        && c[5] == 'R'
        && c[6] == 'E') {
        return STMT_DECLARE;
    } else {
        return STMT_POSSIBLY_ASSIGNMENT;
    }
}

void vm_exec_stmt(struct VmState *state, char **stmt_ptr) {
    switch (vm_guess_stmt_kind_from_first_word(stmt_ptr)) {
        case STMT_DECLARE:
        {
            TODO;//vm_decl_var_in_current_scope();
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

