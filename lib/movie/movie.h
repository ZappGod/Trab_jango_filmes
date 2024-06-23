#ifndef MOVIE_HEADER
#define MOVIE_HEADER

#include <stdlib.h>
#include "../node/node.h"

// Estrutura para o Filme
typedef struct Movie
{
  int id;
  char *title;
  struct Node *neighbors;
} Movie;

// Estrutura para o Array Dinâmico de Filmes (Lista de Adjacências)
typedef struct MovieArray
{
  struct Movie *array;
  size_t size;
  size_t capacity;
} MovieArray;

void initMovieArray(MovieArray *ma, size_t initialCapacity);
void insertMovieArray(MovieArray *ma, Movie movie);
void freeMovieArray(MovieArray *ma);
void readMoviesFile(const char *filename, MovieArray *movies, size_t numMovies);

#endif
