#include	<stdio.h>
#include	<stdlib.h>
#include	<fcntl.h>
#include	<unistd.h>

#define	CLOSE_DUP
#define MAXLINE 100

const char* TARGET_FILE = "/etc/passwd";

int main(int argc, char* argv[]){
	int	fd ;
	int	newfd;
	char	line[MAXLINE];

	fgets( line, MAXLINE, stdin ); printf("%s", line );
	fgets( line, MAXLINE, stdin ); printf("%s", line );
	fgets( line, MAXLINE, stdin ); printf("%s", line );

	fd = open(TARGET_FILE, O_RDONLY);
#ifdef CLOSE_DUP
	close(0);
	newfd = dup(fd);
#else
	newfd = dup2(fd,0);
#endif
	if ( newfd != 0 ){
		fprintf(stderr,"Could not duplicate fd(%d) to 0\n", newfd);
		exit(1);
	}
	close(fd);

	fgets( line, MAXLINE, stdin ); printf("%s", line );
	fgets( line, MAXLINE, stdin ); printf("%s", line );
	fgets( line, MAXLINE, stdin ); printf("%s", line );

	return 0;
}
