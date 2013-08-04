#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "../include/virtualmachine.h"

int assign_variable(VirtualMachine* vm, char* type, char* name, char* value) {
	CodeObject co = (CodeObject) {.type = type, .name = name, .cvalue = NULL, .ivalue = NULL, .value = value};
	CodeObject* cop = (CodeObject*) malloc(sizeof(CodeObject));
	*cop = co;

	if (vm->code_objects == NULL) {
		vm->code_objects = (CodeObject*) malloc(sizeof(CodeObject) * sizeof(cop));

	} else {
		CodeObject** tmp = realloc(vm->code_objects, (sizeof(vm->code_objects) + sizeof(cop)) * sizeof(CodeObject));
		
		if (tmp != NULL) {
			vm->code_objects = tmp;
		} else {
			return -1;
		}
	}

	vm->code_objects[vm->count] = cop;
	vm->count++;

}

VirtualMachine* init_vm() {
	VirtualMachine* vm = (VirtualMachine*) malloc(sizeof(VirtualMachine) * 100);
	vm->code_objects = NULL;
	vm->assign_variable = assign_variable;
	vm->count = 0;
	return vm;
}