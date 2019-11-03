#include "funcoes.c"

#pragma pack(1)

int main(){
    int sSockt, cSockt;
    int gatoSockt, cachorroSockt, passaroSockt;
    int nread, fd;
    struct sockaddr_in client;
    int tamClient = sizeof(client);
    pid_t pid;
    sSockt = criarSocket(portaCliente);
    gatoSockt = criarSocket(portaFifo);
    cachorroSockt = criarSocket(portaMC);
    passaroSockt = criarSocket(portaPipe);

    while (cSockt = accept(sSockt, (struct sockaddr *)&client, &tamClient)){
        if (cSockt < 0){
            perror("Algo deu errado na função accept()\n");
        }
        if ((pid = fork()) < 0){
            perror("Algo deu errado na função fork()\n");
        }
        if (pid <= 0){
            requisicao buffer;
            while (read(cSockt, &buffer, 114)){
                requisicao msgServer;
                int tamServer;
                if (strcmp(buffer.informacao.tipo, "cachorro") == 0){
                    printf ("Requisição do tipo cachorro identificada. Enviando para o servidor.\n");
                    mandarMsg(cachorroSockt, buffer, 114);
                    tamServer = read(cachorroSockt, &msgServer, 114);
                }
                else if (strcmp(buffer.informacao.tipo, "gato") == 0){
                    printf ("Requisição do tipo gato identificada. Enviando para o servidor.\n");
                    mandarMsg(gatoSockt, buffer, 114);
                    tamServer = read(gatoSockt, &msgServer, 114);
                }
                else if (strcmp(buffer.informacao.tipo, "passaro") == 0){
                    printf ("Requisição do tipo pássaro identificada. Enviando para o servidor.\n");
                    mandarMsg(passaroSockt, buffer, 114);
                    tamServer = read(gatoSockt, &msgServer, 114);
                }
                else{
                    printf("Erro na requisição (proxy.c)\n");
                }
                mandarMsg(cSockt, msgServer, tamServer);
            }
            printf ("Encerrando conexão..\n");

        close(cSockt);
        }
    }
    close(sSockt);
    return 0;
}