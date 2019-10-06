#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h> //contém as definições dos tipos de dados usados nas chamadas de sistema
#include<sys/socket.h> //contém definições de structs que os sockets precisam --struct sockaddr
#include<netinet/in.h> //contém as constantes e structs necessárias pros endereços (portas) --struct sockaddr_in
#include<netdb.h> //struct  hostent 

int main(){

    int sockfd = socket(AF_INET, SOCK_STREAM, 0); //dominio: ipv4, tipo de comunicação, protocolo.
    struct sockaddr_in server;


    if (sockfd < 0){
        printf("/nErro: não foi possível criar o socket");
    } 
    
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); //endereço
    server.sin_family = AF_INET; //ipv4
    server.sin_port = htons(8888) //htons converte o numero da porta em bytes pra internet. 

}