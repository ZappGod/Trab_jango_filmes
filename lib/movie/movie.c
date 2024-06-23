#include <stdio.h>
#include <string.h>

#include "movie.h"

void initMovieArray(MovieArray *ma, size_t initialCapacity)
{
  ma->array = (Movie *)malloc(initialCapacity * sizeof(Movie));

  if (ma->array == NULL)
  {
    fprintf(stderr, "Erro ao alocar memória para o array de filmes\n");
    exit(EXIT_FAILURE);
  }

  ma->size = 0;
  ma->capacity = initialCapacity;
}

// Função para inserir um filme no array dinâmico
void insertMovieArray(MovieArray *ma, Movie movie)
{
  if (ma->size == ma->capacity)
  {
    ma->capacity *= 2;
    ma->array = (Movie *)realloc(ma->array, ma->capacity * sizeof(Movie));
    if (ma->array == NULL)
    {
      fprintf(stderr, "Erro ao realocar memória para o array de filmes\n");
      exit(EXIT_FAILURE);
    }
  }
  ma->array[ma->size++] = movie;
}

// Função para liberar memória do array dinâmico de filmes
void freeMovieArray(MovieArray *ma)
{
  size_t i;
  for (i = 0; i < ma->size; i++)
  {
    free(ma->array[i].title);
    Node *current = ma->array[i].neighbors;
    while (current != NULL)
    {
      Node *next = current->next;
      free(current);
      current = next;
    }
  }
  free(ma->array);
}

void readMoviesFile(const char *filename, MovieArray *movies, size_t numMovies)
{
  FILE *file = fopen(filename, "r");

  if (!file)
  {
    fprintf(stderr, "Erro ao abrir o arquivo %s\n", filename);
    exit(EXIT_FAILURE);
  }

  char line[1024];
  fgets(line, sizeof(line), file); // Pula o cabeçalho

  size_t moviesRead = 0;
  while (fgets(line, sizeof(line), file) && moviesRead < numMovies)
  {
    Movie movie;
    char *token = strtok(line, "\t");
    movie.id = atoi(token + 2); // Ignora os dois primeiros caracteres 'tt'

    token = strtok(NULL, "\t");
    if (token == NULL)
    {
      continue; // Caso não haja mais tokens, pula a linha
    }

    token = strtok(NULL, "\t");
    if (token == NULL)
    {
      continue; // Caso não haja mais tokens, pula a linha
    }

    // Verifica se o token começa com aspas, se sim, então ajusta para o início do título
    if (token[0] == '"')
    {
      // Encontra o fim do título que pode estar entre aspas
      char *endQuote = strchr(token + 1, '"');
      if (endQuote != NULL)
      {
        // Copia o título excluindo as aspas
        size_t len = endQuote - token - 1;
        movie.title = (char *)malloc((len + 1) * sizeof(char));
        if (movie.title == NULL)
        {
          fprintf(stderr, "Erro ao alocar memória para o título do filme\n");
          exit(EXIT_FAILURE);
        }
        strncpy(movie.title, token + 1, len);
        movie.title[len] = '\0';
      }
      else
      {
        fprintf(stderr, "Erro ao processar o título do filme: %s\n", token);
        continue; // Se não encontrar o fim da citação, pula a linha
      }
    }
    else
    {
      // Caso contrário, copia o token normalmente
      movie.title = strdup(token);
      if (movie.title == NULL)
      {
        fprintf(stderr, "Erro ao alocar memória para o título do filme\n");
        exit(EXIT_FAILURE);
      }
    }

    movie.neighbors = NULL;

    insertMovieArray(movies, movie);
    // printf("Filme lido: %s\n", movie.title); // Mensagem de depuração

    moviesRead++;
  }

  fclose(file);
}