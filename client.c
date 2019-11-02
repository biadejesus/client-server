#include "includes.h"
#include "structs.c"
#include "funcoes.c"

int main()
{
    int acao, sock, m; 
    info dados;
    requisicao requi;

    sock = conectarSocket(8585);

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