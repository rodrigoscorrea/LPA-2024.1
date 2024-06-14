/*
Aluno: Rodrigo Santos Correa
Matrícula: 22251139
*/

/*

RESPOSTAS QUESTÕES 
1 - 0451
2 - cpp
3 - 31337
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>
/**
* @brief Verifica se uma senha em texto é a mesma de uma senha criptografada.
*
* @param senhaTeste Senha em texto que queremos ver se corresponde à senha criptografada.
* @param senhaCriptografada Senha criptografada como no arquivo /etc/shadow.
* @return int Retorna 0 (zero) se as senhas forem iguais. Outro valor, caso contrário.
*/

int tamanhoMaximo = 5;
char caracteres[] = "0123456789";
char *senhaCriptografada =  "$6$l2xE4w9twgjtnZBz$9YK9krslZFraLffy5VNiahAfT.xZNvB54"
 "j91DMCMIoVFvj335ZKxb11qgVMn.KzU2GqVPPyS2FTBqPSciYq761";

int verificaSenha(char* senhaTeste, char* senhaCriptografada) {
    char *senhaTesteCriptografada = crypt(senhaTeste, senhaCriptografada);
    return strcmp(senhaTesteCriptografada, senhaCriptografada);
}

void geradorSenha(char *pwd, int pos, int siz) {
    int tamanhoChar = strlen(caracteres);
    if (pos < siz) {
        for (int j = 0; j < tamanhoChar; j++) {
            char novaSenha[siz + 1];
            strcpy(novaSenha, pwd);
            novaSenha[pos] = caracteres[j]; //implementação do "pwd + ch", presente no pseudoalgoritmo
            novaSenha[pos + 1] = '\0';
            geradorSenha(novaSenha, pos + 1, siz);
        }
    } else {
        if(!verificaSenha(pwd, senhaCriptografada)) {
            printf("%s --> sim! Senha Encontrada!\n", pwd);
            exit(0);
        }
        else printf("%s --> nao\n", pwd);
    }
}

void main() {
    int tamanhoChar = strlen(caracteres);
    geradorSenha("", 0, tamanhoMaximo);
}