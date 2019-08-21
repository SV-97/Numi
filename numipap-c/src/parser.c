#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "parser.h"

Parser *newParser(char *source)
{
    Parser *p;
    p = (Parser *)malloc(sizeof(Parser));
    p->chars = source;
    p->current_char = source;
    p->operand_stack = emptyStack();
    return p;
}

void freeParser(Parser *p)
{

    if (p)
    {
        if (p->operand_stack)
        {
            freeStackAndNodes(p->operand_stack);
        }
        free(p);
    }
}

void advanceParser(Parser *parser)
{
    if (*parser->current_char != '\0')
    {
        parser->current_char++;
    }
}

void parseNumber(Parser *parser)
{
    int length = 0;
    char *start = parser->current_char;
    while (*parser->current_char != '\0')
    {
        char c = *parser->current_char;
        if (isdigit(c) || (c == '.'))
        {
            advanceParser(parser);
            length++;
        }
        else
        {
            break;
        }
    }
    char c = *parser->current_char;
    parser->chars[length] = '\0';
    double value = atof(start);
    *parser->current_char = c;
    Node *node = newNumber(value);
    pushMut(node, &parser->operand_stack);
    // printStack(parser->operand_stack);
}

void parseFunction(Parser *parser)
{
    int length = 0;
    char *start = parser->current_char;
    while (*parser->current_char != '\0')
    {
        char c = *parser->current_char;
        if (isspace(c))
        {
            break;
        }
        else
        {
            advanceParser(parser);
            length++;
        }
    }
    if (length == 0)
    {
        return;
    }
    char *buf = strndup(start, length);
    Node *node = newFunctionCall(buf);
    pushMut(node, &parser->operand_stack);
}

void parseFrame(Parser *parser)
{
    printf("parseFrame unimplemented!\n");
}

// Dispatch parsing depending on the current char
// returns 1 to signal that parsing should stop
// returns 0 otherwise
int dispatchParser(Parser *parser)
{
    char c = *parser->current_char;
    switch (c)
    {
    case '[':
        parseFrame(parser);
        break;
    case ']':
        return 1;
    default:
        if (isdigit(c))
        {
            parseNumber(parser);
        }
        else if (isspace(c))
        {
            advanceParser(parser);
        }
        else
        {
            parseFunction(parser);
        }
    }
    return 0;
}

// Parse the text of the parser into nodes on the stack
void parseParser(Parser *parser)
{
    while (parser->current_char)
    {
        char *c1 = parser->current_char;
        if (dispatchParser(parser))
        {
            break;
        }
        if (c1 == parser->current_char) // prevent infinite looping if parser fails to parse anything
        {
            break;
        }
    }
}

// Parse a string into a stack
Stack *parse(char *source)
{
    Parser *p = newParser(source);
    parseParser(p);
    Stack *l = reverseStack(p->operand_stack);
    freeStack(p->operand_stack);
    p->operand_stack = NULL;
    freeParser(p);
    return l;
}