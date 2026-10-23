CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11 -Wno-unused-function
LIBS = -lm

OBJS = lex.yy.o parser.tab.o estructuras.o symtab.o semantic.o tac.o codegen.o asm.o
TARGET = compilador_test

all: $(TARGET)

$(TARGET): lex.yy.c parser.tab.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: scanner.lex parser.tab.h
	flex scanner.lex

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	./run_all_tests.sh

clean:
	rm -f $(TARGET) *.o lex.yy.c parser.tab.c parser.tab.h Tests/valid/*.ci Tests/invalid/*.ci Tests/valid/*.s Tests/valid/*.out Tests/invalid/*.s Tests/invalid/*.out

.PHONY: all test clean
