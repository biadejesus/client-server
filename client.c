#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>  //contém as definições dos tipos de dados usados nas chamadas de sistema
#include <sys/socket.h> //contém definições de structs que os sockets precisam --struct sockaddr
#include <netinet/in.h> //contém as constantes e structs necessárias pros endereços (portas) --struct sockaddr_in
#include <netdb.h>      //struct  hostent

int main()
{
    int acao int sockfd = socket(AF_INET, SOCK_STREAM, 0); //dominio: ipv4, tipo de comunicação, protocolo.
    struct sockaddr_in server;

    if (sockfd < 0)
    {
        printf("/nErro: não foi possível criar o socket");
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1"); //endereço
    server.sin_family = AF_INET;                     //ipv4
    server.sin_port = htons(8888)                    //htons converte o numero da porta em bytes pra internet.

        if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Conecção falhou!");
        return 1;
    }

    printf("CHAT DO USUÁRIO");

    while (1)
    {
        switch (acao)
        case (1):
        {
        case /* constant-expression */:
            /* code */
            break;

        default:
            break;
        }
            printf("Entre com a requisicao (enviar mensagem): ");
        scanf("%s", acao);
        clean_buffer = getchar();
        if (!strcmp(acao, "cadastrar"))
        {
            if (send(sock, acao, strlen(acao), 0) < 0)
            {
                puts("Falha ao enviar requisicao\n");
                return 1;
            }
            memset(acao, '\0', strlen(acao));
            if (recv(sock, server_reply, 2000, 0) < 0)
            {
                puts("recv falhou\n");
                break;
            }

            puts(server_reply);
            if (!strcmp(server_reply, "Entre com o nome do livro: "))
            {
                memset(server_reply, '\0', strlen(server_reply));
                fgets(book_name, 1000, stdin);
                book_name[strlen(book_name) - 1] = '\0';
                printf("Entre com o autor: \n");
                fgets(field, 1000, stdin);
                field[strlen(field) - 1] = '\0';
                strcat(book_name, "|");
                strcat(book_name, field);
                memset(field, '\0', strlen(field));
                printf("Entre com o ano de publicacao: \n");
                fgets(field, 1000, stdin);
                strcat(book_name, "|");
                strcat(book_name, field);
                memset(field, '\0', strlen(field));
                if (send(sock, book_name, strlen(book_name), 0) < 0)
                {
                    puts("Falha ao enviar requisicao\n");
                    return 1;
                }

                if (recv(sock, server_reply, 2000, 0) < 0)
                {
                    puts("recv falhou\n");
                    break;
                }
                memset(book_name, '\0', strlen(book_name));
                puts(server_reply);
                memset(server_reply, '\0', strlen(server_reply));
            }
        }
        else if (!strcmp(acao, "buscar"))
        {
            if (send(sock, acao, strlen(acao), 0) < 0)
            {
                puts("Falha ao enviar requisicao\n");
                return 1;
            }
            memset(acao, '\0', strlen(acao));
            if (recv(sock, server_reply, 2000, 0) < 0)
            {
                puts("recv falhou\n");
                break;
            }

            puts(server_reply);
            if (!strcmp(server_reply, "Entre com o nome do livro a ser buscado: "))
            {
                memset(server_reply, '\0', strlen(server_reply));
                fgets(book_name, 1000, stdin);
                if (send(sock, book_name, strlen(book_name), 0) < 0)
                {
                    puts("Falha ao enviar requisicao\n");
                    return 1;
                }

                if (recv(sock, server_reply, 2000, 0) < 0)
                {
                    puts("recv falhou\n");
                    break;
                }
                memset(book_name, '\0', strlen(book_name));
                //puts(server_reply);
                printf("\n");
                campo = strtok(server_reply, "|");
                printf("Titulo: %s\n", campo);
                campo = strtok(NULL, "|");
                printf("Autor: %s\n", campo);
                campo = strtok(NULL, "|");
                printf("Ano de publicacao: %s\n", campo);
                campo = NULL;
                memset(server_reply, '\0', strlen(server_reply));
            }
        }
        else
        {
            printf("Comando nao identificado\n");
        }
    }
    close(sock);
    return 0;
}
}