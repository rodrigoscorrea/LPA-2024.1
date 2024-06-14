/*
Aluno: Rodrigo Santos Correa
Matrícula: 22251139
*/

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

struct png_chunk {
    int length;
    char type[4];
} __attribute__ ((packed));

struct png_chunk_hdr{
    int width;
    int height;
    char bit_depth;
    char colour_type;
    char compression_method;
    char filter_method;
    char interlace_method; 
} __attribute__ ((packed));

typedef struct png_chunk png_chunk;
typedef struct png_chunk_hdr png_chunk_hdr;

int main(int argc, char **argv){
    FILE *png_file = fopen(argv[1], "rb");

    png_chunk* chunk = malloc(sizeof(struct png_chunk));
    png_chunk_hdr* hdr_chunk = malloc(sizeof(struct png_chunk_hdr));

    fseek(png_file,8,SEEK_SET);
    int count = 0;
    while ((fread(chunk, sizeof(struct png_chunk),1,png_file)) == 1){
        count++;
        printf("Lendo chunk %d\n",count);
        printf("\t--> Tamanho: %d\n\t--> Tipo: %.4s\n", ntohl(chunk->length), chunk->type);
        if((strncmp(chunk->type,"IHDR",4) == 0)){
            
            fread(hdr_chunk, sizeof(struct png_chunk_hdr),1,png_file);
            
            printf("\t  --> Largura: %d\n\t  --> Altura: %d\n",ntohl(hdr_chunk->width), ntohl(hdr_chunk->height));
            fseek(png_file,4,SEEK_CUR);
        }
        else if (strncmp(chunk->type, "IEND",4) == 0){
            break;
        }else{
            fseek(png_file,(ntohl(chunk->length)+4),SEEK_CUR);
        }
        
    }
    free(chunk);
    free(hdr_chunk);
    fclose(png_file);

    return 0;
}