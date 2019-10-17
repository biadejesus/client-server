#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h> // biblioteca que importa as funções do socket
#include<arpa/inet.h> //sockaddr_in

int main(){
    //criando um socket
    int socket_descritor, new_socket , c;
    struct sockaddr_in server, client; // utiliza para conectar a um servidor remoto em um determinado número de porta. Para fazer isso é preciso de uma porta e um endereço de IP.

    // a função socket() cria uma socket e retorna um descritor que pode ser usado em outras funções.
    socket_descritor = socket(AF_INET, SOCK_STREAM, 0); //AF_INET( IP versãon 4) = tipo de endereços, SOCK_STREAM = tipo (isso significa protocolo TCP orientado a conexão), Protocolo - 0 [ ou IPPROTO_IP é o IP protocolo]
    if (socket_descritor == -1){
        printf("Nao foi possivel criar o socket\n");
    }
    puts("Socket criado\n");

    //conectando um socket ao server
    server.sin_addr.s_addr = INADDR_ANY; // pega quaquer porta que está disponível no momento
	server.sin_family = AF_INET;
	server.sin_port = htons(8585); // porta aleatória

    if (bind(socket_descritor , (struct sockaddr *)&server , sizeof(server)) < 0){ // bind coloca o socket no lugar reservado para ele 
		puts("Erro na conexão!");
		return 1;
	}
	
	puts("conectado\n");

    //Listen
	listen(socket_descritor , 3); // colocar os sockets em listening mode

}