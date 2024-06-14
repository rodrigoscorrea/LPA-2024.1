#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct lista_vizinhos {
    int vizinho_id;
    struct lista_vizinhos* prox;
} lista_vizinhos;

typedef struct no {
    int id;
    double x;
    double y;
    bool paconte_enviado;
    lista_vizinhos* vizinhos;
} no_t;

typedef no_t* grafo_t;

no_t* criar_no(int id, double x, double y) {
    no_t* no = malloc(sizeof(no_t));
    no->id = id;
    no->x = x;
    no->y = y;
    no->vizinhos = NULL;
    no->paconte_enviado = false;
    return no;
}

bool lista_vizinhos_adicionar(int vizinho, lista_vizinhos** lista) {
    lista_vizinhos* novo = malloc(sizeof(lista_vizinhos));
    if (novo == NULL) return false;
    novo->vizinho_id = vizinho;
    novo->prox = *lista;
    *lista = novo;
    return true;
}

typedef struct evento{
    double tempo;
    int alvo;
    int tipo;
}evento_t;

typedef struct lista_eventos{
    evento_t* evento;
    struct lista_eventos* prox;
} lista_eventos_t;

evento_t* criar_evento(double tempo, int alvo, int tipo){
    evento_t* evento = malloc(sizeof(evento_t));
    evento->tempo = tempo;
    evento->alvo = alvo;
    evento->tipo = tipo;

    return evento;
}

bool lista_eventos_adicionar_ordenado(evento_t* evento, lista_eventos_t** lista) {
    lista_eventos_t* novo = malloc(sizeof(lista_eventos_t));
    if (novo == NULL) return false;

    novo->evento = evento;
    novo->prox = NULL;

    if (*lista == NULL || (novo->evento->tempo) < ((*lista)->evento->tempo)) {
        novo->prox = *lista;
        *lista = novo;
        return true;
    }

    lista_eventos_t* atual = *lista;

    while (atual->prox != NULL && atual->prox->evento->tempo < evento->tempo) {
        atual = atual->prox;
    }

    novo->prox = atual->prox;
    atual->prox = novo;
    return true;
}


void lista_vizinhos_imprimir(lista_vizinhos* lista) {
    lista_vizinhos* aux = lista;
    while (aux != NULL) {
        printf(" %d", aux->vizinho_id);
        aux = aux->prox;
    }
}

grafo_t grafo_criar(int tam) {
    grafo_t grafo = malloc(tam * sizeof(no_t));
    for (int i = 0; i < tam; i++) {
        grafo[i].id = -1; // Marcação de nó não inicializado
        grafo[i].vizinhos = NULL;
    }
    return grafo;
}

void grafo_atualizar_vizinhos(int tam, double raio_comunicacao, grafo_t grafo) {
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            if (i != j && grafo[i].id != -1 && grafo[j].id != -1) {
                double distancia = sqrt(pow(grafo[i].x - grafo[j].x, 2) + pow(grafo[i].y - grafo[j].y, 2));
                if (distancia < raio_comunicacao) {
                    lista_vizinhos_adicionar(j, &grafo[i].vizinhos);
                }
            }
        }
    }
}

void grafo_imprimir(int tam, grafo_t grafo) {
    for (int i = 0; i < tam; i++) {
        printf("NÓ %d:", grafo[i].id);
        lista_vizinhos_imprimir(grafo[i].vizinhos);
        printf("\n");
    }
}

void simulacao_iniciar(lista_eventos_t** lista, grafo_t grafo){
    while (*lista!=NULL){
        evento_t* prim = (*lista)->evento;
        *lista = (*lista)->prox; //aponta a lista para o próximo evento;
        printf("[%3.6f] Nó %d recebeu pacote.\n", prim->tempo,prim->alvo);
        no_t* atual = &grafo[prim->alvo]; //acessa o nó alvo
        if(atual->paconte_enviado == false){
            lista_vizinhos* vizinho_atual = atual->vizinhos;
            while (vizinho_atual){
                int id_vizinho = vizinho_atual->vizinho_id;
                printf("\t--> Repassando pacote para o nó %d ...\n", id_vizinho);
                double novo_tempo = (prim->tempo)+(0.1+(id_vizinho * 0.01));
                evento_t* novo_evento = criar_evento(novo_tempo, id_vizinho, 1);
                lista_eventos_adicionar_ordenado(novo_evento, lista);
                vizinho_atual = vizinho_atual->prox; 
            }
            atual->paconte_enviado = true;
        }
    }
};

int main(int argc, char **argv) {
    char* nome_arq = argv[1];

    FILE* arq = fopen(nome_arq, "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int tam,id;
    double raio_comunicacao,x,y;

    fscanf(arq, "%d %lf",&tam,&raio_comunicacao);
    grafo_t grafo = grafo_criar(tam);

    for(int i = 0; i < tam; i++){
        fscanf(arq, "%d %lf %lf", &id, &x, &y);
        grafo[i] = *criar_no(id,x,y);
    }

    grafo_atualizar_vizinhos(tam,raio_comunicacao,grafo);

    evento_t* prim = criar_evento(1.0,0,1);
    lista_eventos_t* lista = NULL;
    lista_eventos_adicionar_ordenado(prim, &lista);

    simulacao_iniciar(&lista,grafo);

    fclose(arq); 
    return 0;
}