//Author: Rodrigo Santos Correa
//Matricula: 22251139

#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"

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

bool lista_eventos_adicionar_inicio(evento_t *evento, lista_eventos_t **lista) {
    lista_eventos_t *item_novo = malloc(sizeof(lista_eventos_t)); // Aloca o novo item
    if (item_novo == NULL) return false; // Falta memória?
    item_novo->evento = evento; // Seta o novo item
    item_novo->proximo_evento = *lista; // O próximo do novo item será a lista
    *lista = item_novo; // Aqui, estamos mudando o ponteiro da lista
    return true ;
}

bool lista_eventos_adicionar_fim(evento_t *evento, lista_eventos_t **lista){
    lista_eventos_t *item_novo = malloc(sizeof(lista_eventos_t));
    if(item_novo == NULL) return false;
    item_novo -> evento = evento;
    item_novo -> proximo_evento = NULL;

    if(*lista == NULL) {
        *lista = item_novo;
        return true;
    } 
    lista_eventos_t *aux = *lista;
    while(aux -> proximo_evento) {
        aux = aux->proximo_evento;
    }
    aux->proximo_evento = item_novo;
    return true;
}

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


void lista_eventos_listar(lista_eventos_t *lista){
    if(lista == NULL){
        printf("Nao ha elementos na lista");
    }
    while(lista){
        printf("%lf\t%d\t%d\n", lista->evento->tempo, lista->evento->alvo, lista->evento->tipo);
        lista = lista ->proximo_evento;
    }
}

evento_t* criar_evento(double tempo, int alvo, int tipo){
    evento_t* evento = malloc(sizeof(evento_t));
    evento->tempo = tempo;
    evento->alvo = alvo;
    evento->tipo = tipo;
    return evento;
}

int main(int argc, char** argv){

    FILE* arquivo_entrada = fopen(argv[1], "r");

    double tempo = 0.0;
    int alvo = 0;
    int tipo = 0;
    bool insercao;

    evento_t* evento;
    lista_eventos_t* lista;

    //QUESTAO 1 

    while(feof(arquivo_entrada) == false){
        fscanf(arquivo_entrada, "%lf\t%d\t%d\n", &tempo, &alvo, &tipo);
        evento = criar_evento(tempo, alvo, tipo);
        insercao = lista_eventos_adicionar_inicio(evento, &lista);
    }
    
    lista_eventos_listar(lista);
    fclose(arquivo_entrada);


    return 0;
}