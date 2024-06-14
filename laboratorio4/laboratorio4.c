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
    bool pacote_enviado;
    lista_vizinhos_t* vizinhos;
} no_t;

typedef no_t* grafo_t;

no_t* criar_no_grafo(int id, double pos_x, double pos_y){
    no_t* novo_no = malloc(sizeof(no_t));
    novo_no->id = id;
    novo_no->pos_x = pos_x;
    novo_no->pos_y = pos_y;
    novo_no->pacote_enviado=false;
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

//SEÇÃO DE EVENTOS

struct evento_t{
    double tempo;
    int alvo;
    int tipo;
};
typedef struct evento_t evento_t;

struct lista_eventos_t {
    evento_t* evento;
    struct lista_eventos_t* proximo_evento;
};
typedef struct lista_eventos_t lista_eventos_t;

bool lista_eventos_adicionar_ordenado(evento_t *evento, lista_eventos_t **lista){
    lista_eventos_t *item_novo = malloc(sizeof(lista_eventos_t));
    if (item_novo == NULL) return false; // Falta memória?
    item_novo->evento = evento; // Seta o novo item
    item_novo->proximo_evento = NULL;

    if(*lista == NULL) {
        *lista = item_novo;
        return true;
    } 
    lista_eventos_t *item_atual = *lista;

    if(item_novo->evento->tempo < item_atual->evento->tempo){
        item_novo -> proximo_evento = item_atual;
        item_atual = item_novo;
    } else {
        while(item_atual -> proximo_evento != NULL &&  item_atual->proximo_evento->evento->tempo < evento->tempo){
            item_atual = item_atual->proximo_evento;
        }
        item_atual -> proximo_evento = item_novo;
    }

    return true;
}

evento_t* criar_evento(double tempo, int alvo, int tipo){
    evento_t* evento = malloc(sizeof(evento_t));
    evento->tempo = tempo;
    evento->alvo = alvo;
    evento->tipo = tipo;
    return evento;
}

void simulacao_iniciar(lista_eventos_t **lista, grafo_t grafo) {
    while(*lista){
        lista_eventos_t *lista_evento = *lista;
        *lista = (*lista)->proximo_evento;

        evento_t* primeiro = lista_evento->evento;
        printf("[%3.6f] Nó %d recebeu pacote.\n", primeiro->tempo,primeiro->alvo);
        no_t* no_atual = &grafo[primeiro->alvo];
        if(no_atual->pacote_enviado == false){
            lista_vizinhos_t* vizinho = no_atual->vizinhos;
            while(vizinho != NULL){
                int id_vizinho = vizinho->vizinho;
                printf("\t--> Repassando pacote para o nó %d ...\n", id_vizinho);
                evento_t* evento_recebimento = criar_evento((primeiro->tempo) +(0.1+(id_vizinho * 0.01)), id_vizinho, 1);
                lista_eventos_adicionar_ordenado(evento_recebimento, lista); 
                vizinho = vizinho->proximo_vizinho; 
            }
            no_atual->pacote_enviado = true;
        }      
    }
} 

int main(int argc, char** argv){
    FILE* arquivo_entrada = fopen("arquivo_teste.in", "r");

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

    evento_t* evento_inicial = criar_evento(1.0, 0,1);
    lista_eventos_t* lista;
    lista_eventos_adicionar_ordenado(evento_inicial, &lista);
    simulacao_iniciar(&lista, grafo);
    
    free(grafo);
    fclose(arquivo_entrada);
    return 0;
}