#ifndef HEADER_interpreter
#define HEADER_interpreter
#include <stdlib.h>
#include "hashmap.h"
#include "parser.h"
#include "stack.h"
#include "node.h"

typedef HashMap FunctionSet;
typedef void (*StackFunc)(Stack **);

typedef struct Interpreter
{
    Stack *instruction_stack;
    FunctionSet *function_set;
} Interpreter;

void interpret(char *source);

void insertFunctionSet(FunctionSet *fs, char name[], StackFunc f);

void addStack(Stack **stack);

void subStack(Stack **stack);

Interpreter *newInterpreter(Stack *i_stack);

void freeInterpreter(Interpreter *interp);

void interpretInterpreter(Interpreter *interp);

#endif
