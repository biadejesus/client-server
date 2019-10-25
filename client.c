#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h> //makes available the type in_port_t and the type in_addr_t
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
    int acao, sock, m; 
    sock = socket(AF_INET, SOCK_STREAM, 0); //dominio: ipv4, tipo de comunicação, protocolo.
    struct sockaddr_in proxy;

    info dados;

    if (sock < 0) //criando o socket
    {
        perror("/nErro: não foi possível criar o socket");
    }

    proxy.sin_addr.s_addr = inet_addr("127.0.0.1"); //endereço
    proxy.sin_family = AF_INET;                     //ipv4
    proxy.sin_port = htons(8888);                   //htons converte o numero da porta em bytes pra internet.

    // if (connect(sock, (struct sockaddr *)&proxy, sizeof(proxy)) < 0) //realizando conexão
    // {
    //     perror("\nErro: Conexão falhou!\n");
    //     return 1;
    // }

    printf("\n\tSistema de cadastro de animais MiAudota\n");


        printf("\nInsira o tipo de animal: ");
        scanf("%s", dados.tipo);
        fflush(stdin);
        printf("\nInsira a idade do animal: ");
        scanf("%d", &dados.idade);
        printf("\nInsira a raça do animal: ");
        scanf("%s", dados.raca);

        // if( send(sock, dados.tipo , strlen(dados.tipo) , 0) < 0) //enviar dados pelo socket
        // {
        //     perror("\nErro: falha ao enviar dados");
        //     return 1;
        // }
        // if( send(sock, &dados.idade , strlen(dados.idade) , 0) < 0) //enviar dados pelo socket
        // {
        //     perror("\nErro: falha ao enviar dados");
        //     return 1;
        // }
        // if( send(sock, dados.raca , strlen(dados.raca) , 0) < 0) //enviar dados pelo socket
        // {
        //     perror("\nErro: falha ao enviar dados");
        //     return 1;
        // }

        // m = write(sock,dados.tipo,strlen(dados.tipo));//escrever
        // if (m < 0)
        // {
        //     printf("\nErro ao escrever no socket");
        // }

        // m = read(sock,dados.tipo,1024); //ler

        // if (m < 0)
        // {
        //     printf("\nErro ao ler do socket");
        // }

        printf("\n%s", dados.tipo);

        return 0;
    
}