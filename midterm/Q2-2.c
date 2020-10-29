// 2017116186 jungmin

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include <sys/types.h>
#include <sys/time.h>
#include <pwd.h>

#define BUFSIZE 2048
int pid;

void f(int , siginfo_t* , void* );
int main(int argc ,char *argv[]){
	char buf[BUFSIZE];
	struct sigaction handler;

	handler.sa_sigaction = f;
	handler.sa_flags = SA_SIGINFO;

	if(sigaction(SIGQUIT, &handler, NULL) == -1){
		perror("sigaction");
	}

	else{
		while(1){
			printf("Input PID which you want to send SIGINT : ");
			fflush(stdout);
			
			fgets(buf, BUFSIZE, stdin);
			pid = atoi(buf);
			
			kill(pid, SIGINT);
		}
	}
}

void f(int s, siginfo_t* siginfo, void* context){
	printf("Signal number %d\n", siginfo->si_signo);
	printf("Sending PID %d\n", siginfo->si_pid);
	printf("Done handling signal %d\n", s);
	kill(siginfo->si_pid, SIGQUIT);
	sleep(2);
}
