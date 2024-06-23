#ifndef ACTOR_HEADER
#define ACTOR_HEADER

#include <stdlib.h>
#include "../node/node.h"

typedef struct Actor
{
  int id;
  char *name;
  struct Node *movies;
} Actor;

// Estrutura para o Array Dinâmico de Artistas
typedef struct ActorArray
{
  struct Actor *array;
  size_t size;
  size_t capacity;
} ActorArray;

void initActorArray(ActorArray *a, size_t initialCapacity);
void insertActorArray(ActorArray *a, Actor actor);
void freeActorArray(ActorArray *a);
void readActorsFile(const char *filename, ActorArray *actors, size_t numActors);

#endif