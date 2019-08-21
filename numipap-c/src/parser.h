#ifndef HEADER_parser
#define HEADER_parser
#include "linkedList.h"

typedef struct Parser
{
    char *chars;
    char *current_char;
    LinkedList *operand_stack;
} Parser;

Parser *newParser(char *source);

void freeParser(Parser *p);

void advanceParser(Parser *parser);

void parseNumber(Parser *parser);

void parseFunction(Parser *parser);

void parseFrame(Parser *parser);

// Dispatch parsing depending on the current char
// returns 1 to signal that parsing should stop
// returns 0 otherwise
int dispatchParser(Parser *parser);

// Parse the text of the parser into nodes on the stack
void parseParser(Parser *parser);

// Parse a string into a stack
LinkedList *parse(char *source);
#endif