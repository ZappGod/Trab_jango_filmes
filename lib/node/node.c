#include <stdio.h>
#include <stdlib.h>

#include "node.h"

Node *createNode()
{
  Node *newNode = (struct Node *)malloc(sizeof(struct Node));

  if (!newNode)
  {
    fprintf(stderr, "Erro ao alocar memória para o node\n");
    exit(EXIT_FAILURE);
  }

  return newNode;
}