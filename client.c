#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h> //makes available the type in_port_t and the type in_addr_t
#include <sys/types.h>  //contém as definições dos tipos de dados usados nas chamadas de sistema
#include <sys/socket.h> //contém definições de structs que os sockets precisam --struct sockaddr
#include <netinet/in.h> //contém as constantes e structs necessárias pros endereços (portas) --struct sockaddr_in
#include <netdb.h>      //struct  hostent

#define criar 1
#define buscar 2

typedef struct{
    char tipo[30];
    int idade, ID;
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
    printf("\nsocket criado\n");

    proxy.sin_addr.s_addr = inet_addr("127.0.0.1"); //endereço
    proxy.sin_family = AF_INET;                     //ipv4
    proxy.sin_port = htons(8585);                   //htons converte o numero da porta em bytes pra internet.

    if (connect(sock, (struct sockaddr *)&proxy, sizeof(proxy)) < 0) //realizando conexão
    {
        perror("\nErro: Conexão falhou!\n");
        return 1;
    }
    printf("\nsocket conectado\n");

    printf("\n-------Sistema de cadastro de animais MiAudota-------\n");

    do{

        printf("\nEscolha uma opção:");
        printf("\n1- Cadastrar um animal do tipo cachorro");
        printf("\n2- Cadastrar um animal do tipo gato");
        printf("\n3- Cadastrar um animal do tipo passaro");
        printf("\n4- Buscar um cachorro");
        printf("\n5- Buscar um gato");
        printf("\n6- Buscar um passaro");
        printf("\n0- Sair\n");
        scanf("%d", &acao);

        switch (acao)
        {
            case 1:
                dados.tipo == "cachorro";
                printf("\nInsira o nome do animal: ");
                scanf("%s", dados.nome);
                printf("\nInsira a idade do animal: ");
                scanf("%d", &dados.idade);
                requi.informacao.ID++;
                printf("\nID requi: %d", requi.informacao.ID);
                printf("\nID dados: %d", dados.ID);
                requi.flag = 1;
                printf("\nFLAG: %d\n", requi.flag);
                strcpy(requi.informacao.nome , dados.nome);
                requi.informacao.idade = dados.idade;
                strcpy(requi.informacao.tipo , dados.tipo);
                printf("\nenviando dados\n");

                if( send(sock, &requi , sizeof(requi) , 0) < 0) //enviar dados pelo socket
                {
                    perror("\nErro: falha ao enviar dados");
                    return 1;
                }


                if ((read(sock, &requi, sizeof(requi))) < 0) //ler dados do socket
                {
                    printf("\nErro ao ler do socket");
                }
                printf("\nresposta: %s", requi.resposta);
                break;

            case 2:
                dados.tipo == "gato";
                printf("\nInsira o nome do animal: ");
                scanf("%s", dados.nome);
                printf("\nInsira a idade do animal: ");
                scanf("%d", &dados.idade);
                dados.ID++;
                requi.flag = 1;
                printf("\nFLAG: \n", requi.flag);
                requi.informacao = dados;

                if( send(sock, &requi , sizeof(requi) , 0) < 0) //enviar dados pelo socket
                {
                    perror("\nErro: falha ao enviar dados");
                    return 1;
                }

                printf("\n%s", requi.resposta);
                if ((read(sock, &requi, sizeof(requi))) < 0) //ler dados do socket
                {
                    printf("\nErro ao ler do socket");
                }


                break;
            
            case 3:
                dados.tipo == "passaro";
                printf("\nInsira o nome do animal: ");
                scanf("%s", dados.nome);
                printf("\nInsira a idade do animal: ");
                scanf("%d", &dados.idade);
                dados.ID++;
                requi.flag = 1;
                requi.informacao = dados;

                if( send(sock, &requi , sizeof(requi) , 0) < 0) //enviar dados pelo socket
                {
                    perror("\nErro: falha ao enviar dados");
                    return 1;
                }

                printf("\n%s", requi.resposta);
                if ((read(sock, &requi, sizeof(requi))) < 0) //ler dados do socket
                {
                    printf("\nErro ao ler do socket");
                }


                break;


            case 4:
                printf("\nInsira o id do cachorro a ser buscado: ");
                scanf("%d", &requi.informacao.ID);
                requi.flag=2;
                printf("\nCOMEÇO");
                 
                if( (write(sock, &requi, sizeof(requi))) < 0) //enviar dados pelo socket
                {
                    perror("\nErro: falha ao enviar dados");
                    return 1;
                }

                printf("\nFEZ SEND");
                if ((read(sock, &requi, sizeof(requi))) < 0) //ler dados do socket
                {
                    printf("\nErro ao ler do socket");
                }
                printf("\nFEZ READ E SEND");
                printf("\n%s", requi.resposta);
                printf("\n%s", requi.informacao.nome);
                break;

            case 5:
                printf("\nInsira o nome do gato a ser buscado: ");
                scanf("%s", requi.informacao.ID);
                requi.flag=2;

                if( send(sock, &requi.informacao.ID , sizeof(requi) , 0) < 0) //enviar dados pelo socket
                {
                    perror("\nErro: falha ao enviar dados");
                    return 1;
                }

                printf("\n%s", requi.resposta);
                if ((read(sock, &requi, sizeof(requi))) < 0) //ler dados do socket
                {
                    printf("\nErro ao ler do socket");
                }
                break;

            case 6:
                printf("\nInsira o nome do passaro a ser buscado: ");
                scanf("%s", requi.informacao.ID);
                requi.flag=2;

                if( send(sock, &requi.informacao.ID , sizeof(requi) , 0) < 0) //enviar dados pelo socket
                {
                    perror("\nErro: falha ao enviar dados");
                    return 1;
                }

                printf("\n%s", requi.resposta);
                if ((read(sock, &requi, sizeof(requi))) < 0) //ler dados do socket
                {
                    printf("\nErro ao ler do socket");
                }

                break;

            default:
                break;
        }

    }while(acao!=0);
    printf("\nbye\n");
    return 0;
    
}