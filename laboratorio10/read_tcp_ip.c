/*
Aluno: Rodrigo Santos Correa
Matrícula: 22251139
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>

typedef struct {
    uint8_t daddr[6]; // Endereco MAC de destino
    uint8_t saddr[6]; // Endereco MAC de origem (source)
    uint16_t protocol; // Protocolo da próxima camada (IP!)
} ethernet_hdr_t;

typedef struct {
    uint8_t hdr_len:4, // Tamanho Cabeçalho
    version:4;         // Versão do IP
    uint8_t tos;       // Tipo de serviço
    uint16_t tot_len;  // Tamanho total do IP
    uint16_t id;       // Id do Pacote
    uint16_t frag_off; // Fragmentado?
    uint8_t ttl;       // Tempo de vida
    uint8_t protocol;  // Protocolo próxima camada (TCP!)
    uint16_t check;    // Checksum
    uint8_t saddr[4];  // Endereço IP de origem
    uint8_t daddr[4];  // Endereço IP de destino
} ip_hdr_t;

typedef struct {
    uint16_t sport;      // Porta TCP de origem
    uint16_t dport;      // Porta TCP de destino
    uint32_t seq;        // Sequência
    uint32_t ack_seq;    // Acknowledgement
    uint8_t reservado:4; // Não usado
    uint8_t hdr_len:4;   // Tamanho do cabeçalho
    uint8_t flags;       // Flags do TCP
    uint16_t window;     // Tamanho da janela
    uint16_t check;      // Checksum
    uint16_t urg_ptr;    // Urgente
} tcp_hdr_t;

void print_mac(const uint8_t *mac) {
    for (int i = 0; i < 6; i++) {
        printf("%02x", mac[i]);
        if (i < 5) {
            printf(":");
        }
    }
}

void show_info(FILE *file, int data_length) {
    printf("Lendo Dados (HTTP) ..\n");
    printf("    --> Tamanho dos dados: %d bytes\n", data_length);
    printf("    --> Dados:\n");

    int bytes_read = 0;
    while (bytes_read < data_length) {
        int byte = fgetc(file);
        if (byte == EOF) {
            perror("Error reading data");
            break;
        }
        printf("%c", (char)byte);
        bytes_read++;
    }
}

int main(int argc, char *argv[]) {
    FILE *file = fopen(argv[1], "rb");

    ethernet_hdr_t ethernet_hdr;
    fread(&ethernet_hdr, sizeof(ethernet_hdr_t), 1, file);

    printf("Lendo Ethernet ..\n");
    printf("    --> MAC de Origem: ");
    print_mac(ethernet_hdr.saddr);
    printf("\n");
    printf("    --> MAC de Destino: ");
    print_mac(ethernet_hdr.daddr);
    printf("\n");

    ip_hdr_t ip_hdr;
    fread(&ip_hdr, sizeof(ip_hdr_t), 1, file);

    printf("Lendo IP ..\n");
    printf("    --> Versão do IP: %d\n", ip_hdr.version);
    printf("    --> Tamanho do cabeçalho: %d bytes\n", (ip_hdr.hdr_len * 4));
    printf("    --> Tamanho do pacote: %d bytes\n", ntohs(ip_hdr.tot_len));
    printf("    --> Endereço IP de Origem: %d.%d.%d.%d\n",
           ip_hdr.saddr[0], ip_hdr.saddr[1], ip_hdr.saddr[2], ip_hdr.saddr[3]);
    printf("    --> Endereço IP de Destino: %d.%d.%d.%d\n",
           ip_hdr.daddr[0], ip_hdr.daddr[1], ip_hdr.daddr[2], ip_hdr.daddr[3]);

    fseek(file, (ip_hdr.hdr_len * 4 - sizeof(ip_hdr_t)), SEEK_CUR);

    tcp_hdr_t tcp_hdr;
    fread(&tcp_hdr, sizeof(tcp_hdr_t), 1, file);

    printf("Lendo TCP ..\n");
    printf("    --> Porta de Origem: %d\n", ntohs(tcp_hdr.sport));
    printf("    --> Porta de Destino: %d\n", ntohs(tcp_hdr.dport));
    printf("    --> Tamanho do cabeçalho: %d bytes\n", (tcp_hdr.hdr_len * 4));

    fseek(file, (tcp_hdr.hdr_len * 4 - sizeof(tcp_hdr_t)), SEEK_CUR);

    int size = ntohs(ip_hdr.tot_len) - (ip_hdr.hdr_len * 4) - (tcp_hdr.hdr_len * 4);
    show_info(file, size);

    fclose(file);
    return 0;
}