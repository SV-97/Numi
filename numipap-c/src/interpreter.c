
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashmap.h"
#include "parser.h"
#include "stack.h"
#include "node.h"
#include "interpreter.h"

void interpret(char *source)
{
    Stack *s = parse(source);
    Interpreter *interp = newInterpreter(s);
    interpretInterpreter(interp);
    freeInterpreter(interp);
}

void insertFunctionSet(FunctionSet *fs, char name[], StackFunc f)
{
    char *str1;
    str1 = strdup(name);
    insertHashMap(fs, (void *)str1, (void *)f);
}

void addStack(Stack **stack)
{
    Node *l = (Node *)pop(stack);
    Node *r = (Node *)pop(stack);
    if (l->type == NumberLiteral && r->type == NumberLiteral)
    {
        Node *n = newNumber((*(double *)l->value) + (*(double *)r->value));
        freeNode(l);
        freeNode(r);
        pushMut(n, stack);
    }
}

void subStack(Stack **stack)
{
    Node *l = (Node *)pop(stack);
    Node *r = (Node *)pop(stack);
    if (l->type == NumberLiteral && r->type == NumberLiteral)
    {
        Node *n = newNumber((*(double *)l->value) - (*(double *)r->value));
        freeNode(l);
        freeNode(r);
        pushMut(n, stack);
    }
}

void mulStack(Stack **stack)
{
    Node *l = (Node *)pop(stack);
    Node *r = (Node *)pop(stack);
    if (l->type == NumberLiteral && r->type == NumberLiteral)
    {
        Node *n = newNumber((*(double *)l->value) * (*(double *)r->value));
        freeNode(l);
        freeNode(r);
        pushMut(n, stack);
    }
}

void divStack(Stack **stack)
{
    Node *l = (Node *)pop(stack);
    Node *r = (Node *)pop(stack);
    if (l->type == NumberLiteral && r->type == NumberLiteral)
    {
        Node *n = newNumber((*(double *)l->value) / (*(double *)r->value));
        freeNode(l);
        freeNode(r);
        pushMut(n, stack);
    }
}

void swapStack(Stack **stack)
{
    Node *l = (Node *)pop(stack);
    Node *r = (Node *)pop(stack);
    pushMut(l, stack);
    pushMut(r, stack);
}

Interpreter *newInterpreter(Stack *i_stack)
{
    Interpreter *interp = (Interpreter *)malloc(sizeof(Interpreter));
    interp->instruction_stack = i_stack;
    FunctionSet *fs = newHashMap(&basicStringHash, StringKey);
    insertFunctionSet(fs, "+", &addStack);
    insertFunctionSet(fs, "-", &subStack);
    insertFunctionSet(fs, "*", &mulStack);
    insertFunctionSet(fs, "/", &divStack);
    insertFunctionSet(fs, "swap", &divStack);
    interp->function_set = fs;
    return interp;
}

// Set all the functions in the function set to null
// so they don't get freed
void freeHelper(HashMapEntry *hme)
{
    hme->value = NULL;
}

void freeInterpreter(Interpreter *interp)
{
    if (interp)
    {
        if (interp->function_set)
        {
            mapHashMap(interp->function_set, &freeHelper);
            freeHashMap(interp->function_set);
        }
        interp->function_set = NULL;
        if (interp->instruction_stack)
        {
            freeStackAndNodes(interp->instruction_stack);
        }
        interp->instruction_stack = NULL;
        free(interp);
    }
    interp = NULL;
}

void printBuiltIns(HashMapEntry *hme)
{
    if (hme)
    {
        printf("Name: '%s'\n", (char *)hme->key);
    }
}

void interpretInterpreter(Interpreter *interp)
{
    printf("Builtins:\n");
    mapHashMap(interp->function_set, &printBuiltIns);
    printf("\n");
    Stack *exec_stack = emptyStack();
    char *name;
    for (Node *n = pop(&interp->instruction_stack); n != NULL; n = pop(&interp->instruction_stack))
    {
        switch (n->type)
        {
        case NumberLiteral:
            pushMut(n, &exec_stack);
            break;
        case FunctionCall:
            name = (char *)n->value;
            StackFunc f = getHashMap(interp->function_set, (void *)name);
            if (f)
            {
                f(&exec_stack);
            }
            else
            {
                printf("Unknown function: '%s'\n", name);
            }
            freeNode(n);
            break;
        case Error:
            printf("Error: '%s'\n", (char *)n->value);
            break;
        }
    }
    printf("Output:\n");
    printStack(exec_stack);
    printf("\n");
    freeStackAndNodes(exec_stack);
}