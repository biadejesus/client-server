#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h> // biblioteca que importa as funções do socket
#include <arpa/inet.h> //sockaddr_in
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

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

#define PSHARED 1

sem_t semaphore;

int main(){
    //criando um socket
    int socket_desc, c,  client_sock, pipe_[2], socket_final;
	requisicao requi;
	info BD[TAM], animal;
	inicializarBD(BD);

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


	//para fazer multiplas conexões
	key_t key = ftok("shmfile", 65);

    if (key < 0)
    {
        perror("Erro ao criar ftok");
        exit(EXIT_FAILURE);
    }

    int shmid = shmget(key, TAM*sizeof(info), 0666 | IPC_CREAT);

    if (shmid < 0)
    {
        perror("Erro ao criar shmid");
        exit(EXIT_FAILURE);
    }

	//semaforo gerencia as multiplas conexões
    if (sem_init(&semaphore, PSHARED, 1) < 0) {
        perror("Erro na inicialização do semáforo!");
        exit(EXIT_FAILURE);
    }

    info *MC_animal = (info *)shmat(shmid, NULL, 0);

    memcpy(MC_animal, &BD, sizeof(info));

	while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) ){
		process_id = fork(); //fork é utilizado para lidar com as várias requisições que podem ocorrer ao mesmo tempo
		
		if(process_id < 0){
			perror("Não foi possível estabelecer uma conexão!!");
			return false;
		}
		if(process_id == 0){
			puts("a conexão foi estabelecida!");
			
			sem_wait(&semaphore);
            memcpy(&BD, MC_animal, sizeof(info)); //leio da memoria compartilhada para o banco de dados
            sem_post(&semaphore);
		

			if(read(client_sock,&requi,sizeof(requi)) >= 0){
			printf("\n%d", requi.flag);
			strcpy(requi.resposta, "deu bom" );

				switch (requi.flag){
					case post:
						for(int i=0; i<TAM; i++){
							if(BD[i].ID != -1){
								strcpy(BD[i].nome , requi.informacao.nome);
								BD[i].ID = requi.informacao.ID;
								BD[i].idade = requi.informacao.idade;
								strcpy(BD[i].tipo , requi.informacao.tipo);
								animal = BD[i];
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
				write(pipe_[1], &BD, sizeof(BD));
        }

		else{
			perror("Leitura da requisição falhou.\n");
		}

		sem_wait(&semaphore);
        memcpy(MC_animal, &BD, sizeof(info)); //escreve o do banco de dados para a memória compartilhada
        sem_post(&semaphore);
		}
	}
	if(socket_final < 0)
		puts("\n\nFalha na requisição!\n");	
	
	return 0;
}
