//clear && clear && gcc -Wall -g vector.c linkedList.c hashmap.c -lm && valgrind --tool=memcheck --leak-check=full --track-origins=yes ./a.out

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "vector.h"
#include "linkedList.h"
#include "hashmap.h"

HashMapEntry *newHashMapEntry(void *key, void *value)
{
    HashMapEntry *hme = (HashMapEntry *)malloc(sizeof(HashMapEntry));
    hme->key = key;
    hme->value = value;
    return hme;
}

void freeHashMapEntry(HashMapEntry *hme)
{
    if (hme)
    {
        if (hme->key)
        {
            free(hme->key);
            hme->key = NULL;
        }
        if (hme->value)
        {
            free(hme->value);
            hme->value = NULL;
        }
        free(hme);
        hme = NULL;
    }
}

int integerPredicateHashMap(void *value, void *key)
{
    HashMapEntry *hme = (HashMapEntry *)value;
    // printf("val: %d key: %d\n", *(int *)hme->key, *(int *)key);
    return *(int *)hme->key == *(int *)key;
}

int stringPredicateHashMap(void *value, void *key)
{
    HashMapEntry *hme = (HashMapEntry *)value;
    char *entry_key = (char *)hme->key;
    int a = strcmp(entry_key, (char *)key);
    // printf("Comparing key: '%s' with entry key '%s'; strcmp=%d\n", (char *)key, entry_key, a);
    return a == 0;
}

// Maybe add a `withCapacity` variant to allow initializing with a bigger vec so there's
// not as many resizes at the beginning
HashMap *newHashMap(int (*hashfunction)(void *), HashMapKeyType key_type)
{
    HashMap *hm = (HashMap *)malloc(sizeof(HashMap));
    hm->hashfunction = hashfunction;
    hm->items = newVector();
    hm->item_count = 0;
    setResize(hm);
    switch (key_type)
    {
    case IntegerKey:
        hm->predicate = &integerPredicateHashMap;
        break;
    case StringKey:
        hm->predicate = &stringPredicateHashMap;
        break;
    default:
        hm->predicate = NULL;
    }
    return hm;
}

void freeHashMapList(LinkedList *list)
{
    if (list)
    {
        if (list->type == Cons)
        {
            LinkedList *l = (LinkedList *)list;
            map(l, (void (*)(void *)) & freeHashMapEntry);
            freeList(l);
        }
        else
        {
            LinkedList *l = (LinkedList *)list;
            freeList(l);
        }
    }
}

void freeHashMap(HashMap *hm)
{
    if (hm)
    {
        if (hm->items)
        {
            mapVector(hm->items, (void (*)(void *)) & freeHashMapList);
            free(hm->items->fields);
            hm->items->fields = NULL;
            freeVector(hm->items);
            hm->items = NULL;
        }
        free(hm);
        hm = NULL;
    }
}

void setResize(HashMap *hm)
{
    hm->next_resize = trunc(hm->items->capacity * 0.75);
}

int resizeMetric(HashMap *hm)
{
    return hm->item_count + 1 > hm->next_resize;
}

int getHash(HashMap *hm, void *key)
{
    return (hm->hashfunction(key)) % (hm->items->capacity - 1);
}

void resizeHashMap(HashMap *hm)
{
#ifdef DEBUG
    printf("Resizing HashMap: %p from %d ", hm, hm->items->capacity);
#endif
    LinkedList *l = emptyList();
    for (int i = 0; i < hm->items->capacity; i++)
    {
        if (hm->items->fields[i])
        {
            LinkedList *next = (LinkedList *)hm->items->fields[i];
            l = (LinkedList *)fold(next, (void *(*)(void *, void *)) & cons, (void *)l);
            freeList(next);
            hm->items->fields[i] = NULL;
        }
    }
    resizeVec(hm->items);
    setResize(hm);
#ifdef DEBUG
    printf("to %d.\n", hm->items->capacity);
#endif
    LinkedList *next = l;
    while (next->type == Cons)
    {
        HashMapEntry *hme = (HashMapEntry *)next->node;
        int hash = getHash(hm, hme->key);
#ifdef DEBUG
        printf("Reinserting Hash: %d\n", hash);
#endif
        void *key_list = getVector(hm->items, hash);
        if (!key_list)
        {
            key_list = (void *)emptyList();
        }
        consMut((void *)hme, (LinkedList **)&key_list);
        renewVector(hm->items, hash, key_list);
        next = next->next;
    }
    freeList(l);
}

void insertHashMap(HashMap *hm, void *key, void *value)
{
    if (resizeMetric(hm))
    {
        resizeHashMap(hm);
    }
    int hash = getHash(hm, key);
#ifdef DEBUG
    printf("Inserting Hash: %d\n", hash);
#endif
    void *key_list = getVector(hm->items, hash);
    if (!key_list)
    {
        key_list = (void *)emptyList();
    }
    HashMapEntry *hme = newHashMapEntry(key, value);
    consMut((void *)hme, (LinkedList **)&key_list);
    renewVector(hm->items, hash, key_list);
    hm->item_count++;
    /* printf("List after insertion: ");
    printList((LinkedList *)getVector(hm->items, hash)); */
}

void *getHashMap(HashMap *hm, void *key)
{
    int hash = getHash(hm, key);
#ifdef DEBUG
    printf("Extracting Hash: %d\n", hash);
#endif
    LinkedList *l = (LinkedList *)getVector(hm->items, hash);
    /* LinkedList *n = l->next;
    while (n->type == Cons)
    {
        (HashMapEntry *)(n->node)
            n = n->next;
    } */
    HashMapEntry *hme = findList(l, hm->predicate, key);
    if (hme)
    {
        return hme->value;
    }
    else
    {
        return NULL;
    }
}

void mapHashMap(HashMap *hm, void (*f)(HashMapEntry *))
{
    for (int i = 0; i < hm->items->capacity; i++)
    {
        if (hm->items->fields[i])
        {
            map(hm->items->fields[i], (void (*)(void *))f);
        }
    }
}

int id(void *v)
{
    return *(int *)v;
}

int basicStringHash(void *key)
{
    int sum = 0;
    if (!key)
    {
        printf("What the fuck did you just bring upon this cursed land\n");
    }
    for (char *next = key; *next != '\0'; next++)
    {
        sum += (int)*next;
    }
    return sum;
}
