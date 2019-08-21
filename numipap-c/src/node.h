#ifndef HEADER_node
#define HEADER_node

typedef enum
{
    FunctionCall,
    NumberLiteral,
    Error
} NodeType;

typedef struct Node
{
    NodeType type; // type of node, see NodeType for variants
    // value is of type char* for FunctionCall and Error and double for NumberLiteral
    void *value; // pointer to value at the node - e.g. 2.5 for a NumberLiteral Node
} Node;

Node *newNumber(double f);

Node *newFunctionCall(char *f);

void freeNode(Node *n);

void printNode(Node *n);
#endif
