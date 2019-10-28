#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h> // biblioteca que importa as funções do socket
#include<arpa/inet.h> //sockaddr_in
//#include <fcntl.h>

#define true 1
#define false 0

typedef struct{
    char tipo[30];
    int idade;
    char nome[30];
}info;

typedef struct{
    info informacao;
    int flag;
	char resposta[30];
}requisicao;

int main(){
    //criando um socket
    int socket_desc, c,  client_sock;
	requisicao requi;

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
	while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) ){
		// process_id = fork(); //fork é utilizado para lidar com as várias requisições que podem ocorrer ao mesmo tempo
		
		if(process_id < 0){
			perror("Não foi possível estabelecer uma conexão!!");
			return false;
		}
		else{
			puts("a conexão foi estabelecida!");
			
		}
		read(client_sock,&requi,sizeof(requi));
		printf("\n%d", requi.flag);
		strcpy(requi.resposta, "deu bom" );
		if( send(client_sock, &requi , sizeof(requi) , 0) < 0) //enviar dados pelo socket
                {
                    perror("\nErro: falha ao enviar dados");
                    return true;
                }
		
		
		puts("Conexão aceita\n");
		
	}

}
