#ifndef HEADER_tests
#define HEADER_tests
#ifdef TEST
#include <assert.h>
#endif

int approxEqual(double a, double b);

void testStack();

void parserTest();

void vectorTest();

int listTestHelper(void *a, void *b);

void listTest();

void hashMapIntTest();

void hashMapStringTest();

#endif