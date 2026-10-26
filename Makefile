CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11 -Wno-unused-function \
         -Isrc/frontend -Isrc/semantic -Isrc/ir -Isrc/backend -Ibuild
LIBS = -lm

BUILD_DIR = build
TARGET = compilador_test

OBJS = $(BUILD_DIR)/estructuras.o \
       $(BUILD_DIR)/symtab.o \
       $(BUILD_DIR)/semantic.o \
       $(BUILD_DIR)/tac.o \
       $(BUILD_DIR)/codegen.o \
       $(BUILD_DIR)/asm.o \
       $(BUILD_DIR)/parser.tab.o \
       $(BUILD_DIR)/lex.yy.o

VPATH = src/frontend:src/semantic:src/ir:src/backend:$(BUILD_DIR)

all: $(TARGET)

$(TARGET): $(BUILD_DIR) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/parser.tab.c $(BUILD_DIR)/parser.tab.h: src/frontend/parser.y | $(BUILD_DIR)
	bison -d -o $(BUILD_DIR)/parser.tab.c src/frontend/parser.y

$(BUILD_DIR)/lex.yy.c: src/frontend/scanner.lex $(BUILD_DIR)/parser.tab.h | $(BUILD_DIR)
	flex -o $(BUILD_DIR)/lex.yy.c src/frontend/scanner.lex

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	./scripts/run_all_tests.sh

test-asm: $(TARGET)
	./scripts/run_asm_tests.sh

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	rm -f Tests/valid/*.ci Tests/invalid/*.ci Tests/valid/*.s Tests/valid/*.out Tests/invalid/*.s Tests/invalid/*.out
	rm -f Tests/modules/*.ci Tests/modules/*.s Tests/modules/*.out

.PHONY: all test test-asm clean
