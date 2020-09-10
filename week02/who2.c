#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define SHOWHOST

void showtime(long);
void show_info(struct utmp *);

int main(int argc, char *argv[]){
	struct utmp utbuf;
	int utmpfd;

	if((utmpfd = open (UTMP_FILE, O_RDONLY)) == -1){
		perror(UTMP_FILE);
		exit(1);
	}

	while(read(utmpfd, &utbuf, sizeof(utbuf)) == sizeof(utbuf)){
		show_info(&utbuf);
	}
	
	close(utmpfd);
	return 0;
}

void show_info(struct utmp* utbufp){
	if(utbufp->ut_type != USER_PROCESS) return;

	printf(" % - 8.8s", utbufp->ut_name);
	printf(" ");

	printf(" % - 8.8s", utbufp->ut_line);
	printf(" ");

	showtime(utbufp->ut_time);
	printf(" ");

	#ifdef SHOWHOST
		printf(" (%s)", utbufp->ut_host);
	
	#endif
		printf("\n");
}

void showtime(long timeval){
	char *cp;
	cp = ctime(&timeval);

	printf(" % 12.12s", cp+4);
}
