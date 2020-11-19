#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include 	<fcntl.h>
#include 	<unistd.h>
#include 	<sys/wait.h>

#define	CHILD_MESS	"Pipe has a real fun. Yah\n"
#define	PAR_MESS	"Waiting for may Child Message\n"
#define	oops(m,x)	{ perror(m); exit(x); }
#define PIPE_ENDS 2

int main(int argc, char* argv[]){
	int	pipefd[2];
	int	len;
	char buf[BUFSIZ];
	int	read_len;

	if ( pipe( pipefd ) == -1 )
		oops("cannot get a pipe", 1);

	switch( fork() ){
		case -1:
			oops("cannot fork", 2);
	
		case 0:			
			len = strlen(CHILD_MESS);
			while ( 1 ){
				if (write( pipefd[1], CHILD_MESS, len) != len )
					oops("write", 3);
				sleep(7);
			}

		default:		
			len = strlen( PAR_MESS );
			while ( 1 ){
				if ( write( pipefd[1], PAR_MESS, len)!=len )
					oops("write", 4);
				sleep(1);
				read_len = read( pipefd[0], buf, BUFSIZ );
				if ( read_len <= 0 )
					break;
				write( 1 , buf, read_len );
			}
	}
	return 0;
}
