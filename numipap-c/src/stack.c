#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "stack.h"
#include "linkedList.h"

// returns pointer to the empty stack
Stack *emptyStack()
{
    return emptyList();
}

// Construct a stack by adding a node to another stack
// The root should be the empty stack
Stack *push(Node *n, Stack *l2)
{
    return cons((void *)n, l2);
}

void pushMut(Node *n, Stack **l2)
{
    return consMut((void *)n, l2);
}

// Deconstruct the stack by popping the first element off it
Node *pop(Stack **lp)
{
    return (Node *)deCons(lp);
}

// Like map but returns the Nil element
Stack *mapReturnStack(Stack *l, void (*f)(Node *))
{
    return mapReturn(l, (void (*)(void *))f);
}

// Traverse a stack and apply a function to each value
// Stack has to be properly terminated (by a Nil)
void mapStack(Stack *l, void (*f)(Node *))
{
    return map(l, (void (*)(void *))f);
}

void printStack(Stack *l)
{
    mapStack(l, &printNode);
    printf("End of List\n");
}

// Frees a stack ! but not its nodes !
void freeStack(Stack *l)
{
    return freeList(l);
}

// Frees a stack and its nodes
void freeStackAndNodes(Stack *l)
{
    mapStack(l, &freeNode); // Free all nodes
    freeStack(l);
}

// Fold over a stack by calling a function on each node. The function is
// passed an initial value the first time and whatever the previous run
// returned on all consecutive ones.
void *foldStack(Stack *l, void *(*f)(Node *, void *), void *initial)
{
    return fold(l, (void *(*)(void *, void *))f, initial);
}

// Reverse a stack. Doesn't copy nodes so both stacks point to the same nodes!
// One thus has to be freed using freeStack rather than freeStackAndNodes
Stack *reverseStack(Stack *l)
{
    return reverseList(l);
}

// Funtion to avoid external casts from void*
Node *nodeStack(Stack *l)
{
    return (Node *)l->node;
}
