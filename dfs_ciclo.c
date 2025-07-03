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
    int* anterior;  // Array para armazenar o caminho (pai no DFS tree)
    bool* na_pilha; // Array para marcar vértices na pilha de recursão (em processo)
    int* ciclo;     // Array para armazenar o ciclo
} Grafo;

// Cria um novo nó
No* criar_no(int vertice, int peso) {
    No* novo_no = (No*)malloc(sizeof(No));
    if (novo_no == NULL) {
        perror("Erro ao alocar memória para o nó");
        exit(EXIT_FAILURE);
    }
    novo_no->vertice = vertice;
    novo_no->peso = peso;
    novo_no->proximo = NULL;
    return novo_no;
}

// Inicializa um grafo
Grafo* criar_grafo(int num_vertices, bool direcionado) {
    Grafo* g = (Grafo*)malloc(sizeof(Grafo));
    if (g == NULL) {
        perror("Erro ao alocar memória para o grafo");
        exit(EXIT_FAILURE);
    }
    g->num_vertices = num_vertices;
    g->direcionado = direcionado;

    // Alocação de memória para os arrays do grafo
    g->lista_adj = (No**)malloc(num_vertices * sizeof(No*));
    g->visitado = (bool*)malloc(num_vertices * sizeof(bool));
    g->anterior = (int*)malloc(num_vertices * sizeof(int));
    g->na_pilha = (bool*)malloc(num_vertices * sizeof(bool));
    g->ciclo = (int*)malloc((num_vertices + 1) * sizeof(int)); // +1 para o caso de ciclo completo
    
    // Verifica alocação
    if (g->lista_adj == NULL || g->visitado == NULL || g->anterior == NULL || 
        g->na_pilha == NULL || g->ciclo == NULL) {
        perror("Erro ao alocar memória para os componentes do grafo");
        // Libera o que foi alocado antes de sair
        if (g->lista_adj) free(g->lista_adj);
        if (g->visitado) free(g->visitado);
        if (g->anterior) free(g->anterior);
        if (g->na_pilha) free(g->na_pilha);
        if (g->ciclo) free(g->ciclo);
        free(g);
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < num_vertices; i++) {
        g->lista_adj[i] = NULL;
        g->visitado[i] = false;
        g->na_pilha[i] = false;
        g->anterior[i] = -1; // Inicializa com -1
    }
    
    return g;
}

// Adiciona uma aresta entre v1 e v2 com peso opcional
void adicionar_aresta(Grafo* g, int v1, int v2, int peso) {
    if (v1 < 0 || v1 >= g->num_vertices || v2 < 0 || v2 >= g->num_vertices) {
        fprintf(stderr, "Erro: Vértices fora dos limites do grafo.\n");
        return;
    }
    
    // Adiciona v2 na lista de v1
    No* novo_no1 = criar_no(v2, peso);
    novo_no1->proximo = g->lista_adj[v1];
    g->lista_adj[v1] = novo_no1;
    
    // Se não for direcionado, adiciona v1 na lista de v2
    if (!g->direcionado) {
        No* novo_no2 = criar_no(v1, peso); // Crie um novo nó para evitar sobrescrever novo_no1
        novo_no2->proximo = g->lista_adj[v2];
        g->lista_adj[v2] = novo_no2;
    }
}

// Função auxiliar para reconstruir e imprimir o ciclo
void reconstruir_e_imprimir_ciclo(Grafo* g, int vertice_atual, int vertice_ciclo_start) {
    int ciclo_pos = 0;
    int v = vertice_atual;
    
    // Caminha para trás do vertice_atual até o vertice_ciclo_start
    while (v != vertice_ciclo_start) {
        g->ciclo[ciclo_pos++] = v;
        v = g->anterior[v];
    }
    g->ciclo[ciclo_pos++] = vertice_ciclo_start; // Adiciona o vértice de início do ciclo

    printf("Ciclo encontrado: ");
    // Imprime o ciclo na ordem correta (do início do ciclo até o vértice atual, e de volta ao início)
    // Inverte a ordem do caminho reconstruído
    for (int i = ciclo_pos - 1; i >= 0; i--) {
        printf("%d ", g->ciclo[i]);
    }
    printf("%d\n", vertice_ciclo_start); // Adiciona o vértice de início novamente para fechar o ciclo
}

// Função para encontrar o ciclo (DFS modificada para grafos direcionados)
bool dfs_visit(Grafo* g, int vertice, int pai) {
    g->visitado[vertice] = true;
    g->na_pilha[vertice] = true;
    g->anterior[vertice] = pai; // Armazena o pai do vértice atual

    No* atual = g->lista_adj[vertice];
    while (atual != NULL) {
        if (!g->visitado[atual->vertice]) {
            if (dfs_visit(g, atual->vertice, vertice)) {
                return true; // Ciclo encontrado em uma chamada recursiva
            }
        } else if (g->na_pilha[atual->vertice]) { 
            // Encontrou um vértice visitado que está na pilha de recursão. 
            // Isso indica um ciclo em um grafo direcionado.
            reconstruir_e_imprimir_ciclo(g, vertice, atual->vertice);
            return true;
        }
        atual = atual->proximo;
    }

    g->na_pilha[vertice] = false; // Remove o vértice da pilha
    return false;
}

// Função para iniciar a DFS e encontrar o ciclo
void dfs(Grafo* g) {
    // Reinicializa os arrays de estado do grafo
    for (int i = 0; i < g->num_vertices; i++) {
        g->visitado[i] = false;
        g->na_pilha[i] = false;
        g->anterior[i] = -1;
    }
    
    // Tenta encontrar um ciclo a partir de cada vértice não visitado
    for (int i = 0; i < g->num_vertices; i++) {
        if (!g->visitado[i]) {
            // O -1 indica que este é o nó raiz da DFS, sem pai.
            if (dfs_visit(g, i, -1)) { 
                return; // Se um ciclo for encontrado, retorna imediatamente
            }
        }
    }

    printf("Nenhum ciclo encontrado!\n");
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
    if (g == NULL) return; // Evita tentar liberar NULL

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
    free(g->na_pilha);
    free(g->anterior);
    free(g->ciclo);
    free(g);
}

// Exemplo de uso
int main() {
    int num_vertices = 5;
    bool direcionado = true; // Testando com grafo direcionado
    
    Grafo* g = criar_grafo(num_vertices, direcionado);
    
    // Adicionando arestas
    adicionar_aresta(g, 0, 1, 1);
    adicionar_aresta(g, 1, 2, 1);
    adicionar_aresta(g, 2, 3, 1);
    adicionar_aresta(g, 3, 1, 1); // Cria um ciclo entre 1, 2, 3
    adicionar_aresta(g, 4, 0, 1); // Aresta adicional
    
    // Imprimindo o grafo
    imprimir_grafo(g);
    
    // Realizando a busca em profundidade e procurando por um ciclo
    dfs(g);

    // Liberando memória do grafo
    destruir_grafo(g);
    
    return 0;
}