#include "structs.c"
#include "socket.h"

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

    if (bind(sockt, (struct sockaddr *)&server, sizeof(server)) < 0){
        perror("\nErro: Conexão falhou!\n");
        return -1;
    }

    printf("Conexão feita com sucesso.");
    return sockt;
}

int conectarSocket(int PORTA){
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

void mandarMsg(int sockt, requisicao msg, uint32_t msgsize){
    if (write (sockt, &msg, msgsize) < 0){
        printf("Não foi possível mandar mensagem.");
        close(sockt);
        exit(1);
    }
    printf("Mensagem enviada.");
}