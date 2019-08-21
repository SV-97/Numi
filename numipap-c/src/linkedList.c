#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "linkedList.h"

// value initializer for lists (or what it's called again)
LinkedList EMPTY_LIST = {.type = Nil, .node = NULL, .next = NULL};

// returns pointer to the empty list
LinkedList *emptyList()
{
    return &EMPTY_LIST;
}

// Construct a list by adding a node to another list
// The root should be the empty list
LinkedList *cons(void *n, LinkedList *l2)
{
    LinkedList *l = (LinkedList *)malloc(sizeof(LinkedList));
    l->type = Cons;
    l->node = n;
    l->next = l2;
    return l;
}

// See cons
void consMut(void *n, LinkedList **l2)
{
    *l2 = cons(n, *l2);
}

// Deconstruct the list by popping the first element off it
void *deCons(LinkedList **lp)
{
    LinkedList *l = *lp;
    if (l->type == Cons)
    {
        void *n = l->node;
        LinkedList *next = l->next;
        free(l);
        l = NULL;
        *lp = next;
        return n;
    }
    else
    {
        return NULL;
    }
}

// Like map but returns the Nil element
LinkedList *mapReturn(LinkedList *l, void (*f)(void *))
{
    LinkedList *next = l;
    while (next->type == Cons)
    {
        void *value = next->node;
        next = next->next;
        f(value);
    }
    return next;
}

// Traverse a list and apply a function to each value
// List has to be properly terminated (by a Nil)
void map(LinkedList *l, void (*f)(void *))
{
    mapReturn(l, f);
}

// Frees a list ! but not its nodes !
void freeList(LinkedList *l)
{
    if (l)
    {
        while (deCons(&l)) // deconstruct the list
        {
        }
        l = NULL;
    }
}

// Fold over a list by calling a function on each node. The function is
// passed an initial value the first time and whatever the previous run
// returned on all consecutive ones.
void *fold(LinkedList *l, void *(*f)(void *, void *), void *initial)
{
    LinkedList *next = l;
    void *old = initial;
    while (next->type == Cons)
    {
        void *node = next->node;
        next = next->next;
        old = f(node, old);
    }
    return old;
}

void *reverseListHelper(void *node, void *accumulator)
{
    LinkedList *l = (LinkedList *)accumulator;
    consMut(node, &l);
    return (void *)l;
}

// Reverse a list. Doesn't copy nodes so both lists point to the same nodes!
// One thus has to be freed using freeList rather than freeListAndNodes
LinkedList *reverseList(LinkedList *l)
{
    return (LinkedList *)fold(l, &reverseListHelper, emptyList());
}

void printList(LinkedList *l)
{
    LinkedList *next = l;
    if (next)
    {
        while (next->type == Cons)
        {
            printf("Cons.%p -> ", next);
            next = next->next;
        }
        printf("Nil\n");
    }
    else
    {
        printf("Tried to print list @ NULL.\n");
    }
}

// traverse the list and return the first item for which predicate returns
// a non zero value. Returns NULL if there are none. Helper allows to pass an
// additional "helper" value to the function (e.g. for comparisons).
void *findList(LinkedList *l, int (*predicate)(void *, void *), void *helper)
{
    LinkedList *next = l;
    if (!next)
    {
        return NULL;
    }
    while (next->type == Cons)
    {
        if (predicate(next->node, helper))
        {
            return next->node;
        }
        next = next->next;
    }
    return NULL;
}
