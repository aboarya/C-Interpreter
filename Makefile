BUILD_DIR = build
SRC_DIR = src

GCC = gcc
GCC_OPTS = -Wall -g -L$(BUILD_DIR)/ -std=c99



all: interpreter
	./$(BUILD_DIR)/interpreter

mkdir:
	mkdir -p $(BUILD_DIR)

vm: mkdir
	$(GCC) $(GCC_OPTS) -c $(SRC_DIR)/virtualmachine.c -o $(BUILD_DIR)/vm.o

parser: vm
	$(GCC) $(GCC_OPTS) -c $(SRC_DIR)/parser.c -o $(BUILD_DIR)/parser.o

interpreter: parser
	$(GCC) $(GCC_OPTS) -o $(BUILD_DIR)/interpreter $(SRC_DIR)/interpreter.c $(BUILD_DIR)/parser.o $(BUILD_DIR)/vm.o

