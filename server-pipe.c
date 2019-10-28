#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include<unistd.h>
#include <sys/socket.h> // biblioteca que importa as funções do socket
#include<arpa/inet.h> //sockaddr_in
//#include <fcntl.h>

#define post 1
#define get 2
#define TAM 100

typedef struct{
    char tipo[30];
    int idade, ID;
    char nome[30];
}info;

typedef struct{
	int tipo;
    info informacao;
    int flag;
	char resposta[30];
}requisicao;


void inicializarBD(info BD[]){
	for(int i =0; i<TAM; i++){
		BD[i].ID = -1;
	}

}

int main(){
    //criando um socket
    int socket_desc, c,  client_sock, pipe_[2], socket_final;
	requisicao requi;
	info BD[TAM], animal;

    struct sockaddr_in server, client; // utiliza para conectar a um servidor remoto em um determinado número de porta. Para fazer isso é preciso de uma porta e um endereço de IP.
	pid_t process_id;

    // a função socket() cria uma socket e retorna um descritor que pode ser usado em outras funções.
    socket_desc = socket(AF_INET, SOCK_STREAM, 0); //AF_INET( IP versãon 4) = tipo de endereços, SOCK_STREAM = tipo (isso significa protocolo TCP orientado a conexão), Protocolo - 0 [ ou IPPROTO_IP é o IP protocolo]
    if (socket_desc == -1){
        printf("Nao foi possivel criar o socket\n");
    }
    puts("Socket criado\n");

    //conectando um socket ao server
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // pega quaquer porta que está disponível no momento
	server.sin_family = AF_INET;
	server.sin_port = htons(8585); // porta aleatória

    if (bind(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0){ // bind coloca o socket no lugar reservado para ele 
		puts("Erro na conexão!");
		return 1;
	}
	
	puts("conectado\n");

    //Listen
	listen(socket_desc , 3); // colocar os sockets em listening mode

    //Aceita e conexões de entrada
	puts("Esperando por conexões de entrada\n");
	c = sizeof(struct sockaddr_in);

	//pipe
	if (pipe(pipe_) < 0)
    {
        perror("Falha ao criar pipe.\n");
    }

	write(pipe_[1], &BD, sizeof(BD));

	while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) ){
		// process_id = fork(); //fork é utilizado para lidar com as várias requisições que podem ocorrer ao mesmo tempo
		
		if(process_id < 0){
			perror("Não foi possível estabelecer uma conexão!!");
			return false;
		}
		else if(process_id == 0){
			puts("a conexão foi estabelecida!");
			read(pipe_[0], &BD, sizeof(BD));

			if(read(client_sock,&requi,sizeof(requi)) >= 0){
			printf("\n%d", requi.flag);
			strcpy(requi.resposta, "deu bom" );

			switch (requi.tipo){
                case post:
					inicializarBD(BD);
					for(int i=0; i<TAM; i++){
						if(BD[i].ID != -1){
							strcpy(BD[requi.informacao.ID].nome , requi.informacao.nome);
							BD[requi.informacao.ID].ID = requi.informacao.ID;
							BD[requi.informacao.ID].idade = requi.informacao.idade;
							strcpy(BD[requi.informacao.ID].tipo , requi.informacao.tipo);
							animal = BD[i];
							break;
						}
					}
                    
					puts("\nPost:\n");
                    printf("\tNome: %s\n", requi.informacao.nome);
                    printf("\tDiretor: %d\n", requi.informacao.ID);
                    printf("\tGênero: %d\n", requi.informacao.idade);
                    printf("\tDuração: %s\n", requi.informacao.tipo);
                    break;

                case get:
                    animal = requi.informacao;
                    printf("\nGet\n");
                    printf("\tID: %d\n", requi.informacao.ID);
                    break;
                }

                write(socket_final, &animal, sizeof(info));
                write(pipe_[1], &BD, sizeof(BD));
            }
		}
		else{
			perror("Leitura da requisição falhou.\n");
            // write(socket_final, "REQ_FAILED", 11);
		}
	}
	if(socket_final < 0)
		puts("\n\nFalha na requisição!\n");
		

			// if( send(client_sock, &requi , sizeof(requi) , 0) < 0) //enviar dados pelo socket
			// 		{
			// 			perror("\nErro: falha ao enviar dados");
			// 			return true;
			// 		}
			// }	
		return 0;
}
