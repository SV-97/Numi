
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"
#include "linkedList.h"
#include "node.h"
#include "parser.h"
#include "stack.h"
#include "vector.h"

int approxEqual(double a, double b)
{
    return abs(a - b) < 0.0000000001;
}

void testStack()
{
    printf("\nStack Test...\n");
    Node *node1 = newNumber(10);
    Node *node2 = newNumber(5);
    Node *node3 = newNumber(2.5);
    Stack *l1 = push(node1, emptyStack());
    pushMut(node2, &l1); // this and the next line are funtionally equivalent
    l1 = push(node3, l1);
#ifdef DEBUG
    printf("Stack 1:\n");
    printList(l1);
    printf("\n");
#endif
    double v1 = *(double *)((Node *)l1->node)->value;
    double v2 = *(double *)((Node *)l1->next->node)->value;
    double v3 = *(double *)((Node *)l1->next->next->node)->value;
    void *n4 = l1->next->next->next->node;
    assert(approxEqual(v1, 2.5));
    assert(approxEqual(v2, 5.0));
    assert(approxEqual(v3, 10.0));
    assert(n4 == NULL);

    Node *n5 = newNumber(13);
    char *str = (char *)malloc(sizeof("abcdef") + 1);
    strcpy(str, "abcdef");
    Node *n6 = newFunctionCall(str);
    Stack *l2 = push(n5, emptyStack());
    pushMut(n6, &l2);

    // Test reverse
    Stack *reversed1 = reverseStack(l1);
    Stack *reversed2 = reverseStack(l2);
    assert(approxEqual(*(double *)((Node *)reversed1->node)->value, 10.0));
    assert(strcmp((char *)((Node *)reversed2->node)->value, str));
    assert(reversed1->node != reversed2->node);
#ifdef DEBUG
    printf("Reversed Stack 1:\n");
    printList(reversed1);
    printf("\n");
    printList(reversed2);
    printf("\n");
    printList(emptyList());
#endif

    // assert that nothing messed with the empty stack
    assert(emptyStack()->type == Nil);

    freeStack(l1);
    freeStack(l2);
    freeStackAndNodes(reversed1);
    freeStackAndNodes(reversed2);
    printf("Stack Test ✓\n\n");
}

void parserTest()
{
    printf("\nParser Test...\n");
    char *str;
    char *test_str1 = "123.312 2 +";
    str = strdup(&test_str1[0]);
    LinkedList *result1 = parse(str);
    free(str);

    char *test_str2 = "2 1 + 32 - ";
    str = strdup(&test_str2[0]);
    LinkedList *result2 = parse(str);
#ifdef DEBUG
    printList(result1);
    printf("\n");
    printList(result2);
#endif
    free(str);
    freeStackAndNodes(result1);
    freeStackAndNodes(result2);
    printf("Parser Test ✓\n\n");
}

void vectorTest()
{
    printf("\nVector Test...\n");
    Vector *v = newVector();

    for (int i = 0; i < 12; i++)
    {
        int *integer = (int *)malloc(sizeof(int));
        *integer = i;
        pushVector(v, (void *)integer);
    }

    for (int i = 0; i < 20; i++)
    {
        int *integer = (int *)getVector(v, i);
        if (integer)
        {
            assert(*integer == i);
        }
    }

    int *i6 = (int *)malloc(sizeof(int));
    *i6 = 1337;
    insertVector(v, 5, i6);
#ifdef DEBUG
    printf("Out after insert: %d\n", *(int *)getVector(v, 5));
#endif
    assert(*i6 == *(int *)getVector(v, 5));

    freeVector(v);
    printf("Vector Test ✓\n\n");
}

int listTestHelper(void *a, void *b)
{
    return *(int *)a == *(int *)b;
}

void listTest()
{
    printf("\nList Test...\n");

    LinkedList *l = emptyList();

    for (int i = 0; i < 20; i++)
    {
        int *j = (int *)malloc(sizeof(int));
        *j = i;
        consMut(j, &l);
    }
#ifdef DEBUG
    printList(l);
#endif

    int five = 5;
    int *l2 = (int *)findList(l, &listTestHelper, (void *)&five);
    assert(*l2 == 5);
#ifdef DEBUG
    printf("L2 = %d\n", *l2);
#endif

    map(l, &free);
    freeList(l);
    printf("List Test ✓\n\n");
}

void hashMapIntTest()
{
    printf("\nHashMap Test for Integer keys...\n");
    HashMap *hm = newHashMap(&id, IntegerKey);
    for (int i = 5; i < 20; i++)
    {
        int *key1 = (int *)malloc(sizeof(int));
        *key1 = i;
        int *val1 = (int *)malloc(sizeof(int));
        *val1 = i * i;
        insertHashMap(hm, (void *)key1, (void *)val1);
    }

    /* LinkedList *l = getVector(hm->items, 0);
    printf("List from hashmap: ");
    printList(l); */
    int key1 = 5;
    int *out1 = (int *)getHashMap(hm, (void *)&key1);
    assert(*out1 == 25);
#ifdef DEBUG
    if (out1)
    {
        printf("Out 1: %d\n", *out1);
    }
    else
    {
        printf("Out1: NULL - this is an error\n");
    }
#endif
    freeHashMap(hm);
    printf("HashMap Test for Integer keys ✓\n\n");
}

void hashMapStringTest()
{
    printf("\nHashMap Test for String keys...\n");
    HashMap *hm = newHashMap(&basicStringHash, StringKey);
    char *str1;
    char *sstr1 = "abcde";
    str1 = strdup(&sstr1[0]);
    int *val1 = (int *)malloc(sizeof(int));
    *val1 = 5;

    char *str2;
    char *sstr2 = "def";
    str2 = strdup(&sstr2[0]);
    int *val2 = (int *)malloc(sizeof(int));
    *val2 = 23;
    insertHashMap(hm, (void *)str1, (void *)val1);
    insertHashMap(hm, (void *)str2, (void *)val2);
    int *out1 = (int *)getHashMap(hm, (void *)str1);
    int *out2 = (int *)getHashMap(hm, (void *)str2);
#ifdef DEBUG
    printf("Out1: %d Val1: %d\n", *out1, *val1);
    printf("Out2: %d Val2: %d\n", *out2, *val2);
#endif
    assert(*out1 == *val1);
    assert(*out2 == *val2);
    freeHashMap(hm);
    printf("HashMap Test for String keys ✓\n\n");
}
