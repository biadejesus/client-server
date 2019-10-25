#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h> //makes available the type in_port_t and the type in_addr_t
#include <sys/types.h>  //contém as definições dos tipos de dados usados nas chamadas de sistema
#include <sys/socket.h> //contém definições de structs que os sockets precisam --struct sockaddr
#include <netinet/in.h> //contém as constantes e structs necessárias pros endereços (portas) --struct sockaddr_in
#include <netdb.h>      //struct  hostent

#define criar 1
#define buscar 2

typedef struct{
    char tipo[30];
    int idade;
    char nome[30];
}info;

typedef struct{
    info informacao;
    int flag;
    char resposta[30];
}requisicao;

int main()
{
    int acao, sock, m; 
    sock = socket(AF_INET, SOCK_STREAM, 0); //dominio: ipv4, tipo de comunicação, protocolo.
    struct sockaddr_in proxy;

    info dados;
    requisicao requi;

    if (sock < 0) //criando o socket
    {
        perror("/nErro: não foi possível criar o socket");
    }

    proxy.sin_addr.s_addr = inet_addr("127.0.0.1"); //endereço
    proxy.sin_family = AF_INET;                     //ipv4
    proxy.sin_port = htons(8585);                   //htons converte o numero da porta em bytes pra internet.

    if (connect(sock, (struct sockaddr *)&proxy, sizeof(proxy)) < 0) //realizando conexão
    {
        perror("\nErro: Conexão falhou!\n");
        return 1;
    }

    printf("\n-------Sistema de cadastro de animais MiAudota-------\n");

    do{

        printf("\nEscolha uma opção:");
        printf("\n1- Cadastrar um animal");
        printf("\n2- Buscar um animal");
        printf("\n0- Sair\n");
        scanf("%d", &acao);

        switch (acao)
        {
            case 1:
                fflush(stdin);
                printf("\nInsira o nome do animal: ");
                scanf("%s", dados.nome);
                printf("\nInsira o tipo de animal: ");
                scanf("%s", dados.tipo);
                fflush(stdin);
                printf("\nInsira a idade do animal: ");
                scanf("%d", &dados.idade);
                requi.flag = 1;
                requi.informacao = dados;

                if( send(sock, &requi , sizeof(requi) , 0) < 0) //enviar dados pelo socket
                {
                    perror("\nErro: falha ao enviar dados");
                    return 1;
                }

                // m = write(sock,dados.tipo,strlen(dados.tipo));//escrever
                // if (m < 0)
                // {
                //     printf("\nErro ao escrever no socket");
                // }

                m = read(sock, &requi, sizeof(requi)); //ler
                printf("\n%s", requi.resposta);
                if (m < 0)
                {
                    printf("\nErro ao ler do socket");
                }

                break;
            
            case 2:
                printf("\nInsira o nome do animal: ");
                scanf("%s", dados.nome);
                requi.flag=2;

                // if( send(sock, &dados.idade , strlen(dados.idade) , 0) < 0) //enviar dados pelo socket
                // {
                //     perror("\nErro: falha ao enviar dados");
                //     return 1;
                // }

                // m = write(sock,dados.nome,strlen(dados.tipo));//escrever
                // if (m < 0)
                // {
                //     printf("\nErro ao escrever no socket");
                // }

                // m = read(sock,dados.nome,1024); //ler

                // if (m < 0)
                // {
                //     printf("\nErro ao ler do socket");
                // }

            default:
                break;
        }

    }while(acao!=0);
    printf("\nbye\n");
    return 0;
    
}