#ifndef PARSER
#define PARSER

#include "../include/virtualmachine.h"

const int BUFFER_SIZE;

typedef struct parser_t {
	char* input;
	char* input_copy;
	
	int (* parse)();
	int (* is_exit)();
	void (* read_line)();
	void (* do_exit)();
	int (* is_assignment)();
	int (* matches)();
	char* (* trim)();
	int (* split_decl)();
	int (* is_var_decl)();
	int (* parse_var_decl)();
	char* (* extract)();
	int (* is_fun_call)();
	int (* parse_function_call)();
} Parser;

int parse(Parser* parser);
int is_exit(Parser* parser);
void read_line(Parser* parser);
void do_exit(Parser* parser);
int is_assignment(Parser* parser);
int matches (Parser* parser, char* pattern);
char* trim(Parser* parser, char* str);
int has_operators(Parser* parser);
int split_decl(Parser* parser);
int is_var_decl(Parser* parser);
int parse_var_decl(Parser* parser);
char* extract(Parser* parser, char* pattern);
int is_fun_call(Parser* parser);
int parse_function_call(Parser* parser);


#endif