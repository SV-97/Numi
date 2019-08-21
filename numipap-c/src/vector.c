#include <stdlib.h>
#include <stdio.h>
#include "vector.h"

// allocate new vector with capacity for 16 elements
Vector *newVector()
{
    Vector *v = (Vector *)malloc(sizeof(Vector));
    v->capacity = 2;
    void **fields = (void **)malloc(v->capacity * sizeof(void *)); // change this to calloc maybe
    v->currently_occupied = 0;
    v->fields = fields;
    for (int i = 0; i < v->capacity; i++)
    {
        v->fields[i] = NULL;
    }
    return v;
}

void freeVector(Vector *v)
{
    if (v)
    {
        if (v->fields)
        {
            for (int i = 0; i < v->capacity; i++)
            {
                if (v->fields[i])
                {
                    free(v->fields[i]);
                    v->fields[i] = NULL;
                }
            }
            free(v->fields);
        }
        free(v);
    }
}

void resizeVec(Vector *v)
{
    int new_size = v->capacity * 2;
    void **new_fields = (void **)malloc(new_size * sizeof(void *));
    for (int i = 0; i < v->capacity; i++)
    {
        new_fields[i] = v->fields[i];
    }
    for (int i = v->capacity; i < new_size; i++)
    { // can be dropped if using calloc
        new_fields[i] = NULL;
    }
    v->capacity = new_size;
    free(v->fields);
    v->fields = new_fields;
}

void pushVector(Vector *v, void *data)
{
#ifdef DEBUG
    printf("Pushing: %d / %d\n", v->currently_occupied, v->capacity);
#endif
    if (!(v->capacity > v->currently_occupied + 1))
    {
        resizeVec(v);
    }
    v->fields[v->currently_occupied] = data;
    v->currently_occupied++;
}

// Like insert but without prior free
void renewVector(Vector *v, int index, void *data)
{
    if (!v->fields[index])
    {
        v->currently_occupied++;
    }
    v->fields[index] = data;
}

void insertVector(Vector *v, int index, void *data)
{
#ifdef DEBUG
    printf("Insertion at %d\n", index);
#endif
    if (v->fields[index])
    {
        free(v->fields[index]);
    }
    else
    {
        v->currently_occupied++;
    }
    v->fields[index] = data;
}

void *getVector(Vector *v, int index)
{
#ifdef DEBUG
    printf("Retrieving %d\n", index);
#endif
    if ((index >= 0) && (index < v->capacity))
    {
        return v->fields[index];
    }
    else
    {
        return NULL;
    }
}

// apply a function to each element of the vector
void mapVector(Vector *v, void (*f)(void *))
{
    for (int i = 0; i < v->capacity; i++)
    {
        if (v->fields[i])
        {
            f(v->fields[i]);
        }
    }
}

/*
int main()
{
    Vector *v = newVector();

    for (int i = 0; i < 12; i++)
    {
        int *integer = (int *)malloc(sizeof(int));
        *integer = i;
        pushVector(v, (void *)integer);
    }

    int *i5 = (int *)getVector(v, 1);
    if (i5)
    {
        printf("Out: %d\n", *i5);
    }
    else
    {
        printf("hmm\n");
    }

    return EXIT_SUCCESS;
}
*/