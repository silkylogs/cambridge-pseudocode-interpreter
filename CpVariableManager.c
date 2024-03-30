#ifndef CP_SRC_VARIABLE_MANAGER
#define CP_SRC_VARIABLE_MANAGER



struct CpVariableInfo {
	struct CpAsciiStringView name;
	struct CpVariableType type;
	// TODO: associate data?
};

#define CPVARIABLEMANAGER_VAR_STACK_SIZE ((ssize)(16))
struct CpVariableManager {
	ssize stack_idx;
	struct CpVariableInfo var_stack[CPVARIABLEMANAGER_VAR_STACK_SIZE];
};

void CpVariableManager_push_var_to_stack_unchecked(
	struct CpVariableManager *manager,
	struct CpAsciiStringView new_name,
	struct CpVariableType new_type);

void CpVariableManager_push_var_to_stack_unchecked(
	struct CpVariableManager *manager,
	struct CpAsciiStringView new_name,
	struct CpVariableType new_type)
{
	manager->var_stack[manager->stack_idx].name = new_name;
	manager->var_stack[manager->stack_idx].type = new_type;
	manager->stack_idx++;
}

bool CpVariableManager_push_var_to_stack_check(struct CpVariableManager *manager);
bool CpVariableManager_push_var_to_stack_check(struct CpVariableManager *manager)
{
	if (manager->stack_idx >= CPVARIABLEMANAGER_VAR_STACK_SIZE)
	return false;

	return true;
}

bool cp_test_CpVariableManager_push_var_to_stack_unchecked(void);
bool cp_test_CpVariableManager_push_var_to_stack_unchecked(void) {
	struct CpVariableManager manager = {0};
	bool all_ok = true;

	// Test general pushes
	struct CpAsciiStringView dummy_name = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_UNCHECKED("DummyVar");
	struct CpVariableType dummy_type = {
	.name = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_UNCHECKED("DummyType"),
	.id = 0,
	};
	CpVariableManager_push_var_to_stack_unchecked(&manager, dummy_name, dummy_type);

	ssize last_idx = manager.stack_idx - 1;
	all_ok &= CP_ASSERT(
	CpAsciiStringView_equals(
		manager.var_stack[last_idx].name,
		dummy_name));
	
	all_ok &= CP_ASSERT(
	0 == memcmp(
		&manager.var_stack[last_idx].type,
		&dummy_type,
		sizeof (struct CpVariableType)));
	
	
	return all_ok;
}

/*
bool cp_test_variable_management(void) {    
	char *declaration_tests[] = {
	"DECLARE Counter : INTEGER",
	"DECLARE TotalToPay : REAL",
	"DECLARE InputChar : CHAR",
	"DECLARE PlayerName: STRING",
	"DECLARE GameOver : BOOLEAN",
	"DECLARE SaveDate : DATE",
	};
	ssize len = sizeof declaration_tests / sizeof declaration_tests[0];
	
	for (ssize i = 0; i < len; ++i)
	printf("%d/%d: \"%s\"\n", i+1, len, declaration_tests[i]);
	return true;
}
*/

#endif
