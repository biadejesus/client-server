#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h> // biblioteca que importa as funções do socket
#include<arpa/inet.h> //sockaddr_in
#include <pthread.h> // para utilizar o pthread para múltiplas conexões

int main(){
    //criando um socket
    int socket_descritor, new_socket , c, *new_sock;
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

    //Aceita e conexões de entrada
	puts("Esperando por conexões de entrada\n");
	c = sizeof(struct sockaddr_in);
	while( (new_socket = accept(socket_descritor, (struct sockaddr *)client, (socklen_t*)c)) )
	{
		puts("Conexão aceita\n");
		
		//Resposta para o cliente		
		pthread_t sniffer_thread; // faz o threading
		new_sock = malloc(1);
		*new_sock = new_socket;
		
		if( pthread_create( sniffer_thread , NULL ,  connection_handler , (void*) new_sock) &lt; 0){
			perror(&quot;could not create thread&quot;);
			return 1;
		}
		
		//Now join the thread , so that we dont terminate before the thread
		//pthread_join( sniffer_thread , NULL);
		puts(&quot;Handler assigned&quot;);

        if (new_socket&lt;0)
	{
		perror(&quot;accept failed&quot;);
		return 1;
	}
	}

}