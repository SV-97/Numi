// memory leak searching:
// clear && clear && gcc -Wall -g level1.c && valgrind --tool=memcheck --leak-check=full ./a.out
// clear && clear && gcc -Wall -g vector.c node.c linkedList.c parser.c tests.c stack.c hashmap.c level1.c interpreter.c -lm -g && valgrind --tool=memcheck --leak-check=full ./a.out

// #define TEST
// define DEBUG

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "node.h"
#include "stack.h"
#include "linkedList.h"
#include "parser.h"
#include "interpreter.h"

#ifdef TEST
#include "tests.h"
int main()
{
    testStack();
    parserTest();
    vectorTest();
    listTest();
    hashMapIntTest();
    hashMapStringTest();
    return EXIT_SUCCESS;
}
#else
int main()
{
    char *str;
    char *sstr = "5 5 + 3 -";
    str = strdup(&sstr[0]);
    interpret(str);
    free(str);
    return EXIT_SUCCESS;
}
#endif