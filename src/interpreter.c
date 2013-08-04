#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/parser.h"

int run_interpreter() {
	Parser* parser = init_parser();
	while (1) {
		//read line from user
		int code = parser->parse(parser);
		switch (code) {
			case 0:
				//exit has been requested
				free(parser);
				return 0;
				break;
		}
	}
	return 0;
}

int main(int argc, char** argv) {
	return run_interpreter();
}