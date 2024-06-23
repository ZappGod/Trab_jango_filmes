#include <stdio.h>
#include <string.h>

#include "actor.h"

void initActorArray(ActorArray *a, size_t initialCapacity)
{
  a->array = (Actor *)malloc(initialCapacity * sizeof(Actor));

  if (a->array == NULL)
  {
    fprintf(stderr, "Erro ao alocar memória para o array de artistas\n");
    exit(EXIT_FAILURE);
  }

  a->size = 0;
  a->capacity = initialCapacity;
}

// Função para inserir um artista no array dinâmico
void insertActorArray(ActorArray *a, Actor actor)
{
  if (a->size == a->capacity)
  {
    a->capacity *= 2;
    a->array = (Actor *)realloc(a->array, a->capacity * sizeof(Actor));
    if (a->array == NULL)
    {
      fprintf(stderr, "Erro ao realocar memória para o array de artistas\n");
      exit(EXIT_FAILURE);
    }
  }
  a->array[a->size++] = actor;
}

// Função para liberar memória do array dinâmico de artistas
void freeActorArray(ActorArray *a)
{
  size_t i;
  for (i = 0; i < a->size; i++)
  {
    free(a->array[i].name);
    Node *current = a->array[i].movies;
    while (current != NULL)
    {
      Node *next = current->next;
      free(current);
      current = next;
    }
  }
  free(a->array);
}

void readActorsFile(const char *filename, ActorArray *actors, size_t numActors)
{
  FILE *file = fopen(filename, "r");

  if (!file)
  {
    fprintf(stderr, "Erro ao abrir o arquivo %s\n", filename);
    exit(EXIT_FAILURE);
  }

  char line[1024];
  fgets(line, sizeof(line), file); // Pula o cabeçalho

  size_t actorsRead = 0;
  while (fgets(line, sizeof(line), file) && actorsRead < numActors)
  {
    Actor actor;
    char *token = strtok(line, "\t");

    actor.id = atoi(token + 2); // Ignora os dois primeiros caracteres 'nm'
    token = strtok(NULL, "\t");
    actor.name = strdup(token);

    if (actor.name == NULL)
    {
      fprintf(stderr, "Erro ao alocar memória para o nome do ator\n");
      exit(EXIT_FAILURE);
    }

    token = strtok(NULL, "\t"); // Ignora birthYear
    token = strtok(NULL, "\t"); // Ignora deathYear

    token = strtok(NULL, "\t"); // Pega primaryProfession
    token = strtok(NULL, "\t"); // Pega knownForTitles
    actor.movies = NULL;

    char *movieID = strtok(token, ",");
    while (movieID)
    {
      Node *newNode = createNode();

      newNode->movieID = atoi(movieID + 2); // Ignora os dois primeiros caracteres 'tt'
      newNode->next = actor.movies;
      actor.movies = newNode;
      movieID = strtok(NULL, ",");
    }

    insertActorArray(actors, actor);
    // printf("Ator lido: %s\n", actor.name); // Mensagem de depuração

    actorsRead++;
  }

  fclose(file);
}
