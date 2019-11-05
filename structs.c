#define post 1
#define get 2
#define TAM 100
#define criar 1
#define buscar 2
#define portaCliente 8585
#define portaPipe 8565
#define portaMC 8675
#define portaFifo 8685
#define PSHARED 1

#include <arpa/inet.h>

/* struct sockaddr_in {
        short   sin_family;
        unsigned short sin_port;
        struct  in_addr sin_addr;
        char    sin_zero[8];
};

struct in_addr {
   unsigned long s_addr;
}
*/
typedef struct{
    char tipo[30];
    int idade, ID;
    char nome[30];
}info;

typedef struct{
    info informacao;
    int flag;
    char resposta[30];
}requisicao;

typedef struct
{
	info db[TAM];

}DADOS;
struct  hostent {
        char    *h_name;        /* official name of host */
        char    **h_aliases;    /* alias list */
        int     h_addrtype;     /* host address type */
        int     h_length;       /* length of address */
        char    **h_addr_list;  /* list of addresses from name server */
#define h_addr  h_addr_list[0]  /* address, for backward compatiblity */
};
