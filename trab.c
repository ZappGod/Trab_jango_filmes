#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para armazenar os filmes de destaque do artista
struct Node {
    int movieID;
    struct Node *next;
};

// Estrutura para o Artista
struct Actor {
    int id;
    char *name;
    struct Node *movies;
};

// Estrutura para o Array Dinâmico de Artistas
struct ActorArray {
    struct Actor *array;
    size_t size;
    size_t capacity;
};

// Função para inicializar o array dinâmico de artistas
void initActorArray(struct ActorArray *a, size_t initialCapacity) {
    a->array = (struct Actor *)malloc(initialCapacity * sizeof(struct Actor));
    if (a->array == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o array de artistas\n");
        exit(EXIT_FAILURE);
    }
    a->size = 0;
    a->capacity = initialCapacity;
}

// Função para inserir um artista no array dinâmico
void insertActorArray(struct ActorArray *a, struct Actor actor) {
    if (a->size == a->capacity) {
        a->capacity *= 2;
        a->array = (struct Actor *)realloc(a->array, a->capacity * sizeof(struct Actor));
        if (a->array == NULL) {
            fprintf(stderr, "Erro ao realocar memória para o array de artistas\n");
            exit(EXIT_FAILURE);
        }
    }
    a->array[a->size++] = actor;
}

// Função para liberar memória do array dinâmico de artistas
void freeActorArray(struct ActorArray *a) {
    size_t i;
    for (i = 0; i < a->size; i++) {
        free(a->array[i].name);
        struct Node *current = a->array[i].movies;
        while (current != NULL) {
            struct Node *next = current->next;
            free(current);
            current = next;
        }
    }
    free(a->array);
}

// Estrutura para o Filme
struct Movie {
    int id;
    char *title;
    struct Node *neighbors;
};

// Estrutura para o Array Dinâmico de Filmes (Lista de Adjacências)
struct MovieArray {
    struct Movie *array;
    size_t size;
    size_t capacity;
};

// Função para inicializar o array dinâmico de filmes
void initMovieArray(struct MovieArray *ma, size_t initialCapacity) {
    ma->array = (struct Movie *)malloc(initialCapacity * sizeof(struct Movie));
    if (ma->array == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o array de filmes\n");
        exit(EXIT_FAILURE);
    }
    ma->size = 0;
    ma->capacity = initialCapacity;
}

// Função para inserir um filme no array dinâmico
void insertMovieArray(struct MovieArray *ma, struct Movie movie) {
    if (ma->size == ma->capacity) {
        ma->capacity *= 2;
        ma->array = (struct Movie *)realloc(ma->array, ma->capacity * sizeof(struct Movie));
        if (ma->array == NULL) {
            fprintf(stderr, "Erro ao realocar memória para o array de filmes\n");
            exit(EXIT_FAILURE);
        }
    }
    ma->array[ma->size++] = movie;
}

// Função para liberar memória do array dinâmico de filmes
void freeMovieArray(struct MovieArray *ma) {
    size_t i;
    for (i = 0; i < ma->size; i++) {
        free(ma->array[i].title);
        struct Node *current = ma->array[i].neighbors;
        while (current != NULL) {
            struct Node *next = current->next;
            free(current);
            current = next;
        }
    }
    free(ma->array);
}

// Função para ler o arquivo de artistas
void readActorsFile(const char *filename, struct ActorArray *actors, size_t numActors) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[1024];
    fgets(line, sizeof(line), file); // Pula o cabeçalho

    size_t actorsRead = 0;
    while (fgets(line, sizeof(line), file) && actorsRead < numActors) {
        struct Actor actor;
        char *token = strtok(line, "\t");
        actor.id = atoi(token + 2); // Ignora os dois primeiros caracteres 'nm'

        token = strtok(NULL, "\t");
        actor.name = strdup(token);
        if (actor.name == NULL) {
            fprintf(stderr, "Erro ao alocar memória para o nome do ator\n");
            exit(EXIT_FAILURE);
        }

        token = strtok(NULL, "\t"); // Ignora birthYear
        token = strtok(NULL, "\t"); // Ignora deathYear

        token = strtok(NULL, "\t"); // Pega primaryProfession
        token = strtok(NULL, "\t"); // Pega knownForTitles
        actor.movies = NULL;

        char *movieID = strtok(token, ",");
        while (movieID) {
            struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
            if (newNode == NULL) {
                fprintf(stderr, "Erro ao alocar memória para o nó de filme\n");
                exit(EXIT_FAILURE);
            }
            newNode->movieID = atoi(movieID + 2); // Ignora os dois primeiros caracteres 'tt'
            newNode->next = actor.movies;
            actor.movies = newNode;
            movieID = strtok(NULL, ",");
        }

        insertActorArray(actors, actor);
        printf("Ator lido: %s\n", actor.name); // Mensagem de depuração

        actorsRead++;
    }

    fclose(file);
}

// Função para ler o arquivo de filmes
void readMoviesFile(const char *filename, struct MovieArray *movies, size_t numMovies) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[1024];
    fgets(line, sizeof(line), file); // Pula o cabeçalho

    size_t moviesRead = 0;
    while (fgets(line, sizeof(line), file) && moviesRead < numMovies) {
        struct Movie movie;
        char *token = strtok(line, "\t");
        movie.id = atoi(token + 2); // Ignora os dois primeiros caracteres 'tt'

        token = strtok(NULL, "\t");
        if (token == NULL) {
            continue; // Caso não haja mais tokens, pula a linha
        }

        token = strtok(NULL, "\t");
        if (token == NULL) {
            continue; // Caso não haja mais tokens, pula a linha
        }

        // Verifica se o token começa com aspas, se sim, então ajusta para o início do título
        if (token[0] == '"') {
            // Encontra o fim do título que pode estar entre aspas
            char *endQuote = strchr(token + 1, '"');
            if (endQuote != NULL) {
                // Copia o título excluindo as aspas
                size_t len = endQuote - token - 1;
                movie.title = (char *)malloc((len + 1) * sizeof(char));
                if (movie.title == NULL) {
                    fprintf(stderr, "Erro ao alocar memória para o título do filme\n");
                    exit(EXIT_FAILURE);
                }
                strncpy(movie.title, token + 1, len);
                movie.title[len] = '\0';
            } else {
                fprintf(stderr, "Erro ao processar o título do filme: %s\n", token);
                continue; // Se não encontrar o fim da citação, pula a linha
            }
        } else {
            // Caso contrário, copia o token normalmente
            movie.title = strdup(token);
            if (movie.title == NULL) {
                fprintf(stderr, "Erro ao alocar memória para o título do filme\n");
                exit(EXIT_FAILURE);
            }
        }

        movie.neighbors = NULL;

        insertMovieArray(movies, movie);
        printf("Filme lido: %s\n", movie.title); // Mensagem de depuração

        moviesRead++;
    }

    fclose(file);
}

// Função para criar a clique de filmes de destaque dos artistas
void createClique(struct ActorArray *actors, struct MovieArray *movies) {
    size_t i, j;
    for (i = 0; i < actors->size; i++) {
        struct Node *movie1 = actors->array[i].movies;
        while (movie1 != NULL) {
            struct Node *movie2 = movie1->next;
            while (movie2 != NULL) {
                int movie1Index = -1;
                int movie2Index = -1;
                
                // Encontra os índices dos filmes no array de filmes
                for (j = 0; j < movies->size; j++) {
                    if (movies->array[j].id == movie1->movieID) {
                        movie1Index = j;
                    }
                    if (movies->array[j].id == movie2->movieID) {
                        movie2Index = j;
                    }
                }

                printf("Tentando criar conexão entre filme %d e filme %d\n", movie1->movieID, movie2->movieID); // Depuração

                // Se ambos os filmes foram encontrados no array de filmes
                if (movie1Index != -1 && movie2Index != -1) {
                    printf("Filmes encontrados: %s e %s\n", movies->array[movie1Index].title, movies->array[movie2Index].title); // Depuração

                    // Adiciona movie2 como vizinho de movie1
                    struct Node *newNode1 = (struct Node *)malloc(sizeof(struct Node));
                    if (newNode1 == NULL) {
                        fprintf(stderr, "Erro ao alocar memória para o nó de vizinho\n");
                        exit(EXIT_FAILURE);
                    }
                    newNode1->movieID = movie2Index;
                    newNode1->next = movies->array[movie1Index].neighbors;
                    movies->array[movie1Index].neighbors = newNode1;

                    // Adiciona movie1 como vizinho de movie2
                    struct Node *newNode2 = (struct Node *)malloc(sizeof(struct Node));
                    if (newNode2 == NULL) {
                        fprintf(stderr, "Erro ao alocar memória para o nó de vizinho\n");
                        exit(EXIT_FAILURE);
                    }
                    newNode2->movieID = movie1Index;
                    newNode2->next = movies->array[movie2Index].neighbors;
                    movies->array[movie2Index].neighbors = newNode2;

                    printf("Conexão criada: %s -- %s\n", movies->array[movie1Index].title, movies->array[movie2Index].title); // Mensagem de depuração
                } else {
                    printf("Filmes não encontrados: %d e %d\n", movie1->movieID, movie2->movieID); // Depuração
                }
                movie2 = movie2->next;
            }
            movie1 = movie1->next;
        }
    }
    printf("Cliques criadas.\n"); // Mensagem de depuração
}

// Função para imprimir o grafo no formato DOT
void printGraphDOT(struct MovieArray *movies, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fprintf(file, "graph {\n");

    size_t i;
    for (i = 0; i < movies->size; i++) {
        struct Node *neighbor = movies->array[i].neighbors;
        while (neighbor != NULL) {
            if (i < neighbor->movieID) {
                fprintf(file, "  \"%s\" -- \"%s\";\n", movies->array[i].title, movies->array[neighbor->movieID].title);
            }
            neighbor = neighbor->next;
        }
    }

    fprintf(file, "}\n");
    fclose(file);
    printf("Arquivo DOT criado: %s\n", filename); // Mensagem de depuração
}

int main() {
    struct ActorArray actors;
    initActorArray(&actors, 10);
    readActorsFile("name.basics.tsv", &actors, 10);

    struct MovieArray movies;
    initMovieArray(&movies, 10);
    readMoviesFile("title.basics.tsv", &movies, 10);

    createClique(&actors, &movies);

    printGraphDOT(&movies, "graph.dot");

    freeActorArray(&actors);
    freeMovieArray(&movies);

    return 0;
}