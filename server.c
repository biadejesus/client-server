#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h> // biblioteca que importa as funções do socket

int main(){
    int socket_descritor;
    struct sockaddr_in server; // utiliza para conectar a um servidor remoto em um determinado número de porta. Para fazer isso é preciso de uma porta e um endereço de IP.

    // a função socket() cria uma socket e retorna um descritor que pode ser usado em outras funções.
    socket_descritor = socket(AF_INET, SOCK_STREAM, 0); //AF_INET( IP versãon 4) = tipo de endereços, SOCK_STREAM = tipo (isso significa protocolo TCP orientado a conexão), Protocolo - 0 [ ou IPPROTO_IP é o IP protocolo]
    if (socket_descritor == -1)
    {
        printf("Nao foi possivel criar o socket\n");
    }
    puts("Socket criado\n");