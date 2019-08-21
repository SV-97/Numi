#ifndef HEADER_vector
#define HEADER_vector

typedef struct Vector Vector;

typedef struct Vector
{
    int capacity;
    int currently_occupied; // how many fields are currently occupied by some data
    void **fields;
} Vector;

// Allocate new vector with capacity for 2 elements
Vector *newVector();

// Free a Vector
void freeVector(Vector *v);

// Resize a vec by doubling it's capacity
void resizeVec(Vector *v);

// Push some data into the vector
void pushVector(Vector *v, void *data);

// Get some data from the vector
void *getVector(Vector *v, int index);

// apply a function to each element of the vector
void mapVector(Vector *v, void (*f)(void *));

// Insert a value into the vector by index
void insertVector(Vector *v, int index, void *data);

// Like insert but without free
void renewVector(Vector *v, int index, void *data);

#endif