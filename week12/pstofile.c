#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
	int	pid ;
	int	fd;

	printf("About to run ps aux into a file\n");

	if( (pid = fork() ) == -1 ){
		perror("fork"); exit(1);
	}

	if ( pid == 0 ){
		close(1);				
		fd = creat( "processlist", 0644 );
		execlp( "ps", "aux", NULL );
		perror("execlp");
		exit(1);
	}
	/* parent waits then reports */
	if ( pid != 0 ){
		wait(NULL);
		printf("Done running ps aux.  results in processlist\n");
	}
}
