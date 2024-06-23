#ifndef NODE_HEADER
#define NODE_HEADER

typedef struct Node
{
    int movieID;
    struct Node *next;
} Node;

Node *createNode();

#endif