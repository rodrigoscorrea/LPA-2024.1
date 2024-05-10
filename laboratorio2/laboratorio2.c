/*
Autor: Rodrigo Santos Correa
Matricula: 22251139
*/

#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"

int tabela_hash_tam;

typedef struct pessoa_t{
    char nome[51];
    int idade;
    long long int cpf;
} pessoa_t;

typedef struct lista_pessoas_t {
    pessoa_t* pessoa;
    struct lista_pessoas_t* proxima_pessoa;
} lista_pessoas_t;

typedef struct tabela_hash_t{
    lista_pessoas_t** vetListas;
} tabela_hash_t;

bool lista_eventos_adicionar(pessoa_t *pessoa, lista_pessoas_t **lista) {
    lista_pessoas_t *pessoa_nova = malloc(sizeof(lista_pessoas_t)); // Aloca o novo item
    if (pessoa_nova == NULL) return false; // Falta memória?
    pessoa_nova->pessoa = pessoa; // Seta o novo item
    pessoa_nova->proxima_pessoa = *lista; // O próximo do novo item será a lista
    *lista = pessoa_nova; // Aqui, estamos mudando o ponteiro da lista
    return true ;
}

void lista_pessoas_listar(lista_pessoas_t *lista){
    if(lista == NULL){
        printf("Nao ha elementos na lista");
    }
    while(lista){
        printf("%s\t%lld\t%d\n", lista->pessoa->nome, lista->pessoa->cpf, lista->pessoa->idade);
        lista = lista->proxima_pessoa;
    }
}

tabela_hash_t* tabela_hash_pessoas_criar(){
    tabela_hash_t* tabela = (tabela_hash_t *) malloc(sizeof(tabela_hash_t));
    tabela->vetListas = (lista_pessoas_t **) malloc(sizeof(lista_pessoas_t*) * tabela_hash_tam);

    for(int i = 0; i < tabela_hash_tam; i++){
        tabela->vetListas[i] = NULL;
    }
    return tabela;
}

int tabela_hash_pessoas_funcao(pessoa_t *pessoa) {
    return pessoa->cpf % tabela_hash_tam;
}

bool tabela_hash_pessoas_adicionar(pessoa_t *pessoa, tabela_hash_t *tabela_hash){
    int chaveHash = tabela_hash_pessoas_funcao(pessoa);
    lista_pessoas_t* nova_pessoa = (lista_pessoas_t*) malloc(sizeof(lista_pessoas_t));
    if(!nova_pessoa) return false;
    nova_pessoa->pessoa = pessoa;
    nova_pessoa->proxima_pessoa = tabela_hash->vetListas[chaveHash];
    tabela_hash->vetListas[chaveHash] = nova_pessoa;
    return true;
}

void tabela_hash_pessoas_listar(tabela_hash_t tabela_hash){
    for (int i = 0; i < tabela_hash_tam; i++) {
        lista_pessoas_t *lista_alvo = tabela_hash.vetListas[i];
        printf("POSICAO %d TABELA HASH:\n", i);
        if(lista_alvo){
            lista_pessoas_listar(lista_alvo);
        }
    }
}

pessoa_t* criar_pessoa(char* nome, long long int cpf, int idade){
    pessoa_t* pessoa = malloc(sizeof(pessoa_t));
    strcpy(pessoa->nome, nome);
    pessoa->cpf = cpf;
    pessoa->idade=idade;
    return pessoa;
}

int main(int argc, char **argv){
    tabela_hash_tam = atoi(argv[1]);
    FILE* arquivo_entrada = fopen(argv[2], "r");

    tabela_hash_t *tabela = tabela_hash_pessoas_criar();

    char nome[51]; 
    long long int cpf = 0;
    int idade = 0;
    pessoa_t* pessoa;

    while(fscanf(arquivo_entrada, "%[^\t]\t%lld\t%d", nome, &cpf,&idade) != EOF){
        
        pessoa = criar_pessoa(nome, cpf, idade);
        tabela_hash_pessoas_adicionar(pessoa, tabela);
    }
    fclose(arquivo_entrada);
    tabela_hash_pessoas_listar(*tabela);

    return 0;
};