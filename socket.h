#ifndef SOCKET.H 
#define SOCKET.H 

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include "structs.c"

int criarSocket(int PORTA);
int conectarSocket(int PORTA);
void mandarMsg(int sockt, requisicao *msg, uint32_t msgsize);

#endif