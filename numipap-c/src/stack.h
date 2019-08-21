#ifndef HEADER_stack
#define HEADER_stack
#include "node.h"
#include "linkedList.h"

typedef LinkedList Stack;

// returns pointer to the empty stack
Stack *emptyStack();

// Construct a stack by adding a node to another stack
// The root should be the empty stack
Stack *push(Node *n, Stack *l2);

void pushMut(Node *n, Stack **l2);

// Deconstruct the stack by popping the first element off it
Node *pop(Stack **lp);

// Like map but returns the Nil element
Stack *mapReturnStack(Stack *l, void (*f)(Node *));

// Traverse a stack and apply a function to each value
// Stack has to be properly terminated (by a Nil)
void mapStack(Stack *l, void (*f)(Node *));

void printStack(Stack *l);

// Frees a stack ! but not its nodes !
void freeStack(Stack *l);

// Frees a stack and its nodes
void freeStackAndNodes(Stack *l);

// Fold over a stack by calling a function on each node. The function is
// passed an initial value the first time and whatever the previous run
// returned on all consecutive ones.
void *foldStack(Stack *l, void *(*f)(Node *, void *), void *initial);

// Reverse a stack. Doesn't copy nodes so both stacks point to the same nodes!
// One thus has to be freed using freeStack rather than freeStackAndNodes
Stack *reverseStack(Stack *l);

// Funtion to avoid external casts from void*
Node *nodeStack(Stack *l);

#endif
