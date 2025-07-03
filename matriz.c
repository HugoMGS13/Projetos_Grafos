//Aluno: Hugo Martins Gaspar da Silva
//Matrícula: 202311140020

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICES 100

typedef struct {
    int num_vertices;
    bool direcionado;
    int matriz_adj[MAX_VERTICES][MAX_VERTICES];
} Grafo;

// Inicializa um grafo
void inicializar_grafo(Grafo *g, int num_vertices, bool direcionado) {
    g->num_vertices = num_vertices;
    g->direcionado = direcionado;
    
    // Inicializa toda a matriz com 0 (sem arestas)
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            g->matriz_adj[i][j] = 0;
        }
    }
}

// Adiciona uma aresta entre v1 e v2 com peso opcional
void adicionar_aresta(Grafo *g, int v1, int v2, int peso) {
    if (v1 >= 0 && v1 < g->num_vertices && v2 >= 0 && v2 < g->num_vertices) {
        g->matriz_adj[v1][v2] = peso;
        if (!g->direcionado) {
            g->matriz_adj[v2][v1] = peso;
        }
    }
}

// Remove a aresta entre v1 e v2
void remover_aresta(Grafo *g, int v1, int v2) {
    if (v1 >= 0 && v1 < g->num_vertices && v2 >= 0 && v2 < g->num_vertices) {
        g->matriz_adj[v1][v2] = 0;
        if (!g->direcionado) {
            g->matriz_adj[v2][v1] = 0;
        }
    }
}

// Verifica se existe aresta entre v1 e v2
bool existe_aresta(Grafo *g, int v1, int v2) {
    if (v1 >= 0 && v1 < g->num_vertices && v2 >= 0 && v2 < g->num_vertices) {
        return g->matriz_adj[v1][v2] != 0;
    }
    return false;
}

// Retorna o peso da aresta entre v1 e v2
int peso_aresta(Grafo *g, int v1, int v2) {
    if (v1 >= 0 && v1 < g->num_vertices && v2 >= 0 && v2 < g->num_vertices) {
        return g->matriz_adj[v1][v2];
    }
    return -1;
}

// Imprime a matriz de adjacência
void imprimir_grafo(Grafo *g) {
    printf("Matriz de Adjacencia:\n");
    printf("   ");
    for (int i = 0; i < g->num_vertices; i++) {
        printf("%2d ", i);
    }
    printf("\n");
    
    for (int i = 0; i < g->num_vertices; i++) {
        printf("%2d ", i);
        for (int j = 0; j < g->num_vertices; j++) {
            printf("%2d ", g->matriz_adj[i][j]);
        }
        printf("\n");
    }
}

// Exemplo de uso
int main() {
    Grafo g;
    int num_vertices = 5;
    bool direcionado = false;
    
    inicializar_grafo(&g, num_vertices, direcionado);
    
    // Adicionando arestas
    adicionar_aresta(&g, 0, 1, 1);
    adicionar_aresta(&g, 0, 2, 1);
    adicionar_aresta(&g, 1, 3, 1);
    adicionar_aresta(&g, 2, 3, 1);
    adicionar_aresta(&g, 3, 4, 1);
    
    // Imprimindo o grafo
    imprimir_grafo(&g);
    
    // Testando algumas operações
    printf("\nAresta entre 0 e 1 existe? %s\n", existe_aresta(&g, 0, 1) ? "Sim" : "Nao");
    printf("Peso da aresta entre 0 e 1: %d\n", peso_aresta(&g, 0, 1));
    
    printf("\nRemovendo aresta entre 0 e 1...\n");
    remover_aresta(&g, 0, 1);
    
    printf("Aresta entre 0 e 1 existe agora? %s\n", existe_aresta(&g, 0, 1) ? "Sim" : "Nao");
    
    // Imprimindo o grafo após remoção
    printf("\nGrafo apos remocao:\n");
    imprimir_grafo(&g);
    
    // Criando um grafo direcionado
    Grafo g_dir;
    num_vertices = 4;
    direcionado = true;
    
    inicializar_grafo(&g_dir, num_vertices, direcionado);
    
    adicionar_aresta(&g_dir, 0, 1, 1);
    adicionar_aresta(&g_dir, 1, 2, 1);
    adicionar_aresta(&g_dir, 2, 3, 1);
    adicionar_aresta(&g_dir, 3, 0, 1);
    
    printf("\nGrafo direcionado:\n");
    imprimir_grafo(&g_dir);
    
    return 0;
}