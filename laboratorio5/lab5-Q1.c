/*
Aluno: Rodrigo Santos Correa
Matricula: 22251139
*/

#include <stdio.h>
#include <string.h>

int tamanhoMaximo = 3;
char caracteres[] = "abc";

void geradorSenha(char *pwd, int pos, int siz) {
    if (pos < siz) {
        for (int j = 0; j < tamanhoMaximo; j++) {
            char novaSenha[siz + 1];
            strcpy(novaSenha, pwd);
            novaSenha[pos] = caracteres[j]; //implementação do "pwd + ch", presente no pseudoalgoritmo
            novaSenha[pos + 1] = '\0';
            geradorSenha(novaSenha, pos + 1, siz);
        }
    } else {
        printf("%s\n", pwd);
    }
}

int main() {
    
    printf("REPOSTA DA QUESTAO 1\n");
    geradorSenha("", 0, tamanhoMaximo);
    
    return 0;
}
