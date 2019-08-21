#ifndef HEADER_linkedList
#define HEADER_linkedList

typedef enum
{
    Nil, // Marker for empty list
    Cons // Marker "Construct" for list with value
} ListType;

// LinkedList for Nodes - I didn't bother making it generic
typedef struct LinkedList
{
    ListType type;           // type of list element either Nil or Cons
    void *node;              // pointer to value if type is Cons
    struct LinkedList *next; // pointer to next node if type is Cons
} LinkedList;

// returns pointer to the empty list
LinkedList *emptyList();
// Construct a list by adding a node to another list
// The root should be the empty list
LinkedList *cons(void *n, LinkedList *l2);

// Like cons but mutates the list
void consMut(void *n, LinkedList **l2);

// Deconstruct the list by popping the first element off it
void *deCons(LinkedList **lp);

// Like map but returns the Nil element
LinkedList *mapReturn(LinkedList *l, void (*f)(void *));

// Traverse a list and apply a function to each value
// List has to be properly terminated (by a Nil)
void map(LinkedList *l, void (*f)(void *));

// Frees a list ! but not its nodes !
void freeList(LinkedList *l);

// Fold over a list by calling a function on each node. The function is
// passed an initial value the first time and whatever the previous run
// returned on all consecutive ones.
void *fold(LinkedList *l, void *(*f)(void *, void *), void *initial);

// Reverse a list. Doesn't copy nodes so both lists point to the same nodes!
// One thus has to be freed using freeList rather than freeListAndNodes
LinkedList *reverseList(LinkedList *l);

// traverse the list and return the first item for which predicate returns
// a non zero value. Returns NULL if there are none. Helper allows to pass an
// additional "helper" value to the function (e.g. for comparisons).
void *findList(LinkedList *l, int (*predicate)(void *, void *), void *helper);

// Print the structure of a list
void printList(LinkedList *l);



#endif
