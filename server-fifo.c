#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/socket.h> // biblioteca que importa as funções do socket
#include <arpa/inet.h> //sockaddr_in
#include <fcntl.h>
#include <unistd.h> 

#define post 1
#define get 2
#define TAM 100

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


void inicializarBD(info BD[]){
	for(int i =0; i<TAM; i++){
		BD[i].ID = -1;
	}

}

int main(){
    //criando um socket
    int socket_desc, c,  client_sock, socket_final, fifo;
	requisicao requi;
	info BD[TAM], animal;
	inicializarBD(BD);

    struct sockaddr_in server, client; // utiliza para conectar a um servidor remoto em um determinado número de porta. Para fazer isso é preciso de uma porta e um endereço de IP.
	pid_t process_id;

	//FIFO
	
	char * myfifo = "/tmp/myfifo";
	remove(myfifo);
	if(mkfifo(myfifo, 0666) < 0){ //permissão
		perror("\nErro ao criar o FIFO!\n");
	}

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

	fifo = open(myfifo, O_RDWR); //ler e escrever
	write(fifo, &BD, sizeof(BD));

	while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) ){
		process_id = fork(); //fork é utilizado para lidar com as várias requisições que podem ocorrer ao mesmo tempo
		printf("\nCONNECTION ACCEPTED\n");
		if(process_id < 0){
			perror("Não foi possível estabelecer uma conexão!!");
			return false;
		}
		if(process_id == 0){
			puts("a conexão foi estabelecida!");
			read(fifo, &BD, sizeof(BD));

		if(read(client_sock,&requi,sizeof(requi)) >= 0){

			switch (requi.flag){
				case post:
				printf("\nENTROU POST\n");

					for(int i=0; i<TAM; i++){
						if(BD[i].ID == -1){
							printf("\nENTROU IF BD\n");
							strcpy(BD[i].nome , requi.informacao.nome);
							BD[i].ID = requi.informacao.ID;
							BD[i].idade = requi.informacao.idade;
							strcpy(BD[i].tipo , requi.informacao.tipo);
							animal = BD[i];
							printf("\nFLAG: %d", requi.flag);
							printf("\nID: %d", requi.informacao.ID);
							strcpy(requi.resposta, "deu bom");
							printf("\nRESPOSTA: %s", requi.resposta);
							send(client_sock, &requi , sizeof(requi) , 0);
							break;
						}
					}
					
					puts("\nPost:\n");
                    printf("\tNome: %s\n", requi.informacao.nome);
                    printf("\tID: %d\n", requi.informacao.ID);
                    printf("\tIdade: %d\n", requi.informacao.idade);
                    printf("\tTipo: %s\n", requi.informacao.tipo);
                    break;

                case get:
                    printf("\nGet\n");
						printf("\tID: %d\n", requi.informacao.ID);
						for(int i=0; i<TAM; i++){
							if(BD[i].ID == requi.informacao.ID){
								animal = BD[i];
								write(client_sock, &animal, sizeof(animal));
								break;
							}
						}
						//procurar no bd e retornar a struct com esse id e dar um write
						break;
                }

                write(socket_final, &animal, sizeof(info));
                write(fifo, &BD, sizeof(BD));
            }
		else{
			perror("Leitura da requisição falhou.\n");
            // write(socket_final, "REQ_FAILED", 11);
		}
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
