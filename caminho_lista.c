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
    int* anterior;  // Array para armazenar o caminho
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
    g->anterior = (int*)malloc(num_vertices * sizeof(int));
    
    for (int i = 0; i < num_vertices; i++) {
        g->lista_adj[i] = NULL;
        g->visitado[i] = false;
        g->anterior[i] = -1; // Inicializa com -1, indicando que o vértice não foi visitado ainda
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
    
    No* atual = g->lista_adj[vertice];
    while (atual != NULL) {
        if (!g->visitado[atual->vertice]) {
            g->anterior[atual->vertice] = vertice; // Marca o vértice anterior
            dfs_visit(g, atual->vertice);
        }
        atual = atual->proximo;
    }
}

// Função para iniciar o DFS
void dfs(Grafo* g, int vertice_inicial) {
    // Inicializa o array de visitados para todos os vértices
    for (int i = 0; i < g->num_vertices; i++) {
        g->visitado[i] = false;
        g->anterior[i] = -1;
    }
    dfs_visit(g, vertice_inicial);
}

// Função para reconstruir o caminho de um vértice até o vértice inicial
void reconstruir_caminho(Grafo* g, int vertice_inicial, int vertice_destino) {
    if (vertice_inicial == vertice_destino) {
        printf("%d ", vertice_inicial);
        return;
    }
    
    if (g->anterior[vertice_destino] == -1) {
        printf("Caminho nao encontrado!\n");
        return;
    }
    
    reconstruir_caminho(g, vertice_inicial, g->anterior[vertice_destino]);
    printf("%d ", vertice_destino);
}

// Função para encontrar o tamanho do caminho entre dois vértices
int tamanho_caminho(Grafo* g, int vertice_inicial, int vertice_destino) {
    reconstruir_caminho(g, vertice_inicial, vertice_destino);
    printf("\n");
    
    int tamanho = 0;
    int v = vertice_destino;
    while (v != vertice_inicial) {
        v = g->anterior[v];
        tamanho++;
    }
    
    return tamanho;
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
    free(g->anterior);
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
    
    // Solicitando ao usuário os vértices inicial e de destino
    int vertice_inicial, vertice_destino;
    printf("\nInforme o vertice inicial: ");
    scanf("%d", &vertice_inicial);
    
    printf("Informe o vertice de destino: ");
    scanf("%d", &vertice_destino);
    
    // Verificando se os vértices são válidos
    if (vertice_inicial < 0 || vertice_inicial >= g->num_vertices || vertice_destino < 0 || vertice_destino >= g->num_vertices) {
        printf("Vertices invalidos!\n");
        destruir_grafo(g);
        return 1;
    }
    
    // Realizando o DFS
    dfs(g, vertice_inicial);
    
    // Encontrando o caminho e seu tamanho entre os vértices informados
    printf("\nCaminho entre %d e %d: ", vertice_inicial, vertice_destino);
    int tamanho = tamanho_caminho(g, vertice_inicial, vertice_destino);
    printf("Tamanho do caminho: %d\n", tamanho);
    
    // Liberando memória
    destruir_grafo(g);
    
    return 0;
}
