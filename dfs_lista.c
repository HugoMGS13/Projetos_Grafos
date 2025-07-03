// Aluno: Hugo Martins Gaspar da Silva
// Matrícula: 202311140020

//Foi adicionado ao código as funções: dfs_visit para implementar o dfs e dfs para iniciar o processo

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Estrutura para um nó da lista de adjacência
typedef struct No {
    int vertice;
    int peso;
    struct No* proximo;
} No;

// Estrutura para o grafo
typedef struct {
    int num_vertices;
    bool direcionado;
    No** lista_adj; // Array de ponteiros para No
    bool* visitado; // Array para marcar os vértices visitados
} Grafo;

// Cria um novo nó
No* criar_no(int vertice, int peso) {
    No* novo_no = (No*)malloc(sizeof(No));
    novo_no->vertice = vertice;
    novo_no->peso = peso;
    novo_no->proximo = NULL;
    return novo_no;
}

// Inicializa um grafo
Grafo* criar_grafo(int num_vertices, bool direcionado) {
    Grafo* g = (Grafo*)malloc(sizeof(Grafo));
    g->num_vertices = num_vertices;
    g->direcionado = direcionado;
    g->lista_adj = (No**)malloc(num_vertices * sizeof(No*));
    g->visitado = (bool*)malloc(num_vertices * sizeof(bool));
    
    for (int i = 0; i < num_vertices; i++) {
        g->lista_adj[i] = NULL;
        g->visitado[i] = false;
    }
    
    return g;
}

// Adiciona uma aresta entre v1 e v2 com peso opcional
void adicionar_aresta(Grafo* g, int v1, int v2, int peso) {
    if (v1 >= 0 && v1 < g->num_vertices && v2 >= 0 && v2 < g->num_vertices) {
        // Adiciona v2 na lista de v1
        No* novo_no = criar_no(v2, peso);
        novo_no->proximo = g->lista_adj[v1];
        g->lista_adj[v1] = novo_no;
        
        // Se não for direcionado, adiciona v1 na lista de v2
        if (!g->direcionado) {
            novo_no = criar_no(v1, peso);
            novo_no->proximo = g->lista_adj[v2];
            g->lista_adj[v2] = novo_no;
        }
    }
}

// Implementação do algoritmo DFS
void dfs_visit(Grafo* g, int vertice) {
    g->visitado[vertice] = true;
    printf("%d ", vertice);
    
    No* atual = g->lista_adj[vertice];
    while (atual != NULL) {
        if (!g->visitado[atual->vertice]) {
            dfs_visit(g, atual->vertice);
        }
        atual = atual->proximo;
    }
}

// Função para iniciar o DFS
void dfs(Grafo* g, int vertice_inicial) {
    printf("DFS a partir do vertice %d:\n", vertice_inicial);
    // Inicializa o array de visitados para todos os vértices
    for (int i = 0; i < g->num_vertices; i++) {
        g->visitado[i] = false;
    }
    dfs_visit(g, vertice_inicial);
    printf("\n");
}

// Imprime o grafo
void imprimir_grafo(Grafo* g) {
    printf("Listas de Adjacencia:\n");
    for (int i = 0; i < g->num_vertices; i++) {
        printf("%d: ", i);
        No* atual = g->lista_adj[i];
        while (atual != NULL) {
            printf("(%d)", atual->vertice);
            atual = atual->proximo;
        }
        printf("\n");
    }
}

// Libera a memória alocada para o grafo
void destruir_grafo(Grafo* g) {
    for (int i = 0; i < g->num_vertices; i++) {
        No* atual = g->lista_adj[i];
        while (atual != NULL) {
            No* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
    free(g->lista_adj);
    free(g->visitado);
    free(g);
}

// Exemplo de uso
int main() {
    int num_vertices = 5;
    bool direcionado = false;
    
    Grafo* g = criar_grafo(num_vertices, direcionado);
    
    // Adicionando arestas
    adicionar_aresta(g, 0, 1, 1);
    adicionar_aresta(g, 0, 2, 2);
    adicionar_aresta(g, 1, 3, 3);
    adicionar_aresta(g, 2, 3, 4);
    adicionar_aresta(g, 3, 4, 5);
    
    // Imprimindo o grafo
    imprimir_grafo(g);
    
    // Realizando o DFS
    dfs(g, 0);
    
    // Criando um grafo direcionado
    num_vertices = 4;
    direcionado = true;
    
    Grafo* g_dir = criar_grafo(num_vertices, direcionado);
    
    adicionar_aresta(g_dir, 0, 1, 1);
    adicionar_aresta(g_dir, 1, 2, 1);
    adicionar_aresta(g_dir, 2, 3, 1);
    adicionar_aresta(g_dir, 3, 0, 1);
    
    printf("\nGrafo direcionado:\n");
    imprimir_grafo(g_dir);
    
    // Realizando o DFS no grafo direcionado
    dfs(g_dir, 0);
    
    // Liberando memória
    destruir_grafo(g);
    destruir_grafo(g_dir);
    
    return 0;
}
