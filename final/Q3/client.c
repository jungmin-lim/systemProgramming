#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>

#define BUFSIZE 1024
#define oops(msg) { perror(msg); exit(1); }

int main(int ac, char *av[]){
	struct sockaddr_in  servadd;
	struct hostent      *hp; 
	int    sock_id;
	char   message[BUFSIZ];
	char   buf[BUFSIZE];
	int    messlen; 

	fprintf(stdout, "------ 2017116186 jungmin lim ------\n");
	sock_id = socket( AF_INET, SOCK_STREAM, 0 );
	if ( sock_id == -1 ) 
		oops( "socket" );  

	bzero( &servadd, sizeof( servadd ) );

	hp = gethostbyname( av[1] ); 
	if (hp == NULL) 
		oops(av[1]);  
	bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr, hp->h_length);

	servadd.sin_port = htons(atoi(av[2]));
	servadd.sin_family = AF_INET ;

	if ( connect(sock_id,(struct sockaddr *)&servadd, sizeof(servadd)) !=0)
	       oops( "connect" );

	while(1){
		fprintf(stdout, "Type in 'memory' or 'cpu' ('Q' means exit.)\n");
		fgets(buf, sizeof(buf), stdin);
		buf[strlen(buf)-1] = '\0';

		if(!strcmp(buf, "Q")){
			write(sock_id, buf, strlen(buf));
			fprintf(stdout, "Exit!\n");
			break;
		}
		else if(!strcmp(buf, "memory")){
			write(sock_id, buf, strlen(buf));
		}
		else if(!strcmp(buf, "cpu")){
			fprintf(stdout, "sedning %s\n",buf);
			write(sock_id, buf, strlen(buf));
		}
		else{
			fprintf(stdout, "Invalid input try again\n");
			continue;
		}

		messlen = read(sock_id, message, sizeof(message));
		if(messlen <= 0){
			oops("read");
		}
		if(write(1, message, messlen) != messlen){
			oops("write");
		}
	}
	close( sock_id );    
}