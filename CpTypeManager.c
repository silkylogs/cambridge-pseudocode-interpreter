#ifndef CP_SOURCE_TYPE_MANAGER
#define CP_SOURCE_TYPE_MANAGER

/*
// Memory
    struct CpUniquePtr {
	void *data;
	ssize data_len_bytes;
	bool is_valid;
    } data;

#define CPUNIQUEPTR_NULLPTR (struct CpUniquePtr){ .data=NULL; .data_len_bytes=0; .is_valid=false; }
// Memory
*/

    /* Indices double as the type ID */
struct CpTypeStack {
    char *backing_name_array;
    ssize backing_name_array_len;
    
    ssize *backing_name_size_array;
    ssize backing_name_size_array_len;
    
    ssize names_last_idx;
};

void CpTypeStack_init_with_size_unchecked(
    struct CpTypeStack *tstack,
    char *new_backing_name_array,
    ssize new_backing_name_array_len,
    ssize *new_backing_name_size_array,
    ssize new_backing_name_size_array_len);
void CpTypeStack_init_with_size_unchecked(
    struct CpTypeStack *tstack,
    char *new_backing_name_array,
    ssize new_backing_name_array_len,
    ssize *new_backing_name_size_array,
    ssize new_backing_name_size_array_len)
{
    tstack->backing_name_array_len = new_backing_name_array_len;
    tstack->backing_name_size_array_len = new_backing_name_size_array_len;
    
    tstack->backing_name_array = new_backing_name_array;
    tstack->backing_name_size_array = new_backing_name_size_array;
    
    tstack->names_last_idx = 0;
}


bool CpTypeStack_init_with_size_check(
    struct CpTypeStack *tstack,
    char *new_backing_name_array,
    ssize new_backing_name_array_len,
    ssize *new_backing_namelen_array,
    ssize  new_backing_namelen_array_len);

bool CpTypeStack_init_with_size_check(
    struct CpTypeStack *tstack,
    char *new_backing_name_array,
    ssize new_backing_name_array_len,
    ssize *new_backing_namelen_array,
    ssize  new_backing_namelen_array_len)
{
    bool is_tstack_ptr_nonnull, is_struct_zeroed,
	isvalid_new_backing_name_array,
	isvalid_new_backing_name_array_len,
	isvalid_new_backing_namelen_array,
	isvalid_new_backing_namelen_array_len;

    struct CpTypeStack zeroed_tstack;
    memset(&zeroed_tstack, 0, sizeof (struct CpTypeStack));

    is_tstack_ptr_nonnull = tstack != NULL;
    if (!is_tstack_ptr_nonnull) return false;

    is_struct_zeroed =
	0 == memcmp(&zeroed_tstack, tstack, sizeof (struct CpTypeStack));

    isvalid_new_backing_name_array = NULL != new_backing_name_array;
    isvalid_new_backing_name_array_len = new_backing_name_array_len > 0;
    
    isvalid_new_backing_namelen_array = NULL != new_backing_namelen_array;
    isvalid_new_backing_namelen_array_len = new_backing_namelen_array_len > 0;
    
    return is_tstack_ptr_nonnull
	&& is_struct_zeroed
	&& isvalid_new_backing_name_array
	&& isvalid_new_backing_name_array_len
	&& isvalid_new_backing_namelen_array
	&& isvalid_new_backing_namelen_array_len;
}

bool cp_test_CpTypeStack_init_with_size(void);
bool cp_test_CpTypeStack_init_with_size(void) {
    bool all_ok = true;

    /* Invalid sizes */ {
	struct CpTypeStack valid_stack;
	char valid_backing_name_array[0xff];
	ssize valid_backing_namelen_array[0xff];
    
	ssize invalid_size_negative = -23544325;
	ssize valid_size = 0xff;

	memset(&valid_stack, 0, sizeof (struct CpTypeStack));

	all_ok &= CP_ASSERT(
	    !CpTypeStack_init_with_size_check(
		&valid_stack,
		valid_backing_name_array, invalid_size_negative,
		valid_backing_namelen_array, invalid_size_negative));

    }
    
    /* Non zeroed struct */ {
	struct CpTypeStack invalid_stack;
	char valid_backing_name_array[0xff];
	ssize valid_backing_namelen_array[0xff];
	ssize valid_size = 0xff;

	all_ok &= CP_ASSERT(
	    !CpTypeStack_init_with_size_check(
		&invalid_stack,
		valid_backing_name_array, valid_size,
		valid_backing_namelen_array, valid_size));
    }
    
    /* Invalid pointers */ {
	struct CpTypeStack invalid_stack;
	char invalid_backing_name_array[0xff];
	ssize valid_backing_namelen_array[0xff];
	ssize valid_size = 0xff;

	all_ok &= CP_ASSERT(
	    !CpTypeStack_init_with_size_check(
		&invalid_stack,
		invalid_backing_name_array, valid_size,
		valid_backing_namelen_array, valid_size));	
    }
    
    return all_ok;
}

bool cp_test_CpTypeManager(void);
bool cp_test_CpTypeManager(void) {
    bool all_ok = true;
    
    all_ok &= CP_ASSERT(cp_test_CpTypeStack_init_with_size());
    
    return all_ok;
}

#endif
