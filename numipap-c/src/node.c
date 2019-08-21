#include <stdlib.h>
#include <stdio.h>
#include "node.h"

Node *newNumber(double f)
{
    double *v;
    v = (double *)malloc(sizeof(double));
    *v = f;
    Node *n;
    n = (Node *)malloc(sizeof(Node));
    n->type = NumberLiteral;
    n->value = (void *)v;
    return n;
}

Node *newFunctionCall(char *f)
{
    Node *n;
    n = (Node *)malloc(sizeof(Node));
    n->type = FunctionCall;
    n->value = (void *)f;
    return n;
}

void freeNode(Node *n)
{
    if (n)
    {
        if (n->value)
        {
            free(n->value);
            n->value = NULL;
        }
        free(n);
        n = NULL;
    }
}

void printNode(Node *n)
{
    switch (n->type)
    {
    case NumberLiteral:
        printf("Number Node: %f\n", *(double *)n->value);
        break;
    case Error:
    case FunctionCall:
        printf("FuCall Node: %s\n", (char *)n->value); // not sure of pointer type, needs testing
        break;
    }
}