#include "socket.c"

void inicializarBD(DADOS BD){
	for(int i =0; i<TAM; i++){
		BD.db[i].ID = -1;
	}

}

int main(){
    //criando um socket
    int socket_desc, c,  client_sock, pipe_[2], socket_final;
	requisicao requi;
	DADOS BD;
	inicializarBD(BD);

    struct sockaddr_in server, client; // utiliza para conectar a um servidor remoto em um determinado número de porta. Para fazer isso é preciso de uma porta e um endereço de IP.
	pid_t process_id;

    // a função socket() cria uma socket e retorna um descritor que pode ser usado em outras funções.
    socket_desc = criarSocket(portaPipe);

     //Listen
	listen(socket_desc , 3); // colocar os sockets em listening mode

    //Aceita e conexões de entrada
	puts("Esperando por conexões de entrada\n");
	c = sizeof(struct sockaddr_in);

	//pipe
	if (pipe(pipe_) < 0){ //é usado para passar informações de um processo para outro
        perror("Falha ao criar o pipe.\n");
    }

	write(pipe_[1], &BD, sizeof(BD));

	printf("\nANTES WHILE....\n");

	while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) ){
		process_id = fork(); //fork é utilizado para lidar com as várias requisições que podem ocorrer ao mesmo tempo
		printf("\nCONNECTION ACCEPTED\n");

		if(process_id < 0){
			perror("Não foi possível estabelecer uma conexão!!");
			return false;
		}
		else if(process_id == 0){
			puts("a conexão foi estabelecida!");
			read(pipe_[0], &BD, sizeof(BD));
			while(read(client_sock,&requi,sizeof(requi)) >= 0){
				printf("\nENTROU IF");
				switch (requi.flag){
					case post:
						printf("\nENTROU POST\n");
						inicializarBD(BD);
						for(int i=0; i<TAM; i++){
							if(BD.db[i].ID == -1){
								strcpy(BD.db[i].nome , requi.informacao.nome);
								BD.db[i].ID = requi.informacao.ID;
								BD.db[i].idade = requi.informacao.idade;
								strcpy(requi.resposta, "Deu bom Pipe\n");
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

					write(socket_final, &requi, sizeof(requi));
					write(pipe_[1], &BD, sizeof(BD));
				}
		}
	}
	if(socket_final < 0)
		puts("\n\nFalha na requisição!\n");	
	
	return 0;
}