#include "socket.c"

void inicializarBD(DADOS BD){
	for(int i =0; i<TAM; i++){
		BD.db[i].ID = -1;
	}

}

int main(){
    //criando um socket
    int socket_desc, c,  client_sock, socket_final, fifo;
	requisicao requi;
	DADOS BD;
	inicializarBD(BD);

    struct sockaddr_in server, client; // utiliza para conectar a um servidor remoto em um determinado número de porta. Para fazer isso é preciso de uma porta e um endereço de IP.
	pid_t process_id;

	//FIFO
	
	char * myfifo = "/tmp/myfifo";
	remove(myfifo);
	if(mkfifo(myfifo, 0666) < 0){ //permissão
		perror("\nErro ao criar o FIFO!\n");
	}

    socket_desc = criarSocket(portaFifo);

	listen(socket_desc , 3); // colocar os sockets em listening mode

    //Aceita e conexões de entrada
	puts("Esperando por conexões de entrada\n");
	c = sizeof(struct sockaddr_in);

	fifo = open(myfifo, O_RDWR); //ler e escrever
	write(fifo, &BD, sizeof(BD));

	while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) ){
		//fork é utilizado para lidar com as várias requisições que podem ocorrer ao mesmo tempo
		printf("\nCONNECTION ACCEPTED\n");
		process_id = fork();

		if (process_id < 0){
			printf("deu ruim\n");
			return -1;
		}
		if(process_id == 0){
			puts("a conexão foi estabelecida!\n");

			while(read(client_sock,&requi,sizeof(requi)) >= 0){
				read(fifo, &BD, sizeof(BD));
				strcpy(requi.resposta, "Cadastro realizado com sucesso\n");
				switch (requi.flag){
					case post:
					printf("\nENTROU POST\n");
					inicializarBD(BD);
						for(int i=0; i<TAM; i++){
							if(BD.db[i].ID == -1){
								strcpy(BD.db[i].nome , requi.informacao.nome);
								BD.db[i].ID = requi.informacao.ID;
								BD.db[i].idade = requi.informacao.idade;
								strcpy(BD.db[i].tipo , requi.informacao.tipo);
								//mandarMsg(socket_final, &requi , sizeof(requi));
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

				write(socket_final, &requi, sizeof(requi));
			}
			write(fifo, &BD, sizeof(BD));
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
