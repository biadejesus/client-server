#include "structs.c"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int criarSocket(int PORTA){
    int sockt, erro;
    struct sockaddr_in server;

    if ((sockt = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("A criação do socket falhou.");
        exit(1);
    }
    printf("Socket criado com sucesso.");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
    server.sin_port = htons(PORTA);

    if (connect(sockt, (struct sockaddr *)&server, sizeof(server)) < 0){
        perror("\nErro: Conexão falhou!\n");
        return -1;
    }

    printf("Conexão feita com sucesso.");
    return sockt;
}

void mandarMsg(int sockt, void *msg, uint32_t msgsize){
    if (write (sockt, msg, msgsize) < 0){
        printf("Não foi possível mandar mensagem.");
        close(sockt);
        exit(1);
    }
    printf("Mensagem enviada.");
}