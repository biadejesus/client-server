#include "includes.h"
#include "structs.c"
#include "funcoes.c"

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

    struct sockaddr_in server, client; // utiliza para conectar a um servidor remoto em um determinado número de porta. Para fazer isso é preciso de uma porta e um endereço de IP.
	pid_t process_id;

    socket_desc = criarSocket(8585);

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
		// process_id = fork(); //fork é utilizado para lidar com as várias requisições que podem ocorrer ao mesmo tempo
		
		if(process_id < 0){
			perror("Não foi possível estabelecer uma conexão!!");
			return false;
		}
		else if(process_id == 0){
			puts("a conexão foi estabelecida!");
			
			sem_wait(&semaphore);
            memcpy(&BD, MC_animal, sizeof(info)); //leio da memoria compartilhada para o banco de dados
            sem_post(&semaphore);

			if(read(client_sock,&requi,sizeof(requi)) >= 0){
			printf("\n%d", requi.flag);
			strcpy(requi.resposta, "deu bom" );

				switch (requi.flag){
					case post:
						inicializarBD(BD);
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
		}

		sem_wait(&semaphore);
        memcpy(MC_animal, &BD, sizeof(info)); //escreve o do banco de dados para a memória compartilhada
        sem_post(&semaphore);

	}
	if(socket_final < 0)
		puts("\n\nFalha na requisição!\n");	
	
	return 0;
}
