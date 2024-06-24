/*
Autor: Rodrigo Santos Correa
Matricula: 22251139
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    FILE* mpeg_file = fopen(argv[1], "rb");
    unsigned char buffer[4];
    int i;

    while (!feof(mpeg_file)){
        fread(buffer,1,3,mpeg_file) == 3;
        if(buffer[0] == 0x00 && buffer[1] == 0x00 && buffer[2] == 0x01){
            unsigned char stream_type = fgetc(mpeg_file);

            if (stream_type == 0xB3) {
                unsigned char byte1 = fgetc(mpeg_file);
                unsigned char byte2 = fgetc(mpeg_file);
                unsigned char byte3 = fgetc(mpeg_file);
                unsigned char byte4 = fgetc(mpeg_file);

                unsigned int width = byte1 * 16 + (byte2 >> 4);
                unsigned int height = (byte2 & 0x0F) * 256 + byte3;
                unsigned int frame_int = byte4 & 0x0F;

                float frame_rate;

                if (frame_int == 1){
                    frame_rate = 23.976;
                }else if(frame_int == 2){
                    frame_rate = 24.000;
                }else if(frame_int == 3){
                    frame_rate = 25.000;
                }else if(frame_int == 4){
                    frame_rate = 29.970;
                }else if(frame_int == 5){
                    frame_rate = 30.000;
                }else if(frame_int == 6){
                    frame_rate = 50.000;
                }else if(frame_int == 7){
                    frame_rate = 59.940;
                }else if (frame_int == 8){
                    frame_rate = 60.000;
                }
                
                printf("--> Código: %.2x -- Sequence Header -- Width = %d, Height = %d -- Frame rate = %.3ffps\n", stream_type,width,height,frame_rate);

            } else if (stream_type == 0xB8) {
                printf("--> Código: %.2x -- Group of Pictures\n",stream_type);
            } else if (stream_type == 0x00) {
                unsigned char byte1 = fgetc(mpeg_file);
                unsigned char byte2 = fgetc(mpeg_file);
                unsigned int picture_int = (byte2 >> 3) & 0x07;
                char *picture_type;
                if(picture_int == 1){
                    picture_type = "I";
                }else if(picture_int == 2){
                    picture_type = "P";
                }else if(picture_int == 3){
                    picture_type = "B";
                }
                
                printf("--> Código: %.2x -- Picture -- Tipo = %s\n",stream_type, picture_type);

            } else if(stream_type == 0x01){
                printf("--> Código: %.2x -- Slice\n",stream_type);
            }else{
                printf("--> Código: %.2x -- Tipo de stream não implementado\n",stream_type);
            }
        }else{
            if(feof(mpeg_file)){
                break;
            }
            fseek(mpeg_file,-2,SEEK_CUR);
        }
    }

    fclose(mpeg_file);
    return 0;

}