/*
Aluno: Rodrigo Santos Correa
Matricula: 22251139
*/

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"

int main(int argc, char *argv[]) {

    FILE *mpg_file = fopen(argv[1], "rb");
    if (mpg_file == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    int i = 1;
    char nome_arquivo[20];

    sprintf(nome_arquivo, "video_parte_%d.mpg", i);
    printf("Criando arquivo video_parte_%d.mpg .. \n", i);
    FILE *mpg_file_parte = fopen(nome_arquivo, "wb");

    unsigned int tamanho_parte = atoi(argv[2]) * 1024 * 1024;
    unsigned int tamanho_atual = 0;
    unsigned char buffer[4];

    while (fread(buffer, sizeof(unsigned char), 4, mpg_file) == 4) {
        
        if ( (memcmp(buffer, "\x00\x00\x01\xB3", 4) != 0 ) && (!feof(mpg_file)) ) {
            fwrite(buffer, sizeof(unsigned char), 4, mpg_file_parte);
            tamanho_atual += 4;
        }
        
        if ( (tamanho_atual + 4) > tamanho_parte) {
            fclose(mpg_file_parte);
            
            sprintf(nome_arquivo, "video_parte_%d.mpg", ++i);
            printf("Criando arquivo video_parte_%d.mpg .. \n", i);
            mpg_file_parte = fopen(nome_arquivo, "wb");

            if (mpg_file_parte == NULL) {
                printf("Erro ao criar o arquivo de saída.\n");
                fclose(mpg_file);
                return 1;
            }
            
            fwrite(buffer, sizeof(unsigned char), 4, mpg_file_parte);
            tamanho_atual = 4;
        }

        if (feof(mpg_file)) {
            fclose(mpg_file_parte);
            break;
        }
    }
    fclose(mpg_file);

    return 0;
}