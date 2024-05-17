/*
Autor: Rodrigo Santos Correa
Matricula: 22251139
*/

#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "math.h"

typedef struct lista_vizinhos_t {
    int vizinho;
    struct lista_vizinhos_t* proximo_vizinho;
} lista_vizinhos_t;

typedef struct no{
    int id; 
    double pos_x; 
    double pos_y;
    lista_vizinhos_t* vizinhos;
} no_t;

typedef no_t* grafo_t;

no_t* criar_no_grafo(int id, double pos_x, double pos_y){
    no_t* novo_no = malloc(sizeof(no_t));
    novo_no->id = id;
    novo_no->pos_x = pos_x;
    novo_no->pos_y = pos_y;
    novo_no->vizinhos = NULL;
    return novo_no;
}

bool lista_vizinhos_adicionar(int vizinho, lista_vizinhos_t **lista) {
    lista_vizinhos_t *vizinho_novo = malloc(sizeof(lista_vizinhos_t)); // Aloca o novo item
    if (vizinho_novo == NULL) return false; // Falta memória?
    vizinho_novo->vizinho = vizinho; // Seta o novo item
    vizinho_novo->proximo_vizinho = *lista; // O próximo do novo item será a lista
    *lista = vizinho_novo; // Aqui, estamos mudando o ponteiro da lista
    return true ;
}

void lista_vizinhos_imprimir(lista_vizinhos_t *lista) {
    lista_vizinhos_t* aux = lista;
    if(lista == NULL){
        printf("Nao ha vizinhos");
    }
    while(aux){
        printf("%d ", aux->vizinho);
        aux = aux -> proximo_vizinho;
    }
}

grafo_t grafo_criar(int tam) {
    grafo_t grafo = malloc(sizeof(no_t) * tam);
    for(int i = 0; i < tam; i++){
        grafo[i].id = 0;
        grafo[i].vizinhos = NULL;
    }
    return grafo;
}

void grafo_atualizar_vizinhos(int tam, double raio_comunicacao, grafo_t grafo) {
    for(int i = 0; i < tam; i++){
        for(int j = 0; j < tam; j++){
            if(i != j){
                double distancia = sqrt(pow(grafo[i].pos_x - grafo[j].pos_x, 2) + pow(grafo[i].pos_y - grafo[j].pos_y, 2));
                if(distancia < raio_comunicacao) lista_vizinhos_adicionar(j, &grafo[i].vizinhos);
            }
        }
    }
}

void grafo_imprimir(int tam, grafo_t grafo){
    for(int i = 0; i < tam; i++){
        printf("NO %d: ", grafo[i].id);
        lista_vizinhos_imprimir(grafo[i].vizinhos);
        printf("\n");
    }
}

int main(int argc, char **argv) {
    FILE* arquivo_entrada = fopen(argv[1], "r");
    
    int qtd_nos = 0; //representa o tamanho
    double raio = 0.0;

    fscanf(arquivo_entrada,"%d\t%lf\n", &qtd_nos, &raio);
    grafo_t grafo = grafo_criar(qtd_nos);
    int count = 0;

    int id = 0;
    double pos_x = 0;
    double pos_y = 0;
    for(int i = 0; i < qtd_nos; i++){
        fscanf(arquivo_entrada, "%d %lf %lf", &id, &pos_x, &pos_y);
        grafo[i] = *criar_no_grafo(id,pos_x,pos_y);
    }
    grafo_atualizar_vizinhos(qtd_nos, raio, grafo);
    grafo_imprimir(qtd_nos, grafo);

    free(grafo);
    fclose(arquivo_entrada);
    
    return 0;
}