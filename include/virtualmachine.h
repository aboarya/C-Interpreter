#ifndef VIRTUALMACHINE 
#define VIRTUALMACHINE

typedef struct codeobject_t {
	char* type;
	char* name;
	char* value;
	char* cvalue;
	int ivalue;

}CodeObject;

typedef struct virtualmachine_t {
	int count;
	char** basic_types;
	CodeObject** code_objects;
	int (* assign_variable)();
	int (* execute)();
	void (* vm_printf)();
}VirtualMachine;

int assign_variable(VirtualMachine* self, char* type, char* name, char* value);
int execute(VirtualMachine* self);
void vm_printf(VirtualMachine* self, ...);

VirtualMachine* init_vm();

#endif