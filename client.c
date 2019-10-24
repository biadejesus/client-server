#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>  //contém as definições dos tipos de dados usados nas chamadas de sistema
#include <sys/socket.h> //contém definições de structs que os sockets precisam --struct sockaddr
#include <netinet/in.h> //contém as constantes e structs necessárias pros endereços (portas) --struct sockaddr_in
#include <netdb.h>      //struct  hostent

    typedef struct{
        char tipo[30];
        int idade;
        char raca[30];
    }info;

int main()
{
    int acao;
    int sock; 
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); //dominio: ipv4, tipo de comunicação, protocolo.
    struct sockaddr_in proxy;

    info dados;

    if (sockfd < 0)
    {
        printf("/nErro: não foi possível criar o socket");
    }

    proxy.sin_addr.s_addr = inet_addr("127.0.0.1"); //endereço
    proxy.sin_family = AF_INET;                     //ipv4
    proxy.sin_port = htons(8888);                    //htons converte o numero da porta em bytes pra internet.

    if (connect(sock, (struct sockaddr *) & proxy, sizeof(proxy)) < 0)
    {
        perror("Conexão falhou!");
        return 1;
    }

    printf("\n\tSistema de cadastro de animais MiAudota\n");

    do{
        printf("\nInsira o tipo de animal: ");
        scanf("%s", dados.tipo);
        printf("\nInsira a idade do animal: ");
        scanf("%d", dados.idade);
        printf("\nInsira a raça do animal: ");
        scanf("%d", dados.raca);

        

    }while (acao != 0);
    
}