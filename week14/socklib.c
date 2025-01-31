#include "socklib.h"

#define HOSTLEN 256
#define BACKLOG 1

int make_server_socket_q(int, int);

int make_server_socket(int portnum){
    return make_server_socket_q(portnum, BACKLOG);
}

int make_server_socket_q(int portnum, int backlog){
    struct sockaddr_in saddr; 
    int sock_id;  

    sock_id = socket(PF_INET, SOCK_STREAM, 0); 
    if (sock_id == -1)
        return -1;

    bzero((void *)&saddr, sizeof(saddr)); 

    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(portnum);
    saddr.sin_family = AF_INET;
    if (bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
        return -1;

    if (listen(sock_id, backlog) != 0)
        return -1;
    return sock_id;
}

int connect_to_server(char *host, int portnum){
    int sock;
    struct sockaddr_in servadd;
    struct hostent *hp;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        return -1;

    bzero(&servadd, sizeof(servadd));
    hp = gethostbyname(host); 
    if (hp == NULL)
        return -1;
    bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr, hp->h_length);
    servadd.sin_port = htons(portnum);
    servadd.sin_family = AF_INET; 

    if (connect(sock, (struct sockaddr *)&servadd, sizeof(servadd)) != 0)
        return -1;

    return sock;
}