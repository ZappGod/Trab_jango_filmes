#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./lib/node/node.h"
#include "./lib/actor/actor.h"
#include "./lib/movie/movie.h"
#include "./lib/avl/avl.h"

// Função para criar a clique de filmes de destaque dos artistas
void createClick_arr(ActorArray *actors, MovieArray *movies)
{
    size_t i, j;

    for (i = 0; i < actors->size; i++)
    {
        Node *movie1 = actors->array[i].movies;

        while (movie1 != NULL)
        {
            Node *movie2 = movie1->next;
            while (movie2 != NULL)
            {
                int movie1Index = -1;
                int movie2Index = -1;

                // Encontra os índices dos filmes no array de filmes
                for (j = 0; j < movies->size; j++)
                {
                    if (movies->array[j].id == movie1->movieID)
                    {
                        movie1Index = j;
                    }
                    if (movies->array[j].id == movie2->movieID)
                    {
                        movie2Index = j;
                    }
                }

                // printf("Tentando criar conexão entre filme %d e filme %d\n", movie1->movieID, movie2->movieID); // Depuração

                // Se ambos os filmes foram encontrados no array de filmes
                if (movie1Index != -1 && movie2Index != -1)
                {
                    // printf("Filmes encontrados: %s e %s\n", movies->array[movie1Index].title, movies->array[movie2Index].title); // Depuração

                    // Adiciona movie2 como vizinho de movie1
                    Node *newNode1 = createNode();

                    newNode1->movieID = movie2Index;
                    newNode1->next = movies->array[movie1Index].neighbors;
                    movies->array[movie1Index].neighbors = newNode1;

                    // Adiciona movie1 como vizinho de movie2
                    Node *newNode2 = createNode();
                    newNode2->movieID = movie1Index;
                    newNode2->next = movies->array[movie2Index].neighbors;
                    movies->array[movie2Index].neighbors = newNode2;

                    // printf("Conexão criada: %s -- %s\n", movies->array[movie1Index].title, movies->array[movie2Index].title); // Mensagem de depuração
                }
                else
                {
                    // printf("Filmes não encontrados: %d e %d\n", movie1->movieID, movie2->movieID); // Depuração
                }

                movie2 = movie2->next;
            }
            movie1 = movie1->next;
        }
    }
    // printf("Cliques criadas.\n"); // Mensagem de depuração
}

bool verifica();

void createClick_avl(ActorArray *actors, MovieArray *movies, Avl_node *avl)
{
    size_t i, j;

    for (i = 0; i < actors->size; i++)
    {
        Node *movie1 = actors->array[i].movies;

        while (movie1 != NULL)
        {
            Node *movie2 = movie1->next;
            while (movie2 != NULL)
            {
                int movie1Index = -1;
                int movie2Index = -1;

                Avl_node *movie1_avl = search(&avl, movie1->movieID);

                if (movie1_avl)
                {
                    movie1Index = movie1_avl->index;
                }

                Avl_node *movie2_avl = search(&avl, movie2->movieID);

                if (movie2_avl)
                {
                    movie2Index = movie2_avl->index;
                }

                // printf("Tentando criar conexão entre filme %d e filme %d\n", movie1->movieID, movie2->movieID); // Depuração

                // Se ambos os filmes foram encontrados no array de filmes
                if (movie1Index != -1 && movie2Index != -1)
                {
                    // printf("Filmes encontrados: %s e %s\n", movies->array[movie1Index].title, movies->array[movie2Index].title); // Depuração

                    // Adiciona movie2 como vizinho de movie1
                    Node *newNode1 = createNode();

                    newNode1->movieID = movie2Index;
                    newNode1->next = movies->array[movie1Index].neighbors;
                    movies->array[movie1Index].neighbors = newNode1;

                    // Adiciona movie1 como vizinho de movie2
                    Node *newNode2 = createNode();
                    newNode2->movieID = movie1Index;
                    newNode2->next = movies->array[movie2Index].neighbors;
                    movies->array[movie2Index].neighbors = newNode2;

                    // printf("Conexão criada: %s -- %s\n", movies->array[movie1Index].title, movies->array[movie2Index].title); // Mensagem de depuração
                }
                else
                {
                    // printf("Filmes não encontrados: %d e %d\n", movie1->movieID, movie2->movieID); // Depuração
                }

                movie2 = movie2->next;
            }
            movie1 = movie1->next;
        }
    }
    // printf("Cliques criadas.\n"); // Mensagem de depuração
}

// Função para imprimir o grafo no formato DOT
void printGraphDOT(MovieArray *movies, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fprintf(file, "graph { concentrate=true\n");

    size_t i;
    for (i = 0; i < movies->size; i++)
    {
        Node *neighbor = movies->array[i].neighbors;
        while (neighbor != NULL)
        {
            if (i < neighbor->movieID)
            {
                fprintf(file, "  \"%s\" -- \"%s\";\n", movies->array[i].title, movies->array[neighbor->movieID].title);
            }
            neighbor = neighbor->next;
        }
    }

    fprintf(file, "}\n");
    fclose(file);
    // printf("Arquivo DOT criado: %s\n", filename); // Mensagem de depuração
}

int main()
{
    ActorArray actors;
    initActorArray(&actors, 10);
    readActorsFile("./assets/name.basics.tsv", &actors, 200000);

    MovieArray movies;
    initMovieArray(&movies, 10);
    readMoviesFile("./assets/title.basics.tsv", &movies, 200000);

    Avl_node *avl_head = NULL;

    size_t i;
    for (i = 0; i < (&movies)->size; i++)
    {
        insert(&avl_head, (&movies)->array[i].id, i);
    }

    // visit(avl_head);

    // createClick_arr(&actors, &movies);
    createClick_avl(&actors, &movies, avl_head);

    printGraphDOT(&movies, "./data/input.dot");

    freeActorArray(&actors);
    freeMovieArray(&movies);

    return 0;
}