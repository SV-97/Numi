#ifndef HEADER_hashmap
#define HEADER_hashmap
#include "vector.h"
#include "linkedList.h"

typedef enum
{
    IntegerKey,
    StringKey,
} HashMapKeyType;

typedef struct HashMapEntry
{
    void *key;
    void *value;
} HashMapEntry;

HashMapEntry *newHashMapEntry(void *key, void *value);

void freeHashMapEntry(HashMapEntry *hme);

typedef struct HashMap
{
    int (*hashfunction)(void *);      // Function that hashes whatever key value it gets
    Vector *items;                    // Vector of linked lists that hold HashMapEntries
    int item_count;                   // How many items the map currently holds
    int (*predicate)(void *, void *); // predicate that's called by findList internally
    int next_resize;                  // At how many items the HashMap will resize
} HashMap;

int integerPredicateHashMap(void *value, void *key);

int stringPredicateHashMap(void *value, void *key);

// Maybe add a `withCapacity` variant to allow initializing with a bigger vec so there's
// not as many resizes at the beginning
HashMap *newHashMap(int (*hashfunction)(void *), HashMapKeyType key_type);

void freeHashMapList(LinkedList *list);

void freeHashMap(HashMap *hm);

void setResize(HashMap *hm);

int resizeMetric(HashMap *hm);

int getHash(HashMap *hm, void *key);

void resizeHashMap(HashMap *hm);

void insertHashMap(HashMap *hm, void *key, void *value);

void *getHashMap(HashMap *hm, void *key);

// identity function for int
// can be used as a hash function for ints...
int id(void *v);

int basicStringHash(void *key);

void mapHashMap(HashMap *hm, void (*f)(HashMapEntry *));

#endif
