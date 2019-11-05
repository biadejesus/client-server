#include "socket.c"

int main()
{
    int acao, sock, m; 
    info dados;
    requisicao requi;
    sock = conectarSocket(portaCliente);

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
                printf("\nInsira o nome do cachorro: ");
                scanf("%s", requi.informacao.nome);
                printf("\nInsira a idade do cachorro: ");
                scanf("%d", &requi.informacao.idade);
                requi.informacao.ID++;
                requi.flag = 1;
                strcpy(requi.informacao.tipo , "cachorro");

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
                printf("\nInsira o nome do gato: ");
                scanf("%s", requi.informacao.nome);
                printf("\nInsira a idade do gato: ");
                scanf("%d", &requi.informacao.idade);
                requi.informacao.ID++;
                requi.flag = 1;
                strcpy(requi.informacao.tipo , "gato");

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
            
            case 3:
                printf("\nInsira o nome do passaro: ");
                scanf("%s", requi.informacao.nome);
                printf("\nInsira a idade do passaro: ");
                scanf("%d", &requi.informacao.idade);
                requi.informacao.ID++;
                requi.flag = 1;
                strcpy(requi.informacao.tipo , "cachorro");

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

            case 4:
                printf("\nInsira o id do cachorro a ser buscado: ");
                scanf("%d", &requi.informacao.ID);
                requi.flag=2;
                 
                if( (write(sock, &requi, sizeof(requi))) < 0) //enviar dados pelo socket
                {
                    perror("\nErro: falha ao enviar dados");
                    return 1;
                }

                if ((read(sock, &requi, sizeof(requi))) < 0) //ler dados do socket
                {
                    printf("\nErro ao ler do socket");
                }
                printf("\n%s\n", requi.resposta);
                printf("\n%s\n", requi.informacao.nome);
                break;

            case 5:
                printf("\nInsira o id do gato a ser buscado: ");
                scanf("%d", &requi.informacao.ID);
                requi.flag=2;
                 
                if( (write(sock, &requi, sizeof(requi))) < 0) //enviar dados pelo socket
                {
                    perror("\nErro: falha ao enviar dados");
                    return 1;
                }

                if ((read(sock, &requi, sizeof(requi))) < 0) //ler dados do socket
                {
                    printf("\nErro ao ler do socket");
                }
                printf("\n%s\n", requi.resposta);
                printf("\n%s\n", requi.informacao.nome);
                break;

            case 6:
                printf("\nInsira o id do passaro a ser buscado: ");
                scanf("%d", &requi.informacao.ID);
                requi.flag=2;
                 
                if( (write(sock, &requi, sizeof(requi))) < 0) //enviar dados pelo socket
                {
                    perror("\nErro: falha ao enviar dados");
                    return 1;
                }

                if ((read(sock, &requi, sizeof(requi))) < 0) //ler dados do socket
                {
                    printf("\nErro ao ler do socket");
                }
                printf("\n%s", requi.resposta);
                printf("\n%s", requi.informacao.nome);
                break;

            default:
                break;
        }

    }while(acao!=0);
    printf("\nbye\n");
    return 0;
}