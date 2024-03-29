#include "socket.c"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <semaphore.h>

void inicializarBD(DADOS BD){
	for(int i =0; i<TAM; i++){
		BD.db[i].ID = -1;
	}

}

sem_t semaphore;

int main(){
    //criando um socket
    int socket_desc, c,  client_sock, pipe_[2], socket_final;
	requisicao requi;
	DADOS BD;
	inicializarBD(BD);

    struct sockaddr_in server, client; // utiliza para conectar a um servidor remoto em um determinado número de porta. Para fazer isso é preciso de uma porta e um endereço de IP.
	pid_t process_id;

    socket_desc = criarSocket(portaMC);

    //Listen
	listen(socket_desc , 3); // colocar os sockets em listening mode

    //Aceita e conexões de entrada
	puts("Esperando por conexões de entrada\n");
	c = sizeof(struct sockaddr_in);


	//para fazer multiplas conexões

	char *shmfile = "tmp/shmfile";
	remove(shmfile);
	key_t key = ftok(shmfile, 65);

    // if (key < 0)
    // {
    //     perror("Erro ao criar ftok");
    //     exit(EXIT_FAILURE);
    // }

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

    DADOS *MC_animal = (DADOS*)shmat(shmid, NULL, 0);

    //memcpy(MC_animal, &BD, sizeof(BD));

	while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) ){
		process_id = fork(); //fork é utilizado para lidar com as várias requisições que podem ocorrer ao mesmo tempo
		
		if(process_id < 0){
			perror("Não foi possível estabelecer uma conexão!!");
			return false;
		}
		if(process_id == 0){
			puts("a conexão foi estabelecida!");
			
			sem_wait(&semaphore);
            memcpy(&BD, MC_animal, sizeof(DADOS)); //leio da memoria compartilhada para o banco de dados
            sem_post(&semaphore);
		

			while(read(client_sock,&requi,sizeof(requi)) >= 0){
			printf("\n%d", requi.flag);
			strcpy(requi.resposta, "Cadastro realizado com sucesso\n" );

				switch (requi.flag){
					case post:
						for(int i=0; i<TAM; i++){
							if(BD.db[i].ID == -1){
								strcpy(BD.db[i].nome , requi.informacao.nome);
								BD.db[i].ID = requi.informacao.ID;
								BD.db[i].idade = requi.informacao.idade;
								strcpy(BD.db[i].tipo , requi.informacao.tipo);
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
							if(BD.db[i].ID == requi.informacao.ID){
								write(client_sock, &requi, sizeof(requi));
								break;
							}
						}
						//procurar no bd e retornar a struct com esse id e dar um write
						break;
				}

				write(client_sock, &requi, sizeof(requi));
        }

		sem_wait(&semaphore);
        memcpy(MC_animal, &BD, sizeof(DADOS)); //escreve o do banco de dados para a memória compartilhada
        sem_post(&semaphore);
		}
	}
	if(socket_final < 0)
		puts("\n\nFalha na requisição!\n");	
	
	return 0;
}