#include "socket.c"

int main(){
    int xSockt, cSockt;
    int gatoSockt, cachorroSockt, passaroSockt;
    int nread, fd;
    struct sockaddr_in client;
    int tamClient = sizeof(client);
    pid_t pid;
    xSockt = criarSocket(portaCliente);
    listen(xSockt , 3);
    gatoSockt = conectarSocket(portaFifo);
    cachorroSockt = conectarSocket(portaMC);
    passaroSockt = conectarSocket(portaPipe);
    int c = sizeof(struct sockaddr_in);
    //dar bind

    while ((cSockt = accept(xSockt, (struct sockaddr *)&client, (socklen_t*)&c)) ){
        if (cSockt < 0){
            perror("Algo deu errado na função accept()\n");
            continue;
        }
        if ((pid = fork()) < 0){
            perror("Algo deu errado na função fork()\n");
            return -1;
        }
        if (pid <= 0){
            requisicao buffer;
            while (read(cSockt, &buffer, sizeof(requisicao))){ //n é melhor fazer switch case????
                requisicao msgServer;
                int tamServer;
                printf("tipo de animal: %s", buffer.informacao.nome);
                if (strcmp(buffer.informacao.tipo, "cachorro") == 0){//memcpy? conectar com o cliente?conectar com o server? diferenciar os
                    printf ("Requisição do tipo cachorro identificada. Enviando para o servidor.\n");
                    mandarMsg(cachorroSockt, &buffer, sizeof(requisicao));
                    tamServer = read(cachorroSockt, &msgServer, sizeof(requisicao));
                }
                else if (strcmp(buffer.informacao.tipo, "gato") == 0){
                    printf ("Requisição do tipo gato identificada. Enviando para o servidor.\n");
                    mandarMsg(gatoSockt, &buffer, sizeof(requisicao));
                    tamServer = read(gatoSockt, &msgServer, sizeof(requisicao));
                }
                else if (strcmp(buffer.informacao.tipo, "passaro") == 0){
                    printf ("Requisição do tipo pássaro identificada. Enviando para o servidor.\n");
                    mandarMsg(passaroSockt, &buffer, sizeof(requisicao));
                    tamServer = read(passaroSockt, &msgServer, sizeof(requisicao));
                }
                else{
                    printf("Erro na requisição (proxy.c)\n");
                }
                mandarMsg(cSockt, &msgServer, tamServer);
            }
            printf ("Encerrando conexão..\n");

        close(cSockt);
        }
    }
    close(xSockt);
    return 0;
}