#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "../include/parser.h"
#include "../include/virtualmachine.h"

const int BUFFER_SIZE = 500;
const char* EXIT = "exit()";

VirtualMachine* vm;

char* trim(Parser* self, char* str) {
    char *end;
    // Trim leading space
    if (str == "" || str == "0") return str;
    while(isspace(*str)) str++;
    if(*str == 0)  // All spaces?
        return str;
    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace(*end)) end--;
    // Write new null terminator
    *(end+1) = 0;
    return str;
}

int is_var_decl(Parser* self) {
    char* pattern = "^((int|char)[ \t]+)([a-z][a-zA-Z0-9]*[ \t]*)([ \t]*=[ \t]*)([0-9a-zA-Z]+)(;)";
    regex_t re;
    regmatch_t match;

    if(regcomp(&re, pattern, REG_EXTENDED) != 0) {
        return 0;
    }

    if (regexec(&re, self->input, 1, &match, 0) == 0) {
        return 1;
    }
    return 0;
}

char* extract(Parser* self, char* pattern) {
    regex_t re;
    regmatch_t match;

    if(regcomp(&re, pattern, REG_EXTENDED) != 0) {
        return "0";
    }

    if (regexec(&re, self->input_copy, 1, &match, 0) == 0) {
        int len = (int) match.rm_eo;
        char* result = malloc(sizeof(char) * (len+1));
        // memcpy(result, &self->input_copy, ((int) match.rm_eo));
        strncpy(result, self->input_copy, len);

        result[len] = '\0';
        self->input_copy += match.rm_eo;
        return result;
    }

    return "0";
}

int parse_var_decl(Parser* self) {
    self->input_copy = (char*) malloc(sizeof(char) * BUFFER_SIZE);
    strcpy(self->input_copy, self->input);

    char* type = self->trim(self, self->extract(self, "^(int|char)"));
    char* name = self->trim(self, self->extract(self, "[ \t]*[a-z][a-zA-Z0-9]*[ \t]*"));
    self->extract(self, "[=]");
    char* value = self->trim(self, self->extract(self, "[0-9]+[^;]"));

    return 0;
}

int is_fun_call(Parser* self) {
    char* pattern = "^([a-z][a-zA-Z0-9]*)[(]([\"]?[a-z][a-zA-Z0-9]*[ \t]*[\n]*[\"]?[,]?[ \t]*)*[)][;]";
    regex_t re;
    regmatch_t match;

    if(regcomp(&re, pattern, REG_EXTENDED) != 0) return 0;

    if (regexec(&re, self->input, 1, &match, 0) == 0) {
        return 1;
    }

    return 0;

}

int parse_function_call(Parser* self) {
    self->input_copy = (char*) malloc(sizeof(char) * BUFFER_SIZE);
    strcpy(self->input_copy, self->input);

    char* name = self->trim(self, self->extract(self, "^([a-z][a-zA-Z0-9]*)"));
    self->extract(self, "[(]");

    char** args = NULL;
    char* arg = NULL;
    int counter = 0;

    arg = self->trim(self, self->extract(self, "([\"]?[a-z][a-zA-Z0-9]*[ \t]*[\n]*[\"]?)"));
    
    while(arg != "0") {
        if (args == NULL) {
            args = (char*) malloc(sizeof(char) * sizeof(arg));
        } else {
            char* tmp = realloc(args, sizeof(char) * (sizeof(arg) + sizeof(args)));
            args = tmp;
        }
        args[counter] = arg;
        counter++;
        
        arg = self->trim(self, self->extract(self, "[,]"));
        if (arg == "0") {
            break;
        } else {
            arg = self->trim(self, self->extract(self, "([\"]?[a-z][a-zA-Z0-9]*[ \t]*[\n]*[\"]?)"));
        }

    }
    
    
    return 0;

}

void do_exit(Parser* self) {
    free(self->input);
    free(vm);
}

int is_exit(Parser* self) {
    return strcmp(self->input, EXIT) == 0;
}

void read_line(Parser* self) {
    if (self->input != NULL) {
        free(self->input);
    }
    self->input = (char*) malloc(sizeof(char) * BUFFER_SIZE);
    printf(">>>>>>> ");
    fgets(self->input, BUFFER_SIZE, stdin);
    self->input[strlen(self->input) - 1] = '\0';
}

int parse(Parser* self) {
    self->read_line(self);
    
    if (self->is_exit(self)) {
        self->do_exit(self);
        return 0;
    } else {
        
        if (self->is_var_decl(self)) {
            if (self->parse_var_decl(self)) {
                printf("good to go\n");
            }
            return 1;
        } else {
            if (self->is_fun_call(self)) {
                self->parse_function_call(self);
            }
        }
        return 1;
    }
    return 0;
}

Parser* init_parser() {
    Parser* parser = (Parser*) malloc(sizeof(Parser) * BUFFER_SIZE);
    parser->input = NULL;
    parser->parse = parse;
    parser->read_line = read_line;
    parser->is_exit = is_exit;
    parser->do_exit = do_exit;
    parser->trim = trim;
    parser->is_var_decl = is_var_decl;
    parser->extract = extract;
    parser->parse_var_decl = parse_var_decl;
    parser->is_fun_call = is_fun_call;
    parser->parse_function_call = parse_function_call;
    vm = init_vm();
    return parser;
}

