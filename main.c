#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>

#include "./lib/node/node.h"
#include "./lib/actor/actor.h"
#include "./lib/movie/movie.h"
#include "./lib/avl/avl.h"

#define ROWS_LIMIT 250000

void createClick_arr(ActorArray *actors, MovieArray *movies);
bool verify_edge(Movie *movie1, int movie2index);
void createClick_avl(ActorArray *actors, MovieArray *movies, Avl_node *avl);
void printGraphDOT(MovieArray *movies, const char *filename);
void fill_avl_tree(Avl_node **avl_head, MovieArray *movies);

void searchById(Avl_node *avl_head, MovieArray *movies, int id);
void searchByActor(ActorArray *actors, MovieArray *movies, int id);

int main()
{
    ActorArray actors;
    initActorArray(&actors, 10);
    readActorsFile("./assets/name.basics.tsv", &actors, ROWS_LIMIT);

    MovieArray movies;
    initMovieArray(&movies, 10);
    readMoviesFile("./assets/title.basics.tsv", &movies, ROWS_LIMIT);

    Avl_node *avl_head = NULL;

    fill_avl_tree(&avl_head, &movies);

    // createClick_arr(&actors, &movies);
    createClick_avl(&actors, &movies, avl_head);

    printGraphDOT(&movies, "./data/input.dot");

    int menu;
    int id_search;
    int id_actor_search;

    printf("\n");

    while (menu != 0)
    {
        printf("1. Buscar filme por ID\n");
        printf("2. Buscar filme por ator\n");
        printf("0. Sair\n");
        printf("-> ");
        scanf("%d", &menu);
        printf("\n");

        switch (menu)
        {
        case 1:
            printf("Digite o id do filme: ");
            scanf("%d", &id_search);
            printf("\n");
            searchById(avl_head, &movies, id_search);
            break;
        case 2:
            printf("Digite o ID do ator: ");
            scanf("%d", &id_actor_search);
            searchByActor(&actors, &movies, id_actor_search);
            break;

        default:
            break;
        }
    }

    freeActorArray(&actors);
    freeMovieArray(&movies);

    return 0;
}

void fill_avl_tree(Avl_node **avl_head, MovieArray *movies)
{
    size_t i;
    for (i = 0; i < movies->size; i++)
    {
        insert(avl_head, movies->array[i].id, i);
    }
}

bool verify_edge(Movie *movie1, int movie2index)
{
    Node *neighbor = movie1->neighbors;
    while (neighbor != NULL)
    {
        if (neighbor->movieID == movie2index)
        {
            return true;
        }
        neighbor = neighbor->next;
    }

    return false;
};

// Função para criar a clique de filmes de destaque dos artistas (usando AVL)
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

                if (movie1_avl != NULL)
                {
                    movie1Index = movie1_avl->index;
                }

                Avl_node *movie2_avl = search(&avl, movie2->movieID);

                if (movie2_avl != NULL)
                {
                    movie2Index = movie2_avl->index;
                }

                // printf("Tentando criar conexão entre filme %d e filme %d\n", movie1->movieID, movie2->movieID); // Depuração

                // Se ambos os filmes foram encontrados no array de filmes
                if (movie1Index != -1 && movie2Index != -1)
                {
                    // printf("Filmes encontrados: %s e %s\n", movies->array[movie1Index].title, movies->array[movie2Index].title); // Depuração

                    bool edge_exists = verify_edge(&movies->array[movie1Index], movie2Index);

                    if (!edge_exists)
                    {
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
    printf("Cliques criadas.\n"); // Mensagem de depuração
}

// Função para criar arquivo de grafo
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
    printf("Arquivo DOT criado: %s\n", filename); // Mensagem de depuração
}

void searchById(Avl_node *avl_head, MovieArray *movies, int id)
{
    int movieIndex = -1;

    Avl_node *movie_avl = search(&avl_head, id);

    if (movie_avl == NULL)
    {
        printf("Filme não encontrado!\n\n");
        return;
    }

    movieIndex = movie_avl->index;
    Movie movie = movies->array[movieIndex];

    if (!movie.id)
    {
        printf("Ocorreu algum erro!\n");
        return;
    }

    printf("Título: %s\n", movie.title);

    Node *neighbor = movie.neighbors;
    while (neighbor != NULL)
    {
        printf("Filme associado: %s\n", movies->array[neighbor->movieID].title);
        neighbor = neighbor->next;
    }
    printf("\n\n");
}

void searchByActor(ActorArray *actors, MovieArray *movies, int id)
{
    size_t i;
    for (i = 0; i < actors->size; i++)
    {
        if (actors->array[i].id == id)
        {
            printf("Nome do ator: %s\n", actors->array[i].name);
            Node *current_movie = actors->array[i].movies;

            if (current_movie->movieID >= ROWS_LIMIT)
            {
                printf("Nenhum filme encontrado\n\n");
                return;
            }

            while (current_movie != NULL)
            {
                printf("Filmes: %s\n", movies->array[current_movie->movieID].title);
                current_movie = current_movie->next;
            }
            printf("\n");
            return;
        }
    }
}

// Função para criar a clique de filmes de destaque dos artistas (usando Array)
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
